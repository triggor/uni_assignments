/*
 * This file is based on example source code 'hello_ll.c'
 *
 * FUSE: Filesystem in Userspace
 * Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
 *
 * This program can be distributed under the terms of the GNU GPL.
 */

#define FUSE_USE_VERSION 30

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse_lowlevel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fat16.h"
#include "blkio.h"

#define UNUSED __attribute__((unused))

typedef struct fuse_file_info fuse_file_info_t;

static const char *fat16_str = "Hello WWWWorldsdfdf!\n";
static const char *fat16_name = "ello";
struct direntry* rootDirEntries;
unsigned numOfRootEntries;

static int fat16_stat(fuse_ino_t ino, struct stat *stbuf) {
  stbuf->st_ino = ino;
  
  if (ino < numOfRootEntries - 1) { //root children
    struct direntry inode = rootDirEntries[ino];
    if (inode.deAttributes & ATTR_DIRECTORY) {//is directory
      stbuf -> st_mode = S_IFDIR | 0755;
      stbuf->st_nlink = 2;
    } 
    else { //is regular file
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = inode.deFileSize;
      if(inode.deFileSize % BLKSIZE == 0){
        stbuf->st_blocks = inode.deFileSize / BLKSIZE;
      }
      else {
        stbuf->st_blocks = inode.deFileSize / BLKSIZE + 1;
      }
    }
    return 0;
  } 
  else {
    fprintf(stderr, "ERROR: invalid inode number\n");
    exit(1);
  }
  
  return 0;
}

static void fat16_getattr(fuse_req_t req, fuse_ino_t ino,
                          fuse_file_info_t *fi UNUSED) {
  struct stat stbuf;

  memset(&stbuf, 0, sizeof(stbuf));
  if (fat16_stat(ino, &stbuf) == -1) {
    fuse_reply_err(req, ENOENT);
  } else {
    fuse_reply_attr(req, &stbuf, 1.0);
  }
}

static void fat16_lookup(fuse_req_t req, fuse_ino_t parent, const char *name) {
  struct fuse_entry_param e;

  if (parent == 0) {
    for(unsigned i=1; i < numOfRootEntries; i++){
      if (strcmp(name, (char*)rootDirEntries[i].deName) == 0){
        memset(&e, 0, sizeof(e));
        e.ino = i;
        e.attr_timeout = 1.0;
        e.entry_timeout = 1.0;
        fat16_stat(e.ino, &e.attr);
        fuse_reply_entry(req, &e);
      }
      else{
        fuse_reply_err(req, ENOENT);
      }
    }
  }
}

typedef struct {
  char *p;
  size_t size;
} dirbuf_t;

static void dirbuf_add(fuse_req_t req, dirbuf_t *b, const char *name,
                       fuse_ino_t ino) {
  struct stat stbuf;
  size_t oldsize = b->size;
  b->size += fuse_add_direntry(req, NULL, 0, name, NULL, 0);
  b->p = (char *)realloc(b->p, b->size);
  memset(&stbuf, 0, sizeof(stbuf));
  stbuf.st_ino = ino;
  fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf,
                    b->size);
}

#define min(x, y) ((x) < (y) ? (x) : (y))

static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize,
                             off_t off, size_t maxsize) {
  if (off < (ssize_t)bufsize) {
    return fuse_reply_buf(req, buf + off, min(bufsize - off, maxsize));
  } else {
    return fuse_reply_buf(req, NULL, 0);
  }
}

static void fat16_opendir(fuse_req_t req, fuse_ino_t ino UNUSED,
                          fuse_file_info_t *fi) {
  fuse_reply_open(req, fi);
}

static void fat16_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
                          off_t off, fuse_file_info_t *fi UNUSED) {
  if (ino != 0) {
    fuse_reply_err(req, ENOTDIR);
  }
  else {
    dirbuf_t b;

    memset(&b, 0, sizeof(b));
    dirbuf_add(req, &b, ".", 1);
    dirbuf_add(req, &b, "..", 1);
    
    for (unsigned i = 1; i < numOfRootEntries; i++) {
      dirbuf_add(req, &b, (char*)rootDirEntries[i].deName, i);
    }
    
    reply_buf_limited(req, b.p, b.size, off, size);
    free(b.p);
  }
}

static void fat16_releasedir(fuse_req_t req, fuse_ino_t ino UNUSED,
                             fuse_file_info_t *fi UNUSED) {
  fuse_reply_err(req, ENOENT);
}

static void fat16_open(fuse_req_t req, fuse_ino_t ino, fuse_file_info_t *fi) {
  /*if (ino != 2)
    fuse_reply_err(req, EISDIR);
  else */
  if ((fi->flags & 3) != O_RDONLY)
    fuse_reply_err(req, EACCES);
  else
    fuse_reply_open(req, fi);
}


static void fat16_read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off,
                       fuse_file_info_t *fi UNUSED) {

  reply_buf_limited(req, fat16_str, strlen(fat16_str), off, size);
}


static void fat16_release(fuse_req_t req, fuse_ino_t ino UNUSED,
                          fuse_file_info_t *fi UNUSED) {
  fuse_reply_err(req, ENOENT);
}

static void fat16_statfs(fuse_req_t req, fuse_ino_t ino UNUSED) {
  struct statvfs statfs;
  memset(&statfs, 0, sizeof(statfs));
  fuse_reply_statfs(req, &statfs);
}

static struct fuse_lowlevel_ops fat16_oper = {
  .lookup = fat16_lookup,
  .getattr = fat16_getattr,
  .opendir = fat16_opendir,
  .readdir = fat16_readdir,
  .releasedir = fat16_releasedir,
  .open = fat16_open,
  .read = fat16_read,
  .release = fat16_release,
  .statfs = fat16_statfs
};



int main(int argc, char *argv[]) {
  
  if(argc != 3){
    fprintf(stderr, "Should be 3 arguments: fat16 image mountpoint\n");
    //fprintf (stderr, "err=%d: %s\n", errno, strerror (errno));
    exit(1);
  }
  const char* fs_image = argv[1];
  printf("fs_image = %s\n", fs_image);
  argv[1] = argv[2];
  argv[2] = NULL;
  argc--;
  
  int fd = blk_open(fs_image);
  struct bootsector50 bootRecord;
  struct bpb50* biosParBlock;
  int rootDirStart;
  int dataStart;
  int zerothCluster;

  

  blk_read(fd, &bootRecord, 0, 1);

  
  
  biosParBlock = (struct bpb50*)&bootRecord.bsBPB;

  rootDirStart = biosParBlock->bpbResSectors + (biosParBlock->bpbFATs * biosParBlock->bpbFATsecs);
  printf("bpbFATsecs: %d\n", biosParBlock->bpbFATsecs);
  printf("Root directory offset: %d\n", rootDirStart);
  
  numOfRootEntries = biosParBlock->bpbRootDirEnts;
  int numOfRootBlocks = numOfRootEntries * sizeof(struct direntry) / biosParBlock->bpbBytesPerSec;
  printf("numOfRootBlocks: %d\n", numOfRootBlocks);

  printf("bpbRootDirEnts * sizeof struct direntry: %ld\n", numOfRootEntries * sizeof(struct direntry));
  
  if((rootDirEntries = (struct direntry*)malloc(numOfRootEntries * sizeof(struct direntry)))==NULL){
    fprintf(stderr, "ERROR: malloc\n");
    exit(1);
  }
  else printf("malloc OK\n");
  blk_read(fd, rootDirEntries, rootDirStart, numOfRootBlocks);

  dataStart = rootDirStart + numOfRootBlocks;
  printf("dataStart: %d\n", dataStart);
  zerothCluster = dataStart - biosParBlock->bpbSecPerClust * 2;
  printf("zerothCluster: %d\n", zerothCluster);
  
    
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
  struct fuse_chan *ch;
  char *mountpoint;
  int err = -1;

  if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 &&
      (ch = fuse_mount(mountpoint, &args)) != NULL) {
    struct fuse_session *se;
     printf("mountpoint: %s\n", mountpoint);
    se = fuse_lowlevel_new(&args, &fat16_oper, sizeof(fat16_oper), NULL);
    if (se != NULL) {
      if (fuse_set_signal_handlers(se) != -1) {
        fuse_session_add_chan(se, ch);
        err = fuse_session_loop(se);
        fuse_remove_signal_handlers(se);
        fuse_session_remove_chan(ch);
      }
      fuse_session_destroy(se);
      
    }
    fuse_unmount(mountpoint, ch);
  }
  fuse_opt_free_args(&args);
  free(rootDirEntries);
  blk_close(fd);
  
  
  printf("err=%d\n", err);

  return err ? 1 : 0;
}

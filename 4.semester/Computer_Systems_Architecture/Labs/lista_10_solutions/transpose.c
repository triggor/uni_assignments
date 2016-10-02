/*
 * Matrix transposition with and without blocking.
 *
 * $ ./transpose -n 13 -v 0
 * Time elapsed: 3.398053 seconds.
 * $ ./transpose -n 13 -v 1
 * Time elapsed: 0.584577 seconds.
 */
#include "common.h"

void fill(int *dst, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      dst[i * n + j] = i * n + j;
}

void transpose1(int *dst, int *src, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      dst[j * n + i] = src[i * n + j];
}
//==================================================================================
/* Igor Tryhub:
 * ./transpose -n 13 -v 0
 * Time elapsed: 3.036009 seconds.
 * ./transpose -n 13 -v 1
 * Time elapsed: 0.568544 seconds.
 */
void transpose2(int *dst, int *src, int n) {
  int i, j, i1, j1, num, size = 8;
  num = n/size;
  if (n % size) num++;
  for (i = 0; i < num; i++)
	  for (j = 0; j < num; j++)
		  for (i1 = 0; i1 < (i==num-1 ? n%size : size); i1++)
			  for (j1 = 0; j1 < (j==num-1 ? n%size : size); j1++)
				  dst[(size*i + i1) + (size*j + j1)*n] = src[(size*i + i1)*n + (size*j + j1)];
}
//==================================================================================
int main(int argc, char **argv) {
  int opt, exp = -1, var = -1;
  bool err = false;

  while ((opt = getopt(argc, argv, "n:v:")) != -1) {
    if (opt == 'n')
      exp = atoi(optarg);
    else if (opt == 'v')
      var = atoi(optarg);
    else
      err = true;
  }

  if (err || exp < 0 || var < 0 || var >= 2) {
    fprintf(stderr, "Usage: %s -n log2(size) -v variant\n", argv[0]);
    return 1;
  }

  int n = 1 << exp;
  int size = n * n * sizeof(int);
  int *src = NULL, *dst = NULL;

  posix_memalign((void **)&src, getpagesize(), size);
  posix_memalign((void **)&dst, getpagesize(), size);

  printf("Generate matrix %d x %d (%d KiB)\n", n, n, size >> 10);

  fill(src, n);
  bzero(dst, size);
  flush_cache();

  printf("Performing matrix transposition.\n");

  _timer_t timer;
  timer_reset(&timer);
  timer_start(&timer);
  if (var == 0) 
    transpose1(dst, src, n);
  else
    transpose2(dst, src, n);
  timer_stop(&timer);
  timer_print(&timer);

  free(src);
  free(dst);

  return 0;
}

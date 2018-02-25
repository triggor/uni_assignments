//Author: Igor Tryhub, 275235
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void fromPDP(FILE* input, FILE* output, bool endian);
void intoPDP(FILE* input, FILE* output, bool endian);

int main(int argc, char *argv[])
{

  if(argc != 4) {
    printf("Niepoprawne uzycie. Prosze wprowadzic `plik in`, `plik out`, `tryb`(0-fromPDP, 1-intoPDP)\n");
    return 1;
  }

  bool big;
  uint32_t x = 1;
  char *y = (char*)&x;
  if (*y == 1) {
      big = false; //0D 0C 0B 0A
      printf("The system uses little-endian formating.\n");
  }
  else {
      big = true; //0A 0B 0C 0D
      printf("The system uses big-endian formating.\n");
  }

  char* inputPath = argv[1];
  char* outputPath = argv[2];
  int mode = atoi(argv[3]);
  FILE *in, *out;
  in = fopen(inputPath, "rb");
  out = fopen(outputPath, "wb");
  if(mode == 0) fromPDP(in, out, big);
  else intoPDP(in, out, big);
  fclose(in);
  fclose(out);
  return 0;
}

void fromPDP(FILE* input, FILE* output, bool endian){
  uint32_t PDPword;
  uint32_t OUTword = 0;
  uint8_t atom[4];
  if (endian){ //0B 0A 0d 0C  -->  0A 0B 0C 0D
    while(fread(&PDPword, sizeof(uint32_t), 1, input)==1){
      atom[2] = (uint8_t) (PDPword >>0);
      atom[3] = (uint8_t) (PDPword >>8);
      atom[0] = (uint8_t) (PDPword >>16);
      atom[1] = (uint8_t) (PDPword >>24);
      OUTword |= atom[0];
      OUTword = (OUTword<<8) | atom[1];
      OUTword = (OUTword<<8) | atom[2];
      OUTword = (OUTword<<8) | atom[3];
      fwrite(&OUTword, sizeof(char), 1, output);
    }
  }
  else { //0B 0A 0d 0C  -->  0D 0C 0B 0A
    while(fread(&PDPword, sizeof(uint32_t), 1, input)==1){
      atom[1] = (uint8_t) (PDPword >>0);
      atom[0] = (uint8_t) (PDPword >>8);
      atom[3] = (uint8_t) (PDPword >>16);
      atom[2] = (uint8_t) (PDPword >>24);
      OUTword |= atom[0];
      OUTword = (OUTword<<8) | atom[1];
      OUTword = (OUTword<<8) | atom[2];
      OUTword = (OUTword<<8) | atom[3];
      fwrite(&OUTword, sizeof(char), 1, output);
    }
  }
}

void intoPDP(FILE* input, FILE* output, bool endian){
  uint32_t INword;
  uint32_t PDPword = 0;
  uint8_t atom[4];
    if (endian){ //0B 0A 0d 0C  <--  0A 0B 0C 0D
    while(fread(&INword, sizeof(uint32_t), 1, input)==1){
      atom[2] = (uint8_t) (INword >>0);
      atom[3] = (uint8_t) (INword >>8);
      atom[0] = (uint8_t) (INword >>16);
      atom[1] = (uint8_t) (INword >>24);
      PDPword |= atom[0];
      PDPword = (PDPword<<8) | atom[1];
      PDPword = (PDPword<<8) | atom[2];
      PDPword = (PDPword<<8) | atom[3];
      fwrite(&PDPword, sizeof(char), 1, output);
    }
  }
  else { //0B 0A 0d 0C  <--  0D 0C 0B 0A
    while(fread(&INword, sizeof(uint32_t), 1, input)==1){
      atom[1] = (uint8_t) (INword >>0);
      atom[0] = (uint8_t) (INword >>8);
      atom[3] = (uint8_t) (INword >>16);
      atom[2] = (uint8_t) (INword >>24);
      PDPword |= atom[0];
      PDPword = (PDPword<<8) | atom[1];
      PDPword = (PDPword<<8) | atom[2];
      PDPword = (PDPword<<8) | atom[3];
      fwrite(&PDPword, sizeof(char), 1, output);
    }
  }
}


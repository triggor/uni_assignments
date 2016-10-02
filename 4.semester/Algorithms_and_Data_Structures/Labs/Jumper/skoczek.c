#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned wierszy;
unsigned kolumny;
char wczytane[3][5005];
unsigned long long policzone[3][5005];
unsigned long long stokrotki;
unsigned power7[10];

void inicjalizacja(void);
void init_power7(void);
void skok_dol(void);
void skok_gora(void);
void next_step(void);
unsigned long long get_max(void);

//==========================================================================

int main (void){

  init_power7();

  inicjalizacja();

  for (unsigned w=3; w < wierszy; w++){
    next_step();
  }

  stokrotki = get_max();

  printf("%llu", stokrotki);

  return 0;
}

//==========================================================================

void inicjalizacja(void){
  scanf("%u %u", &wierszy, &kolumny);
  for (unsigned w=0; w<3; w++){
    getchar();
    if (fgets (wczytane[w], kolumny+1, stdin) == NULL){
      printf("ERROR: fgets\n");
      exit(1);
    }
  }

  for (unsigned k=0; k<kolumny; k++){
    policzone[0][k] = power7[(unsigned)wczytane[0][k] - 48];
    policzone[1][k] = 0;
    policzone[2][k] = 0;
  }

  skok_dol();
}

//==========================================================================

void init_power7(void){
  unsigned result = 1;
  for(unsigned i=0; i<10; i++){
    power7[i]=result;
    result *= 7;
  }
}

//==========================================================================

void skok_dol(void){
  for (unsigned k=0; k<kolumny; k++){
     if(k < kolumny-1){
      policzone[2][k+1] = policzone[0][k] + power7[(unsigned)wczytane[2][k+1] - 48];
    }
    if(k > 0){
      if (policzone[2][k-1] < policzone[0][k] + power7[(unsigned)wczytane[2][k-1] - 48]){
        policzone[2][k-1] = policzone[0][k] + power7[(unsigned)wczytane[2][k-1] - 48];
      }
    }
  }
}

//==========================================================================

void skok_gora(void){
  for (unsigned k=0; k<kolumny; k++){
    if(k < kolumny-2){
      unsigned long long newvalue = policzone[1][k] + power7[(unsigned)wczytane[0][k+2] - 48];
      if (policzone[0][k+2] < newvalue){
          policzone[0][k+2] = newvalue;
      }
    }
    if(k > 1){
      unsigned long long newvalue = policzone[1][k] + power7[(unsigned)wczytane[0][k-2] - 48];
      if (policzone[0][k-2] < newvalue){
          policzone[0][k-2] = newvalue;
      }
    }
  }
}

//==========================================================================

void next_step(void){
  //char * strcpy ( char * destination, const char * source );
  strcpy(wczytane[0], wczytane[1]);
  strcpy(wczytane[1], wczytane[2]);
  getchar();
  if (fgets (wczytane[2], kolumny+1, stdin) == NULL){
    printf("ERROR: fgets\n");
    exit(1);
  }

  //void * memcpy ( void * destination, const void * source, size_t num );
  memcpy(policzone[0], policzone[1], sizeof(policzone[0]));
  memcpy(policzone[1], policzone[2], sizeof(policzone[0]));
  memset(policzone[2], 0, 1);//moze byc problem

  skok_gora();
  skok_dol();
}

//==========================================================================

unsigned long long get_max(void){
  unsigned long long maxim = 0;
  for (unsigned k=0; k<kolumny; k++){
    if (policzone[2][k] > maxim){
      maxim = policzone[2][k];
    }
  }
  return maxim;
}

//==========================================================================

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define MILION 1000000

inline int pop2(unsigned long long x);
unsigned long long result = 0;
uint32_t size;

unsigned long long pow2to[64];

typedef struct sznurek{
  unsigned long long dlugosc;
  unsigned long long kawalki;
} Sznurek;

Sznurek tab[MILION];

int wczytywanie(void);
int obrachunki(void);
int compare_function(const void *a,const void *b);
long long bin_search(unsigned long long key);
unsigned long long init_pow2to(void);
inline unsigned long long pow2(uint32_t i);

int main(void){

  init_pow2to();
  wczytywanie();

  qsort(tab, size, sizeof(Sznurek), compare_function);

  obrachunki();


  printf("%llu", result);


  return 0;
}

//==========================================================================

int wczytywanie(){//працюэ правильно


  unsigned long long length;
  unsigned long long pieces;

  scanf("%u", &size);
  for(uint32_t i = 0; i < size; i++){
    scanf("%llu %llu", &length, &pieces);
    tab[i].dlugosc = length;
    tab[i].kawalki = pieces;
  }

  return 0;
}

//==========================================================================

int obrachunki(void){
  for(uint32_t n = 0; n < size; n++){ //проходимо кожен елемент вчитаноъ таблицы
    if((tab[n].kawalki)%2==1)//елемент непарний
    {
      result++;
      (tab[n].kawalki)--;
    }
    for(uint32_t i = 1; tab[n].dlugosc>0 && i < sizeof(unsigned long long)*8; i++){//скачемо по елементах з ындексами x*(2^i)

        unsigned long long key = tab[n].dlugosc * pow2to[i];//шукаэмо наступний ключ
        unsigned long long index = bin_search(key);//шукаэмо ындекс структури з потрыбним ключем
        tab[n].kawalki = (tab[n].kawalki)/2;//звязуэмо шнурки
        if(index != -1){//елемент з таким ключем ыснуэ в нашый таблицы

          tab[n].kawalki += tab[index].kawalki;
          tab[index].kawalki=0;
          tab[index].dlugosc=0;
          if(tab[n].kawalki%2==1)//елемент непарний
          {
            result++;
            (tab[n].kawalki)--;
          }
        }
        else{//елементу з таким ключем вже немаэ в таблицы
          result += pop2(tab[n].kawalki);//рахуэмо скыльки раз вын дасть непарне число шнуркыв, якшо його звязувати поки не стане нулем (якы быти свытяться)
          tab[n].kawalki=0;//не обовязково. але коли ми проскакали всю таблицю з даним префыксом, можемо його обнулити шоб не мышав
          break;
        }
    }
  }
  return 0;
}

//==========================================================================

int compare_function(const void *a,const void *b){//працюэ правильно
  unsigned long long adlugosc = *((unsigned long long *) a);
  unsigned long long bdlugosc = *((unsigned long long *) b);
  return adlugosc - bdlugosc;
}

//==========================================================================

long long bin_search(unsigned long long key)//можуть бути нюанси, але ныби працюэ правильно
{
  unsigned long long low = 0;
  unsigned long long high = size-1;
  unsigned long long mid;

  while(low <= high){
    mid = (low + high)/2;
    if(tab[mid].dlugosc < key){
      low = mid + 1;
    }
    else if(tab[mid].dlugosc > key){
      high = mid-1;
    }
    else if(tab[mid].dlugosc == key)
    {
      return (long long)mid;
    }
  }

  return -1;
}

//==========================================================================

unsigned long long init_pow2to(void){//працюэ правильно
  for(uint32_t i = 0; i < sizeof(unsigned long long)*8; i++){
    pow2to[i] = pow2(i);
  }
  return 0;
}

//==========================================================================

inline unsigned long long pow2(uint32_t i){//працюэ правильно
  unsigned long long res = 1;
  while(i>0){
    i=i-1;
    res=res<<1;
  }
  return res;
}

//==========================================================================

inline int pop2(unsigned long long x)//працюэ правильно
{
    x = x - ((x >> 1) & 0x5555555555555555);
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    x = x + (x >> 32);
    return x & 0xFF;
}

//==========================================================================

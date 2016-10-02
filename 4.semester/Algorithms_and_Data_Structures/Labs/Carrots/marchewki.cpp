//Author: Igor Tryhub, 275235
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <algorithm>

const unsigned PPP=9999991;

using namespace std;

void wczytywanie(unsigned* k, vector <unsigned>* LLL);
void wypisywanie(unsigned m, unsigned a, unsigned* AAA, vector <unsigned>* KKK);

int main()
{
  unsigned N, m, a;
  vector <unsigned> LLL; //numery seryjne
  wczytywanie(&N, &LLL);
  m=N;
  bool done = false;
  do{
  //===========================INICJALIZACJA dla kolejnego `m`<`N`===================================
    unsigned AAA[m];
    vector <unsigned> BBB[m]; //surowa tablica grazdek
    vector <unsigned> KKK[m]; //tablica grazdek
    a=1;
    for (size_t i=0; i<m; i++){
      AAA[i]=0;
      KKK[i].assign(1, 0);
    }
    for (auto elem : LLL){
      BBB[((a*elem)%PPP)%m].push_back(elem); //dowiadujemy sie do ktorych grzadek trafia marchewki o numerze z `LLL`
    }
    unsigned YYY[m]; //dlugosc grzadek
    unsigned Y_sum = 0; //suma dlugosci grzadek
    for (size_t i=0; i<m; i++){
      YYY[i] = BBB[i].size();
      Y_sum += YYY[i];
    }
    bool koniec = false;
    bool blad = false;
    //===========================ITERACJE po kolejnych `a`<`m`=================================
    while (!koniec){
      size_t i;
      blad = false;
      //===========================ITERACJE po grzadkach `i` przy jednoczesnym dopasowaniu `AAA[i]` oraz dlugosci grzadki `YYY[i]`==========
      for (i=0; i<m; i++){
        if(blad) break;
        //===========================ITERACJE po elementach w `i`-tej grzadce surowej=============================
        for (auto b_elem : BBB[i]){
          unsigned pole = ((AAA[i]*b_elem)%PPP)%YYY[i]; //dowiadujemy sie o indeksie pola w grzadce przy tak dobranych parametrach `m`,`YYY[i]`,`a`,`AAA[i]`
          if(KKK[i][pole]==0){ // wszystko OK: brak kolizji
            KKK[i][pole]= b_elem;
          }
          else{ //kolizja na danym polu
            if(AAA[i] < min(YYY[i],PPP)){ //jezeli zmiana `AAA[i]` ma sens
              AAA[i]++;
            }
            else if(Y_sum < 3*N){ //jezeli zmiana `YYY[i]` jest dozwolona
              AAA[i]=0;
              YYY[i]++;
              Y_sum++;
              KKK[i].assign(YYY[i],0);
            }
            else{ //musimy sprobowac dobrac nowe `a`, zresetowac dotychczasowe `AAA`,`YYY`,`Y_sum`,`KKK` i sprawdzic calosc od nowa
              blad = true;
              Y_sum=0;
              for (size_t s=0; s<m; s++){
                AAA[s]=0;
                YYY[s]=BBB[s].size();
                Y_sum += YYY[s];
                KKK[s].assign(YYY[s], 0);
              }
              if(a < min(m,PPP)) a++; //dobor nowego `a` ma sens
              else koniec=true; //musimy zmieniac `m`
              i++; //dla wyrownania
            }
            i--;
            KKK[i+1].assign(YYY[i+1], 0);
            break;
          }
        }
        if(i==m-1) koniec = true, done = true; //ostatnia marchewka z sukcesem znalazla swoje miejsce
      }
    }

    if (done) wypisywanie(m, a, AAA, KKK);

    if(m>1)m--;
  } while(!done);

  return 0;
}

//============================================================================

void wczytywanie(unsigned* N, vector <unsigned>* LLL){
  scanf("%u", N); //nie czytamy `\n`, bo nastepna instrukcja - `getchar()`
  unsigned L;
  for (unsigned i=0; i<*N; i++){
      getchar();
      scanf("%u", &L);
      LLL->push_back(L);
  }
}
//============================================================================

void wypisywanie(unsigned m, unsigned a, unsigned* AAA, vector <unsigned>* KKK){
  printf("%u\n", m);

  for (size_t i=0; i<m; i++){
      printf("%u ", KKK[i].size());
  }
  printf("\n%u\n", a);
  for (size_t i=0; i<m; i++){
    printf("%u ", AAA[i]);
  }
  printf("\n");
}

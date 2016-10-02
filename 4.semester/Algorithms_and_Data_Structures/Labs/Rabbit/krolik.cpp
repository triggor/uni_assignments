#include <stdio.h>
#include <stdbool.h>
#include <algorithm>
#include <vector>

typedef struct node {
  unsigned long long maximum = 0;
  unsigned long m_coord;
  unsigned long n_coord;
  unsigned long carrots = 0;
  struct node* prev = NULL;
  struct node* original = NULL;
} Node;

void wczytaj_mnk(unsigned long* m, unsigned long* n, unsigned long* k);
void wczytywanie(unsigned long k, Node* starting);
int cmpfunc_m (const void *a, const void *b);
unsigned long get_m_set(unsigned long k, Node* starting);
void fill_tab_m(Node* starting, volatile unsigned long* tab_m, unsigned long rozmiar_m);
unsigned long get_n_set(unsigned long k, Node* starting);
bool compare_by_n (Node a, Node b);
void fill_tab_n(Node* starting, volatile unsigned long* tab_n, unsigned long rozmiar_n);
void wypisywanie(unsigned long long global_maximum, Node* point2start, Node* zeroth, volatile unsigned long* tab_m, volatile unsigned long* tab_n);

//============================================================================

int main (void){

  unsigned long k, m, n;
  wczytaj_mnk(&m, &n, &k);
  Node starting[k];
  wczytywanie(k, starting);

  qsort(starting, (size_t)k, (size_t)sizeof(Node), cmpfunc_m);

  unsigned long rozmiar_m = get_m_set(k, starting);

  unsigned long tab_m[rozmiar_m];
  fill_tab_m(starting, tab_m, rozmiar_m);


  std::vector<Node> myvector;
  myvector.assign(starting, starting+k);
  std::stable_sort (myvector.begin(), myvector.end(), compare_by_n);
  for (size_t i=0; i<k; i++)
  {
      starting[i] = myvector[i];
  }
  unsigned long rozmiar_n = get_n_set(k, starting);

  unsigned long tab_n[rozmiar_n];
  fill_tab_n(starting, tab_n, rozmiar_n);

  Node* point2start = &starting[k-1];
  unsigned long counter = k-1;
  Node current_row[rozmiar_m];

  //«јѕќ¬ЌёЁћќ ѕ≈–Ў»… ≈Ћ≈ћ≈Ќ“
  if (point2start->m_coord==tab_m[rozmiar_m-1] && point2start->n_coord==tab_n[rozmiar_n-1]){//€кшо верхный правий э в оригыналы
      current_row[rozmiar_m-1].carrots = point2start->carrots;
      current_row[rozmiar_m-1].original = point2start;
      current_row[rozmiar_m-1].prev = NULL;
      point2start->prev = NULL;

      if (counter>0){
        counter--;
        point2start--;
      }
  }

  else current_row[rozmiar_m-1].carrots = 0;

  current_row[rozmiar_m-1].carrots++; //додаэмо 1 морковку до правого верхнього рогу
  current_row[rozmiar_m-1].maximum = current_row[rozmiar_m-1].carrots;

  //«јѕќ¬ЌёЁћќ ѕ≈–Ў»… –яƒќ  (крым першого елемента)
  if (rozmiar_m > 1){
    for (long col = rozmiar_m-2; col >= 0; col--){
      if (point2start->n_coord == tab_n[rozmiar_n-1] && point2start->m_coord == tab_m[col]){//€кшо э в оригыналы
        current_row[col].carrots = point2start->carrots;
        current_row[col].original = point2start;
        if (current_row[col+1].carrots > 0) current_row[col].prev = current_row[col+1].original;//€кшо попередный був реальним вузлом
        else current_row[col].prev = current_row[col+1].prev;
        point2start->prev = current_row[col].prev;

        if (counter>0){
          counter--;
          point2start--;
        }
      }
      else {
        current_row[col].carrots = 0;
        if (current_row[col+1].carrots > 0) current_row[col].prev = current_row[col+1].original;//€кшо попередный був реальним вузлом
        else current_row[col].prev = current_row[col+1].prev;
      }
      current_row[col].maximum = current_row[col].carrots + current_row[col+1].maximum;
    }
  }

  //«јѕќ¬ЌёЁћќ –≈Ў“” –яƒ џ¬
  if (rozmiar_n > 1){

    for (unsigned long x = 2; x <= rozmiar_n; x++){
      unsigned long row = tab_n[rozmiar_n-x];

      //«јѕќ¬ЌёЁћќ Ќј…ѕ–ј¬џЎ»… ¬”«ќЋ
      Node upper;
      upper.carrots = current_row[rozmiar_m-1].carrots;
      upper.prev = current_row[rozmiar_m-1].prev;
      upper.original = current_row[rozmiar_m-1].original;
      upper.maximum = current_row[rozmiar_m-1].maximum;

      if (point2start->n_coord==row && point2start->m_coord==tab_m[rozmiar_m-1]){//€кшо правий э в оригыналы
        current_row[rozmiar_m-1].carrots = point2start->carrots;
        current_row[rozmiar_m-1].original = point2start;
        if (upper.carrots > 0) current_row[rozmiar_m-1].prev = upper.original;//€кшо верхный був реальним вузлом
        else current_row[rozmiar_m-1].prev = upper.prev;
        point2start->prev = current_row[rozmiar_m-1].prev;

        if (counter>0){
          counter--;
          point2start--;
        }
      }

      else {
        current_row[rozmiar_m-1].carrots = 0;
        if (upper.carrots > 0) current_row[rozmiar_m-1].prev = upper.original;//€кшо верхный був реальним вузлом
        else current_row[rozmiar_m-1].prev = upper.prev;
      }


      current_row[rozmiar_m-1].maximum = current_row[rozmiar_m-1].carrots + upper.maximum;

      //«јѕќ¬ЌёЁћќ –≈Ў“” ¬”«Ћџ¬ –яƒ ј
      for (unsigned long y = 2; y <= rozmiar_m; y++){
        unsigned long col = rozmiar_m-y;
        upper.carrots = current_row[col].carrots;
        upper.prev = current_row[col].prev;
        upper.original = current_row[col].original;
        upper.maximum = current_row[col].maximum;

        if (point2start->n_coord==row && point2start->m_coord==tab_m[col]){//€кшо э в оригыналы
          current_row[col].carrots = point2start->carrots;
          current_row[col].original = point2start;

          if (current_row[col+1].maximum >= upper.maximum){
            current_row[col].maximum = current_row[col].carrots + current_row[col+1].maximum;
            if (current_row[col+1].carrots > 0) current_row[col].prev = current_row[col+1].original; //€кшо правий э в оригыналы
            else current_row[col].prev = current_row[col+1].prev;
          }
          else {
            current_row[col].maximum = current_row[col].carrots + upper.maximum;
            if (upper.carrots > 0) current_row[col].prev = upper.original; //€кшо верхный э в оригыналы
            else current_row[col].prev = upper.prev;
          }

          point2start->prev = current_row[col].prev;

          if (counter>0){
            counter--;
            point2start--;
          }
        }
        else {
          current_row[col].carrots = 0;
          if (current_row[col+1].maximum >= upper.maximum){
            current_row[col].maximum = current_row[col].carrots + current_row[col+1].maximum;
            if (current_row[col+1].carrots > 0) current_row[col].prev = current_row[col+1].original; //€кшо правий э в оригыналы
            else current_row[col].prev = current_row[col+1].prev;
          }
          else {
            current_row[col].maximum = current_row[col].carrots + upper.maximum;
            if (upper.carrots > 0) current_row[col].prev = upper.original; //€кшо верхный э в оригыналы
            else current_row[col].prev = upper.prev;
          }
        }
      }
    }
  }

  unsigned long long global_maximum = current_row[0].maximum-1; //выднымаэмо 1 морковку (бо була в правому верхньому розы)

  wypisywanie(global_maximum, point2start, &current_row[0], tab_m, tab_n); //[0]-ий елемент задекларованоъ перед циклом таблицы мыститиме ынформацыю про максимум ы наступника на стежцы

  return 0;
}
//============================================================================

void wczytaj_mnk(unsigned long* m, unsigned long* n, unsigned long* k){
  scanf("%lu %lu\n", m, n);
  scanf("%lu", k);//не читаэмо символу кынц€ р€дка, бо наступний getchar()
}

//============================================================================

void wczytywanie(unsigned long k, Node* starting){
  for (unsigned long i=0; i<k; i++){
      getchar();
      scanf("%lu %lu %lu", &(starting[i].m_coord), &(starting[i].n_coord), &(starting[i].carrots));
  }
}
//============================================================================

void wypisywanie(unsigned long long global_maximum, Node* point2start, Node* zeroth, volatile unsigned long* tab_m, volatile unsigned long* tab_n){
  printf("%llu\n", global_maximum); //глобальний максимум

  unsigned long x_ings = 0;
  if (point2start->m_coord==tab_m[0] && point2start->n_coord==tab_n[0]){ //лывий нижный э в оригыналы
    x_ings++;
  }
  for (Node* pointer = zeroth->prev; pointer != NULL; pointer = pointer->prev){
    x_ings++;
  }
  printf("%lu\n", x_ings); //кылькысть перехресть

  if (point2start->m_coord==tab_m[0] && point2start->n_coord==tab_n[0]){ //лывий нижный э в оригыналы
    printf("%lu %lu\n", tab_m[0], tab_n[0]);
  }
  for (Node* pointer = zeroth->prev; pointer != NULL; pointer = pointer->prev){ //стежка
    printf("%lu %lu\n", pointer->m_coord, pointer->n_coord);
  }
}

//============================================================================

int cmpfunc_m (const void *a, const void *b){
  Node c = *(Node*)a;
  Node d = *(Node*)b;
  return ( c.m_coord - d.m_coord );//хочему ростущий пор€док
}

//============================================================================

unsigned long get_m_set(unsigned long k, Node* starting){
  unsigned long counter = 1;
  for (unsigned long i=1; i<k; i++){
    if (starting[i-1].m_coord == starting[i].m_coord) continue;
    else counter++;
  }
  return counter;
}

//============================================================================

void fill_tab_m(Node* starting, volatile unsigned long* tab_m, unsigned long rozmiar_m){
  tab_m[0] = starting[0].m_coord;
  for (unsigned long i=1, j=1; j<rozmiar_m; i++){
    if (starting[i-1].m_coord == starting[i].m_coord) continue;
    else {
      tab_m[j] = starting[i].m_coord;
      j++;
    }
  }
}

//============================================================================

bool compare_by_n (Node a, Node b){
  return (a.n_coord < b.n_coord);//хочемо ростущий пор€док
}

//============================================================================

unsigned long get_n_set(unsigned long k, Node* starting){
unsigned long counter = 1;
  for (unsigned long i=1; i<k; i++){
    if (starting[i-1].n_coord == starting[i].n_coord) continue;
    else counter++;
  }
  return counter;
}

//============================================================================

void fill_tab_n(Node* starting, volatile unsigned long* tab_n, unsigned long rozmiar_n){
  tab_n[0] = starting[0].n_coord;
  for (unsigned long i=1, j=1; j<rozmiar_n; i++){
    if (starting[i-1].n_coord == starting[i].n_coord) continue;
    else {
      tab_n[j] = starting[i].n_coord;
      j++;
    }
  }
}

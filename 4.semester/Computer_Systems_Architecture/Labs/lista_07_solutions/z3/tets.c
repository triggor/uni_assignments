#include <stdio.h>
#include <stdlib.h>
 
void insert_sort(long* first, long* last);

int main()
{
  unsigned rozmiar;
  int i;
  printf("Rozmiar: ");
  scanf("%lu", &rozmiar);
  long tab[rozmiar];
  printf("Tab: ");
  for (i=0; i<rozmiar; i++)
    scanf("%ld", &tab[i]);
  insert_sort(&tab[0], &tab[rozmiar-1]);
  printf("Res: ");
  for (i=0; i<rozmiar; i++)
    printf("%ld ", tab[i]);
  printf("\n");
  return 0;
} 

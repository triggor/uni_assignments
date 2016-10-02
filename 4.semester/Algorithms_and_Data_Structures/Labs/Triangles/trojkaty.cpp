#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <algorithm>    // std::stable_sort
#include <vector>       // std::vector

typedef struct {
  long x_coord;
  long y_coord;
} Point;

Point error = {20000000, 20000000};


void wczytywanie(unsigned rozmiar, Point *tab);
int cmpfunc_y (const void *a, const void *b);
double perimeter(long left_x, long right_x, Point* tab, unsigned rozmiar);
Point* bin_search(long key, unsigned rozmiar, Point* tab);
double base_case(Point* first_elem, size_t set_size);
bool compare_by_x (Point i, Point j);
double conquer(Point* first_elem, Point* last_plus_one_elem, long left, long right, long mediana, unsigned long width, size_t set_size, unsigned rozmiar, Point* tab);
int cmpfunc_p (const void *a, const void *b);

//============================================================================

int main (void) {
  unsigned rozmiar;
  scanf("%u", &rozmiar);
  Point tab[rozmiar];


  wczytywanie(rozmiar, tab);




  qsort(tab, (size_t)rozmiar, (size_t)sizeof(Point), cmpfunc_y);
  std::vector<Point> myvector;
  myvector.assign(tab, tab+rozmiar);
  std::stable_sort (myvector.begin(), myvector.end(), compare_by_x);

  for (size_t i=0; i<rozmiar; i++)
  {
      tab[i] = myvector[i];
      //printf("x: %ld %ld\n", tab[i].x_coord, tab[i].y_coord);
  }

  long left_x = tab[0].x_coord;
  long right_x = tab[rozmiar-1].x_coord;

  double smallest_perimeter = perimeter(left_x, right_x, tab, rozmiar);
  printf("%lf", smallest_perimeter);

  return 0;
}

//============================================================================

double perimeter(long left_x, long right_x, Point* tab, unsigned rozmiar){
    unsigned long width = right_x-left_x;

    Point* last_plus_one_elem = bin_search(right_x+1, rozmiar, tab);
    Point* first_elem = bin_search(left_x, rozmiar, tab);
    //printf("left:%ld\n", left_x);
    //printf("right:%ld\n", right_x);
    size_t set_size;
    if(first_elem != &error){
      if(last_plus_one_elem != &error){
        set_size = last_plus_one_elem - first_elem;
      }
      else{
        set_size = &tab[rozmiar-1] - first_elem + 1;
      }
    }
    else{
      printf("ERROR: first_elem is out of range\n");
      exit(1);
    }

    if(set_size<3){
        //printf("set_size<3\n");
        return ULLONG_MAX/8-2;
    }

    if (width==0){
        //printf("width==0\n");
        return base_case(first_elem, set_size);
    }

    long mediana = tab[0].x_coord + width/2;
    //printf("mediana=%ld\n", mediana);
    double left = perimeter(left_x, mediana, tab, rozmiar);
    double right = perimeter(mediana+1, right_x, tab, rozmiar);
    double middle = conquer(first_elem, last_plus_one_elem, left, right, mediana, width, set_size, rozmiar, tab);
    double perim = left>right ? right : left;
    perim = perim > middle ? middle : perim;

    return perim;

}

//============================================================================

Point* bin_search(long key, unsigned rozmiar, Point* tab){
  long low = 0;
  long high = rozmiar-1;
  long mid;

  if (tab[low].x_coord > key || tab[high].x_coord < key) return &error;

  while (high-low>=0){
    mid = (high+low)/2;
    if (tab[mid].x_coord == key){
      while (mid != 0 && tab[mid-1].x_coord == key) mid--;
      return &tab[mid];
    }
    else if (high-low<=1) return &tab[high];
    else if (tab[mid].x_coord > key) high = mid;
    else if (tab[mid].x_coord < key) low = mid;
  }
  return &error;
}

//============================================================================

double base_case(Point* first_elem, size_t set_size){
  double perim = ULLONG_MAX/8-2;
  //sprawdzamy tylko 3 pod rzad (sasiednie)


  if (( (first_elem+2)->x_coord != first_elem->x_coord )){
    printf("ERROR: bad assertion>=3 elem\n");
    exit(1);
  }
  while ( (first_elem+2)->x_coord == first_elem->x_coord ) {

    //printf("%ld %ld %ld\n", (first_elem+2)->x_coord, (first_elem+1)->x_coord, first_elem->x_coord);
    //printf("%ld %ld %ld\n", (first_elem+2)->y_coord, (first_elem+1)->y_coord, first_elem->y_coord);

    double local_perim = 2 * ( first_elem->y_coord - (first_elem+2)->y_coord );
    //printf("local_perim=%llu\n", local_perim);
    if (local_perim < perim) {
      perim = local_perim;
    }
    first_elem++;
  }
  return perim;
}

//============================================================================

void wczytywanie(unsigned rozmiar, Point *tab){//, Point **x_sorted, Point **y_sorted){
  for (size_t i=0; i<rozmiar; i++){
      getchar();
      scanf("%ld %ld", &(tab[i].x_coord), &(tab[i].y_coord));
      //x_sorted[i] = y_sorted[i] = &tab[i];
  }
}

//============================================================================

int cmpfunc_p (const void *a, const void *b){
  Point *c = *(Point**)a;
  Point *d = *(Point**)b;
  return ( d->y_coord - c->y_coord );
}

//============================================================================

int cmpfunc_y (const void *a, const void *b){
  Point c = *(Point*)a;
  Point d = *(Point*)b;
  return ( d.y_coord - c.y_coord );
}

//============================================================================
double conquer(Point* first_elem, Point* last_plus_one_elem, long left, long right, long mediana, unsigned long width, size_t set_size, unsigned rozmiar, Point* tab){
  double smallest_per = left<right ? left : right;
  long x1;
  long x2;
  //printf("width=%lu\n", width);
  //printf("smallest_per=%llu\n", smallest_per);
  if ((long)smallest_per%2==1)smallest_per++;

  if(width > smallest_per/2){
    x1 = mediana - smallest_per/2;
    x2 = mediana + smallest_per/2;
    first_elem = bin_search(x1, rozmiar, tab);
    last_plus_one_elem = bin_search(x2, rozmiar, tab);
    //printf("width > smallest_per/2\n");
  }
  else {
    x1 = first_elem->x_coord;
    x2 = (first_elem+set_size-1)->x_coord;
    //printf("width <= smallest_per/2\n");
  }
  //printf("x1=%ld\n", x1);
 // printf("x2=%ld\n", x2);
  //printf("first_elem=%ld:%ld\n", first_elem->x_coord,first_elem->y_coord);
  //printf("first_elem+1=%ld:%ld\n", (first_elem+1)->x_coord,(first_elem+1)->y_coord);
  //printf("first_elem+2=%ld:%ld\n", (first_elem+2)->x_coord,(first_elem+2)->y_coord);
  //printf("last_plus_one_elem-1=%ld:%ld\n", (last_plus_one_elem-1)->x_coord, (last_plus_one_elem-1)->y_coord);


  Point* y_sorted[set_size];
  for (int i=0; i<set_size; i++){
    y_sorted[i] = first_elem+i;
    //printf("y_sorted[%d]=%ld:%ld\n", i, y_sorted[i]->x_coord,y_sorted[i]->y_coord);
  }
  qsort(y_sorted, (size_t)set_size, (size_t)sizeof(Point*), cmpfunc_p);
  for (int i=0; i<set_size; i++){
    y_sorted[i] = first_elem+i;
   // printf("y_sorted[%d]=%ld:%ld\n", i,y_sorted[i]->x_coord,y_sorted[i]->y_coord);
  }

  for (Point* first = y_sorted[0]; first <= y_sorted[set_size-1]; first++)
      {
      //printf("first=%ld:%ld\n", first->x_coord,first->y_coord);
      for (Point* second = first+1; second <= y_sorted[set_size-1]; second++)
          {
          //if (!(second->y_coord <= first->y_coord) || !(second->y_coord >= first->y_coord-smallest_per/2)) continue;
          //printf("second=%ld:%ld\n", second->x_coord,second->y_coord);
          for (Point* third = second+1; third <= y_sorted[set_size-1]; third++)
              {
                  //if (!(third->y_coord <= first->y_coord) || !(third->y_coord >= first->y_coord-smallest_per/2)) continue;
                  //printf("third=%ld:%ld\n", third->x_coord,third->y_coord);
                  double local_per = sqrt((first->x_coord - second->x_coord)*(first->y_coord - second->y_coord)) +
                                                  sqrt((first->x_coord - third->x_coord)*(first->y_coord - third->y_coord)) +
                                                  sqrt((third->x_coord - second->x_coord)*(third->y_coord - second->y_coord));
                  //printf("local_per=%llu\n", local_per);
                  if (local_per<smallest_per) smallest_per = local_per;

              }

          }
      }
  return smallest_per;
}


//============================================================================

bool compare_by_x (Point i, Point j)
{
  return (i.x_coord < j.x_coord);
}

//============================================================================


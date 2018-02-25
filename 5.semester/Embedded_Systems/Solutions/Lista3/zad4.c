//Author: Igor Tryhub, 275235
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc!=2) {
      printf("Niepoprawne uzycie. Prosze podac argument.\n");
      return 1;
    }
    unsigned n = (unsigned)atoi(argv[1]);
    int counter = 1000;

    clock_t begin = clock();
    while (counter) {
      for (unsigned i=1; i<=n; i++){
          for (unsigned j=1; j<=i; j++){
            putchar('*');
          }
          putchar('\n');
      }
      counter--;
    }
    clock_t end = clock();
    double time_spent_one = (double)(end - begin) / CLOCKS_PER_SEC;

    counter = 1000;
    begin = clock();
    while (counter) {
      char string[n+1];
      memset(string, 32, n);
      string[n]=0;
      for (unsigned i=0; i<n; i++){
        string[i] = '*';
        puts(string);
      }
      counter--;
    }
    end = clock();
    double time_spent_two = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime for the first: %f\n", time_spent_one);
    printf("Time for the second: %f\n", time_spent_two);

    return 0;
}

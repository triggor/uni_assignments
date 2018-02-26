//Author: Igor Tryhub, 275235
/*
 * Zadanie 1.
 * Zaprogramuj semafory, o niżej zadanym interfejsie, dla wątków pthreads(7)
 * używając muteksów pthread_mutex_init(3) oraz zmiennych warunkowych
 * pthread_cond_init(3). Pamiętaj,  że  jedynym  wątkiem  uprawnionym  do  
 * zwolnienia  blokady  jest  jej  właściciel  –  tj.  wątek, który założył
 * tę blokadę. By wymusić sprawdzanie poprawności operacji na blokadzie nadaj 
 * jej wartość początkową PTHREAD_MUTEX_ERRORCHECK, a wynik operacji sprawdzaj
 * z użyciem assert(3).
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct {
  int value;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} sem_t;


void sem_init(sem_t *sem, unsigned value);
void sem_wait(sem_t *sem);
void sem_post(sem_t *sem);
void sem_getvalue(sem_t *sem, int *sval);

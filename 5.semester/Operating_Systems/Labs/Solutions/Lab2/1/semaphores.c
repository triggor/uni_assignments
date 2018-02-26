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

#include "semaphores.h"

void sem_init(sem_t *sem, unsigned value){
  sem->value = value;
  pthread_mutexattr_t attr;
  if(pthread_mutexattr_init (&attr))
    handle_error("pthread_mutexattr_init");
  if(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK))
    handle_error("pthread_mutexattr_settype");
  pthread_mutex_destroy(&(sem->mutex));
  if(pthread_mutex_init(&(sem->mutex), &attr))
    handle_error("pthread_mutex_init");
  if(pthread_cond_init(&(sem->cond), NULL))
    handle_error("pthread_cond_init");
}

void sem_wait(sem_t *sem){
  assert(pthread_mutex_lock(&(sem->mutex))!=EDEADLK);
  sem->value -= 1;
  if(sem->value < 0){
    if(pthread_cond_wait(&(sem->cond), &(sem->mutex)))
      handle_error("pthread_cond_wait");
  }
  
  assert(pthread_mutex_unlock(&(sem->mutex))!=EPERM);
}

void sem_post(sem_t *sem){
  assert(pthread_mutex_lock(&(sem->mutex))!=EDEADLK);
  sem->value += 1;
  if(sem->value <= 0){
    if(pthread_cond_signal(&(sem->cond)))
      handle_error("pthread_cond_signal");
  }
  assert(pthread_mutex_unlock(&(sem->mutex))!=EPERM);
}

void sem_getvalue(sem_t *sem, int *sval){
  *sval = sem->value;
}
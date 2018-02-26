//Author: Igor Tryhub, 275235
/*
 * Zadanie 6.
 * Przypuśćmy, że istnieją trzy wątki palaczy i wątek agenta. Zrobienie i 
 * zapalenie papierosa wymaga posiadania tytoniu, bibułki i zapałek.  Każdy  
 * palacz  posiada  nieskończoną  ilość  wyłącznie  jednego zasobu – 
 * tj. pierwszy ma tytoń, drugi bibułki, a trzeci zapałki. Agent kładzie na 
 * stole dwa wylosowane składniki. Palacz, który ma brakujący składnik podnosi 
 * ze stołu resztę, skręca papierosa i go zapala. Agent czeka, aż palacz 
 * zacznie palić po czym powtarza akcję.
 * Używając semaforów z zadania pierwszego zaimplementuj wątki agenta i 
 * palaczy, tak aby spełniały podane  wyżej  założenia. Palacze mają  być  
 * wybudzani  tylko  wtedy,  gdy  pojawią  się  dokładnie  dwa zasoby, których 
 * dany palacz potrzebuje.
 */

#include "semaphores.h"
#include "smokers.h"

sem_t smoke;
sem_t tobacco;
sem_t paper;
sem_t matches;

int has_tob = 0; //true if smoker3_mat() has acquired tobacco
int has_pap = 0; //true if smoker1_tob() has acquired paper
int has_mat = 0; //true if smoker2_pap() has acquired matches


pthread_t thread_id[4];
void *res;
int sval;


void agent(void){
  printf("executing agent\n");
  while(1){
      printf("	in agent...\n");
      sem_wait(&smoke);

      //sleep(1);
      
      int x = rand() % 3;
      if(x==0){ //waking up smoker3_mat()
	sem_post(&tobacco);
	sem_post(&paper);
      }
      if(x==1){ //waking up smoker2_pap()
	sem_post(&matches);
	sem_post(&tobacco);
      }
      if(x==2){ //waking up smoker1_tob()
	sem_post(&paper);
	sem_post(&matches);
    }
  }
}

void smoker1_tob(void){
  printf("executing smoker1_tob\n");
  while(1){
    sem_wait(&paper);
    has_pap = 1;
    printf("has_pap=1\n");
    if(!has_mat && has_tob){
      printf("!has_mat && has_tob\n");
      sem_post(&paper);
      has_pap = 0;
      continue;
    }
    sem_wait(&matches);
    has_pap = 0;
    printf("	smoker1 smoking...\n");
    sem_post(&smoke);
  }
}

void smoker2_pap(void){
  printf("executing smoker2_tob\n");
  while(1){
    sem_wait(&matches);
    has_mat = 1;
    printf("has_mat=1\n");
    if(!has_tob && has_pap){
      printf("!has_tob && has_pap\n");
      sem_post(&tobacco);
      has_mat = 0;
      continue;
    }
    sem_wait(&tobacco);
    has_mat = 0;
    printf("	smoker2 smoking...\n");
    sem_post(&smoke);
  }
}

void smoker3_mat(void){
  printf("executing smoker3_tob\n");
  while(1){
    sem_wait(&tobacco);
    has_tob = 1;
    printf("has_tob=1\n");
    if(!has_pap && has_mat){
      printf("!has_pap && has_mat\n");
      sem_post(&tobacco);
      has_tob = 0;
      continue;
    }
    sem_wait(&paper);
    has_tob = 0;
    printf("	smoker3 smoking...\n");
    sem_post(&smoke);
  }
}

int main(void){
  
  sem_init(&smoke, 3);
  sem_init(&tobacco, 0);
  sem_init(&paper, 0);
  sem_init(&matches, 0);
  

  
  /*
  for(int tnum=0; tnum < 4; tnum++){
    if(pthread_create(&thread_id[tnum], NULL, void * (*start_routine)(void *), NULL))
      handle_error("pthread_create");
  }
  
  for(int tnum=0; tnum < 4; tnum++){
    if(pthread_join(&thread_id[tnum], res))
      handle_error("pthread_join");
  }
  */
  if(pthread_create(&thread_id[0], NULL, (void *)&agent, NULL))
      handle_error("pthread_create");
  if(pthread_create(&thread_id[1], NULL, (void *)&smoker1_tob, NULL))
      handle_error("pthread_create");
  if(pthread_create(&thread_id[2], NULL, (void *)&smoker2_pap, NULL))
      handle_error("pthread_create");
  if(pthread_create(&thread_id[3], NULL, (void *)&smoker3_mat, NULL))
      handle_error("pthread_create");
  
  if(pthread_join(thread_id[0], res))
      handle_error("pthread_join");
  if(pthread_join(thread_id[1], res))
      handle_error("pthread_join");
  if(pthread_join(thread_id[2], res))
      handle_error("pthread_join");
  if(pthread_join(thread_id[3], res))
      handle_error("pthread_join");

  
  return 0;
}

//=================================================================================================================

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

#include <unistd.h>

void agent(void);
void smoker1_tob(void);
void smoker2_pap(void);
void smoker3_mat(void);

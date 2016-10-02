#ifndef TWATCHER_H_INCLUDED
#define TWATCHER_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

#define LICZBA_PRZEDMIOTOW 10
#define LICZBA_AKTYWNOSCI 7
#define FLUSH fflush(stdin); //probowalem uzywac "while(getchar()!='\n');", jak Pan polecal, ale niepoprawnie dzialalo,
                            //bo potrzebowalo ENTER'a z klawiatury, co nie kest wygodne dla uzytkownika

struct tm czas_from;
struct tm czas_till;
typedef struct przedmioty
{
    unsigned char przedm_1;
    unsigned char przedm_2;
    unsigned char przedm_3;
    unsigned char przedm_4;
    unsigned char przedm_5;
    unsigned char przedm_6;
    unsigned char przedm_7;
    unsigned char przedm_8;
    unsigned char przedm_9;
    unsigned char przedm_10;
} Przedmioty;
typedef struct oceny
{
    unsigned ocena_1: 3;
    unsigned ocena_2: 3;
    unsigned ocena_3: 3;
    unsigned ocena_4: 3;
    unsigned ocena_5: 3;
    unsigned ocena_6: 3;
    unsigned ocena_7: 3;
    unsigned ocena_8: 3;
    unsigned ocena_9: 3;
    unsigned ocena_10: 3;
} Oceny;
typedef struct wezel
{
    unsigned id_przedmiotu;
    struct wezel *nast;
} Wezel;

char przedmiot[75];
unsigned id_przedmiotu;
unsigned id_oceny;
unsigned long int czas [LICZBA_PRZEDMIOTOW][LICZBA_AKTYWNOSCI];
unsigned long int ostatni_czas [LICZBA_PRZEDMIOTOW][LICZBA_AKTYWNOSCI];
unsigned logowanie (void);
int szukaj_imie (const unsigned nr_albumu);
int nowy_student (const unsigned nr_albumu, char *nazwisko, char *imie, bool plec);
int wybierz_przedmiot (const unsigned nr_albumu, const unsigned num_przedm, const bool usuniecie,const bool plec);
int szukaj_przedmiot (const char *napis, Wezel **lista);
int zapisz_przedmiot (const unsigned nr_albumu, unsigned id_przedmiotu, char *przedmiot, int p);
char* zwroc_nazwe (unsigned char id_przedmiotu);
int wybierz_aktualne (const unsigned nr_albumu, Przedmioty subjects, bool plec);
int zapisz_czas (const unsigned nr_albumu, const unsigned akt_przedm, const unsigned akt_aktywn, const unsigned num_przedm);
int zmiana_przedmiotow (const unsigned nr_albumu, Przedmioty subjects, bool plec);
int do_studenci (const unsigned nr_albumu, const unsigned akt_aktywn, const unsigned num_przedm, const unsigned rezsec);
void wyswietl_przedmioty (Przedmioty subjects);
int sprawdz_imie (const char *imie);
int pokaz_menu (const unsigned nr_albumu, Przedmioty subjects, const bool plec, const char *imie);
int z_studenci (const unsigned nr_albumu, Przedmioty *subjects);
int do_czasy (const unsigned nr_albumu, const unsigned akt_przedm, const unsigned akt_aktywn, const struct tm czas_from, const struct tm czas_till);
int ost_studenci (const unsigned nr_albumu, unsigned long int  *ost);
int aktualizacja (const unsigned nr_albumu, const bool plec, const unsigned akt_przedm, const unsigned akt_aktywn, const unsigned num_przedm, const char tryb);
int wyswietl_statystyke (const unsigned nr_albumu);

#endif // TWATCHER_H_INCLUDED

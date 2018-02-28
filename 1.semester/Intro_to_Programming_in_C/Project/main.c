#include "..\twatcher.h"

/*****************************************************************************************/
int main()
{
    unsigned nr_albumu;
    printf ("\t\t\"TimeWatcher Student v.1.0\" by Igor Tryhub\n\n");
    nr_albumu = logowanie();
    szukaj_imie (nr_albumu);


    return 0;
}
/*****************************************************************************************/
unsigned logowanie(void) //generujemy poprawny nr_albumu
{
    printf ("Podaj swoj numer albumu: ");
    char n[7];
    bool blad;
    unsigned nr_albumu=0;
    int pow;
    while (!nr_albumu)
    {
        nr_albumu=0;
        pow=1;
        blad = 0;
        for (int i = 0; i<6; ++i)
        {
            n[i]=getchar();
            if (!isdigit(n[i]))
            {
                blad=1;
                break;
            }
        }
        if (!blad)
        {
            if(!isspace(n[6]=getchar()))
            {
                blad=1;
            }
        }
        if (blad)
        {
            printf ("Blad wpisywania. Sproboj jeszcze raz: ");
            FLUSH
            continue;
        }
        for (int i = 0; i<6; ++i)
        {
            nr_albumu += (unsigned)(n[5-i]-'0')*pow;
            pow *= 10;
        }
    }
    return nr_albumu;
}
/*****************************************************************************************/
int szukaj_imie (const unsigned nr_albumu) //szukamy czy osoba juz jest zarejestrowana
{
    FILE *plik;
    char nazwisko[30];
    char imie[30];
    bool plec; // (0=K, 1=M)
    unsigned album;
    bool eof = 1; // (0=koniec pliku; 1=nie);
    Przedmioty subjects;
                                                                                            /**ODCZYTUJEMY DANE Z PLIKU "OSOBOWE.DAT"**/
    if ((plik = fopen("..\\osobowe.dat", "rb"))==NULL) // plik jeszcze nie istnieje
    {
        plik = fopen("..\\osobowe.dat", "wb");
        fclose(plik);
        plik = fopen("..\\osobowe.dat", "rb");
    }
    for (int i = 0; ; ++i) // plik juz istnieje
    {
        if (fseek (plik,(i==0?0:(sizeof(unsigned)+30+30+sizeof(bool))),(i==0?SEEK_SET:SEEK_CUR)))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        eof = fread (&album, sizeof(unsigned),1,plik);
        fseek (plik,-sizeof(unsigned), SEEK_CUR);
        if (!eof)                                                                           /**osoba NIE jest zarejestrowana**/
        {
            if (nowy_student(nr_albumu, nazwisko, imie, plec))
            {
                printf("Blad rejestracji nowego studenta.");
                exit (1);
            }
            break;
        }
        else if (album == nr_albumu)                                                        /**osoba JEST zarejestrowana**/
        {                                                                                   /**pobieramy jej dane osobowe**/
            fseek (plik, sizeof(unsigned), SEEK_CUR);
            fread (nazwisko,30,1,plik);
            fread (imie,30,1,plik);
            fread (&plec,sizeof(bool),1,plik);
            fclose (plik);
            break;
        }
    }
    printf ("\n\t\t\tWitaj, %s %s!\n", imie, nazwisko);

    z_studenci (nr_albumu, &subjects);
    pokaz_menu (nr_albumu, subjects, plec, imie);
    return 0;
}
/*****************************************************************************************/
int nowy_student (const unsigned nr_albumu, char *nazwisko, char *imie, bool plec) //rejestrujemy nowego studenta
{
    FILE *plik;
                                                                                            /**ZAPISUJEMY DANE DO PLIKU "OSOBOWE.DAT"**/
    char sex = 0;
    plik = fopen("..\\osobowe.dat", "r+b");
    if (fseek (plik,0,SEEK_END))
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    printf ("\nStudent z numerem albumu %u w bazie nie istnieje.\n"\
            "Dla rejestracji w systemie wpisz swoje dane (nie uzywamy polskich znakow):\n\n", nr_albumu);
    printf ("\tNazwisko: ");
    scanf ("%s", nazwisko);
    while (!sprawdz_imie (nazwisko))
    {
        printf ("\nBlad: uzyles niedopuszczalnego symbolu. Sprobuj jeszcze raz. \n\n");
        printf ("\tNazwisko: ");
        scanf ("%s", nazwisko);
    }
    printf ("\tImie: ");
    scanf ("%s", imie);
    while (!sprawdz_imie (imie))
    {
        printf ("Blad: uzyles niedopuszczalnego symbolu. Sprobuj jeszcze raz. \n\n");
        printf ("\tImie: ");
        scanf ("%s", imie);
    }
    nazwisko[0]=toupper(nazwisko[0]);
    imie[0]=toupper(imie[0]);
    for (int i=1; i<29 && (nazwisko[i] || imie[i]); ++i)
    {
        nazwisko[i]=tolower(nazwisko[i]);
        imie[i]=tolower(imie[i]);
    }
    printf ("\tPlec (M lub K): ");
    FLUSH
    sex = tolower(getchar());
    if ((sex!='m')&&(sex!='k'))
    {
        do
        {
            printf ("\nBlad: nieoczekiwany symbol. Sproboj ponownie.\n\n");
            printf ("\tPlec (M lub K): ");
            FLUSH
            sex = tolower(getchar());
        } while ((sex!='m')&&(sex!='k'));
    }
    plec = (sex == 'm' ? 1 : 0);

    if (!fwrite (&nr_albumu, sizeof(unsigned), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (nazwisko, 30, 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (imie, 30, 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&plec, sizeof(bool), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    fclose (plik);
                                                                                            /**INICJULIZUJEMY DANE W PLIKU "STUDENCI.DAT"**/
    bool status = 1;
    Przedmioty subjects = {0};
    unsigned long int czas[LICZBA_PRZEDMIOTOW][LICZBA_AKTYWNOSCI]={{0}};
    unsigned int ostatni_czas[LICZBA_PRZEDMIOTOW][LICZBA_AKTYWNOSCI]={{0}};
    Oceny marks = {0};

    plik = fopen("..\\studenci.dat", "ab");
    if (!fwrite (&nr_albumu, sizeof(unsigned), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&status, sizeof(bool), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&subjects, sizeof(Przedmioty), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&czas, sizeof(czas), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&ostatni_czas, sizeof(ostatni_czas), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&marks, sizeof(Oceny), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    fclose (plik);

    if (wybierz_przedmiot(nr_albumu, 0, 0, plec))
    {
        printf ("Blad funkcji zapisu przedmiotow do bazy");
        exit (1);
    }
    return 0;
}
/*****************************************************************************************/
int wybierz_przedmiot(const unsigned nr_albumu, const unsigned num_przedm, const bool usuniecie, const bool plec) //wybieramy nowy przedmiot dla studenta
{
    FILE *plik;
    bool eof = 1; // (0=koniec pliku; 1=nie);
    bool eof2 = 1; // (0=koniec pliku; 1=nie);
    unsigned subject;
    bool juz_istnieje = 0;
    Przedmioty subjects;
    int p;
    unsigned album;
    Wezel *lista = NULL;
    Wezel *pom;
    bool z_listy = 0;

    if (usuniecie)
    {
        p = num_przedm-1;
        if (zapisz_przedmiot(nr_albumu, 0, 0, p))
        {
            printf ("Przedmiotu nie usunieto.");
            exit (1);
        }
        return 0;
    }

    do
    {
        printf ("\nWpisz dowolne slowo (min 5 symboli) z nazwy kazdego z przedmiotow,\nktore wybrales."\
            " Dla wyjscia do glownego menu wcisnij '0': \n");
        if(!num_przedm)
        {
            printf ("\nPrzedmiot 1: ");
        }
        else
        {
            printf ("\nPrzedmiot %u: ", num_przedm);
        }
        bool ostatni = 0;
        for (p = 0; p<9 && !ostatni; ++p)
        {
            if (num_przedm)                                                                     /**sprawdzamy: tryb inicjalizacji czy redagowania**/
            {
                p = num_przedm-1;
            }
            bool wybrano = 0;
            bool poprawny = 0;
            char napis[75];
            while (!poprawny)                                                                   /**sprawdzamy poprawnosc wprowadzonego napisu**/
            {
                FLUSH
                scanf ("%74s", napis);
                if (napis[0]=='0')
                {
                    ostatni = 1;
                    break;
                }
                for (int i = 0; i<5; ++i)
                {
                    if (napis[i]==0)
                    {
                        printf ("\nZa krotki napis!\nWpisz co najmniej 5 symboli z nazwy przedmiotu lub '0' dla zakonczenia: ");
                        break;
                    }
                    if (i==4)
                    {
                        poprawny = 1;
                    }
                }
            }
            if (poprawny)
            {
                if (szukaj_przedmiot (napis, &lista))
                {
                    printf ("Przedmiotu o nazwie w ktorej wystepuje napis '%s' nie znaleziono.\nSproboj jeszcze raz.", napis);
                    p=p-1;
                }

                else                                                                            /**znaleziono co najmniej 1 przedmiot z takim napisem**/
                {
                    while (!wybrano)
                    {
                        printf ("\nWybierz z powyzszej listy wlasciwy przedmiot i podaj jego numer\n\tdla dokonania wyboru: ", napis);
                        FLUSH
                        char n[6];
                        bool blad;
                        int pow;
                        id_przedmiotu=0;
                        while (!id_przedmiotu)                                                   /**generujemy poprawne ID przedmiotu**/
                        {
                            id_przedmiotu=0;
                            pow=1;
                            blad = 0;
                            int i;
                            for (i = 0; i<6; ++i)
                            {
                                n[i]=getchar();
                                if (n[i]=='\n')
                                {
                                    break;
                                }
                                if (!isdigit(n[i]))
                                {
                                    blad=1;
                                    break;
                                }
                            }
                            if (blad)
                            {
                                printf ("Blad wpisywania. Sproboj jeszcze raz: ");
                                FLUSH
                                continue;
                            }
                            for (int j = 0; j<i; ++j)
                            {
                                id_przedmiotu += (unsigned)(n[i-j-1]-'0')*pow;
                                pow *= 10;
                            }
                        }
                                                                                                 /**sprawdzamy liste wynikow wyszukiwania**/
                        pom = lista;
                        while (pom)
                        {
                            if (id_przedmiotu == pom->id_przedmiotu)
                            {
                                z_listy = 1;
                            }
                            pom = pom->nast;
                        }
                        if (!z_listy)
                        {
                            printf ("\nBlad: przedmiotu z tym numerem nie ma w wynikach wyszukiwania!\nSproboj jeszcze raz.\n\n");
                            continue;
                        }
                        else
                        {
                            while (lista)
                            {
                                pom = lista;
                                lista = lista->nast;
                                free (pom);
                            }
                        }
                                                                                                    /**sprawdzamy czy student juz jest zapisany na ten przedmiot**/
                        if((plik = fopen("..\\studenci.dat", "rb"))==NULL)
                        {
                            printf ("Blad szukania danych w bazie.");
                            exit(1);
                        }
                        for (int i = 0; ; ++i) // plik juz istnieje
                        {
                            if (fseek (plik,(i==0?0:(sizeof(unsigned)+sizeof(bool)+sizeof(Przedmioty)+sizeof(czas)+sizeof(ostatni_czas)+sizeof(Oceny))),(i==0?SEEK_SET:SEEK_CUR)))
                            {
                                printf ("Blad szukania danych w bazie.");
                                exit(1);
                            }
                            eof2 = fread (&album, sizeof(unsigned),1,plik);
                            fseek (plik,-sizeof(unsigned), SEEK_CUR);
                            if (!eof2) //jezeli nie ma - blad, bo musi byc
                            {
                                printf ("Blad szukania danych w bazie.");
                                exit(1);
                            }
                            if (album == nr_albumu) //jezeli istnieje - kopiujemy do tymczasowej zmiennej i zmieniamy potrzebny przedmiot
                            {
                                fseek (plik, sizeof(unsigned)+sizeof(bool), SEEK_CUR);
                                fread (&subjects,sizeof(Przedmioty),1,plik);
                                fclose(plik);
                                break;
                            }
                        }
                        if (id_przedmiotu==(unsigned)subjects.przedm_1 || id_przedmiotu==(unsigned)subjects.przedm_2 || id_przedmiotu==(unsigned)subjects.przedm_3\
                             || id_przedmiotu==(unsigned)subjects.przedm_4 || id_przedmiotu==(unsigned)subjects.przedm_5 || id_przedmiotu==(unsigned)subjects.przedm_6\
                             || id_przedmiotu==(unsigned)subjects.przedm_7 || id_przedmiotu==(unsigned)subjects.przedm_8 || id_przedmiotu==(unsigned)subjects.przedm_9\
                             || id_przedmiotu==(unsigned)subjects.przedm_10)
                        {
                            printf ("\nBlad: juz jestes zapisan%c na ten przedmiot! Sproboj jeszcze raz.\n", plec==1?'y':'a');
                            juz_istnieje = 1;
                            break;
                        }
                        else
                        {
                            juz_istnieje = 0;
                        }
                                                                                                    /**jezeli jeszcze nie zapisany, szukamy przedmiot z takim ID**/
                        if ((plik = fopen("..\\przedmioty.dat", "rb"))==NULL) // plik nie istnieje
                        {
                            printf ("Blad szukania danych w bazie.");
                            exit(1);
                        }
                        for (int i = 0; ; ++i)
                        {
                            bool znaleziono = 0;
                            if (fseek (plik,(i==0?0:(sizeof(unsigned)+75)),(i==0?SEEK_SET:SEEK_CUR)))
                            {
                                printf ("Blad szukania danych w bazie.");
                                exit(1);
                            }
                            eof = fread (&subject,sizeof(unsigned),1,plik);
                            if (!eof) // nie znaleziono (jesli nie udalo sie przeczytac)
                            {
                                fclose (plik);
                                printf ("\nPrzedmiotu o podanym numerze nie istnieje.\nSproboj jeszcze raz.\n");
                                wybrano = 0;
                                break;
                            }
                            if (id_przedmiotu == subject)
                            {
                                znaleziono = 1;
                            }
                            if (znaleziono)
                            {
                                fread (przedmiot,75,1,plik);
                                fclose (plik);
                                if ((plik = fopen("..\\studenci.dat", "rb"))==NULL) // plik jeszcze nie istnieje
                                {
                                    plik = fopen("..\\studenci.dat", "wb");
                                }
                                fclose(plik);
                                if (zapisz_przedmiot(nr_albumu, id_przedmiotu, przedmiot, p))
                                {
                                    printf ("Przedmiotu nie zapisano.");
                                    exit (1);
                                }
                                wybrano = 1;
                                break;
                            }
                            fseek (plik,-(sizeof(unsigned)), SEEK_CUR);
                        }
                    }
                }
            }
            if (juz_istnieje || (wybrano && num_przedm))                                            /**sprawdzamy: tryb inicjalizacji czy redagowania**/
            {
                break;
            }
            if (!ostatni)
            {
                printf ("\nPrzedmiot %d: ", p+2);
            }
        }
    } while (juz_istnieje);

    return 0;
}
/*****************************************************************************************/
int szukaj_przedmiot (const char* napis, Wezel **lista) //szukamy przedmioty o podanym napisie i wyswietlamy je
{
    FILE *plik;
    bool eof = 1; // (0=koniec pliku; 1=nie)
    bool czy_success = 0;
    bool znaleziono = 0;
    bool koniec; // (1=koniec nazwy; 0=nie)
    int k; // numer litery nazwy przedmiotu
    Wezel *pom;


    if ((plik = fopen("..\\przedmioty.dat", "rb"))==NULL) // plik nie istnieje
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    if (fseek (plik,sizeof(unsigned),SEEK_SET))
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    for (int i = 0; ; ++i)                                                                  /**przeszukiwanie na liscie przedmiotow**/
    {

        if (fseek (plik,(i==0?0:(sizeof(unsigned)+75)),SEEK_CUR))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        eof = fread (przedmiot,75,1,plik);
        if (!eof) // nie znaleziono (jesli nie udalo sie przeczytac)
        {
            fclose (plik);
            if (czy_success)
            {
                return 0;
            }
            return 1;
        }
        fseek (plik,-75, SEEK_CUR);
        koniec = 0;
        k=0;
        while (!znaleziono && !koniec)                                                      /**przeszukiwanie w srodku nazwy przedmiotu**/
        {
            while (k < 75 && (tolower(napis[0]) != tolower(przedmiot[k])))
            {
                k++;
            }
            if (k==75)
            {
                koniec = 1;
                break;
            }
            for (int j = 0; k+j<75; j++)
            {
                if (napis[j] == 0)
                {
                    znaleziono = 1;
                    czy_success = 1;
                    break;
                }
                if (k+j == 74)
                {
                    koniec = 1;
                    break;
                }
                if (tolower(napis[j]) != tolower(przedmiot[k+j]))
                {
                    k++;
                    break;
                }
            }
        }
        if (znaleziono)
        {
            fseek (plik, -sizeof(unsigned), SEEK_CUR);
            fread (&id_przedmiotu,sizeof(unsigned),1,plik);
                                                                                            /**zapamietujemy wyniki wyszukiwania**/
            if (!*lista)
            {
                if((*lista = (Wezel*)malloc(sizeof(Wezel)))==NULL)
                {
                    printf ("Blad przydzielania pamieci.");
                    exit(1);
                }
                (*lista)->id_przedmiotu = id_przedmiotu;
                (*lista)->nast = NULL;
            }
            else
            {
                pom = *lista;
                while (pom->nast)
                {
                    pom = pom->nast;
                }
                if((pom->nast = (Wezel*)malloc(sizeof(Wezel)))==NULL)
                {
                    printf ("Blad przydzielania pamieci.");
                    exit(1);
                }
                pom->nast->id_przedmiotu = id_przedmiotu;
                pom->nast->nast = NULL;
            }

            printf ("\t#%s%u ", id_przedmiotu<10 ? "00" : id_przedmiotu<100 ? "0" :"",id_przedmiotu);
            printf ("%s\n", przedmiot);
            znaleziono = 0;
        }
    }
}
/*****************************************************************************************/
int zapisz_przedmiot(const unsigned nr_albumu, unsigned id_przedmiotu, char *przedmiot, int p) //zapisujemy (edytujemy) przedmioty wybrane przez studenta
{
    unsigned album;
    FILE *plik;
    Przedmioty subjects;

    if((plik = fopen("..\\studenci.dat", "r+b"))==NULL)
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
                                                                                            /**szukamy studenta**/
    bool eof = 1; // (0=koniec pliku; 1=nie);
    for (int i = 0; ; ++i) // plik juz istnieje
    {
        if (fseek (plik,(i==0?0:(sizeof(unsigned)+sizeof(bool)+sizeof(Przedmioty)+sizeof(czas)+sizeof(ostatni_czas)+sizeof(Oceny))),(i==0?SEEK_SET:SEEK_CUR)))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        eof = fread (&album, sizeof(unsigned),1,plik);
        fseek (plik,-sizeof(unsigned), SEEK_CUR);
        if (!eof) //jezeli nie ma - blad, bo musi byc
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        if (album == nr_albumu) //jezeli istnieje - kopiujemy do tymczasowej zmiennej i zmieniamy potrzebny przedmiot
        {
            fseek (plik, sizeof(unsigned)+sizeof(bool), SEEK_CUR);
            fread (&subjects,sizeof(Przedmioty),1,plik);
            fseek (plik, -sizeof(Przedmioty), SEEK_CUR);
            switch (p)
            {
                case 0:
                {
                    subjects.przedm_1 = (unsigned char)id_przedmiotu;
                    break;
                }
                case 1:
                {
                    subjects.przedm_2 = (unsigned char)id_przedmiotu;
                    break;
                }
                case 2:
                {
                    subjects.przedm_3 = (unsigned char)id_przedmiotu;
                    break;
                }
                case 3:
                {
                    subjects.przedm_4 = (unsigned char)id_przedmiotu;
                    break;
                }
                case 4:
                {
                    subjects.przedm_5 = (unsigned char)id_przedmiotu;
                    break;
                }
                case 5:
                {
                    subjects.przedm_6 = (unsigned char)id_przedmiotu;
                    break;
                }
                case 6:
                {
                    subjects.przedm_7 = (unsigned char)id_przedmiotu;
                    break;
                }
                case 7:
                {
                    subjects.przedm_8 = (unsigned char)id_przedmiotu;
                    break;
                }
                case 8:
                {
                    subjects.przedm_9 = (unsigned char)id_przedmiotu;
                    break;
                }
                case 9:
                {
                    subjects.przedm_10 = (unsigned char)id_przedmiotu;
                    break;
                }

            }
            if (!fwrite (&subjects, sizeof(Przedmioty), 1, plik))
            {
                printf ("Blad zapisu danych do bazy");
                exit (1);
            }
            if (przedmiot)
            {
                printf ("\nPrzedmiot \"%s\" \n\tzostal domyslnie zapisany na pozycje %d.\n", przedmiot, p+1);
            }
            fclose (plik);
            return 0;
        }
    }

    return 1;
}
/*****************************************************************************************/
char* zwroc_nazwe (unsigned char id_przedmiotu) //zwraca nazwe przedmiotu dla jego ID
{
    FILE *plik;
    bool eof = 1;
    char *nazwa;
    unsigned subject;
    bool znaleziono = 0;
    if ((plik = fopen("..\\przedmioty.dat", "rb"))==NULL) // plik nie istnieje
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    for (int i = 0; ; ++i) // plik juz istnieje
    {

        if (fseek (plik,(i==0?0:(sizeof(unsigned)+75)),(i==0?SEEK_SET:SEEK_CUR)))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        eof = fread (&subject,sizeof(unsigned),1,plik);
        if (!eof) // nie znaleziono (jesli nie udalo sie przeczytac)
        {
            fclose (plik);
            printf ("\nPrzedmiotu o podanym numerze nie istnieje.\n\n");
            exit(1);
        }
        if ((unsigned)id_przedmiotu == subject)
        {
            znaleziono = 1;
        }
        if (znaleziono)
        {
            if((nazwa = (char*)malloc(sizeof (char)*75))==NULL)
            {
                printf ("Blad przydzielania pamieci.");
                exit(1);
            }
            fread (nazwa,75,1,plik);
            fclose (plik);
            break;
        }
        fseek (plik,-(sizeof(unsigned)), SEEK_CUR);
    }
    return nazwa;
}
/*****************************************************************************************/
int wybierz_aktualne (const unsigned nr_albumu, Przedmioty subjects, const bool plec) //wybieramy przedmiot i aktywnosc dla rejestracji poswiaconego czasu
{
    unsigned akt_przedm;
    unsigned num_przedm;
    bool wlasciwy = 0;
    FILE *plik;
    char aktywnosc[35];
    char tryb;
    char tryb2;

    while (!wlasciwy)                                                                       /**wybieramy przedmiot**/
    {
        printf ("\nPodaj numer przedmiotu, ktorego w tym momencie sie uczysz: ");
        while (1)
        {
            FLUSH
            scanf ("%u", &num_przedm);
            if (0<num_przedm && num_przedm<11)
            {
                break;
            }
            printf ("\nNiewlasciwy numer. Podaj liczbe w zakresie od 1 do 10: ");
        }
        switch (num_przedm)
        {
            case 1:
            {
                if((akt_przedm = subjects.przedm_1))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("Przedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
            case 2:
            {
                if((akt_przedm = subjects.przedm_2))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("\nPrzedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
            case 3:
            {
                if((akt_przedm = subjects.przedm_3))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("\nPrzedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
            case 4:
            {
                if((akt_przedm = subjects.przedm_4))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("\nPrzedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
            case 5:
            {
                if((akt_przedm = subjects.przedm_5))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("\nPrzedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
            case 6:
            {
                if((akt_przedm = subjects.przedm_6))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("\nPrzedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
            case 7:
            {
                if((akt_przedm = subjects.przedm_7))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("\nPrzedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
            case 8:
            {
                if((akt_przedm = subjects.przedm_8))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("\nPrzedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
            case 9:
            {
                if((akt_przedm = subjects.przedm_9))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("\nPrzedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
            case 10:
            {
                if((akt_przedm = subjects.przedm_10))
                {
                    wlasciwy = 1;
                }
                else
                {
                    printf ("\nPrzedmiot pod tym numerem nie istnieje.\n");
                }
                break;
            }
        }
    }
    char *m = zwroc_nazwe(akt_przedm);
    printf ("\nWybral%cs przedmiot \"%s\".\n", plec==1?'e':'a', m);
    free(m);
                                                                                            /**wybieramy aktywnosc**/
    printf ("\n\tMozliwe aktywnosci: \n");

    if ((plik = fopen("..\\aktywnosci.dat", "rb"))==NULL) // plik nie istnieje
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    for (int i = 0; i<LICZBA_AKTYWNOSCI; ++i) // plik juz istnieje
    {
        if (fseek (plik,sizeof(unsigned),i==0?SEEK_SET:SEEK_CUR))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        fread (aktywnosc,35,1,plik);
        printf ("#%u %s\n", i+1, aktywnosc);
    }
    fclose (plik);
    printf ("\nWybierz numer uprawianej aktywnosci dla tego przedmiotu: ");
    unsigned akt_aktywn;
    unsigned num_aktywn;
    while (1)
    {
        FLUSH
        scanf ("%u", &num_aktywn);
        if (0<num_aktywn && num_aktywn<8)
        {
            break;
        }
        printf ("\nNiewlasciwy numer. Podaj liczbe w zakresie od 1 do 7: ");
    }
    akt_aktywn = num_aktywn-1;
    printf ("\nWybral%cs aktywnosc ", plec==1?'e':'a');
    switch (num_aktywn)
    {
        case 1:
        {
            printf("\"Czytanie podrecznika\".\n");
            break;
        }
        case 2:
        {
            printf("\"Wyszukiwanie w Internecie\".\n");
            break;
        }
        case 3:
        {
            printf("\"Ogladanie wideomaterialow\".\n");
            break;
        }
        case 4:
        {
            printf("\"Rozwiazywanie zadan\".\n");
            break;
        }
        case 5:
        {
            printf("\"Programowianie\".\n");
            break;
        }
        case 6:
        {
            printf("\"Interakcja z kolegami\".\n");
            break;
        }
        case 7:
        {
            printf("\"Inne aktywnosci\".\n");
            break;
        }
    }
    num_przedm--;

    printf("\n\tWcisnij 'ENTER' dla uruchamiania timeru");
    printf("\n\tWcisnij 'W'+'ENTER' dla wpisywania czasu od reki\n");
    while (1)
    {
        FLUSH
        tryb = getchar();
        switch (tryb)
        {
            case '\n':
            {
                zapisz_czas (nr_albumu, akt_przedm, akt_aktywn, num_przedm);
                break;
            }
            /*case 'T':
            {
                zapisz_czas (nr_albumu, akt_przedm, akt_aktywn, num_przedm);
                break;
            }*/
            case 'w':
            {
                printf ("\nWybral%cs tryb wpisywania czasu od reki.\n", plec==1?'e':'a');
                printf("\n\tWcisnij 'N' dla wpisywania nowego czasu");
                printf("\n\tWcisnij 'A' dla aktualizacji zeszlego wpisu\n");
                printf("\t\t\t\t\t\t...oraz wcisnij ENTER: ");
                while (1)
                {
                    FLUSH
                    tryb2 = getchar();
                    switch (tryb2)
                    {
                        case 'n':
                        {
                            aktualizacja (nr_albumu, plec, akt_przedm, akt_aktywn, num_przedm, tryb2);
                            break;
                        }
                        case 'N':
                        {
                            aktualizacja (nr_albumu, plec, akt_przedm, akt_aktywn, num_przedm, tryb2);
                            break;
                        }
                        case 'a':
                        {
                            aktualizacja (nr_albumu, plec, akt_przedm, akt_aktywn, num_przedm, tryb2);
                            break;
                        }
                        case 'A':
                        {
                            aktualizacja (nr_albumu, plec, akt_przedm, akt_aktywn, num_przedm, tryb2);
                            break;
                        }
                        default:
                        {
                            printf ("\nNiewlasciwy symbol. Podaj litere 'N' lub 'A' i wcisnij ENTER: ");
                            continue;
                        }
                    }
                    break;
                }
                break;
            }
            case 'W':
            {
                printf ("\nWybral%cs tryb wpisywania czasu od reki.\n", plec==1?'e':'a');
                printf("\n\tWcisnij 'N' dla wpisywania nowego czasu");
                printf("\n\tWcisnij 'A' dla aktualizacji zeszlego wpisu\n");
                printf("\t\t\t\t\t\t...oraz wcisnij ENTER: ");
                while (1)
                {
                    FLUSH
                    tryb2 = getchar();
                    switch (tryb2)
                    {
                        case 'n':
                        {
                            aktualizacja (nr_albumu, plec, akt_przedm, akt_aktywn, num_przedm, tryb2);
                            break;
                        }
                        case 'N':
                        {
                            aktualizacja (nr_albumu, plec, akt_przedm, akt_aktywn, num_przedm, tryb2);
                            break;
                        }
                        case 'a':
                        {
                            aktualizacja (nr_albumu, plec, akt_przedm, akt_aktywn, num_przedm, tryb2);
                            break;
                        }
                        case 'A':
                        {
                            aktualizacja (nr_albumu, plec, akt_przedm, akt_aktywn, num_przedm, tryb2);
                            break;
                        }
                        default:
                        {
                            printf ("\nNiewlasciwy symbol. Podaj litere 'N' lub 'A' i wcisnij ENTER: ");
                            continue;
                        }
                    }
                    break;
                }
                break;
            }
            default:
            {
                printf ("\nNiewlasciwy symbol. Wcisnij 'ENTER' lub 'W'+'ENTER'\n");
                continue;
            }
        }
        break;
    }

    return 0;
}
/*****************************************************************************************/
int zapisz_czas (const unsigned nr_albumu, const unsigned akt_przedm, const unsigned akt_aktywn, const unsigned num_przedm) //zapisujemy poswiecony czas do bazy
{
    time_t rawtime;
    struct tm czas_from;
    struct tm czas_till;
    clock_t beg;
    clock_t end;
    unsigned rezsec;

    if ((beg=clock())==-1)                                                                  /**operacje na czasach**/
    {
        printf ("Blad operacji na czasach.");
        exit(1);
    }
    time (&rawtime);
    czas_from = *(localtime (&rawtime));
    printf ("\n* * * * * * * * * * Odliczenie czasu rozpoczeto o %s%d:%s%d:%s%d. * * * * * * * * * *\n\n\t      Dla zakonczenia odliczenia wcisnij klawisz ENTER...\n", \
            czas_from.tm_hour<10?"0":"", czas_from.tm_hour, czas_from.tm_min<10?"0":"", czas_from.tm_min, czas_from.tm_sec<10?"0":"", czas_from.tm_sec);
    FLUSH
    if (getchar())
    {
        if ((end=clock())==-1)
        {
            printf ("Blad operacji na czasach.");
            exit(1);
        }
    }
    rezsec = (end-beg)/CLOCKS_PER_SEC;
    time (&rawtime);
    czas_till = *(localtime (&rawtime));
    printf ("* * * * * * * * * * Odliczenie czasu zakonczono o %s%d:%s%d:%s%d. * * * * * * * * * *\n\n", \
            czas_till.tm_hour<10?"0":"", czas_till.tm_hour, czas_till.tm_min<10?"0":"", czas_till.tm_min, czas_till.tm_sec<10?"0":"", czas_till.tm_sec);
    if (rezsec < 60)
    {
        printf ("\t\t\t     Twoj czas: %u sec.\n\n", rezsec);
    }
    else
    {
        printf ("\t\t\tTwoj czas: %u min. %u sec.\n\n", rezsec/60, rezsec%60);
    }
                                                                                            /**zapisywanie do CZASY.DAT**/
    do_czasy(nr_albumu, akt_przedm, akt_aktywn, czas_from, czas_till);
                                                                                            /**zapisywanie do STUDENCI.DAT**/
    do_studenci (nr_albumu, akt_aktywn, num_przedm, rezsec);
    printf ("Informacja zostala zapisana do systemu.\n");
    return 0;
}
/*****************************************************************************************/
int zmiana_przedmiotow (const unsigned nr_albumu, Przedmioty subjects, const bool plec) //dodawanie studentowi nowego przedmiotu lub zmiana istniejacego
{
    unsigned akt_przedm;
    unsigned num_przedm;
    bool wlasciwy = 0;
    FILE *plik;
    //char tryb;
    unsigned album;
    unsigned subject;
    const bool czy_aktualny = 0;
    bool eof = 1; // (0=koniec pliku; 1=nie);
    char tryb;
    bool usuniecie;
                                                                                            /**wybieramy przedmiot**/
    printf ("\nPodaj numer przedmiotu, ktory chcesz zmienic: ");
    while (1)
    {
        FLUSH
        scanf ("%u", &num_przedm);
        if (0<num_przedm && num_przedm<11)
        {
            break;
        }
        printf ("\nNiewlasciwy numer. Podaj liczbe w zakresie od 1 do 10: ");
    }
    switch (num_przedm)
    {
        case 1:
        {
            if((akt_przedm = subjects.przedm_1))
            {
                wlasciwy = 1;
            }
            break;
        }
        case 2:
        {
            if((akt_przedm = subjects.przedm_2))
            {
                wlasciwy = 1;
            }
            break;
        }
        case 3:
        {
            if((akt_przedm = subjects.przedm_3))
            {
                wlasciwy = 1;
            }
            break;
        }
        case 4:
        {
            if((akt_przedm = subjects.przedm_4))
            {
                wlasciwy = 1;
            }
            break;
        }
        case 5:
        {
            if((akt_przedm = subjects.przedm_5))
            {
                wlasciwy = 1;
            }
            break;
        }
        case 6:
        {
            if((akt_przedm = subjects.przedm_6))
            {
                wlasciwy = 1;
            }
            break;
        }
        case 7:
        {
            if((akt_przedm = subjects.przedm_7))
            {
                wlasciwy = 1;
            }
            break;
        }
        case 8:
        {
            if((akt_przedm = subjects.przedm_8))
            {
                wlasciwy = 1;
            }
            break;
        }
        case 9:
        {
            if((akt_przedm = subjects.przedm_9))
            {
                wlasciwy = 1;
            }
            break;
        }
        case 10:
        {
            if((akt_przedm = subjects.przedm_10))
            {
                wlasciwy = 1;
            }
            break;
        }
    }
                                                                                            /**edytujemy istniejacy przedmiot**/
    if (wlasciwy)
    {
        char *m = zwroc_nazwe(akt_przedm);
        printf ("\nWybral%cs przedmiot \"%s\".\n", plec==1?'e':'a', m);
        free(m);

        printf("\n\tWcisnij 'U' dla usuniecia tego przedmiotu");
        printf("\n\tWcisnij 'A' dla aktualizacji tego przedmiotu\n");
        printf("\t\t\t\t\t\t...oraz wcisnij ENTER: ");
        while (1)
        {
            FLUSH
            tryb = getchar();
            switch (tryb)
            {
                case 'u':
                {
                    printf ("\nWybral%cs tryb usuniecia przedmiotu.\n", plec==1?'e':'a');
                    usuniecie = 1;
                    break;
                }
                case 'U':
                {
                    printf ("\nWybral%cs tryb usuniecia przedmiotu.\n", plec==1?'e':'a');
                    usuniecie = 1;
                    break;
                }
                case 'a':
                {
                    printf ("\nWybral%cs tryb aktualizacji przedmiotu.\n", plec==1?'e':'a');
                    usuniecie = 0;
                    break;
                }
                case 'A':
                {
                    printf ("\nWybral%cs tryb aktualizacji przedmiotu.\n", plec==1?'e':'a');
                    usuniecie = 0;
                    break;
                }
                default:
                {
                    printf ("\nNiewlasciwy symbol. Podaj litere 'U' lub 'A' i wcisnij ENTER: ");
                    continue;
                }
            }
            break;
        }
                                                                                            /**inicjulizujemy czasy**/
        do_studenci(nr_albumu, UINT_MAX, num_przedm, 0);
        printf ("Informacja zostala zapisana do systemu.\n");
        plik = fopen("..\\czasy.dat", "r+b");
        if (fseek (plik,sizeof(unsigned)+sizeof(bool), SEEK_SET))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        for (int i = 0; ; ++i)
        {
            if (fseek (plik,(i==0?0:(sizeof(unsigned)+sizeof(bool)+sizeof(unsigned)+sizeof(unsigned)+sizeof(unsigned)+sizeof(struct tm)+sizeof(struct tm))),(i==0?SEEK_SET:SEEK_CUR)))
            {
                printf ("Blad szukania danych w bazie.");
                exit(1);
            }
            eof = fread (&album, sizeof(unsigned),1,plik);
            fseek (plik,-sizeof(unsigned), SEEK_CUR);
            if (!eof) // koniec pliku
            {
                break;
            }
            if (album == nr_albumu) // znaleziono
            {
                fseek (plik, sizeof(unsigned), SEEK_CUR);
                fread (&subject,sizeof(unsigned),1,plik);
                fseek (plik, -sizeof(unsigned), SEEK_CUR);
                if (subject == akt_przedm)
                {
                    fseek (plik, -(sizeof(bool)+sizeof(unsigned)+sizeof(unsigned)), SEEK_CUR);
                    if (!fwrite (&czy_aktualny, sizeof(bool), 1, plik))
                    {
                        printf ("Blad zapisu danych do bazy");
                        exit (1);
                    }
                }
            }
        }
        fclose (plik);
        if (usuniecie)                                                                      /**usuwamy przedmiot**/
        {
            wybierz_przedmiot(nr_albumu, num_przedm, 1, plec);
        }
        else
        {
            wybierz_przedmiot(nr_albumu, num_przedm, 0, plec);
        }
    }
                                                                                            /**tworzymy nowy przedmiot**/
    else
    {
        printf ("\nWybral%cs pozycje numer %u.\n", plec==1?'e':'a', num_przedm);
        wybierz_przedmiot(nr_albumu, num_przedm, 0, plec);
    }

    return 0;
}
/*****************************************************************************************/
int do_studenci (const unsigned nr_albumu, const unsigned akt_aktywn, const unsigned num_przedm, const unsigned rezsec) //aktualizujemy dane w pliku studenci.dat
{
    FILE *plik;
    unsigned album;
    bool eof = 1; // (0=koniec pliku; 1=nie);
    unsigned long int czas [LICZBA_PRZEDMIOTOW][LICZBA_AKTYWNOSCI];
    unsigned long int ostatni_czas [LICZBA_PRZEDMIOTOW][LICZBA_AKTYWNOSCI];

    if((plik = fopen("..\\studenci.dat", "r+b"))==NULL)
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    //szukamy aktualnego studenta w bazie
    for (int i = 0; ; ++i)
    {
        if (fseek (plik,(i==0?0:(sizeof(unsigned)+sizeof(bool)+sizeof(Przedmioty)+sizeof(czas)+sizeof(ostatni_czas)+sizeof(Oceny))),(i==0?SEEK_SET:SEEK_CUR)))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        eof = fread (&album, sizeof(unsigned),1,plik);
        fseek (plik,-sizeof(unsigned), SEEK_CUR);
        if (!eof) // nie znaleziono (jesli nie udalo sie przeczytac)
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }

        if (album == nr_albumu) // znaleziono
        {
            fseek (plik, sizeof(unsigned)+sizeof(bool)+sizeof(Przedmioty), SEEK_CUR);
            fread (czas,sizeof(czas),1,plik);
            fseek (plik, -sizeof(czas), SEEK_CUR);
            if (akt_aktywn < LICZBA_AKTYWNOSCI) //sprawdzamy tryb: aktualizacja czy zerowanie przy zmianie przedmiotu
            {
                czas[num_przedm][akt_aktywn] += (unsigned long int)rezsec; //aktualizujemy ogolny czas
            }
            else //zmieniamy przedmiot
            {
                for (int n = 0; n < LICZBA_AKTYWNOSCI; ++n)
                {
                    czas[num_przedm-1][n] = 0;
                }
            }
            if (!fwrite (czas, sizeof(czas), 1, plik))
            {
                printf ("Blad zapisu danych do bazy");
                exit (1);
            }
            fread (ostatni_czas,sizeof(ostatni_czas),1,plik);
            fseek (plik, -sizeof(ostatni_czas), SEEK_CUR);
            if (akt_aktywn < LICZBA_AKTYWNOSCI) //sprawdzamy tryb: aktualizacja czy zerowanie przy zmianie przedmiotu
            {
                ostatni_czas[num_przedm][akt_aktywn] = (unsigned long int)rezsec; //aktualizujemy ostatni czas
            }
            else //zmieniamy przedmiot
            {
                for (int n = 0; n < LICZBA_AKTYWNOSCI; ++n)
                {
                    ostatni_czas[num_przedm][n] = 0;
                }
            }
            if (!fwrite (ostatni_czas, sizeof(ostatni_czas), 1, plik))
            {
                printf ("Blad zapisu danych do bazy");
                exit (1);
            }
            fclose (plik);
            break;
        }
    }
    //printf ("Informacja zostala zapisana do systemu.\n");
    return 0;
}
/*****************************************************************************************/
void wyswietl_przedmioty (Przedmioty subjects) //wyswietla przedmioty danego uzytkownika
{
    printf ("\n\tTwoje przedmioty:\n");
    char *m;
    if (subjects.przedm_1)
    {
        m = zwroc_nazwe(subjects.przedm_1);
        printf (" #1 \"%s\"\n", m);
        free(m);
    }
    if (subjects.przedm_2)
    {
        m = zwroc_nazwe(subjects.przedm_2);
        printf (" #2 \"%s\"\n", m);
        free(m);
    }
    if (subjects.przedm_3)
    {
        m = zwroc_nazwe(subjects.przedm_3);
        printf (" #3 \"%s\"\n", m);
        free(m);
    }
    if (subjects.przedm_4)
    {
        m = zwroc_nazwe(subjects.przedm_4);
        printf (" #4 \"%s\"\n", m);
        free(m);
    }
    if (subjects.przedm_5)
    {
        m = zwroc_nazwe(subjects.przedm_5);
        printf (" #5 \"%s\"\n", m);
        free(m);
    }
    if (subjects.przedm_6)
    {
        m = zwroc_nazwe(subjects.przedm_6);
        printf (" #6 \"%s\"\n", m);
        free(m);
    }
    if (subjects.przedm_7)
    {
        m = zwroc_nazwe(subjects.przedm_7);
        printf (" #7 \"%s\"\n", m);
        free(m);
    }
    if (subjects.przedm_8)
    {
        m = zwroc_nazwe(subjects.przedm_8);
        printf (" #8 \"%s\"\n", m);
        free(m);
    }
    if (subjects.przedm_9)
    {
        m = zwroc_nazwe(subjects.przedm_9);
        printf (" #9 \"%s\"\n", m);
        free(m);
    }
    if (subjects.przedm_10)
    {
        m = zwroc_nazwe(subjects.przedm_10);
        printf ("#10 \"%s\"\n", m);
        free(m);
    }
}
/*****************************************************************************************/
int sprawdz_imie (const char *imie) //sprawdzamy czy imie sklada sie z dopuszczalnych symboli
{
    char *kopia = (char*)imie;
    bool ost_kreska = 0;
    for (int i = 0; kopia[i]!=0; ++i)
    {
        if (kopia[i]=='-')
        {
            if (i==0)
            {
                return 0;
            }
            ost_kreska = 1;
        }
        else
        {
            ost_kreska = 0;
        }
        if (!((kopia[i]=='-') || ('A'<=kopia[i] && kopia[i]<='Z') || ('a'<=kopia[i] && kopia[i]<='z')))
        {
            return 0;
        }
    }
    if (ost_kreska)
    {
        return 0;
    }
    return 1;
}
/*****************************************************************************************/
int pokaz_menu (const unsigned nr_albumu, Przedmioty subjects, const bool plec, const char *imie) //pokazujemy menu i wybieramy tryb
{
    char tryb;
    while (1)
    {
        printf("\n\tWcisnij 'Z' dla dodawania nowego przedmiotu lub zmiany istniejacego");
        printf("\n\tWcisnij 'S' dla podgladu statystyk");
        printf("\n\tWcisnij 'N' dla rejestracji czasu nauki");
        printf("\n\tWcisnij 'Q' dla zakonczenia dzialania programu\n");
        printf("\t\t\t\t\t\t...oraz wcisnij ENTER: ");

        while (1)
        {
            FLUSH
            tryb = getchar();
            switch (tryb)
            {
                case 'z':
                {
                    printf ("\nWybral%cs tryb zmiany danych.\n", plec==1?'e':'a');
                    wyswietl_przedmioty (subjects);
                    zmiana_przedmiotow (nr_albumu, subjects, plec);
                    z_studenci (nr_albumu, &subjects); //aktualizujemy liste przedmiotow
                    wyswietl_przedmioty (subjects);
                    break;
                }
                case 'Z':
                {
                    printf ("\nWybral%cs tryb zmiany danych.\n", plec==1?'e':'a');
                    wyswietl_przedmioty (subjects);
                    zmiana_przedmiotow (nr_albumu, subjects, plec);
                    z_studenci (nr_albumu, &subjects); //aktualizujemy liste przedmiotow
                    wyswietl_przedmioty (subjects);
                    break;
                }
                case 's':
                {
                    printf ("\n\t\t\t  Twoje statystyki (min.):\n\n");
                    wyswietl_statystyke (nr_albumu);
                    break;
                }
                case 'S':
                {
                    printf ("\n\t\t\t  Twoje statystyki (min.):\n\n");
                    wyswietl_statystyke (nr_albumu);
                    break;
                }
                case 'n':
                {
                    printf ("\nWybral%cs tryb rejestracji czasu nauki.\n", plec==1?'e':'a');
                    wyswietl_przedmioty (subjects);
                    wybierz_aktualne (nr_albumu, subjects, plec);
                    break;
                }
                case 'N':
                {
                    printf ("\nWybral%cs tryb rejestracji czasu nauki.\n", plec==1?'e':'a');
                    wyswietl_przedmioty (subjects);
                    wybierz_aktualne (nr_albumu, subjects, plec);
                    break;
                }
                case 'q':
                {
                    printf ("\n\t\t  Do zobaczenia wkrotce, %s!\n", imie);
                    return 0;
                }
                case 'Q':
                {
                    printf ("\n\t\t  Do zobaczenia wkrotce, %s!\n", imie);
                    return 0;
                }
                default:
                {
                    printf ("\nNiewlasciwy symbol. Podaj litere 'Z', 'S', 'N' lub 'Q' i wcisnij ENTER: ");
                    continue;
                }
            }
            break;
        }
    }
}
/*****************************************************************************************/
int z_studenci (const unsigned nr_albumu, Przedmioty *subjects) //ODCZYTUJEMY DANE Z PLIKU "STUDENCI.DAT"
{
    FILE *plik;
    unsigned album;
    bool eof = 1; // (0=koniec pliku; 1=nie);

    if ((plik = fopen("..\\studenci.dat", "rb"))==NULL) // plik jeszcze nie istnieje
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    for (int i = 0; ; ++i) // plik juz istnieje
    {
        if (fseek (plik,(i==0?0:(sizeof(unsigned)+sizeof(bool)+sizeof(Przedmioty)+sizeof(czas)+sizeof(ostatni_czas)+sizeof(Oceny))),(i==0?SEEK_SET:SEEK_CUR)))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        eof = fread (&album, sizeof(unsigned),1,plik);
        fseek (plik,-sizeof(unsigned), SEEK_CUR);
        if (!eof) // jezeli nie ma - blad, bo musi byc
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        if (album == nr_albumu) //jezeli znaleziono
        {
            fseek (plik, sizeof(unsigned)+sizeof(bool), SEEK_CUR);
            fread (subjects,sizeof(Przedmioty),1,plik);                                    /**pobieramy info o jej przedmiotach**/
            fclose (plik);
            break;
        }
    }
    return 0;
}
/*****************************************************************************************/
int do_czasy (const unsigned nr_albumu, const unsigned akt_przedm, const unsigned akt_aktywn, const struct tm czas_from, const struct tm czas_till) //zapisywanie do pliku CZASY.DAT
{
    FILE *plik;
    bool czy_aktualny = 1; // (1=aktualny,0=usuniety rekord)
    unsigned id_logu;

    plik = fopen("..\\czasy.dat", "a+b");
    //szukamy wlasciwy id_logu
    if (fseek(plik,-(sizeof(unsigned)+sizeof(bool)+sizeof(unsigned)+sizeof(unsigned)+sizeof(unsigned)+sizeof(struct tm)+sizeof(struct tm)),SEEK_END))
    {
        id_logu = 0;
    }
    else if (!fread(&id_logu,sizeof(unsigned),1,plik))
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    else
    {
        id_logu++;
    }
    //zapisujemy aktualne dane
    if (fseek(plik,0,SEEK_END))
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    if (!fwrite (&id_logu, sizeof(unsigned), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&czy_aktualny, sizeof(bool), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&nr_albumu, sizeof(unsigned), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&akt_przedm, sizeof(unsigned), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&akt_aktywn, sizeof(unsigned), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&czas_from, sizeof(struct tm), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    if (!fwrite (&czas_till, sizeof(struct tm), 1, plik))
    {
        printf ("Blad zapisu danych do bazy");
        exit (1);
    }
    fclose(plik);
    return 0;
}
/*****************************************************************************************/
int ost_studenci (const unsigned nr_albumu, unsigned long int *ost) //pobieramy wpisy o ostatnich czasach pliku studenci.dat
{
    FILE *plik;
    unsigned album;
    bool eof = 1; // (0=koniec pliku; 1=nie);

    if ((plik = fopen("..\\studenci.dat", "rb"))==NULL) // plik jeszcze nie istnieje
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    for (int i = 0; ; ++i) // plik juz istnieje
    {
        if (fseek (plik,(i==0?0:(sizeof(unsigned)+sizeof(bool)+sizeof(Przedmioty)+sizeof(czas)+sizeof(ostatni_czas)+sizeof(Oceny))),(i==0?SEEK_SET:SEEK_CUR)))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        eof = fread (&album, sizeof(unsigned),1,plik);
        fseek (plik,-sizeof(unsigned), SEEK_CUR);
        if (!eof) // jezeli nie ma - blad, bo musi byc
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        if (album == nr_albumu) //jezeli znaleziono
        {
            fseek (plik, sizeof(unsigned)+sizeof(bool)+sizeof(Przedmioty)+sizeof(czas), SEEK_CUR);
            fread (ost,sizeof(ostatni_czas),1,plik);                                    /**pobieramy info o jej ostatnich czasach**/
            fclose (plik);
            break;
        }
    }
    return 0;
}
/*****************************************************************************************/
int aktualizacja (const unsigned nr_albumu, const bool plec, const unsigned akt_przedm, const unsigned akt_aktywn, const unsigned num_przedm, const char tryb) //wprowadzanie do bazy czasu od reki
{
    FILE *plik;
    unsigned long int ost[LICZBA_PRZEDMIOTOW][LICZBA_AKTYWNOSCI];
    unsigned czas_nauki;
    unsigned rezsec;
    time_t rawtime;
    struct tm czas_from;
    struct tm czas_till;
    unsigned album;
    unsigned subject;
    unsigned activity;
    const bool czy_aktualny = 0;
    bool eof = 1;

    if (tryb=='a' || tryb=='A')
    {
        printf ("\nWybral%cs tryb aktualizacji zeszlego wpisu.\n", plec==1?'e':'a');
        ost_studenci (nr_albumu, *ost);
        printf ("\n\tOstatnio zadeklarowal%cs dla tego przedmiotu i aktywnosci %u min.\n", plec==1?'e':'a', ost[num_przedm][akt_aktywn]/60);
        printf ("\nDla aktualizacji wprowadz czas nauki w minutach: ");
    }
    else if (tryb=='n' || tryb=='N')
    {
        printf ("\nWybral%cs tryb wpisywania nowego czasu.\n", plec==1?'e':'a');
        printf ("\nWprowadz czas nauki w minutach: ");
    }
    else if (tryb=='b')
    {
        printf ("\nWprowadz nowy czas nauki w minutach lub wcisnij '0' dla wyjscia: ");
    }

    while (1)
    {
        FLUSH
        scanf ("%u", &czas_nauki);
        if (czas_nauki==0)
        {
            return 0;
        }
        else if (0<czas_nauki && czas_nauki<24*60)
        {
            break;
        }
        printf ("\nNiewlasciwy czas. Podaj liczbe min w zakresie 1 doby: ");
    }


    if (tryb=='a' || tryb=='A')
    {
        plik = fopen("..\\czasy.dat", "r+b");
        if (fseek (plik,-(sizeof(unsigned)+sizeof(unsigned)+sizeof(unsigned)+sizeof(struct tm)+sizeof(struct tm)), SEEK_END))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        for (int i = 0; ; ++i)
        {
            if (fseek (plik,(i==0?0:-(sizeof(unsigned)+sizeof(bool)+sizeof(unsigned)+sizeof(unsigned)+sizeof(unsigned)+sizeof(struct tm)+sizeof(struct tm))),SEEK_CUR))
            {
                printf ("\nW bazie nie znaleziono poprzedniego rekordu.\n");
                aktualizacja (nr_albumu, plec, akt_przedm, akt_aktywn, num_przedm, 'b');
                return 0;
            }
            eof = fread (&album, sizeof(unsigned),1,plik);
            fseek (plik,-sizeof(unsigned), SEEK_CUR);
            if (!eof) // koniec pliku
            {
                printf ("Blad szukania danych w bazie.");
                exit(1);
            }
            if (album == nr_albumu) // znaleziono
            {
                fseek (plik, sizeof(unsigned), SEEK_CUR);
                fread (&subject,sizeof(unsigned),1,plik);
                fseek (plik, -(sizeof(unsigned)+sizeof(unsigned)), SEEK_CUR);

                if (subject == akt_przedm)
                {
                    fseek (plik, sizeof(unsigned)+sizeof(unsigned), SEEK_CUR);
                    fread (&activity,sizeof(unsigned),1,plik);
                    fseek (plik, -(sizeof(unsigned)+sizeof(unsigned)+sizeof(unsigned)), SEEK_CUR);

                    if (activity == akt_aktywn)
                    {
                        fseek (plik, -sizeof(bool), SEEK_CUR);
                        if (!fwrite (&czy_aktualny, sizeof(bool), 1, plik))
                        {
                            printf ("Blad zapisu danych do bazy");
                            exit (1);
                        }
                        break;
                    }
                }
            }
        }
        fclose (plik);

        do_studenci (nr_albumu, akt_aktywn, num_przedm, -ost[num_przedm][akt_aktywn]);
    }

    rezsec = czas_nauki*60;
    time (&rawtime);
    czas_till = *(localtime (&rawtime));
    rawtime = rawtime - rezsec;
    czas_from = *(localtime (&rawtime));

    do_studenci (nr_albumu, akt_aktywn, num_przedm, rezsec);
    do_czasy (nr_albumu, akt_przedm, akt_aktywn, czas_from, czas_till);
    printf ("\nInformacja zostala zapisana do systemu.\n");

    return 0;
}
/*****************************************************************************************/
int wyswietl_statystyke (const unsigned nr_albumu) //wypisuje sumaryczny czas nauki dla każdego przedmiotu z listy, oraz od najnowszych do najstarszych rekordy studenta z tablicy czasy.dat
{
    FILE *plik;
    unsigned album;
    bool eof = 1; // (0=koniec pliku; 1=nie)
    unsigned long int times[LICZBA_PRZEDMIOTOW][LICZBA_AKTYWNOSCI];
    Przedmioty subjects;
    char *m;
    bool koniec = 0;
    bool czy_aktualny;
    unsigned subject;
    unsigned activity;
    struct tm czas_from;
    struct tm czas_till;
    int rawtime;
    int l = 0;

    z_studenci (nr_albumu, &subjects);
    if ((plik = fopen("..\\studenci.dat", "rb"))==NULL) // plik jeszcze nie istnieje
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    for (int i = 0; ; ++i) // plik juz istnieje
    {
        if (fseek (plik,(i==0?0:(sizeof(unsigned)+sizeof(bool)+sizeof(Przedmioty)+sizeof(czas)+sizeof(ostatni_czas)+sizeof(Oceny))),(i==0?SEEK_SET:SEEK_CUR)))
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        eof = fread (&album, sizeof(unsigned),1,plik);
        fseek (plik,-sizeof(unsigned), SEEK_CUR);
        if (!eof) // jezeli nie ma - blad, bo musi byc
        {
            printf ("Blad szukania danych w bazie.");
            exit(1);
        }
        if (album == nr_albumu) //jezeli znaleziono
        {
            fseek (plik, sizeof(unsigned)+sizeof(bool)+sizeof(Przedmioty), SEEK_CUR);
            fread (times,sizeof(czas),1,plik);                                    /**pobieramy info o jej ostatnich czasach**/
            fclose (plik);
            break;
        }
    }

    printf("\t\t\t\tLISTA REKORDOW:\n\n");
    printf ("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    printf (" #   Godz.   Data      Nazwa przedmiotu          Nazwa aktywnosci      Czas     ");
    printf ("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    plik = fopen("..\\czasy.dat", "rb");
    if (fseek (plik,sizeof(unsigned)+sizeof(bool), SEEK_SET))
    {
        printf ("Blad szukania danych w bazie.");
        exit(1);
    }
    for (int i = 0; ; ++i)
    {
        if (fseek (plik,(i==0?0:(sizeof(unsigned)+sizeof(bool)+sizeof(unsigned)+sizeof(unsigned)+sizeof(unsigned)+sizeof(struct tm)+sizeof(struct tm))),SEEK_CUR))
        {
            break;
        }
        eof = fread (&album, sizeof(unsigned),1,plik);
        fseek (plik,-sizeof(unsigned), SEEK_CUR);
        if (!eof) // koniec pliku
        {
            break;
        }
        if (album == nr_albumu) // znaleziono
        {
            fseek (plik, -sizeof(bool), SEEK_CUR);
            fread (&czy_aktualny,sizeof(bool),1,plik);
            if (czy_aktualny)
            {
                l++;
                fseek (plik, sizeof(unsigned), SEEK_CUR);
                fread (&subject,sizeof(unsigned),1,plik);
                fread (&activity,sizeof(unsigned),1,plik);
                fread (&czas_from,sizeof(struct tm),1,plik);
                fread (&czas_till,sizeof(struct tm),1,plik);
                fseek (plik, -(sizeof(unsigned)+sizeof(unsigned)+sizeof(unsigned)+sizeof(struct tm)+sizeof(struct tm)), SEEK_CUR);
                printf ("#%s%d  ", l>9?"":" ", l);
                printf("%s%d:%s%d %s%d/%s%d/%s%d  ", czas_from.tm_hour<10?"0":"", czas_from.tm_hour, czas_from.tm_min<10?"0":"", czas_from.tm_min, czas_from.tm_mday<10?"0":"", czas_from.tm_mday, czas_from.tm_mon<10?"0":"", czas_from.tm_mon, czas_from.tm_year%100<10?"0":"", czas_from.tm_year%100);
                m = zwroc_nazwe(subject);
                for(int i = 0; i < 20; ++i)
                {
                    if (m[i]==0 && !koniec)
                    {
                        koniec = 1;
                    }
                    if (koniec)
                    {
                        printf (" ");
                    }
                    else
                    {
                        printf ("%c", m[i]);
                    }
                }
                free(m);
                if(!koniec)
                {
                    printf (".. ");
                }
                else
                {
                    printf ("   ");
                }
                koniec = 0;
                switch (activity+1)
                {
                    case 1:
                    {
                        printf(" Czytanie podrecznika ");
                        break;
                    }
                    case 2:
                    {
                        printf(" Wyszukiwanie w Int.. ");
                        break;
                    }
                    case 3:
                    {
                        printf(" Ogladanie wideomat.. ");
                        break;
                    }
                    case 4:
                    {
                        printf(" Rozwiazywanie zadan  ");
                        break;
                    }
                    case 5:
                    {
                        printf(" Programowianie       ");
                        break;
                    }
                    case 6:
                    {
                        printf(" Interakcja z koleg.. ");
                        break;
                    }
                    case 7:
                    {
                        printf(" Inne aktywnosci      ");
                        break;
                    }
                }
                rawtime = (int)difftime (mktime(&czas_till), mktime(&czas_from));
                printf ("%s%d\n", rawtime/60>9999999?"":rawtime/60>999999?" ":rawtime/60>99999?"  ":rawtime/60>9999?"   ":rawtime/60>999?"    ":rawtime/60>99?"     ":rawtime/60>9?"      ":"       ", rawtime/60);

            }
        }
    }
    fclose (plik);

    printf("\n\t\t\t\tTABLICA CZASOW:\n\n");
    printf ("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    printf (" #   Nazwa przed.    Czyt.  Intern.  Wideom.  Zadania   Progr.  Kolegi    Inne  ");
    printf ("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    if (subjects.przedm_1)
    {
        m = zwroc_nazwe(subjects.przedm_1);
        printf (" #1 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[0][a]/60>9999999?"":times[0][a]/60>999999?" ":times[0][a]/60>99999?"  ":times[0][a]/60>9999?"   ":times[0][a]/60>999?"    ":times[0][a]/60>99?"     ":times[0][a]/60>9?"      ":"       ", times[0][a]/60);
        }
    }
    if (subjects.przedm_2)
    {
        m = zwroc_nazwe(subjects.przedm_2);
        printf (" #2 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[1][a]/60>9999999?"":times[1][a]/60>999999?" ":times[1][a]/60>99999?"  ":times[1][a]/60>9999?"   ":times[1][a]/60>999?"    ":times[1][a]/60>99?"     ":times[1][a]/60>9?"      ":"       ", times[1][a]/60);
        }
    }
    if (subjects.przedm_3)
    {
        m = zwroc_nazwe(subjects.przedm_3);
        printf (" #3 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[2][a]/60>9999999?"":times[2][a]/60>999999?" ":times[2][a]/60>99999?"  ":times[2][a]/60>9999?"   ":times[2][a]/60>999?"    ":times[2][a]/60>99?"     ":times[2][a]/60>9?"      ":"       ", times[2][a]/60);
        }
    }
    if (subjects.przedm_4)
    {
        m = zwroc_nazwe(subjects.przedm_4);
        printf (" #4 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[3][a]/60>9999999?"":times[3][a]/60>999999?" ":times[3][a]/60>99999?"  ":times[3][a]/60>9999?"   ":times[3][a]/60>999?"    ":times[3][a]/60>99?"     ":times[3][a]/60>9?"      ":"       ", times[3][a]/60);
        }
    }
    if (subjects.przedm_5)
    {
        m = zwroc_nazwe(subjects.przedm_5);
        printf (" #5 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[4][a]/60>9999999?"":times[4][a]/60>999999?" ":times[4][a]/60>99999?"  ":times[4][a]/60>9999?"   ":times[4][a]/60>999?"    ":times[4][a]/60>99?"     ":times[4][a]/60>9?"      ":"       ", times[4][a]/60);
        }
    }
    if (subjects.przedm_6)
    {
        m = zwroc_nazwe(subjects.przedm_6);
        printf (" #6 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[5][a]/60>9999999?"":times[5][a]/60>999999?" ":times[5][a]/60>99999?"  ":times[5][a]/60>9999?"   ":times[5][a]/60>999?"    ":times[5][a]/60>99?"     ":times[5][a]/60>9?"      ":"       ", times[5][a]/60);
        }
    }
    if (subjects.przedm_7)
    {
        m = zwroc_nazwe(subjects.przedm_7);
        printf (" #7 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[6][a]/60>9999999?"":times[6][a]/60>999999?" ":times[6][a]/60>99999?"  ":times[6][a]/60>9999?"   ":times[6][a]/60>999?"    ":times[6][a]/60>99?"     ":times[6][a]/60>9?"      ":"       ", times[6][a]/60);
        }
    }
    if (subjects.przedm_8)
    {
        m = zwroc_nazwe(subjects.przedm_8);
        printf (" #8 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[7][a]/60>9999999?"":times[7][a]/60>999999?" ":times[7][a]/60>99999?"  ":times[7][a]/60>9999?"   ":times[7][a]/60>999?"    ":times[7][a]/60>99?"     ":times[7][a]/60>9?"      ":"       ", times[7][a]/60);
        }
    }
    if (subjects.przedm_9)
    {
        m = zwroc_nazwe(subjects.przedm_9);
        printf (" #9 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[8][a]/60>9999999?"":times[8][a]/60>999999?" ":times[8][a]/60>99999?"  ":times[8][a]/60>9999?"   ":times[8][a]/60>999?"    ":times[8][a]/60>99?"     ":times[8][a]/60>9?"      ":"       ", times[8][a]/60);
        }
    }
    if (subjects.przedm_10)
    {
        m = zwroc_nazwe(subjects.przedm_10);
        printf ("#10 ");
        for(int i = 0; i < 10; ++i)
        {
            if (m[i]==0 && !koniec)
            {
                koniec = 1;
            }
            if (koniec)
            {
                printf (" ");
            }
            else
            {
                printf ("%c", m[i]);
            }
        }
        free(m);
        if(!koniec)
        {
            printf (".. ");
        }
        else
        {
            printf ("   ");
        }
        koniec = 0;
        for (int a=0; a<LICZBA_AKTYWNOSCI; ++a)
        {
            printf("%s%u ", times[9][a]/60>9999999?"":times[9][a]/60>999999?" ":times[9][a]/60>99999?"  ":times[9][a]/60>9999?"   ":times[9][a]/60>999?"    ":times[9][a]/60>99?"     ":times[9][a]/60>9?"      ":"       ", times[9][a]/60);
        }
    }
    printf ("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

    //читаємо від останнього всі рекорди студента з таблиці czasy
    return 0;
}

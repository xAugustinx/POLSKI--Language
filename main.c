#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include "a"

#define sprawdzayZnakWFunkcji spf
#define smallestInt -32767

extern char funkcje[];

typedef struct {char nazwaID[10]; int id;  int grupa; int wartosc; } obiekt;

int idObiektow = 0;


obiekt * pamiec;
long iloscObiektow = 1;

int ostatniZnakPliku;
byte turnON = TRUE;

int liniaKoduLiczba = 0;

FILE * plikKodu;

int czyZnaelziono(char nazwa[])
{
    for (int i = 1; i < iloscObiektow; i++)
    {
        for (int pe = 0; pe < 10; pe++)
        {
            if (pamiec[i].nazwaID[pe] == 0 && nazwa[pe] == 0) return pamiec[i].wartosc;
            else if (pamiec[i].nazwaID[pe] != nazwa[pe]) break;  
        }
    }
    return smallestInt;
}
void nowyElement(obiekt musztarda)
{
    idObiektow++;
    musztarda.id = idObiektow;

    iloscObiektow++;
    obiekt * tymczasowy = realloc(pamiec, iloscObiektow * sizeof(obiekt));
    pamiec = tymczasowy;
    pamiec[iloscObiektow-1] = musztarda;

}
void edycjaElementu(int elementModefikowany, int wartoscxZmiana)
{
    pamiec[elementModefikowany].wartosc = wartoscxZmiana;
}

int sprawdzKodFunkcji(int code,char tablica[])
{
    for (int codeL = code; TRUE; codeL++)
    {
        if (funkcje[codeL] == '|') {return TRUE;}
        else if (funkcje[codeL] != tablica[codeL - code]) {return FALSE;}
    }
}
int kodPodajnik(char tablica[])
{
    byte czyToMozeBycTen = 1;
    int b = 0;
    int sprawdzanyKod = 0;

    for (int i = 0; i < sizeof(funkcje); i++ )
    {   
        if (funkcje[i] == '|') {
            if (czyToMozeBycTen) return  sprawdzanyKod;
            sprawdzanyKod = i+1;
            b = -1; czyToMozeBycTen = 1;  
        } 
        else if (tablica[b] != funkcje[i]) czyToMozeBycTen = 0;
        b++;
    }
    return -1;
}

int doNajblizszegoZnaku(int znak)
{
    
    char znakDoNajblizszego;
    for (int i = znak; TRUE; i++)
    {
        fseek(plikKodu,znak,0);
        fscanf(plikKodu,"%c",&znakDoNajblizszego);
        if (i >= ostatniZnakPliku - 1) return -2;
        else if (znakDoNajblizszego == '\n') liniaKoduLiczba++;
        else if (znakDoNajblizszego != ' ' && znakDoNajblizszego != '\n') return znak;
    }
}


int main(int argc, char *qrgv[])
{
    pamiec = (obiekt*) malloc(iloscObiektow * sizeof(obiekt));

    if (argc > 1) 
    {
        byte rozmiar;
        for (rozmiar = 0; TRUE; rozmiar++) if (qrgv[1][rozmiar] == 0) break;
        char pierwszyArgument[rozmiar];
        for (byte i = 0; i < rozmiar; i++) pierwszyArgument[i] = qrgv[1][i];
        plikKodu = fopen(pierwszyArgument, "r");
    }
    else plikKodu = fopen("main.pl","r");

    fseek(plikKodu, 0,2);
    ostatniZnakPliku = ftell(plikKodu);

    int obecnyCzytanyZnak = 0;
    int ostatniZnakLini;

    while (turnON)
    {
        char znakWPetli1;
        for (ostatniZnakLini = obecnyCzytanyZnak; TRUE; ostatniZnakLini++)
        {
            fseek(plikKodu,ostatniZnakLini,0  );
            fscanf(plikKodu,"%c", &znakWPetli1 );
            if (znakWPetli1 == ';') break;  
        }
        char aktualnaLinijkaKodu[ostatniZnakLini - obecnyCzytanyZnak+1];
        aktualnaLinijkaKodu[ostatniZnakLini - obecnyCzytanyZnak] = 0;
    
        for (int i = obecnyCzytanyZnak; i < ostatniZnakLini; i++  )
        {
            fseek(plikKodu,i,0);
            fscanf(plikKodu,"%c",&aktualnaLinijkaKodu[i - obecnyCzytanyZnak]);
        }
        //początek operacji
        printf("%s\n",aktualnaLinijkaKodu);
        
        int aktualnyKodFunkcji = kodPodajnik(aktualnaLinijkaKodu);

        int sprawdzayZnakWFunkcji = 0;


        if (aktualnyKodFunkcji == -1) {printf("Nie Istniejąca Funkcja, linia %d ",liniaKoduLiczba); return 0;}
        else if (aktualnyKodFunkcji == 16  )
        {
            for (int i = 1; i < sizeof(aktualnaLinijkaKodu); i++ )
            {
                if (aktualnaLinijkaKodu[i] != ' ' && aktualnaLinijkaKodu[i-1] == ' ' ) spf = i;
            }
            if (!spf) {printf("niepoprawna operacja na zmiennych, linia %d ",liniaKoduLiczba); return 1;};
            
            int dlugoscNazwy;
            for (dlugoscNazwy = spf; dlugoscNazwy < sizeof(aktualnaLinijkaKodu); dlugoscNazwy++)
            {if (aktualnaLinijkaKodu[dlugoscNazwy] == ' ') break; }
            char nazwaZmiennej[10];
            nazwaZmiennej[dlugoscNazwy-1] = 0;
            obiekt nowyObiekt;
            for (int i = spf; i < spf + 10; i++)
            {
                if (i < spf + dlugoscNazwy) {nazwaZmiennej[i - spf] = aktualnaLinijkaKodu[i]; nowyObiekt.nazwaID[i - spf] = aktualnaLinijkaKodu[i];}
                else {nazwaZmiennej[i - spf] = 0; nowyObiekt.nazwaID[i - spf] = 0; }
            }
            
            byte czyWartoscUstawiona = 0;

            

            for (spf =  spf + dlugoscNazwy; spf < sizeof(aktualnaLinijkaKodu); spf++ ) {
                if (aktualnaLinijkaKodu[spf] != ' ' && czyWartoscUstawiona ) break;
                if (aktualnaLinijkaKodu[spf] == '=' ) czyWartoscUstawiona = 1;
            }
            
            int wartoscUstawiona = 0;

            if (czyWartoscUstawiona)
            {
                char znakUzyty = '+';
                int wartoscPojedyncza = 0;
                
                byte czyTerazJestZmienna = 0;
                int pierwszyZnakZmiennej = 0;
                int ostatniZnak = 0;

                for (spf; spf < sizeof(aktualnaLinijkaKodu); spf++)
                {
                    if (aktualnaLinijkaKodu[spf] == '+' || aktualnaLinijkaKodu[spf] == '-' || aktualnaLinijkaKodu[spf] == '*' || aktualnaLinijkaKodu[spf] == '/' || sizeof(aktualnaLinijkaKodu)-1 == spf )
                    {
                        if (czyTerazJestZmienna)
                        {
                            char tablicaZNazwaZmiennej[10];
                            for (int i = pierwszyZnakZmiennej; i < 10; i++)
                            {
                                if (i < ostatniZnak) tablicaZNazwaZmiennej[i-pierwszyZnakZmiennej] = aktualnaLinijkaKodu[i]; 
                                else tablicaZNazwaZmiennej[i-pierwszyZnakZmiennej] = 0;
                            }
                            int wartoscKtoramZajebiemy = czyZnaelziono(tablicaZNazwaZmiennej);
                            if (wartoscKtoramZajebiemy != smallestInt)
                            {
                                wartoscPojedyncza = pamiec[wartoscKtoramZajebiemy].wartosc;
                            }
                        }

                        switch (znakUzyty){
                            case '+': wartoscUstawiona += wartoscPojedyncza; break;
                            case '-': wartoscUstawiona -= wartoscPojedyncza; break;
                            case '/': wartoscUstawiona /= wartoscPojedyncza; break;
                            case '*': wartoscUstawiona *= wartoscPojedyncza; break;
                        }
                        znakUzyty = aktualnaLinijkaKodu[spf];
                        wartoscPojedyncza = 0;

                        czyTerazJestZmienna = 0;
                    }
                    else if (aktualnaLinijkaKodu[spf] > 47 && aktualnaLinijkaKodu[spf] < 58)
                    {
                        wartoscPojedyncza = (wartoscPojedyncza * 10) + aktualnaLinijkaKodu[spf] - 48;
                        czyTerazJestZmienna = 0;
                    }
                    else if (aktualnaLinijkaKodu[spf] != ' ' )
                    {
                        if (!czyTerazJestZmienna) 
                        {
                            czyTerazJestZmienna = 1;
                            pierwszyZnakZmiennej = spf;
                            ostatniZnak = spf;
                        }
                        else ostatniZnak++;
                    }
                }
            }

            if (wartoscUstawiona > 0)
            {
                printf("%d\n",wartoscUstawiona);
            }

            if (czyZnaelziono(nazwaZmiennej) == smallestInt)
            {
                nowyObiekt.grupa = 0;
                nowyObiekt.wartosc = wartoscUstawiona;
                nowyElement(nowyObiekt);
            }
        }
        


        //koniec operacji
        ostatniZnakLini+=1;
        char znakWPetli2 = ' ';
        for (ostatniZnakLini; TRUE ; ostatniZnakLini++  )
        {
            if (ostatniZnakLini >= ostatniZnakPliku - 1) {turnON = 0; break;};
            fseek(plikKodu,ostatniZnakLini, 0);
            fscanf(plikKodu,"%c",&znakWPetli2);
            if (znakWPetli2 == '\n') liniaKoduLiczba++;
            else if (znakWPetli2 != ' ' && znakWPetli2 != '\n') break;

        }

        usleep(100);
        obecnyCzytanyZnak = ostatniZnakLini;

        liniaKoduLiczba++;
    }
    free(pamiec);
    fclose(plikKodu);
}
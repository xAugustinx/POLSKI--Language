#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "a"

#define sprawdzayZnakWFunkcji spf
#define smallestInt -32767
#define printERROR printf
#define SPACE 32

typedef struct {char nazwaID[10]; int id;  int grupa; float wartosc; byte typeofData; } obiekt;

int idObiektow = 0;


obiekt * pamiec;
long iloscObiektow = 1;

int ostatniZnakPliku;
byte turnON = TRUE;

int liniaKoduLiczba = 0;

FILE * plikKodu;

void blad(char nazwa[])
{
    printf("%s, LINIA: %d\n",nazwa, liniaKoduLiczba ); 
}

int czyZnaelziono(char nazwa[])
{
    for (int i = 1; i < iloscObiektow; i++)
    {
        for (int pe = 0; pe < 10; pe++)
        {
            if (pamiec[i].nazwaID[pe] == 0 && nazwa[pe] == 0 || (pamiec[i].nazwaID[pe] == 0 && nazwa[pe] == '|' ) ) return i;
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
        
        int aktualnyKodFunkcji = kodPodajnik(aktualnaLinijkaKodu);

        int sprawdzayZnakWFunkcji = 0;


        if (aktualnyKodFunkcji == -1) {printERROR("Nie Istniejąca Funkcja, linia %d ",liniaKoduLiczba); turnON = 0;}
        else if (aktualnyKodFunkcji == 7)
        {
            for (int i = 1; i < strlen(aktualnaLinijkaKodu); i++ )
            {
                if (aktualnaLinijkaKodu[i] != SPACE   ) {
                    if (aktualnaLinijkaKodu[i-1] == SPACE) {spf = i; break;}
                }
            }
            if (!spf) {printERROR("niepoprawna operacja na zmiennych, linia %d ",liniaKoduLiczba); turnON = 0;};
            
            int dlugoscNazwy;
            for (dlugoscNazwy = spf; dlugoscNazwy < strlen(aktualnaLinijkaKodu); dlugoscNazwy++)
            {if (aktualnaLinijkaKodu[dlugoscNazwy] == ' ') break; }
            char nazwaZmiennej[10];
            nazwaZmiennej[dlugoscNazwy-1] = 0;
            obiekt nowyObiekt;
            byte czyBylaSpacja = 0;

            for (int i = spf; i < spf + 10; i++)
            {
                if (i - spf <  dlugoscNazwy - spf) {nazwaZmiennej[i - spf] = aktualnaLinijkaKodu[i]; nowyObiekt.nazwaID[i - spf] = aktualnaLinijkaKodu[i];}
                else {nazwaZmiennej[i - spf] = 0; nowyObiekt.nazwaID[i - spf] = 0; }
            }

            byte czyWartoscUstawiona = 0;

            for (spf =  dlugoscNazwy; spf < strlen(aktualnaLinijkaKodu); spf++ ) {
                if (aktualnaLinijkaKodu[spf] != ' ' && czyWartoscUstawiona ) break;
                else if (aktualnaLinijkaKodu[spf] == '=' ) czyWartoscUstawiona = 1;
            }
            
            float wartoscUstawiona = 0;
            byte typZmiennej = 0;

            if (czyWartoscUstawiona)
            {
                char znakUzyty = '+';
                float wartoscPojedyncza = 0;
                
                byte czyTerazJestZmienna = 0;
                int pierwszyZnakZmiennej = 0;
                int ostatniZnak = 0;
                int przecinek = 0;

                for (spf; spf < strlen(aktualnaLinijkaKodu)+1; spf++)
                {
                    if (aktualnaLinijkaKodu[spf] == '+' || aktualnaLinijkaKodu[spf] == '-' || aktualnaLinijkaKodu[spf] == '*' || aktualnaLinijkaKodu[spf] == '/' || strlen(aktualnaLinijkaKodu) == spf )
                    {
                        if (czyTerazJestZmienna)
                        {
                            char tablicaZNazwaZmiennej[10];
                            for (int i = pierwszyZnakZmiennej; i < pierwszyZnakZmiennej + 10; i++)
                            {
                                if (i <= ostatniZnak) tablicaZNazwaZmiennej[i-pierwszyZnakZmiennej] = aktualnaLinijkaKodu[i];
                                else tablicaZNazwaZmiennej[i-pierwszyZnakZmiennej] = 0;
                            }
                            int wartoscKtoramZajebiemy = czyZnaelziono(tablicaZNazwaZmiennej);
                            if (wartoscKtoramZajebiemy != smallestInt)
                            {
                                wartoscPojedyncza = pamiec[wartoscKtoramZajebiemy].wartosc;
                            }
                            przecinek = 0;
                        }

                        switch (znakUzyty){
                            case '+': wartoscUstawiona += wartoscPojedyncza; break;
                            case '-': wartoscUstawiona -= wartoscPojedyncza; break;
                            case '/': wartoscUstawiona /= wartoscPojedyncza; break;
                            case '*': wartoscUstawiona *= wartoscPojedyncza; break;
                        }
                        znakUzyty = aktualnaLinijkaKodu[spf];
                        wartoscPojedyncza = 0;
                        przecinek = 0;
                        czyTerazJestZmienna = 0;
                    }
                    else if (aktualnaLinijkaKodu[spf] > 47 && aktualnaLinijkaKodu[spf] < 58 && !czyTerazJestZmienna )
                    {
                        if (!przecinek) wartoscPojedyncza = (wartoscPojedyncza * 10) + aktualnaLinijkaKodu[spf] - 48;
                        else 
                        {
                            wartoscPojedyncza+=((float)(aktualnaLinijkaKodu[spf] - 48))/(float)(przecinek*10);
                            przecinek++;
                        }
                    }
                    else if (aktualnaLinijkaKodu[spf] != ' ' )
                    {
                        if (aktualnaLinijkaKodu[spf] != ',')
                        {
                            if (!czyTerazJestZmienna) 
                            {
                                czyTerazJestZmienna = 1;
                                pierwszyZnakZmiennej = spf;
                                ostatniZnak = spf;
                            }
                            else ostatniZnak++;
                        }
                        else przecinek++;  
                        if (aktualnaLinijkaKodu[spf] == '"') typZmiennej = 1;

                    }
                }
            }

            nowyObiekt.grupa = 0;
            nowyObiekt.wartosc = wartoscUstawiona;
            nowyObiekt.typeofData = typZmiennej;

            int ktoryElement = czyZnaelziono(nazwaZmiennej);

            if (czyZnaelziono(nazwaZmiennej) == ktoryElement)nowyElement(nowyObiekt);
            else pamiec[ktoryElement] = nowyObiekt;

        }
        else if (aktualnyKodFunkcji == 0) {
            int pierwszy = -1;
            int ostatni = -1;

            for (int i = 7; i < sizeof(aktualnaLinijkaKodu);  i++ ) {if (aktualnaLinijkaKodu[i] != ' ') {pierwszy = i; break;}}
            for (int i = pierwszy; i < sizeof(aktualnaLinijkaKodu); i++) if ((aktualnaLinijkaKodu[i] == ' ' || aktualnaLinijkaKodu[i] == 0  )) {ostatni = i; break;}

            if (aktualnaLinijkaKodu[pierwszy] == '"' && aktualnaLinijkaKodu[ostatni-1] == '"')
            {
                for (int i = pierwszy+1; i < ostatni-1; i++) 
                {
                    if (aktualnaLinijkaKodu[i] == '|') printf("%c",' ');
                    else printf("%c",aktualnaLinijkaKodu[i]);
                    
                }
                printf("\n");
            }
            else if (aktualnaLinijkaKodu[pierwszy] == '"' || aktualnaLinijkaKodu[ostatni-1] == '"') {blad("Jeden '' "); return 1; }
            else
            {
                char nazwaZmiennej[ostatni - pierwszy + 1];
                nazwaZmiennej[ostatni - pierwszy] = 0;
                for (int i = pierwszy; i < ostatni; i++) nazwaZmiennej[i- pierwszy] = aktualnaLinijkaKodu[i];
                int element = czyZnaelziono(nazwaZmiennej);
                if (element == smallestInt) {printf("BŁĄD LINIA: %d\n",liniaKoduLiczba );   return 1;}

                if (!pamiec[element].typeofData) printf("%f\n", pamiec[element].wartosc);
                else if (pamiec[element].typeofData == 1 )
                {
                    int iloscZnakow = iloscLiczb((int)pamiec[element].wartosc);
                    char string[iloscZnakow+1] ;
                    sprintf(string, "%d",(int)pamiec[element].wartosc );

                    byte razDwaTrzy = 0;
                    byte wartoscPojedyncza = 0;

                    for (int i = 0; i < iloscZnakow; i++ )
                    {
                        if (string[i] == 0) string[i]+=48;
                        if (!(i == iloscZnakow)) wartoscPojedyncza = wartoscPojedyncza*10+(string[i]-48);
                        razDwaTrzy++;
                        if (razDwaTrzy > 2  )
                        {
                            printf("%c",wartoscPojedyncza);
                            wartoscPojedyncza = 0;
                            razDwaTrzy = 0;
                        }
                    }
                }
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

        //liniaKoduLiczba++;
    }
    free(pamiec);
    fclose(plikKodu);
}
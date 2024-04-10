#pragma once
#include <limits.h>
#include "global.h"

#define PRIOR_ERROR INT_MIN
#define POS_ERROR -1


typedef struct
{
	PQINFO* pInfo;
	int nPrior;
}PQItem;//definiowana tylko dla modulu

typedef struct
{
	PQItem* pPQueue;  // kolejka realizowana w postaci stogu  (dynamiczna tablica)
	int nPQSize;      // rozmiar calkowity kolejki priorytetowej
	int  nPQCurrSize; // nPQCurrSize  // ilosc elelemntow w kolejce (jednoczensie indeks pierwszego wolnego miejsc w kolejce) 
}PQueue;

/* sortowanie od najwiekszego do najmniejszego
void HeapSort( int* tab, int nSize )
{
	int l = nSize / 2;
	int p = nSize - 1;

	/*while( l-- > 0 )
		update( l, p, tab );
	*/
/*
	for ( int i = l - 1; i > 0; i-- )
		update( i, p, tab );

	for ( int i = p; i > 0; i-- )
	{
		int temp = tab[0];
		tab[0] = tab[i];
		tab[i] = temp;
		update( 0, i - 1, tab );
	}
}

void update( int l, int p, int* tab )
{
	if ( l == p ) return;
	int i = l;
	int j = 2 * i + 1;
	int temp = tab[i];
	while ( j <= p ) ///////////
	{
		if ( j < p )
			if ( tab[j] >= tab[j + 1] )
				j++;

		if ( tab[j] > temp ) ///////
			break;
		tab[i] = tab[j];
		i = j;
		j = 2 * i + 1;
	}
	tab[i] = temp;
}
*/


PQueue* PQCreate( int nSize );   // kreuje kolejke o zadanym rozmiarze i zwraca wskaznik do niej lub NULL gdy blad


int PQisEmpty( PQueue* queue );  // zwraca 0 -nie pusta lub 1 - kolejka pusta


int PQSize( PQueue* queue );     // zwraca ilosc elementow w kolejce

int PQMaxPrior( PQueue* queue ); // zwraca najwiekszy priorytet w kolejce (z zerowej pozycji) lub PRIOR_ERROR

int PQEnqueue( PQueue* queue, int nPrior, PQINFO* pInfo );  // wstawia do kolejki informacje o zadanym priorytecie, zwraca 0 (niepowodzenie) lub 1 (powodzenie)

PQINFO* PQDequeue( PQueue* queue );  // zwraca informacje o najwyzszym priorytecie lub NULL gdy niepowodzenie

//-------
void PQClear( PQueue* queue, void( __cdecl* freemem )(const void*) );//dlatego przekazac trzeba funkcje uzytkownika do zwolnienia pamieci przez parametr
//-------

void PQRelease( PQueue** queue, void( __cdecl* freemem )( const void* ) );  // usuwa cala kolejke, zwraca w parametrze we-wy NULL (kolejka) usuwaj¹c wszyskto 
                   // wczesniej (czyszczenie kolejki)

void PQPrint( PQueue* queue, void( __cdecl* Printinfo )( const void* ),int p );  // drukuje w porzadku preorder (rekurencyjnie) od zadanej pozycji, aby wydrukowaæ info uzytkownika 
                 // przekazac trzeba funkcje drukujaca ta informacje. Dodatkowo, drukuje priorytet w okraglych nawiasach 
								 // oraz pozycje w  kolejce w kwadratowych nawiasow

int PQSetPrior( PQueue* queue, int( __cdecl* comp )( const void*, const void* ), PQINFO* pInfo, int nprior ); // ustawia nowy priorytet dla zadanej informacji uzytkownika, nie znana struktura informacji, która trzeba 
                  // wyszukac dlatego nalezy przekazac funkcje porownujaca informacje uzytkownika. 
                  // Funkcja zwraca poprzedni priorytet lub PRIOR_ERROR 

int PQsetPrior( PQueue* queue,int position, int nprior ); // ustawia nowy priorytet dla informacji uzytkownika bedacej na zadanej pozycji w kolejce 
                  // (indeks w tablicy). Funkcja zwraca poprzedni priorytet. W przypadku bledu zwraca PRIOR_ERROR 


int PQGetPrior( PQueue* queue, PQINFO* pInfo, int( __cdecl* comp )( const void*, const void* ) ); // zwraca priorytet informacji uzytkownika w kolejce lub PRIOR_ERROR, nie znana struktura informacji, która 
                  // trzeba wyszukac dlatego nalezy przekazac funkcje porownujaca informacje uzytkownika  


int PQgetPrior( PQueue* queue, int position ); // zwraca priorytet informacji uzytkownika z zadanej pozycji w kolejce lub PRIOR_ERROR
  
int PQFind( PQueue* queue, int( __cdecl* comp )( const void*, const void* ), PQINFO* pInfo );     // zwraca indeks wyszukiwanej informacji uzytkownika, nie znana struktura informacji uzytkownika dlatego
                  // przekazac trzeba funkjce porownujaca informacje uzytkownika
                  // Zwaraca POS_ERROR jesli nie ma info w kolejce

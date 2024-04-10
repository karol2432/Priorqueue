#include <stdio.h>
#include "PQueue.h"
#include <stdlib.h>
#include <memory.h>
#include "global.h"

void UpdateDown( int l, int p, PQItem* tab );
void UpdateUp( int l, int p, PQItem* tab );


void UpdateDown( int l, int p, PQItem* pTab )
{
  if( l >= p ) return; //jesli dziecko lewe jest wikesze niz gorne ograniczenie to nie ma potrzeby uaktualniania 
  int i = l;
  int j = 2 * i + 1;  //indeks lewego dziecka korzenia
  PQItem temp = pTab[i]; //zmienna pomocnicza w ktorej bedzie przechowywac wartosc elementu tab[i]
  while ( j <= p )  //dopoki indeks lewego dziecka mamy w zakresie drzewa
  {
    if( j < p )
    {
      if( pTab[j].nPrior <= pTab[j + 1].nPrior ) //porownuje lewe dziecko z prawym 
      {
        j++;
      }
    }

    if( pTab[j].nPrior < temp.nPrior )
    {
      break; //element ma nizszy priorytet niz dziecko
    }
    pTab[i] = pTab[j]; 
    i = j;         //przypisuje i wartoisc zeby kontynuowac przesuwanie w dol drzewa
    j = 2 * i + 1;//zmieniam indeks dziecka
  }
  pTab[i] = temp;
}

void UpdateUp( int l, int p, PQItem* pTab )//analogicznie jak w updatedown tylko uaktualniam w gore i jest jeden rodzic
{
  
  if( l >= p ) return;
  int i = p;           //indeks elementu ktory chcemy uaktualnic w gore
  int j = (i - 1) / 2; //indeks rodzica dla elementu i w kopcu
  PQItem temp = pTab[i];
  while( j >= l && pTab[j].nPrior < temp.nPrior )
  {
    pTab[i] = pTab[j]; //przesuwa elem rodzica do obecnej
    i = j;             //uaktualniam indeks zeby kontynuowac przesuwanie po drzewie
    j = (i + 1) / 2 - 1; //zmieniam indeks rodzica pisze nie jest tak samo jak na gorze bo dzielenie jest calkowite, zaczynamy z i =0 i gdtby bylo jak na gorze
                         // to j=0 i wtedy petla by sie zapetlila, wiec zeby obliczyc j do i dodaje 1 zeby cofnac wczesniejsze odejmpowanie
                         //i po podzieleniu odejmuje jeszcze jeden aby sposob indeksowania tablicy zostal zachowany
  }
  pTab[i] = temp;
  
}





//-------------------------------------

PQueue* PQCreate(int nSize) // kreuje kolejke o zadanym rozmiarze i zwraca wskaznik do niej lub NULL gdy blad
{
  if( nSize < 0 )
  {
    return NULL;
  }

  PQueue* temp = ( PQueue* )calloc( 1, sizeof( PQueue ) );
  if( !temp )
  {
    return NULL;
  }
  temp->pPQueue = ( PQItem* )calloc( nSize, sizeof( PQItem ) );
  if( !(temp->pPQueue) )
  {
    return NULL;
  }
  temp->nPQSize = nSize;

  return temp;
}

int PQisEmpty( PQueue* queue ) //nie uzywac ifa  zwraca 0 -nie pusta lub 1 - kolejka pusta
{
  return !queue || !queue->nPQCurrSize;
}


int PQSize( PQueue* queue ) // zwraca ilosc elementow w kolejce
{
  return queue->nPQCurrSize;
}


int PQMaxPrior( PQueue* queue ) // zwraca najwiekszy priorytet w kolejce (z zerowej pozycji) lub PRIOR_ERROR
{
  return (!PQisEmpty( queue )) ? PRIOR_ERROR : PQgetPrior( queue, 0 );
}

int PQEnqueue( PQueue* queue, int nPrior, PQINFO* pInfo )// wstawia do kolejki informacje o zadanym priorytecie, zwraca 0 (niepowodzenie) lub 1 (powodzenie)
{ 
  if( PQSize(queue) >= queue->nPQSize ) //zwraca 0 gdy ilosc elemntow jest >= od calkowitej wielkosci kolejki
  {
    return 0;
  }
  
  int size = PQSize( queue );
  queue->pPQueue[size].nPrior = nPrior; //wstawiam prior na koncu kolejki (tam gdzie wolne miejsce)
  queue->pPQueue[size].pInfo = pInfo;//wstawia do kolejki informacje uzytkownika
  if( size > 0 ) UpdateUp( 0, size, queue->pPQueue );//uaktualniam w gore czyli tam mi sprowadzi ten wstawiony priorytet na odpowiednie miejsce
  ++(queue->nPQCurrSize); //nPQCurrSize sobie zwiekszam o jeden bo dodalem jeden element

  return 1;
}


PQINFO* PQDequeue( PQueue* queue ) // zwraca informacje o najwyzszym priorytecie lub NULL gdy niepowodzenie
{
  if( PQisEmpty( queue ) )
  {
    return NULL;
  }
  
  int temp = --(queue->nPQCurrSize);//najpierw zmniejsz rozmiar i podstawiam pod y
  PQINFO* najpriorytet = queue->pPQueue[0].pInfo; //na indeksie 0 jest najwyzszy priorytet
  // wpisac ostatni do 0-wego
  if( temp )
  {
    queue->pPQueue[0] = queue->pPQueue[temp]; // wpisuje ostatni do 0-wego a pozniej jesli bedzie konieczne uaktualni sie w dol
  }
  memset( queue->pPQueue + temp, 0, sizeof( PQItem ) ); //wyzerowac pamiec dla elementu o indeksie y
  if( PQSize( queue ) > 1 )
  {
    UpdateDown( 0, temp - 1, queue->pPQueue ); //uaktualniam w dol  
  }
  
  return najpriorytet; // zwraca informacje o najwyzszym priorytecie
}


void PQClear( PQueue* queue, void( __cdecl* freemem )(const void*) )// przekazac trzeba funkcje uzytkownika do zwolnienia pamieci przez parametr
{
  if( !queue || !freemem )
  {
    printf( "kolejka nie istnieje lub funkcja freemem nie istnieje\n\n" );
    return;
  }

  while( !PQisEmpty( queue ) )//dopoki niepusta
  {
    freemem( PQDequeue( queue ) );
  }
}


void PQRelease( PQueue** queue, void( __cdecl* freemem )(const void*) ) // usuwa cala kolejke, zwraca w parametrze we-wy NULL (kolejka) usuwaj¹c wszyskto 
{                                                                       //wczesniej czyszczenie kolejki
  if( !queue || !*queue ) //POPRAWKA!!!
  {
    printf( "kolejka nie istnieje \n\n" );
    return;
  }

  PQClear( *queue, freemem );
  free( (*queue)->pPQueue );
  free( *queue );
  *queue = NULL;
}


void PQPrint( PQueue* queue, void( __cdecl* Printinfo )(const void*), int position ) // drukuje w porzadku preorder (rekurencyjnie) od zadanej pozycji, aby wydrukowaæ info uzytkownika 
{
                                                                                     // pozycje w  kolejce w kwadratowych nawiasow
  if( !queue || position < 0 || !Printinfo )                                         // przekazac trzeba funkcje drukujaca ta informacje. Dodatkowo, drukuje priorytet w okraglych nawiasach 
  {
    printf( "kolejka nie istnieje lub funkcja Printinfo jest zle zdefiniowana lub p(indeks) jest na zbyt malej pozycji\n" );
    return;
  }
  
  if( position >= PQSize( queue ) )// pozycja od ktorej zaczynamy musi byc mniejsza od rozmiaru kolejki
  {
    return;
  }
 
  if( position < PQSize( queue ) )
  {
    Printinfo( queue->pPQueue[position].pInfo );
    printf( "\tPriorytet elementu: (%d), Pozycja elementu: [%d]", queue->pPQueue[position].nPrior, position );
  }
  PQPrint( queue, Printinfo, 2 * position + 1 );
  PQPrint( queue, Printinfo, 2 * position + 2 );
}


int PQSetPrior( PQueue* queue, int( __cdecl* comp )(const void*, const void*), PQINFO* pInfo, int nprior ) // Funkcja zwraca poprzedni priorytet lub PRIOR_ERROR 
{
  return PQsetPrior( queue, PQFind( queue, comp, pInfo), nprior );//(mam kolejke, znajduje elem,ustawiam nowy priorytet)
}


int PQsetPrior( PQueue* queue, int position, int nprior ) // ustawia nowy priorytet dla informacji uzytkownika bedacej na zadanej pozycji w kolejce
{
  if( PQisEmpty( queue ) )return PRIOR_ERROR;

  int temp = PQgetPrior( queue, position ); //biore sobie priorytet
  if( temp == POS_ERROR )return PRIOR_ERROR; //jesli zwroci mi PRIOR_ERROR to nie udalo sie pobrac priorytetu

  queue->pPQueue[position].nPrior = nprior; //ustawiam nowy priorytet bedacy na zadanej pozycji w kolejce
  if( nprior < temp )UpdateDown( position,PQSize( queue ) - 1, queue->pPQueue );//uaktualniam w gore

  else if( nprior > temp ) UpdateUp( 0, position, queue->pPQueue ); // -||- w dol 
  
  return temp;
}


int PQGetPrior( PQueue* queue,PQINFO* pInfo, int( __cdecl* comp )(const void*, const void*) ) //tak jak w PQSetPrior tylko ze ona zwraca a nie ustawia nowy priorytet
{

  return PQgetPrior( queue, PQFind( queue, comp, pInfo ) );
}


int PQgetPrior( PQueue* queue, int position ) //zwraca priorytet uzytkownika z zadanej pozycji w kolejce
{
  if( PQisEmpty( queue ) )
  {
    return PRIOR_ERROR;
  }
  if( position >= 0 || position >= PQSize( queue ) )
  {
    return queue->pPQueue[position].nPrior;
  }

  return POS_ERROR;
}


int PQFind( PQueue* queue, int( __cdecl* comp )(const void*, const void*), PQINFO* pInfo ) //zwraca indeks wyszukiwanej pozycji uzytkownika 
{
  if( PQisEmpty( queue ) )
  {
    return PRIOR_ERROR;
  }
  if( !comp )
  {
    return POS_ERROR;
  }
  for( int i = 0; i < PQSize(queue) ; i++ ) //zaczynam od nPQSize -1 ktory jest rozmiarem kolejki
  {
    if( !comp( pInfo, queue->pPQueue[i].pInfo ) )
      return i;
  }
  return POS_ERROR;
}



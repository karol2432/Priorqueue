// priorqueue.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "PQueue.h"
#include "global.h"

#define SIZE 20
void Printinfo( const void* pTabr );// naglowek funkcji drukujacej info uzytkownika (parametr typu const void* )
int comp( const void* elem1, const void* elem2 );// naglowek funkcji porownujacej info uzytkowniak. Zwraca -1 pierwszy mniejszy, 1 jesli pierwszy wiekszy else 0.
void freemem( const void* eleminfo );// naglowek funkcji zwalniajacej pamiec (parametr typu const void* )
PQINFO* createinf();

int main()
{
  
  PQueue* queue = PQCreate( SIZE );
  if( !queue )
  {
    printf( "blad podczas kreowania kolejki\n" );
    return 1;
  }
  //------------------
  PQINFO* pInfo1 = createinf();
  if( !pInfo1 )
  {
    printf( "blad podczas kreowania pInfo1\n" );
    return 2;
  }
  pInfo1->nKey = 3;
  pInfo1->pTab[0] = rand() % SIZE;
  //PQEnqueue( queue, 1, pInfo1 );
  if( !PQEnqueue( queue, 1, pInfo1 ) )
  {
    printf( "blad alokacji 1\n" );
    return 3;
  }

  PQINFO* pInfo2 = createinf();
  if( !pInfo2 )
  {
    printf( "blad podczas kreowania pInfo2\n" );
    return 4;
  }
  pInfo2->nKey = 4;
  pInfo2->pTab[0] = rand() % SIZE;           //  PQEnqueue( PQueue * queue, int nPrior, PQINFO * pInfo )
  if( !PQEnqueue( queue, 2, pInfo2 ) )
  {
    printf( "blad alokacji 2\n" );
    return 5;
  }
  

  PQINFO* pInfo3 = createinf();
  if( !pInfo3 )
  {
    printf( "blad podczas kreowania pInfo3\n" );
    return 6;
  }
  pInfo3->nKey = 5;
  pInfo3->pTab[0] = rand() % SIZE;
  if( !PQEnqueue( queue, 3, pInfo3 ) )
  {
    printf( "blad alokacji 3\n" );
    return 7;
  }

  PQINFO* pInfo4 = createinf();
  if( !pInfo4 )
  {
    printf( "blad podczas kreowania pInfo4\n" );
    return 8;
  }
  pInfo4->nKey = 22;
  pInfo4->pTab[0] = rand() % SIZE;
  if( !PQEnqueue( queue, 13, pInfo4 ) )
  {
    printf( "blad alokacji 4\n" );
    return 9;
  }

  PQINFO* pInfo5 = createinf();
  if( !pInfo5 )
  {
    printf( "blad podczas kreowania pInfo5\n" );
    return 10;
  }
  pInfo5->nKey = 9;
  pInfo5->pTab[0] = rand() % SIZE;
  if( !PQEnqueue( queue, 20, pInfo5 ) )
  {
    printf( "blad alokacji 5\n" );
    return 11;
  }

  PQINFO* pInfo6 = createinf();
  if( !pInfo6 )
  {
    printf( "blad podczas kreowania pInfo6\n" );
    return 12;
  }
  pInfo6->nKey = 15;
  pInfo6->pTab[0] = rand() % SIZE;
  if( !PQEnqueue( queue, 9, pInfo6 ) )
  {
    printf( "blad alokacji 6\n" );
    return 13;
  }

  //tu sobie dodam 6 elementow

  printf( "kolejka po dodaniu 6 elementow\n" );
  PQPrint( queue, Printinfo, 0 );
  printf( "\n\n" );
  //-------------------

  PQINFO* pInfo10 = PQDequeue( queue ); //sprawdzic czy sie powiodlo, jesli tak zwolnic pamiec swoja funkcja
  if( pInfo10 )
  {
    printf( "klucz najwyzszego priorytetu: % d \n\n", pInfo10->nKey );
    freemem( pInfo10 );
  }
  else
  {
    printf( "blad podczas usuwania najwyzszego priorytetu\n" );
    return 14;
  }

  printf( "kolejka po usunieciu elem z najwyzszym priorytetem:\n" );
  PQPrint( queue, Printinfo, 0 );
  printf( "\n\n" );
  //-------------------

  //tu sobie dodam dwa elementy
  PQINFO* pInfo7 = createinf();
  if( !pInfo7 )
  {
    printf( "blad podczas kreowania pInfo7\n" );
    return 15;
  }
  pInfo7->nKey = 10;
  pInfo7->pTab[0] = rand() % SIZE;
  if( !PQEnqueue( queue, 21, pInfo7 ) )
  {
    printf( "blad alokacji 7\n" );
    return 16;
  }

  PQINFO* pInfo8 = createinf();
  if( !pInfo8 )
  {
    printf( "blad podczas kreowania pInfo8\n" );
    return 17;
  }
  pInfo8->nKey = 14;
  pInfo8->pTab[0] = rand() % SIZE;
  if( !PQEnqueue( queue, 12, pInfo8 ) )
  {
    printf( "blad alokacji 8\n" );
    return 18;
  }
  
  printf( "kolejka po dodaniu dwoch elem:\n" );
  PQPrint( queue, Printinfo, 0 );
  printf( "\n\n" );
  //--------------------
  //szukam jednego elementu a nastepnie zwiekszam jego priorytet
  //PQsetPrior( queue, PQFind( queue, comp, pInfo3 ), 11 );
  if( !PQSetPrior( queue, comp, pInfo3, 11 ) ) //sprawdzic czy nie zwrocil priorerror!!!
  {
    printf( "blad podczas ustawiania priorytetu z zadana informacja\n" );
    return 19;
  }
  printf( "kolejka po wyszukaniu elem pInfo3 i ustawienia nowego priorytetu: 11\n" );
  PQPrint( queue, Printinfo, 0 );
  printf( "\n\n" );
  //---------------------
  //zmniejszam priorytet jednego elementu
  if( !PQSetPrior( queue, comp, pInfo5, 17 ) )
  {
    printf( "blad podczas ustawiania priorytetu z zadana informacja\n" );
    return 20;
  }
  
  printf( "kolejka po zmianie elem pInfo5 priorytetu:\n" );
  PQPrint( queue, Printinfo, 0 );
  printf( "\n\n" );
  //----------------------
  //kasuje kolejke
  PQRelease( &queue, freemem );


  return 0;
}

PQINFO* createinf() // tworze sobie ta  funkcje do tworzenia struktury PQINFO która ma informacje o elementach w kolejce  
{
  PQINFO* pInfo = ( PQINFO* )calloc( 1, sizeof( PQINFO ) );
  if( !pInfo )
  {
    return NULL;
  }
  pInfo->pTab = ( int* )calloc( 2, sizeof( int ) );
  if( !(pInfo->pTab) )
  {
    return NULL;
  }
  return pInfo;
}

void Printinfo( const void* pTabr )
{
  if( !pTabr )return;
  PQINFO* p = ( PQINFO* )pTabr;
  printf( "\nklucz: %d,  elem: %d", p->nKey, p->pTab[0] );
}

int comp( const void* elem1, const void* elem2)
{
  if( ( (PQINFO*)elem1 )->nKey > ( (PQINFO*)elem2 )->nKey )
  {
    return 1;
  }
  if( ( (PQINFO*)elem1 )->nKey < ( (PQINFO*)elem2 )->nKey )
  {
    return -1;
  }
  return 0;
}

void freemem( const void* eleminfo )
{
  free( ( (PQINFO*)eleminfo )->pTab );
  free( (PQINFO*)eleminfo );
}

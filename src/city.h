#pragma once

#include "road.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct RoadList RoadList;

///struktura przechowuj¹ca informacje o miastach
typedef struct City {
	char* name;
	int *temporaryData;
	RoadList *roads;
}City;

//struktura przechowuj¹ca listê miast
typedef struct CityList {
	City* c;
	struct CityList *next;
}CityList;

///struktura hash table dla miast
typedef struct CityHashTable {
	int a, b;
	int size;
	int numberOfElements;
	CityList **tab;
}CityHashTable;

////////////////lista/////////////

///usuwa listê
void cityListDelete(CityList *l);

///usuwa element r z listy miast
void cityListDeleteElement(CityList **l, City *r);

//zwalnia pamiêæ po elementach przechowywanych w liœcie
void cityListDeleteElements(CityList *l);

///dodaje element do listy
bool cityListAdd(CityList **l, City *c);

///dodaje na pocz¹tek listy l1 listê l2
void cityListAddList(CityList **l1, CityList *l2);

///zwraca wskaŸnik do szukanego miasta c lub NULL jeœli miasta nie ma na liœcie
City *cityListFind(CityList *l, City *c);

///zwraca wskaŸnik do szukanego miasta c lub NULL jeœli miasta nie ma na liœcie
City *cityListFindStr(CityList *l, const char *str);

/////////////hash table/////////////////////

//tworzy hash table z listy miast, zwraca NULL gdy nie uda siê utworzyæ struktury
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities);

///usuwa strukturê
void cityHashTableDelete(CityHashTable *t);

///dodaje do hash tablicy miasto o podanej nazwie jeœli jeszcze nie istnieje i zwraca wskaŸnik na nie, w przypadku b³êdu zwraca NULL
City *cityHashTableAdd(CityHashTable **t, const char *str);

//zwraca wskaŸnij do szukanego miasta lub NULL, gdy takie miasto nie istnieje
City *cityHashTableFind(CityHashTable *t, const char *str);

////////////////////inne//////////////////////

///zwraca hash podanego miasta
unsigned cityHash(City *c);

//³¹czy dwa miasta drog¹, zwraca true jeœli uda siê dodaæ drogê lub false jeœli nie
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear);

///usuwa miasto oraz prowadz¹ce do niego drogi
void cityDelete(City *c);

//usuwa zmienne tymczasowe z miasta i wszystkich miast do których mo¿na z niego dojœæ drogami
void cityDeleteTemporaryData(City *c);
#pragma once

#include "road.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct RoadList RoadList;

///struktura przechowuj¹ca informacje o miastach
typedef struct City {
	char* name;
	//unsigned distance;
	unsigned *temporaryData;
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


///usuwa listê
void cityListDelete(CityList *l);
///dodaje element do listy
bool cityListAdd(CityList **l, City *c);

///zwraca hash podanego miasta
unsigned cityHash(City *c);

//tworzy hash table z listy miast, zwraca NULL gdy nie uda siê utworzyæ struktury
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities);

///usuwa strukturê
void cityHashTableDelete(CityHashTable *t);

///dodaje do hash tablicy miasto o podanej nazwie jeœli jeszcze nie istnieje i zwraca wskaŸnik na nie, w przypadku b³êdu zwraca NULL
City *cityHashTableAdd(CityHashTable **t, const char *str);

//³¹czy dwa miasta drog¹, zwraca true jeœli uda siê dodaæ drogê lub false jeœli nie
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear);
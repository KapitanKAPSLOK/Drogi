#pragma once

#include "road.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct RoadList RoadList;

///struktura przechowuj�ca informacje o miastach
typedef struct City {
	char* name;
	//unsigned distance;
	unsigned *temporaryData;
	RoadList *roads;
}City;

//struktura przechowuj�ca list� miast
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


///usuwa list�
void cityListDelete(CityList *l);
///dodaje element do listy
bool cityListAdd(CityList **l, City *c);

///zwraca hash podanego miasta
unsigned cityHash(City *c);

//tworzy hash table z listy miast, zwraca NULL gdy nie uda si� utworzy� struktury
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities);

///usuwa struktur�
void cityHashTableDelete(CityHashTable *t);

///dodaje do hash tablicy miasto o podanej nazwie je�li jeszcze nie istnieje i zwraca wska�nik na nie, w przypadku b��du zwraca NULL
City *cityHashTableAdd(CityHashTable **t, const char *str);

//��czy dwa miasta drog�, zwraca true je�li uda si� doda� drog� lub false je�li nie
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear);
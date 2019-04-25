#pragma once
#include <stdbool.h>
#include <stdlib.h>
typedef struct CityTrie CityBST;

///struktura przechowuj�ca informacje o miastach
typedef struct City {
	char* name;
	unsigned distance;
	//RoadBST *roads;
}City;

///struktura hash table dla miast
typedef struct CityHashTable {
	int a, b;
	int size;
	CityList **tab;
}CityHashTable;

//struktura przechowuj�ca list� miast
typedef struct CityList {
	City* c;
	struct CityList *next;
	//struct CityList *prev;
}CityList;

///usuwa list�
void cityListDelete(CityList *l);
///dodaje element do listy
bool cityListAdd(CityList **l, City *c);

///zwraca hash podanego miasta
unsigned cityHash(City *c);

//tworzy hash table z listy miast
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities);
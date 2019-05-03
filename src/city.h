/** @file
 * Interfejs klasy miast
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 29.04.2019
 */
#pragma once

#include "road.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct RoadList RoadList;

///struktura przechowuj�ca informacje o miastach
typedef struct City {
	char* name; ///< nazwa miasta
	int *temporaryData; ///< miejsce, gdzie algorytmy korzystaj�ce ze struktury mog� umieszcza� dane tymczasowe
	RoadList *roads;  ///< lista odcink�w dr�g jakie wychodz� z miasta
}City;

///struktura przechowuj�ca list� miast
typedef struct CityList {
	City* c;
	struct CityList *next;
}CityList;

///struktura hash tablicy dla miast
typedef struct CityHashTable {
	int a, b;  ///< wsp�czynniki wykorzystywane przy hashowaniu
	int size;  ///< aktualny rozmiar tablicy tab na dane
	int numberOfElements; ///< ilo�� element�w przechowywana w tablicy
	CityList **tab; ///< tablica rozmiaru size przechowuj�ca listy element�w
}CityHashTable;

//////////////////cityList/////////////////

///usuwa list�
void cityListDelete(CityList *l);

///usuwa element r z listy miast
void cityListDeleteElement(CityList **l, City *r);

//zwalnia pami�� po elementach przechowywanych w li�cie
void cityListDeleteElements(CityList *l);

///dodaje element do listy
bool cityListAdd(CityList **l, City *c);

///dodaje na pocz�tek listy l1 list� l2
void cityListAddList(CityList **l1, CityList *l2);

///zwraca wska�nik do szukanego miasta c lub NULL je�li miasta nie ma na li�cie
City *cityListFind(CityList *l, City *c);

///zwraca wska�nik do szukanego miasta c lub NULL je�li miasta nie ma na li�cie
City *cityListFindStr(CityList *l, const char *str);

////////////////////cityHashTable/////////////////////////

//tworzy hash table z listy miast, zwraca NULL gdy nie uda si� utworzy� struktury
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities);

///usuwa struktur�
void cityHashTableDelete(CityHashTable *t);

///dodaje do hash tablicy miasto o podanej nazwie je�li jeszcze nie istnieje i zwraca wska�nik na nie, w przypadku b��du zwraca NULL
City *cityHashTableAdd(CityHashTable **t, const char *str);

//zwraca wska�nij do szukanego miasta lub NULL, gdy takie miasto nie istnieje
City *cityHashTableFind(CityHashTable *t, const char *str);

///////////////////////inne///////////////////////////

///zwraca hash podanego miasta
unsigned cityHash(City *c);

//��czy dwa miasta drog�, zwraca true je�li uda si� doda� drog� lub false je�li nie
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear);

///usuwa miasto oraz prowadz�ce do niego drogi
void cityDelete(City *c);

//usuwa zmienne tymczasowe z miasta i wszystkich miast do kt�rych mo�na z niego doj�� drogami
void cityDeleteTemporaryData(City *c);
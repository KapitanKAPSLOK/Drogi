#pragma once

#include "city.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct City City;
///struktura przechowuj�ca informacje o drogach
typedef struct Road {
	unsigned length;
	int modificationDate;
	City *city1, *city2;
}Road;

///struktura przechowuj�ca list� dr�g
typedef struct RoadList {
	Road* r;
	struct RoadList *next;
}RoadList;

///dodaje element na pocz�tek listy dr�g
bool roadListAdd(RoadList **l, Road *r);

///dodaje na pocz�tek listy l1 list� l2
void roadListAddList(RoadList **l1, RoadList *l2);

///usuwa list� dr�g
void roadListDelete(RoadList *l);

///usuwa element r z listy dr�g
void roadListDeleteElement(RoadList **l, Road *r);

///zwalnia pami�� po elementach przechowywanych w li�cie
void roadListDeleteElements(RoadList *r);

///zwraca wska�nik do drogi zawieraj�cej z miasta o nazwie str lub NULL je�li nie ma takiej drogi nie ma na li�cie
Road *roadListFindStr(RoadList *l, const char *str);

///odwraca list�
RoadList *roadListReverse(RoadList *l);

///zwraca true je�li kt�ra� z dr�g w li�cie l dochodzi do miasta c
Road *roadListContain(RoadList *l, City *c);


///zmienia rok ostatniej modyfikacji drogi
bool roadRepair(Road *r, int year);

///zwraca jedno z miast, kt�re ��czy droga, ale r�ne od miasta c
City *roadGetCity(Road *r, City *c);

///sprawdza czy dana droga dochodzi do podanego miasta
bool roadConnectCity(Road *r, City *c);

///zwraca informacje o drodze w formacie ";d�ugo��;data modyfikacji;miasto inne ni� c"
char *roadGetDescription(Road *r, City *c);
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

//dodaje element na pocz�tek listy dr�g
bool roadListAdd(RoadList **l, Road *r);

//usuwa list� dr�g
void roadListDelete(RoadList *l);

///usuwa element r z listy dr�g
void roadListDeleteElement(RoadList **l, Road *r);

///zwraca wska�nik do drogi zawieraj�cej z miasta o nazwie str lub NULL je�li nie ma takiej drogi nie ma na li�cie
Road *roadListFindStr(RoadList *l, const char *str);


///zmienia rok ostatniej modyfikacji drogi
bool roadRepair(Road *r, int year);
#pragma once

#include "city.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct City City;
///struktura przechowuj¹ca informacje o drogach
typedef struct Road {
	unsigned length;
	int modificationDate;
	City *city1, *city2;
}Road;

///struktura przechowuj¹ca listê dróg
typedef struct RoadList {
	Road* r;
	struct RoadList *next;
}RoadList;

///dodaje element na pocz¹tek listy dróg
bool roadListAdd(RoadList **l, Road *r);

///dodaje na pocz¹tek listy l1 listê l2
void roadListAddList(RoadList **l1, RoadList *l2);

///usuwa listê dróg
void roadListDelete(RoadList *l);

///usuwa element r z listy dróg
void roadListDeleteElement(RoadList **l, Road *r);

///zwalnia pamiêæ po elementach przechowywanych w liœcie
void roadListDeleteElements(RoadList *r);

///zwraca wskaŸnik do drogi zawieraj¹cej z miasta o nazwie str lub NULL jeœli nie ma takiej drogi nie ma na liœcie
Road *roadListFindStr(RoadList *l, const char *str);

///odwraca listê
RoadList *roadListReverse(RoadList *l);

///zwraca true jeœli któraœ z dróg w liœcie l dochodzi do miasta c
Road *roadListContain(RoadList *l, City *c);


///zmienia rok ostatniej modyfikacji drogi
bool roadRepair(Road *r, int year);

///zwraca jedno z miast, które ³¹czy droga, ale ró¿ne od miasta c
City *roadGetCity(Road *r, City *c);

///sprawdza czy dana droga dochodzi do podanego miasta
bool roadConnectCity(Road *r, City *c);

///zwraca informacje o drodze w formacie ";d³ugoœæ;data modyfikacji;miasto inne ni¿ c"
char *roadGetDescription(Road *r, City *c);
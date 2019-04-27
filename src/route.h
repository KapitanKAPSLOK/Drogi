#pragma once

#include "road.h"

///struktura przechowuj�ca informacj� o drogach krajowych
typedef struct Route {
	unsigned id;
	RoadList *roads;
}Route;

///struktura przechowuj�ca list� dr�g
typedef struct RouteList {
	Route* r;
	struct RouteList *next;
}RouteList;


///tworzy now� drog� krajow�, je�li nie uda si� jej utworzy� zwraca NULL
Route *routeMake(unsigned nr, RoadList *roads);


///dodaje element na pocz�tek listy dr�g krajowych
bool routeListAdd(RouteList **l, Route *r);

///usuwa list� dr�g krajowych
void routeListDelete(RouteList *l);

///usuwa element r z listy dr�g krajowych
void routeListDeleteElement(RouteList **l, Route *r);

///zwraca drog� krajow� o podanym numerze lub NULL je�li nie ma takiej na li�cie
Route *routeListFind(RouteList *l, unsigned nr);

///zwraca najkr�tszy ci�g dr�g mi�dzy miastami, je�li nie istnieje lub w przypadku b��du zwraca NULL
RoadList *connectCities(CityHashTable *t, const char *c1, const char *c2);
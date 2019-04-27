#pragma once

#include "road.h"

///struktura przechowuj¹ca informacjê o drogach krajowych
typedef struct Route {
	unsigned id;
	RoadList *roads;
}Route;

///struktura przechowuj¹ca listê dróg
typedef struct RouteList {
	Route* r;
	struct RouteList *next;
}RouteList;


///tworzy now¹ drogê krajow¹, jeœli nie uda siê jej utworzyæ zwraca NULL
Route *routeMake(unsigned nr, RoadList *roads);


///dodaje element na pocz¹tek listy dróg krajowych
bool routeListAdd(RouteList **l, Route *r);

///usuwa listê dróg krajowych
void routeListDelete(RouteList *l);

///usuwa element r z listy dróg krajowych
void routeListDeleteElement(RouteList **l, Route *r);

///zwraca drogê krajow¹ o podanym numerze lub NULL jeœli nie ma takiej na liœcie
Route *routeListFind(RouteList *l, unsigned nr);

///zwraca najkrótszy ci¹g dróg miêdzy miastami, jeœli nie istnieje lub w przypadku b³êdu zwraca NULL
RoadList *connectCities(CityHashTable *t, const char *c1, const char *c2);
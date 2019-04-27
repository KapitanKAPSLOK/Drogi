#pragma once

#include "road.h"

///struktura przechowuj�ca informacj� o drogach krajowych
typedef struct Route {
	unsigned id;
	RoadList *r;
}Route;

///struktura przechowuj�ca list� dr�g
typedef struct RouteList {
	Route* r;
	struct RouteList *next;
}RouteList;


//dodaje element na pocz�tek listy dr�g krajowych
bool routeListAdd(RouteList **l, Route *r);

//usuwa list� dr�g krajowych
void routeListDelete(RouteList *l);

///usuwa element r z listy dr�g krajowych
void routeListDeleteElement(RouteList **l, Route *r);
#pragma once

#include "road.h"

///struktura przechowuj¹ca informacjê o drogach krajowych
typedef struct Route {
	unsigned id;
	RoadList *r;
}Route;

///struktura przechowuj¹ca listê dróg
typedef struct RouteList {
	Route* r;
	struct RouteList *next;
}RouteList;


//dodaje element na pocz¹tek listy dróg krajowych
bool routeListAdd(RouteList **l, Route *r);

//usuwa listê dróg krajowych
void routeListDelete(RouteList *l);

///usuwa element r z listy dróg krajowych
void routeListDeleteElement(RouteList **l, Route *r);
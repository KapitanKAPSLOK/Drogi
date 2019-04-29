#pragma once

#include "road.h"

///struktura przechowuj�ca informacj� o drogach krajowych
typedef struct Route {
	City *start,*end;
	unsigned id;
	RoadList *roads;
}Route;

///struktura przechowuj�ca list� dr�g
typedef struct RouteList {
	Route* r;
	struct RouteList *next;
}RouteList;


///tworzy now� drog� krajow�, je�li nie uda si� jej utworzy� zwraca NULL
Route *routeMake(unsigned nr, RoadList *roads, City *start, City *end);


///dodaje element na pocz�tek listy dr�g krajowych
bool routeListAdd(RouteList **l, Route *r);

///usuwa list� dr�g krajowych
void routeListDelete(RouteList *l);

///usuwa element r z listy dr�g krajowych
void routeListDeleteElement(RouteList **l, Route *r);

///zwraca drog� krajow� o podanym numerze lub NULL je�li nie istnieje
Route *routeListFind(RouteList *l, unsigned nr);

///zwraca drog� krajow� o podanym numerze lub NULL je�li nie ma takiej na li�cie
Route *routeListFind(RouteList *l, unsigned nr);

///tworzy list� z miast przez kt�re przechodzi droga krajowa
CityList *routeMakeCityList(Route *r);

///zwraca najkr�tszy ci�g dr�g mi�dzy miastami, je�li nie istnieje lub w przypadku b��du zwraca NULL
RoadList *connectCities(CityHashTable *t, const char *c1, const char *c2);

///zwraca najkr�tszy ci�g dr�g mi�dzy miastami, je�li nie istnieje lub w przypadku b��du zwraca NULL
RoadList *connectCitiesExtend(City *start, City *end, CityList *cities, bool notDirectly);

///sprawdza czy mo�na usun�� odcinek drogi tak aby mo�na by�o naprawi� drog� krajow�
bool routeCanExtend(Route *r, City *c1, City *c2);

///dodaje do drogi krajowej najkr�tsz� �cie�k� ��cz�c� c1 i c2, ale nie bezpo�rednio
void routeFix(Route *r, City *c1, City *c2);
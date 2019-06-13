#include "map.h"
#include "city.h"
#include "route.h"
#include "road.h"
#include "myString.h"

#include <string.h>

///Struktura przechowująca mapę dróg krajowych.
typedef struct Map {
	RouteList *routes;
	CityHashTable *cities;
}Map;

//Tworzy nową strukturę.
Map* newMap(void) {
	Map *m = malloc(sizeof(*m));
	if (m == NULL) return NULL;
	m->routes = NULL;
	//tworzy nową hash tablicę z miejscem dla domyślnej liczby 16 elementów
	if(!(m->cities=cityHashTableMake(NULL, 16))){
		//nie udało się zaalokować tyle pamięci, próba zaalokowania mniejszej ilości
		if (!(m->cities = cityHashTableMake(NULL, 3))) {
			free(m);
			return NULL;
		}
	}
	//udało się zainicjować wszystkie struktury mapy
	return m;
}

//Usuwa strukturę.
void deleteMap(Map *map) {
	routeListDelete(map->routes);
	cityHashTableDelete(map->cities);
	free(map);
}

//Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
bool addRoad(Map *map, const char *city1, const char *city2, unsigned length, int builtYear) {
	if (!cityCheckName(city1) || !cityCheckName(city2)) return false; //niepoprawna nazwa miasta

	if(!strcmp(city1, city2)) return false; //nazwy są identyczne
	if (builtYear == 0) return false; //zły rok
	if (length <= 0) return false; //niepoprawna długość drogi
	City *c1 = cityHashTableAdd(&(map->cities), city1);
	if (c1 == NULL) return false; //nie udało się dodać miasta city1
	City *c2 = cityHashTableAdd(&(map->cities), city2);
	if (c2 == NULL) return false; //nie udało się dodać miasta city2

	//TODO:usuwanie pierwszego miasta jak nie uda się dodać drugiego

	if (!cityAddRoad(c1, c2, length, builtYear)) return false;
	return true;
}

//Modyfikuje rok ostatniego remontu odcinka drogi.
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
	if (!strcmp(city1, city2)) return false; //miasta są identyczne, nie może być między nimi drogi
	City *c = cityHashTableFind(map->cities, city1);
	if (c == NULL) return false; //nie ma takiego miasta
	Road *r=roadListFindStr(c->roads, city2);
	if (r == NULL) return false; //nie ma takiej drogi
	return roadRepair(r, repairYear);
}

//Łączy dwa różne miasta drogą krajową.
bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
	if (map == NULL) return false;
	if (!strcmp(city1, city2)) return false; //nazwy miast są takie same
	if (routeId < 1 || routeId>999) return false; //niepoprawny identyfikator drogi krajowej
	if (routeListFind(map->routes, routeId)) return false; //droga krajowa o podanym numerze już istnieje
	City *c1 = cityHashTableFind(map->cities, city1);
	City *c2 = cityHashTableFind(map->cities, city2);
	RoadList *r = connectCities(c1, c2, NULL, false);
	if (r == NULL) return false; //nie udało się wyznaczyć jednoznacznie najkrótszej drogi między miastami
	Route *route = routeMake(routeId, r, c1, c2);
	if (route == NULL) return false; //nie udało się utworzyć drogi krajowej
	if(routeListAdd(&(map->routes), route)) return true;
	return false;
}

//Wydłuża drogę krajową do podanego miasta.
bool extendRoute(Map *map, unsigned routeId, const char *city) {
	Route *r = routeListFind(map->routes,routeId);
	if (r == NULL) return false; //nie ma drogi krajowej o podanym numerze
	City *c = cityHashTableFind(map->cities, city);
	if (c == NULL) return false; //miasto o podanej nazwie nie istnieje
	if (roadListContain(r->roads, c)) return false; //droga krajowa przebiega już przez to miasto
	CityList *cit = routeMakeCityList(r);
	cityListDeleteElement(&cit, r->end);
	RoadList *extendRoads = connectCities(r->end, c, cit, false);
	if (extendRoads == NULL) return false; //nie udało się przedłużyć drogi
	roadListAddList(&(r->roads), extendRoads);
	r->end = c;
	return true;
}

//Usuwa odcinek drogi między dwoma różnymi miastami.
bool removeRoad(Map *map, const char *city1, const char *city2) {
	City *c1 = cityHashTableFind(map->cities, city1);
	City *c2 = cityHashTableFind(map->cities, city2);
	if (c1 == NULL || c2 == NULL) return false; //któreś z podanych miast nie istnieje
	if(!roadListFindStr(c1->roads, city2)) return false; //sprawdzanie czy miasta sąpołączone
	RouteList *route = map->routes;
	while (route != NULL) {
		if (!routeCanChange(route->r, c1, c2)) return false;
		route = route->next;
	}
	//wszystkie drogi krajowe zostały sprawdzone, można zacząć je zmieniać
	Road *r = roadListContain(c1->roads, c2);
	route = map->routes;
	while (route != NULL) {
		routeFix(route->r, c1, c2);
		roadListDeleteElement(&(route->r->roads), r);
		route = route->next;
	}
	//drogi krajowe mają zmienione trasy, można bezpiecznie usunąć odcinek drogi
	roadListDeleteElement(&(c1->roads), r);
	roadListDeleteElement(&(c2->roads), r);
	free(r);

	return true;
}

//Udostępnia informacje o drodze krajowej.
char const* getRouteDescription(Map *map, unsigned routeId) {
	Route *r = routeListFind(map->routes, routeId);
	char *str=malloc(sizeof(str));
	if (str == NULL) return NULL; //brak pamięci
	*str = '\0';
	if (r == NULL) return str; //nie istnieje droga krajowa o podanym numerze
	if (!(str=myStringAppendInt(str, routeId))) return NULL; //nie udało się zaalokować pamięci
	RoadList *l = r->roads;
	if (l == NULL) return str;
	l=roadListReverse(l); //drogi przechowywane są w odwrotnej kolejności
	RoadList *start = l; //przed wyjściem z funkcji należy ponownie odwórić listę
	City *c=r->start;
	if (!(str = myStringAppendString(str, ";"))) {
		roadListReverse(start);
		return NULL;
	}
	if (!(str = myStringAppendString(str, c->name))) {
		roadListReverse(start);
		return NULL;
	}
	char *temp;
	while (l != NULL) {
		temp = roadGetDescription(l->r, c);
		c = roadGetCity(l->r, c);
		if (temp == NULL) { //zabrakło pamięci
			roadListReverse(start);
			free(str); //zwalnianie dotychczas zaalokowanej pamięci
			return NULL; 
		}
		if (!(str = myStringAppendString(str, temp))) {
			free(temp);
			roadListReverse(start);
			return NULL;
		}
		free(temp);
		l = l->next;
	}
	roadListReverse(start); //trzeba naprawić odwróconą listę
	return str;
}

//dodaje odcinek drogi do istniejącej drogi krajowej
bool addToRoute(Map *map, unsigned routeId, const char *city, unsigned length, int year) {
	if (map == NULL || city == NULL) return false;
	Route *r = routeListFind(map->routes, routeId);
	if (r == NULL) return false;
	Road *road = roadListFindStr(r->end->roads, city);
	if (road == NULL) {
		//nie ma odcinka drogi do miasta o podanej nazwie, trzeba go stworzyć
		if (!addRoad(map, r->end->name, city, length, year)) return false;
		road = roadListFindStr(r->end->roads, city);
	}
	else {
		if (road->length != length) return false; //długość odcinka drogi się nie zgadza
		if (!roadRepair(road, year)) return false; //zła data remontu
	}

	if (!roadListAdd(&(r->roads), road)) return false; //nie udało się dodać odcinka drogi do drogi krajowej
	r->end = roadGetCity(road, r->end);
	return true;
}

//tworzy nową drogę krajową między podanymi miastami łącząc je bezpośrenio odcinkiem drogi
bool makeRoute(Map *map, unsigned routeId, const char *city1, const char *city2, unsigned length, int year) {
	if (routeId < 1 || routeId>999) return false; //niepoprawny identyfikator drogi krajowej
	if (!strcmp(city1, city2)) return false; //nazwy miast są takie same
	if (routeListFind(map->routes, routeId)) return false; //droga krajowa o podanym numerze już istnieje
	addRoad(map, city1, city2, length, year);
	if (!repairRoad(map, city1, city2, year)) return false;
	City *c = cityHashTableFind(map->cities, city1);
	if (c == NULL) return false; //miasto powinno być dodane w addRoad, ale mogło zabraknąć pamięci
	Road *road = roadListFindStr(c->roads, city2);
	if (road == NULL) return false;
	RoadList *l=NULL;
	if (!roadListAdd(&l, road)) return false;
	Route *route = routeMake(routeId, l, c, cityHashTableFind(map->cities, city2));
	if (route == NULL) {
		return false;
	}
	if (!routeListAdd(&(map->routes), route)) return false; //nie udało się dodać drogi krajowej do mapy
	return true;
}

//usuwa drogę krajową o podanym numerze, nie usuwa zawartych w niej odcinków dróg i miast
bool removeRoute(Map *map, unsigned routeId) {
	Route *r = routeListFind(map->routes, routeId);
	if (r == NULL) return false; //nie istnieje droga krajowa o podanym numerze
	routeListDeleteElement(&(map->routes), r);
	roadListDelete(r->roads);
	free(r);
	return true;
}
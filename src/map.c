#include "map.h"
#include "city.h"
#include "route.h"
#include "road.h"
#include "myString.h"

#include <string.h>

///Struktura przechowuj�ca map� dr�g krajowych.
typedef struct Map {
	RouteList *routes;
	CityHashTable *cities;
}Map;

//Tworzy now� struktur�.
Map* newMap(void) {
	Map *m = malloc(sizeof(*m));
	if (m == NULL) return NULL;
	m->routes = NULL;
	//tworzy now� hash tablic� z miejscem dla domy�lnej liczby 16 element�w
	if(!(m->cities=cityHashTableMake(NULL, 16))){
		//nie uda�o si� zaalokowa� tyle pami�ci, pr�ba zaalokowania mniejszej ilo�ci
		if (!(m->cities = cityHashTableMake(NULL, 3))) {
			free(m);
			return NULL;
		}
	}
	//uda�o si� zainicjowa� wszystkie struktury mapy
	return m;
}

//Usuwa struktur�.
void deleteMap(Map *map) {
	routeListDelete(map->routes);
	cityHashTableDelete(map->cities);
	free(map);
}

//Dodaje do mapy odcinek drogi mi�dzy dwoma r�nymi miastami.
bool addRoad(Map *map, const char *city1, const char *city2, unsigned length, int builtYear) {
	if (!cityCheckName(city1) || !cityCheckName(city2)) return false; //niepoprawna nazwa miasta

	if(!strcmp(city1, city2)) return false; //nazwy s� identyczne
	if (builtYear == 0) return false; //z�y rok
	if (length <= 0) return false; //niepoprawna d�ugo�� drogi
	City *c1 = cityHashTableAdd(&(map->cities), city1);
	if (c1 == NULL) return false; //nie uda�o si� doda� miasta city1
	City *c2 = cityHashTableAdd(&(map->cities), city2);
	if (c2 == NULL) return false; //nie uda�o si� doda� miasta city2

	//TODO:usuwanie pierwszego miasta jak nie uda si� doda� drugiego

	if (!cityAddRoad(c1, c2, length, builtYear)) return false;
	return true;
}

//Modyfikuje rok ostatniego remontu odcinka drogi.
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
	if (!strcmp(city1, city2)) return false; //miasta s� identyczne, nie mo�e by� mi�dzy nimi drogi
	City *c = cityHashTableFind(map->cities, city1);
	if (c == NULL) return false; //nie ma takiego miasta
	Road *r=roadListFindStr(c->roads, city2);
	if (r == NULL) return false; //nie ma takiej drogi
	return roadRepair(r, repairYear);
}

#include <stdio.h>
//��czy dwa r�ne miasta drog� krajow�.
bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
	if (map == NULL) return false;
	if (!strcmp(city1, city2)) return false; //nazwy miast s� takie same
	if (routeId < 1 || routeId>999) return false; //niepoprawny identyfikator drogi krajowej
	if (routeListFind(map->routes, routeId)) return false; //droga krajowa o podanym numerze ju� istnieje
	City *c1 = cityHashTableFind(map->cities, city1);
	City *c2 = cityHashTableFind(map->cities, city2);
	RoadList *r = connectCities(c1, c2, NULL, false);
	if (r == NULL) return false; //nie uda�o si� wyznaczy� jednoznacznie najkr�tszej drogi mi�dzy miastami
	Route *route = routeMake(routeId, r, c1, c2);
	if (route == NULL) return false; //nie uda�o si� utworzy� drogi krajowej
	if(routeListAdd(&(map->routes), route)) return true;
	return false;
}

//Wyd�u�a drog� krajow� do podanego miasta.
bool extendRoute(Map *map, unsigned routeId, const char *city) {
	Route *r = routeListFind(map->routes,routeId);
	if (r == NULL) return false; //nie ma drogi krajowej o podanym numerze
	City *c = cityHashTableFind(map->cities, city);
	if (c == NULL) return false; //miasto o podanej nazwie nie istnieje
	if (roadListContain(r->roads, c)) return false; //droga krajowa przebiega ju� przez to miasto
	CityList *cit = routeMakeCityList(r);
	cityListDeleteElement(&cit, r->end);
	RoadList *extendRoads = connectCities(r->end, c, cit, false);
	if (extendRoads == NULL) return false; //nie uda�o si� przed�u�y� drogi
	roadListAddList(&(r->roads), extendRoads);
	r->end = c;
	return true;
}

//Usuwa odcinek drogi mi�dzy dwoma r�nymi miastami.
bool removeRoad(Map *map, const char *city1, const char *city2) {
	City *c1 = cityHashTableFind(map->cities, city1);
	City *c2 = cityHashTableFind(map->cities, city2);
	if (c1 == NULL || c2 == NULL) return false; //kt�re� z podanych miast nie istnieje
	RouteList *route = map->routes;
	while (route != NULL) {
		if (!routeCanChange(route->r, c1, c2)) return false;
		route = route->next;
	}
	//wszystkie drogi krajowe zosta�y sprawdzone, mo�na zacz�� je zmienia�
	Road *r = roadListContain(c1->roads, c2);
	route = map->routes;
	while (route != NULL) {
		routeFix(route->r, c1, c2);
		roadListDeleteElement(&(route->r->roads), r);
		route = route->next;
	}
	//drogi krajowe maj� zmienione trasy, mo�na bezpiecznie usun�� odcinek drogi
	roadListDeleteElement(&(c1->roads), r);
	roadListDeleteElement(&(c2->roads), r);
	free(r);

	return true;
}


//Udost�pnia informacje o drodze krajowej.
char const* getRouteDescription(Map *map, unsigned routeId) {
	Route *r = routeListFind(map->routes, routeId);
	char *str=malloc(sizeof(str));
	if (str == NULL) return NULL; //brak pami�ci
	*str = '\0';
	if (r == NULL) return str; //nie istnieje droga krajowa o podanym numerze
	if (!(str=myStringAppendInt(str, routeId))) return NULL; //nie uda�o si� zaalokowa� pami�ci
	RoadList *l = r->roads;
	if (l == NULL) return str;
	l=roadListReverse(l); //drogi przechowywane s� w odwrotnej kolejno�ci
	RoadList *start = l; //przed wyj�ciem z funkcji nale�y ponownie odw�ri� list�
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
		if (temp == NULL) { //zabrak�o pami�ci
			roadListReverse(start);
			free(str); //zwalnianie dotychczas zaalokowanej pami�ci
			return NULL; 
		}
		if (!(str = myStringAppendString(str, temp))) {
			roadListReverse(start);
			return NULL;
		}
		l = l->next;
	}
	roadListReverse(start); //trzeba naprawi� odwr�con� list�
	return str;
}
#include "map.h"
#include "city.h"
#include "route.h"
#include "road.h"
#include "myString.h"

#include <string.h>

///Struktura przechowuj¹ca mapê dróg krajowych.
typedef struct Map {
	RouteList *routes;
	CityHashTable *cities;
}Map;

//Tworzy now¹ strukturê.
Map* newMap(void) {
	Map *m = malloc(sizeof(*m));
	if (m == NULL) return NULL;
	//tworzy now¹ hash tablicê z miejscem dla domyœlnej liczby 16 elementów
	if(!(m->cities=cityHashTableMake(NULL, 16))){
		//nie uda³o siê zaalokowaæ tyle pamiêci, próba zaalokowania mniejszej iloœci
		if (!(m->cities = cityHashTableMake(NULL, 3))) {
			free(m);
			return NULL;
		}
	}
	//uda³o siê zainicjowaæ wszystkie struktury mapy
	return m;
}

//Usuwa strukturê.
void deleteMap(Map *map) {
	routeListDelete(map->routes);
	cityHashTableDelete(map->cities);
	free(map);
}

//Dodaje do mapy odcinek drogi miêdzy dwoma ró¿nymi miastami.
bool addRoad(Map *map, const char *city1, const char *city2, unsigned length, int builtYear) {
	if (city1 == NULL || city2 == NULL) return false; //niepoprawna nazwa miasta
	if(!strcmp(city1, city2)) return false; //nazwy s¹ identyczne
	if (builtYear == 0) return false; //z³y rok
	if (length <= 0) return false; //niepoprawna d³ugoœæ drogi
	City *c1 = cityHashTableAdd(&(map->cities), city1);
	if (c1 == NULL) return false; //nie uda³o siê dodaæ miasta city1
	City *c2 = cityHashTableAdd(&(map->cities), city2);
	if (c2 == NULL) return false; //nie uda³o siê dodaæ miasta city2

	//TODO:usuwanie pierwszego miasta jak nie uda siê dodaæ drugiego

	if (!cityAddRoad(c1, c2, length, builtYear)) return false;
	return true;
}

//Modyfikuje rok ostatniego remontu odcinka drogi.
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
	if (!strcmp(city1, city2)) return false; //miasta s¹ identyczne, nie mo¿e byæ miêdzy nimi drogi
	City *c = cityHashTableFind(map->cities, city1);
	if (c == NULL) return false; //nie ma takiego miasta
	Road *r=roadListFindStr(c->roads, city2);
	if (r == NULL) return false; //nie ma takiej drogi
	return roadRepair(r, repairYear);
}

//£¹czy dwa ró¿ne miasta drog¹ krajow¹.
bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
	if (map == NULL) return false;
	if (!strcmp(city1, city2)) return false; //nazwy miast s¹ takie same
	if (routeId < 1 || routeId>999) return false; //niepoprawny identyfikator drogi krajowej
	if (routeListFind(map->routes, routeId)) return false; //droga krajowa o podanym numerze ju¿ istnieje
	RoadList *r = connectCities(map->cities, city1, city2);
	if (r == NULL) return false; //nie uda³o siê wyznaczyæ jednoznacznie najkrótszej drogi miêdzy miastami
	Route *route = routeMake(routeId, r, cityHashTableFind(map->cities,city1), cityHashTableFind(map->cities, city2));
	if (route == NULL) return false; //nie uda³o siê utworzyæ drogi krajowej
	if(routeListAdd(&(map->routes), route)) return true;
	return false;
}

//Wyd³u¿a drogê krajow¹ do podanego miasta.
bool extendRoute(Map *map, unsigned routeId, const char *city) {
	Route *r = routeListFind(map->routes,routeId);
	if (r == NULL) return false; //nie ma drogi krajowej o podanym numerze
	City *c = cityHashTableFind(map->cities, city);
	if (c == NULL) return false; //miasto o podanej nazwie nie istnieje
	if (roadListContain(r->roads, c)) return false; //droga krajowa przebiega ju¿ przez to miasto
	CityList *cit = routeMakeCityList(r);
	RoadList *extendRoads = connectCitiesExtend(r->end, c, cit, false);
	if (extendRoads == NULL) return false; //nie uda³o siê przed³u¿yæ drogi
	roadListAddList(&(r->roads), extendRoads);
	r->end = c;
	return true;
}

//Usuwa odcinek drogi miêdzy dwoma ró¿nymi miastami.
bool removeRoad(Map *map, const char *city1, const char *city2) {
	City *c1 = cityHashTableFind(map->cities, city1);
	City *c2 = cityHashTableFind(map->cities, city2);
	if (c1 == NULL || c2 == NULL) return false; //któreœ z podanych miast nie istnieje
	RouteList *route = map->routes;
	while (route != NULL) {
		if (!routeCanExtend(route->r, c1, c2)) return false;
		route = route->next;
	}
	//wszystkie drogi krajowe zosta³y sprawdzone, mo¿na zacz¹æ je zmieniaæ
	Road *r = roadListContain(c1->roads, c2);
	while (route != NULL) {
		routeFix(route->r, c1, c2);
		roadListDeleteElement(&(route->r->roads), r);
		route = route->next;
	}
	//drogi krajowe maj¹ zmienione trasy, mo¿na bezpiecznie usun¹æ odcinek drogi
	roadListDeleteElement(&(c1->roads), r);
	roadListDeleteElement(&(c2->roads), r);
	free(r);

	return true;
}

//Udostêpnia informacje o drodze krajowej.
char const* getRouteDescription(Map *map, unsigned routeId) {
	Route *r = routeListFind(map->routes, routeId);
	if (r == NULL) return NULL; //nie istnieje droga krajowa o podanym numerze
	char *str = "";
	if (!myStringAppendInt(str, routeId)) return NULL; //nie uda³o siê zaalokowaæ pamiêci
	RoadList *l = r->roads;
	if (l == NULL) return str;
	l=roadListReverse(l); //drogi przechowywane s¹ w odwrotnej kolejnoœci
	City *c=r->start;
	if (!myStringAppendString(str, ";")) return NULL;
	if (!myStringAppendString(str, c->name)) return NULL;
	while (l != NULL) {
		if (!roadGetDescription(l->r, c)) return NULL; //jeœi prawdziwe znaczy zabrak³o pamiêci
		c = roadGetCity(l->r, c);
		l = l->next;
	}
	return str;
}
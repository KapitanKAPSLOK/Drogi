#include "map.h"
#include "city.h"
#include "route.h"
#include "road.h"

#include <string.h>

struct Map {
	RouteList *routes;
	CityHashTable *cities;
};

//Tworzy now� struktur�.
Map* newMap(void) {
	Map *m = malloc(sizeof(*m));
	if (m == NULL) return NULL;
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
	if (city1 == NULL || city2 == NULL) return false; //niepoprawna nazwa miasta
	if(strcmp(city1, city2)) return false; //nazwy s� identyczne
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

////Modyfikuje rok ostatniego remontu odcinka drogi.
//bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
//	//TODO:
//}
//
////��czy dwa r�ne miasta drog� krajow�.
//bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
//	//TODO
//}
//
////Wyd�u�a drog� krajow� do podanego miasta.
//bool extendRoute(Map *map, unsigned routeId, const char *city) {
//	//TODO
//}
//
////Usuwa odcinek drogi mi�dzy dwoma r�nymi miastami.
//bool removeRoad(Map *map, const char *city1, const char *city2) {
//	//TODO
//}
//
////Udost�pnia informacje o drodze krajowej.
//char const* getRouteDescription(Map *map, unsigned routeId) {
//	//TODO
//}
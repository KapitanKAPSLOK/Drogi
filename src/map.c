#include "map.h"
#include "city.h"
#include "route.h"
#include "road.h"

#include <string.h>

struct Map {
	RouteList *routes;
	CityHashTable *cities;
};

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
	if(strcmp(city1, city2)) return false; //nazwy s¹ identyczne
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

////Modyfikuje rok ostatniego remontu odcinka drogi.
//bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
//	//TODO:
//}
//
////£¹czy dwa ró¿ne miasta drog¹ krajow¹.
//bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
//	//TODO
//}
//
////Wyd³u¿a drogê krajow¹ do podanego miasta.
//bool extendRoute(Map *map, unsigned routeId, const char *city) {
//	//TODO
//}
//
////Usuwa odcinek drogi miêdzy dwoma ró¿nymi miastami.
//bool removeRoad(Map *map, const char *city1, const char *city2) {
//	//TODO
//}
//
////Udostêpnia informacje o drodze krajowej.
//char const* getRouteDescription(Map *map, unsigned routeId) {
//	//TODO
//}
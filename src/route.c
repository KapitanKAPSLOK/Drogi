#include "route.h"
#include "road.h"
#include "city.h"
#include "min_heap.h"

#include <limits.h>

int min(int a, int b) {
	return (a < b) ? a : b;
}

//tworzy now¹ drogê krajow¹, jeœli nie uda siê jej utworzyæ zwraca NULL
Route *routeMake(unsigned nr, RoadList *roads, City *start, City *end) {
	Route *r = malloc(sizeof(*r));
	if (r == NULL) return NULL;
	r->id = nr;
	r->roads = roads;
	r->start = start;
	r->end = end;
	return r;
}

//tworzy listê z miast przez które przechodzi droga krajowa
CityList *routeMakeCityList(Route *r) {
	CityList *l;
	if (!cityListAdd(&l, r->end)) {
		//nie uda³o siê dodaæ elementu do listy
		cityListDelete(l);
		return NULL;
	}
	RoadList *road = r->roads;
	City *c = r->end;
	while (road != NULL) {
		if (!cityListAdd(&l, roadGetCity(road->r,c))) {
			//nie uda³o siê dodaæ elementu do listy
			cityListDelete(l);
			return NULL;
		}
		c = roadGetCity(road->r, c);
		road = road->next;
	}
	return l;
}

//usuwa listê dróg krajowych
void routeListDelete(RouteList *l) {
	if (l == NULL)return;
	RouteList *p = l->next;
	RouteList *temp;
	while (p != NULL) {
		temp = p->next;
		free(p);
		p = temp;
	}
	free(l);
}

//dodaje element na pocz¹tek listy dróg krajowych
bool routeListAdd(RouteList **l, Route *r) {
	RouteList *temp;
	temp = malloc(sizeof(*temp));
	if (temp == NULL) return false;
	if (*l != NULL) temp->next = *l;
	else temp->next = NULL;
	temp->r = r;
	*l = temp;
	return true;
}

///usuwa element r z listy dróg krajowych
void routeListDeleteElement(RouteList **l, Route *r) {
	if (l == NULL || r == NULL) return;
	if ((*l)->r == r) { //element r znajduje siê na pocz¹tku
		RouteList *temp = *l;
		*l = (*l)->next;
		free(temp);
	}
	while ((*l)->next != NULL) {
		if ((*l)->next->r == r) {
			RouteList *temp = (*l)->next;
			(*l)->next = (*l)->next->next;
			free(temp);
			return;
		}
	}
	return;
}

//zwraca drogê krajow¹ o podanym numerze lub NULL jeœli nie ma takiej na liœcie
Route *routeListFind(RouteList *l, unsigned nr) {
	while (l != NULL) {
		if (l->r->id == nr) return l->r;
	}
	return NULL;
}

//alokuje rekurencyjnie pamiêæ pod zmienn¹ temporaryData potrzebn¹ podczas wyszukiwania œcie¿ki miêdzy mistami
bool prepareCities(City *c) {
	if (c->temporaryData != NULL) return true; //to miasto ju¿ sota³o przygotowane
	//miasto nie by³o jeszcze odwiedzone, alokacja dwóch pól na dane i inicjalizacja ich
	c->temporaryData = malloc(2 * sizeof(*(c->temporaryData)));
	if (c->temporaryData == NULL) return false; //nie uda³o siê zaalokowaæ pamiêci
	c->temporaryData[0] = INT_MAX; //na tym polu bêdzie odleg³oœæ od miasta pocz¹tkowego
	c->temporaryData[1] = -1; //na tym polu bêdzie indeks w tablicy w strukturze kopca
	//alokowanie pamiêci rekurencyjnie
	RoadList *l = c->roads;
	while (l != NULL) {
		if (!prepareCities(l->r->city1) || !prepareCities(l->r->city2)) {
			return false; //nie uda³o siê zaalokowaæ potrzebnej pamiêci
		}	
	}
	return true;
}

//zwraca najkrótsz¹ drogê pomiêdzy miastami start i end
RoadList *getMinimalRoute(City *start, City *end, int *age, bool *ambigunity) {
	if (end == start) {
		*age = INT_MAX;
		*ambigunity = false;
		return NULL;
	}
	int maxAge = INT_MIN; //data ostatniej modyfikacji najstarszego odcinka drogi w ci¹gu dróg minimalRoute
	bool ambiguityLocal = false;
	RoadList *list = end->roads;
	RoadList *minimalRoute = NULL;
	while (list != NULL) {
		City *c = roadGetCity(list->r, end);
		if (end->temporaryData[0] == c->temporaryData[0] + (int)(list->r->length)) {
			RoadList *route = getMinimalRoute(start, c, age, ambigunity);
			*age = min(*age, list->r->modificationDate);
			if (*age > maxAge) {
				ambiguityLocal = *ambigunity;
				if (!roadListAdd(&route, list->r)) { //za ma³o pamiêci
					roadListDelete(route);
					return NULL; 
				}
				minimalRoute = route;
				maxAge = *age;
			}
			else if (*age == maxAge) {
				ambiguityLocal = true;
				roadListDelete(route);
			}
			else {
				roadListDelete(route);
			}
		}
		list = list->next;
	}
	*ambigunity = ambiguityLocal;
	return minimalRoute;
}

//zwraca najkrótszy ci¹g dróg miêdzy miastami, jeœli nie istnieje lub w przypadku b³êdu zwraca NULL
RoadList *connectCities(CityHashTable *t, const char *c1, const char *c2) {
	City *start = cityHashTableFind(t, c1);
	City *end = cityHashTableFind(t, c2);
	if (start == NULL || end == NULL) return NULL; //któreœ z miast nie istnieje
	MinHeap *h = minHeapCreate();
	if (h == NULL) return NULL; //nie uda³o siê zalokowaæ pamiêci na kopiec
	if (!prepareCities(start)) {
		//nie uda³o siê przygotowaæ pamiêci potrzebnej do dzia³ania algorytmu
		cityDeleteTemporaryData(start); //usuwanie zaalokowanej pamiêci przed wyst¹pieniem b³êdu
		return NULL; 
	}
	if (end->temporaryData == NULL) {
		//nie ma po³¹czenia miêdzy podanymi miastami
		cityDeleteTemporaryData(start); //zwalnianie zaalokowanej pamiêci
		return NULL;
	}
	//argumenty funkcji s¹ poprawne i zosta³o zaalokowana pamiêæ i ustawione zmienne pomocnicze
	start->temporaryData[0] = 0;
	minHeapAdd(h, start);
	City *act;
	while ((act = minHeapPeak(h))) {
		RoadList *road = act->roads;
		while (road != NULL) {
			City *temp = roadGetCity(road->r, act);
			if ((int)(road->r->length) + act->temporaryData[0] < temp->temporaryData[0]) {
				//znaleziono krótsz¹ œcie¿kê do miasta temp
				temp->temporaryData[0] = road->r->length + act->temporaryData[0];
				if (temp->temporaryData[1] > 0) {
					//miasto zosta³o ju¿ dodane do kopca 
					//odleg³oœæ od miasta pocz¹tkowego zmniejszy³a siê, wiêc miasto byæ mo¿e powinno byæ wy¿ej w kopcu
					minHeapRepairUp(h, temp->temporaryData[1] + 1);
				}
				else {
					//domyœlna wartoœæ -1 nie zosta³a zmieniona, a wiêc miasta nie by³o jeszcze w kopcu
					minHeapAdd(h, temp);
				}
			}
			road = road->next;
		}
	}
	//we wszystkich miastach po³¹czonych z miastem pocz¹tkowym zosta³a ustawiona minimalna odleg³oœæ od niego
	int age;
	bool ambiguity;
	RoadList *l = getMinimalRoute(start, end, &age, &ambiguity);
	cityDeleteTemporaryData(start); //usuwanie zaalokowanej pamiêci tymczasowej
	if (ambiguity) return NULL; //nie uda³o siê jednoznacznie wyznaczyæ minimalnej œcie¿ki
	return l;
}



//alokuje rekurencyjnie pamiêæ pod zmienn¹ temporaryData potrzebn¹ podczas wyszukiwania œcie¿ki miêdzy mistami
bool prepareCitiesExtend(City *c, CityList *cities) {
	if (c->temporaryData != NULL) return true; //to miasto ju¿ sota³o przygotowane

	if (!cityListFind(cities, c)) return true; //droga krajowa nie mo¿e mieæ samoprzeciêæ

	//miasto nie by³o jeszcze odwiedzone, alokacja dwóch pól na dane i inicjalizacja ich
	c->temporaryData = malloc(2 * sizeof(*(c->temporaryData)));
	if (c->temporaryData == NULL) return false; //nie uda³o siê zaalokowaæ pamiêci
	c->temporaryData[0] = INT_MAX; //na tym polu bêdzie odleg³oœæ od miasta pocz¹tkowego
	c->temporaryData[1] = -1; //na tym polu bêdzie indeks w tablicy w strukturze kopca
	//alokowanie pamiêci rekurencyjnie
	RoadList *l = c->roads;
	while (l != NULL) {
		if (!prepareCities(l->r->city1) || !prepareCities(l->r->city2)) {
			return false; //nie uda³o siê zaalokowaæ potrzebnej pamiêci
		}
	}
	return true;
}

//zwraca najkrótszy ci¹g dróg miêdzy miastami, jeœli nie istnieje lub w przypadku b³êdu zwraca NULL
RoadList *connectCitiesExtend(CityHashTable *t, const char *c1, const char *c2, CityList *cities) {
	City *start = cityHashTableFind(t, c1);
	City *end = cityHashTableFind(t, c2);
	if (start == NULL || end == NULL) return NULL; //któreœ z miast nie istnieje
	MinHeap *h = minHeapCreate();
	if (h == NULL) return NULL; //nie uda³o siê zalokowaæ pamiêci na kopiec
	if (!prepareCities(start)) {
		//nie uda³o siê przygotowaæ pamiêci potrzebnej do dzia³ania algorytmu
		cityDeleteTemporaryData(start); //usuwanie zaalokowanej pamiêci przed wyst¹pieniem b³êdu
		return NULL;
	}
	if (end->temporaryData == NULL) {
		//nie ma po³¹czenia miêdzy podanymi miastami
		cityDeleteTemporaryData(start); //zwalnianie zaalokowanej pamiêci
		return NULL;
	}
	//argumenty funkcji s¹ poprawne i zosta³o zaalokowana pamiêæ i ustawione zmienne pomocnicze
	start->temporaryData[0] = 0;
	minHeapAdd(h, start);
	City *act;
	while ((act = minHeapPeak(h))) {
		RoadList *road = act->roads;
		while (road != NULL) {
			City *temp = roadGetCity(road->r, act);
			if (!cityListFind(cities, temp)) { //droga krajowa nie mo¿e mieæ samoprzeciêæ
				if ((int)(road->r->length) + act->temporaryData[0] < temp->temporaryData[0]) {
					//znaleziono krótsz¹ œcie¿kê do miasta temp
					temp->temporaryData[0] = road->r->length + act->temporaryData[0];
					if (temp->temporaryData[1] > 0) {
						//miasto zosta³o ju¿ dodane do kopca 
						//odleg³oœæ od miasta pocz¹tkowego zmniejszy³a siê, wiêc miasto byæ mo¿e powinno byæ wy¿ej w kopcu
						minHeapRepairUp(h, temp->temporaryData[1] + 1);
					}
					else {
						//domyœlna wartoœæ -1 nie zosta³a zmieniona, a wiêc miasta nie by³o jeszcze w kopcu
						minHeapAdd(h, temp);
					}
				}
			}
			road = road->next;
		}
	}
	//we wszystkich miastach po³¹czonych z miastem pocz¹tkowym zosta³a ustawiona minimalna odleg³oœæ od niego
	int age;
	bool ambiguity;
	RoadList *l = getMinimalRoute(start, end, &age, &ambiguity);
	cityDeleteTemporaryData(start); //usuwanie zaalokowanej pamiêci tymczasowej
	if (ambiguity) return NULL; //nie uda³o siê jednoznacznie wyznaczyæ minimalnej œcie¿ki
	return l;
}
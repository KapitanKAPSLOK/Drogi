#include "route.h"
#include "road.h"
#include "city.h"
#include "min_heap.h"

#include <limits.h>

int min(int a, int b) {
	return (a < b) ? a : b;
}

//tworzy now� drog� krajow�, je�li nie uda si� jej utworzy� zwraca NULL
Route *routeMake(unsigned nr, RoadList *roads, City *start, City *end) {
	Route *r = malloc(sizeof(*r));
	if (r == NULL) return NULL;
	r->id = nr;
	r->roads = roads;
	r->start = start;
	r->end = end;
	return r;
}

//tworzy list� z miast przez kt�re przechodzi droga krajowa
CityList *routeMakeCityList(Route *r) {
	CityList *l;
	if (!cityListAdd(&l, r->end)) {
		//nie uda�o si� doda� elementu do listy
		cityListDelete(l);
		return NULL;
	}
	RoadList *road = r->roads;
	City *c = r->end;
	while (road != NULL) {
		if (!cityListAdd(&l, roadGetCity(road->r,c))) {
			//nie uda�o si� doda� elementu do listy
			cityListDelete(l);
			return NULL;
		}
		c = roadGetCity(road->r, c);
		road = road->next;
	}
	return l;
}

//usuwa list� dr�g krajowych
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

//dodaje element na pocz�tek listy dr�g krajowych
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

///usuwa element r z listy dr�g krajowych
void routeListDeleteElement(RouteList **l, Route *r) {
	if (l == NULL || r == NULL) return;
	if ((*l)->r == r) { //element r znajduje si� na pocz�tku
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

//zwraca drog� krajow� o podanym numerze lub NULL je�li nie ma takiej na li�cie
Route *routeListFind(RouteList *l, unsigned nr) {
	while (l != NULL) {
		if (l->r->id == nr) return l->r;
	}
	return NULL;
}

//alokuje rekurencyjnie pami�� pod zmienn� temporaryData potrzebn� podczas wyszukiwania �cie�ki mi�dzy mistami
bool prepareCities(City *c) {
	if (c->temporaryData != NULL) return true; //to miasto ju� sota�o przygotowane
	//miasto nie by�o jeszcze odwiedzone, alokacja dw�ch p�l na dane i inicjalizacja ich
	c->temporaryData = malloc(2 * sizeof(*(c->temporaryData)));
	if (c->temporaryData == NULL) return false; //nie uda�o si� zaalokowa� pami�ci
	c->temporaryData[0] = INT_MAX; //na tym polu b�dzie odleg�o�� od miasta pocz�tkowego
	c->temporaryData[1] = -1; //na tym polu b�dzie indeks w tablicy w strukturze kopca
	//alokowanie pami�ci rekurencyjnie
	RoadList *l = c->roads;
	while (l != NULL) {
		if (!prepareCities(l->r->city1) || !prepareCities(l->r->city2)) {
			return false; //nie uda�o si� zaalokowa� potrzebnej pami�ci
		}	
	}
	return true;
}

//zwraca najkr�tsz� drog� pomi�dzy miastami start i end
RoadList *getMinimalRoute(City *start, City *end, int *age, bool *ambigunity) {
	if (end == start) {
		*age = INT_MAX;
		*ambigunity = false;
		return NULL;
	}
	int maxAge = INT_MIN; //data ostatniej modyfikacji najstarszego odcinka drogi w ci�gu dr�g minimalRoute
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
				if (!roadListAdd(&route, list->r)) { //za ma�o pami�ci
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

//zwraca najkr�tszy ci�g dr�g mi�dzy miastami, je�li nie istnieje lub w przypadku b��du zwraca NULL
RoadList *connectCities(CityHashTable *t, const char *c1, const char *c2) {
	City *start = cityHashTableFind(t, c1);
	City *end = cityHashTableFind(t, c2);
	if (start == NULL || end == NULL) return NULL; //kt�re� z miast nie istnieje
	MinHeap *h = minHeapCreate();
	if (h == NULL) return NULL; //nie uda�o si� zalokowa� pami�ci na kopiec
	if (!prepareCities(start)) {
		//nie uda�o si� przygotowa� pami�ci potrzebnej do dzia�ania algorytmu
		cityDeleteTemporaryData(start); //usuwanie zaalokowanej pami�ci przed wyst�pieniem b��du
		return NULL; 
	}
	if (end->temporaryData == NULL) {
		//nie ma po��czenia mi�dzy podanymi miastami
		cityDeleteTemporaryData(start); //zwalnianie zaalokowanej pami�ci
		return NULL;
	}
	//argumenty funkcji s� poprawne i zosta�o zaalokowana pami�� i ustawione zmienne pomocnicze
	start->temporaryData[0] = 0;
	minHeapAdd(h, start);
	City *act;
	while ((act = minHeapPeak(h))) {
		RoadList *road = act->roads;
		while (road != NULL) {
			City *temp = roadGetCity(road->r, act);
			if ((int)(road->r->length) + act->temporaryData[0] < temp->temporaryData[0]) {
				//znaleziono kr�tsz� �cie�k� do miasta temp
				temp->temporaryData[0] = road->r->length + act->temporaryData[0];
				if (temp->temporaryData[1] > 0) {
					//miasto zosta�o ju� dodane do kopca 
					//odleg�o�� od miasta pocz�tkowego zmniejszy�a si�, wi�c miasto by� mo�e powinno by� wy�ej w kopcu
					minHeapRepairUp(h, temp->temporaryData[1] + 1);
				}
				else {
					//domy�lna warto�� -1 nie zosta�a zmieniona, a wi�c miasta nie by�o jeszcze w kopcu
					minHeapAdd(h, temp);
				}
			}
			road = road->next;
		}
	}
	//we wszystkich miastach po��czonych z miastem pocz�tkowym zosta�a ustawiona minimalna odleg�o�� od niego
	int age;
	bool ambiguity;
	RoadList *l = getMinimalRoute(start, end, &age, &ambiguity);
	cityDeleteTemporaryData(start); //usuwanie zaalokowanej pami�ci tymczasowej
	if (ambiguity) return NULL; //nie uda�o si� jednoznacznie wyznaczy� minimalnej �cie�ki
	return l;
}



//alokuje rekurencyjnie pami�� pod zmienn� temporaryData potrzebn� podczas wyszukiwania �cie�ki mi�dzy mistami
bool prepareCitiesExtend(City *c, CityList *cities) {
	if (c->temporaryData != NULL) return true; //to miasto ju� sota�o przygotowane

	if (!cityListFind(cities, c)) return true; //droga krajowa nie mo�e mie� samoprzeci��

	//miasto nie by�o jeszcze odwiedzone, alokacja dw�ch p�l na dane i inicjalizacja ich
	c->temporaryData = malloc(2 * sizeof(*(c->temporaryData)));
	if (c->temporaryData == NULL) return false; //nie uda�o si� zaalokowa� pami�ci
	c->temporaryData[0] = INT_MAX; //na tym polu b�dzie odleg�o�� od miasta pocz�tkowego
	c->temporaryData[1] = -1; //na tym polu b�dzie indeks w tablicy w strukturze kopca
	//alokowanie pami�ci rekurencyjnie
	RoadList *l = c->roads;
	while (l != NULL) {
		if (!prepareCities(l->r->city1) || !prepareCities(l->r->city2)) {
			return false; //nie uda�o si� zaalokowa� potrzebnej pami�ci
		}
	}
	return true;
}

//zwraca najkr�tszy ci�g dr�g mi�dzy miastami, je�li nie istnieje lub w przypadku b��du zwraca NULL
RoadList *connectCitiesExtend(CityHashTable *t, const char *c1, const char *c2, CityList *cities) {
	City *start = cityHashTableFind(t, c1);
	City *end = cityHashTableFind(t, c2);
	if (start == NULL || end == NULL) return NULL; //kt�re� z miast nie istnieje
	MinHeap *h = minHeapCreate();
	if (h == NULL) return NULL; //nie uda�o si� zalokowa� pami�ci na kopiec
	if (!prepareCities(start)) {
		//nie uda�o si� przygotowa� pami�ci potrzebnej do dzia�ania algorytmu
		cityDeleteTemporaryData(start); //usuwanie zaalokowanej pami�ci przed wyst�pieniem b��du
		return NULL;
	}
	if (end->temporaryData == NULL) {
		//nie ma po��czenia mi�dzy podanymi miastami
		cityDeleteTemporaryData(start); //zwalnianie zaalokowanej pami�ci
		return NULL;
	}
	//argumenty funkcji s� poprawne i zosta�o zaalokowana pami�� i ustawione zmienne pomocnicze
	start->temporaryData[0] = 0;
	minHeapAdd(h, start);
	City *act;
	while ((act = minHeapPeak(h))) {
		RoadList *road = act->roads;
		while (road != NULL) {
			City *temp = roadGetCity(road->r, act);
			if (!cityListFind(cities, temp)) { //droga krajowa nie mo�e mie� samoprzeci��
				if ((int)(road->r->length) + act->temporaryData[0] < temp->temporaryData[0]) {
					//znaleziono kr�tsz� �cie�k� do miasta temp
					temp->temporaryData[0] = road->r->length + act->temporaryData[0];
					if (temp->temporaryData[1] > 0) {
						//miasto zosta�o ju� dodane do kopca 
						//odleg�o�� od miasta pocz�tkowego zmniejszy�a si�, wi�c miasto by� mo�e powinno by� wy�ej w kopcu
						minHeapRepairUp(h, temp->temporaryData[1] + 1);
					}
					else {
						//domy�lna warto�� -1 nie zosta�a zmieniona, a wi�c miasta nie by�o jeszcze w kopcu
						minHeapAdd(h, temp);
					}
				}
			}
			road = road->next;
		}
	}
	//we wszystkich miastach po��czonych z miastem pocz�tkowym zosta�a ustawiona minimalna odleg�o�� od niego
	int age;
	bool ambiguity;
	RoadList *l = getMinimalRoute(start, end, &age, &ambiguity);
	cityDeleteTemporaryData(start); //usuwanie zaalokowanej pami�ci tymczasowej
	if (ambiguity) return NULL; //nie uda�o si� jednoznacznie wyznaczy� minimalnej �cie�ki
	return l;
}
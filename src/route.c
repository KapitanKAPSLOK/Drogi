#include "route.h"
#include "road.h"
#include "city.h"
#include "min_heap.h"

#include <limits.h>
#include <string.h>


int min(int a, int b) {
	return (a < b) ? a : b;
}

//////////////////////routeList/////////////////////////

//usuwa listę dróg krajowych
void routeListDelete(RouteList *l) {
	if (l == NULL)return;
	RouteList *p = l->next;
	RouteList *temp;
	while (p != NULL) {
		roadListDelete(p->r->roads);
		free(p->r);
		temp = p->next;
		free(p);
		p = temp;
	}
	roadListDelete(l->r->roads);
	free(l->r);
	free(l);
}

//dodaje element na początek listy dróg krajowych
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
	if ((*l)->r == r) { //element r znajduje się na początku
		RouteList *temp = *l;
		*l = (*l)->next;
		free(temp);
		return;
	}
	RouteList *ptr = *l;
	while (ptr->next != NULL) {
		if (ptr->next->r == r) {
			RouteList *temp = ptr->next;
			ptr->next = ptr->next->next;
			free(temp);
			return;
		}
		ptr = ptr->next;
	}
	return;
}

//zwraca drogę krajową o podanym numerze lub NULL jeśli nie ma takiej na liście
Route *routeListFind(RouteList *l, unsigned nr) {
	while (l != NULL) {
		if (l->r->id == nr) return l->r;
		l = l->next;
	}
	return NULL;
}


/////////////////////////inne//////////////////////////////

//tworzy nową drogę krajową, jeśli nie uda się jej utworzyć zwraca NULL
Route *routeMake(unsigned nr, RoadList *roads, City *start, City *end) {
	Route *r = malloc(sizeof(*r));
	if (r == NULL) return NULL;
	r->id = nr;
	r->roads = roads;
	r->start = start;
	r->end = end;
	return r;
}

//tworzy listę z miast przez które przechodzi droga krajowa
CityList *routeMakeCityList(Route *r) {
	CityList *l=NULL;
	if (!cityListAdd(&l, r->end)) {
		//nie udało się dodać elementu do listy
		cityListDelete(l);
		return NULL;
	}
	RoadList *road = r->roads;
	City *c = r->end;
	while (road != NULL) {
		if (!cityListAdd(&l, roadGetCity(road->r, c))) {
			//nie udało się dodać elementu do listy
			cityListDelete(l);
			return NULL;
		}
		c = roadGetCity(road->r, c);
		road = road->next;
	}
	return l;
}

//dodaje do drogi krajowej najkrótszą ścieżkę łączącą c1 i c2, ale nie bezpośrednio
void routeFix(Route *r, City *c1, City *c2) {
	RoadList *l = r->roads;
	City *crossedRoads = NULL; //trzyma nazwę pierwszego spotkanego miasta c1 lub c2
	while (l != NULL) {
		if (roadConnectCity(l->r, c1) || roadConnectCity(l->r, c2)) {
			if (roadConnectCity(l->r, c1) && roadConnectCity(l->r, c2)) {
				//odcinek drogi należy do tej drogi krajowej
				CityList *temp = routeMakeCityList(r);
				if (temp == NULL) return;
				cityListDeleteElement(&temp, c2);
				cityListDeleteElement(&temp, c1);
				
				RoadList *fix=connectCities(c1, c2, temp, true); //zawsze się uda, bo było wcześniej sprawdzane
				cityListDelete(temp);
				if (!roadConnectCity(fix->r,crossedRoads)) {
					if (crossedRoads != NULL || !roadConnectCity(fix->r, r->end)) {
						//naprawiono część drogi w przeciwnym kierunku względem drogi krajowej, więc trzeba odwrócić
						fix = roadListReverse(fix);
					}

				}
				//podłączanie nowego kawałka drogi po odcinku który ma zostać usunięty
				roadListAddList(&(l->next), fix);
				return;
			}
			//nie może być pętli, więc tylko jeden odcinek drogi z danego miasta może być wykorzystany
			//po pierwszym wystąpieniu miasta następny odcinek drogi musi być tym który jest szukany lub już go nie będzie
			if (crossedRoads) return;
			//ustalanie kierunku drogi krajowej
			if (roadConnectCity(l->r, c1))
				crossedRoads = c1;
			else
				crossedRoads = c2;
		}
		l = l->next;
	}
	return;
}

//sprawdza czy można usunąć odcinek drogi tak aby można było naprawić drogę krajową
bool routeCanChange(Route *r, City *c1, City *c2) {
	RoadList *l = r->roads;
	bool crossedRoads = false;
	while (l != NULL) {
		if (roadConnectCity(l->r,c1) || roadConnectCity(l->r, c2)) {
			if (roadConnectCity(l->r,c1) && roadConnectCity(l->r, c2)) {
				//odcinek drogi należy do tej drogi krajowej
				CityList *temp=routeMakeCityList(r);
				if (temp == NULL) return false;
				cityListDeleteElement(&temp, c2);
				cityListDeleteElement(&temp, c1);
				bool result;
				RoadList *temp2 = connectCities(c1, c2, temp, true);
				if (temp2 != NULL) 
					result = true;
				else 
					result = false;

				roadListDelete(temp2);
				cityListDelete(temp);
				return result;
			}
			//nie może być pętli, więc tylko jeden odcinek drogi z danego miasta może być wykorzystany
			//po pierwszym wystąpieniu miasta następny odcinek drogi musi być tym który jest szukany lub już go nie będzie
			if(crossedRoads) return true;
			crossedRoads = true;
		}
		l = l->next;
	}
	return true;
}

//zwraca najkrótszą drogę pomiędzy miastami start i end
RoadList *getMinimalRoute(City *start, City *end, int *age, bool *ambigunity) {
	if (end == start) {
		*age = INT_MAX;
		*ambigunity = false;
		return NULL;
	}
	int maxAge = INT_MIN; //data ostatniej modyfikacji najstarszego odcinka drogi w ciągu dróg minimalRoute
	bool ambiguityLocal = false;
	RoadList *list = end->roads;
	RoadList *minimalRoute = NULL;
	while (list != NULL) {
		City *c = roadGetCity(list->r, end);
		if (c->temporaryData!=NULL && end->temporaryData[0] == c->temporaryData[0] + (int)(list->r->length)) {
			RoadList *route = getMinimalRoute(start, c, age, ambigunity);
			*age = min(*age, list->r->modificationDate);
			if (*age > maxAge) {
				ambiguityLocal = *ambigunity;
				if (!roadListAdd(&route, list->r)) { //za mało pamięci
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

//alokuje rekurencyjnie pamięć pod zmienną temporaryData potrzebną podczas wyszukiwania ścieżki między mistami
//prowadzi drogę tylko przez miasta nie należące do listy cities
bool prepareCities(City *start, City *end, City *c, CityList *cities, bool notDirectly) {
	if (cityListFind(cities, c)) return true; //droga krajowa nie może mieć samoprzecięć
	if (c->temporaryData != NULL) return true; //to miasto już sotało przygotowane

	//miasto nie było jeszcze odwiedzone, alokacja dwóch pól na dane i inicjalizacja ich
	c->temporaryData = malloc(2 * sizeof(*(c->temporaryData)));
	if (c->temporaryData == NULL) return false; //nie udało się zaalokować pamięci
	c->temporaryData[0] = INT_MAX; //na tym polu będzie odległość od miasta początkowego
	c->temporaryData[1] = -1; //na tym polu będzie indeks w tablicy w strukturze kopca
	//alokowanie pamięci rekurencyjnie
	RoadList *l = c->roads;
	while (l != NULL) {
		City *temp = roadGetCity(l->r,c);
		if (!(notDirectly && (c == start && temp == end))) { //sprawdza czy jest to bezpośrednie połączenie między c1 i c2
			if (!prepareCities(start, end, temp, cities, notDirectly)) {
				return false; //nie udało się zaalokować pamięci
			}
		}
		l = l->next;
	}
	return true;
}

//zwraca najkrótszy ciąg dróg między miastami, jeśli nie istnieje lub w przypadku błędu zwraca NULL
//prowadzi drogę tylko przez miasta nie należące do listy cities
RoadList *connectCities(City *start, City *end, CityList *cities, bool notDirectly) {
	if (start == NULL || end == NULL) return NULL; //któreś z miast nie istnieje
	MinHeap *h = minHeapCreate();
	if (h == NULL) return NULL; //nie udało się zalokować pamięci na kopiec
	if (!prepareCities(start, end, start, cities, notDirectly)) {
		//nie udało się przygotować pamięci potrzebnej do działania algorytmu
		cityDeleteTemporaryData(start); //usuwanie zaalokowanej pamięci przed wystąpieniem błędu
		minHeapDelete(h);
		return NULL;
	}
	if (end->temporaryData == NULL) {
		//nie ma połączenia między podanymi miastami
		cityDeleteTemporaryData(start); //zwalnianie zaalokowanej pamięci
		minHeapDelete(h);
		return NULL;
	}
	//argumenty funkcji są poprawne i zostało zaalokowana pamięć i ustawione zmienne pomocnicze
	start->temporaryData[0] = 0;
	minHeapAdd(h, start);
	City *act;
	while ((act = minHeapPeak(h))) {
		RoadList *road = act->roads;
		while (road != NULL) {
			City *temp = roadGetCity(road->r, act);
			if (!(notDirectly && (act==start && temp==end))) { //sprawdza czy jest to bezpośrednie połączenie między c1 i c2
				if (!cityListFind(cities, temp)) { //droga krajowa nie może mieć samoprzecięć
					if ((int)(road->r->length) + act->temporaryData[0] < temp->temporaryData[0]) {
						//znaleziono krótszą ścieżkę do miasta temp
						temp->temporaryData[0] = road->r->length + act->temporaryData[0];
						if (temp->temporaryData[1] > 0) {
							//miasto zostało już dodane do kopca 
							//odległość od miasta początkowego zmniejszyła się, więc miasto być może powinno być wyżej w kopcu
							minHeapRepairUp(h, temp->temporaryData[1] + 1);
						}
						else {
							//domyślna wartość -1 nie została zmieniona, a więc miasta nie było jeszcze w kopcu
							minHeapAdd(h, temp);
						}
					}
				}
			}
			road = road->next;
		}
	}
	//we wszystkich miastach połączonych z miastem początkowym została ustawiona minimalna odległość od niego
	if (end->temporaryData[1] == -1) {
		//miasto nie było odwiedzone z czego wynika,
		//że notDirectly jest true i jedyna ścieżka do miasta docelowego prowadziła bezpośrednio z c1 o c2
		cityDeleteTemporaryData(start);
		minHeapDelete(h);
		return NULL;
	}
	int age;
	bool ambiguity; //przechowuje informacje czy wyznaczanie najkrótszej trasy było jednoznaczne
	RoadList *l = getMinimalRoute(start, end, &age, &ambiguity);
	cityDeleteTemporaryData(start); //usuwanie zaalokowanej pamięci tymczasowej
	minHeapDelete(h);
	if (ambiguity) return NULL; //nie udało się jednoznacznie wyznaczyć minimalnej ścieżki
	return l;
}
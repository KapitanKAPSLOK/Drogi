#include "city.h"
#include "road.h"

#include<string.h>
#include <limits.h>
#include <time.h>
//////////////funkcje wewnętrzne/////////////////

//liczy hash dla podanego parametru x przy ustalonych parametrach a i b i zwraca wynik modulo size
long tableHash(unsigned x, int a, int b, int size) {
	//2147483629 jest to największa liczba pierwsza mniejsza od INT_MAX, pozwala wrócić z większych liczb na zakres inta bez utraty liniowości
	return ((a*x + b) % 2147483629 % size);
}

unsigned stringHash(const char *str) {
	int len = strlen(str);
	unsigned hash = 1;
	const unsigned prime = 97; //prime number approximately equal to possible characters used in [str]
	unsigned pow = 1; //next powers of [prime]
	for (int i = 0; i < len; ++i) {
		hash = pow * ((int)str[i]-31) + hash;
		pow *= prime;
	}
	return hash;
}

//tworzy listę ze wszystkich miast przechowywanych w hash tabeli
CityList *cityHashTableGetCityList(CityHashTable *t) {
	CityList *l = NULL;
	for (int i = 0; i < t->size; ++i) {
		cityListAddList(&l, t->tab[i]);
	}
	return l;
}

//usuwa strukturę, ale nie rekurencyjnie, nie usuwa przechowywanych w niej elementów
void cityHashTableDelete2(CityHashTable *t) {
	free(t->tab);
	free(t);
}

//gdy liczby elementów hash tablicy jest większa od połowy jej rozmiaru tworzy nową większą tablicę
void cityHashTableEnlarge(CityHashTable **t) {
	if ((*t)->numberOfElements > (*t)->size / 2) {
		//tworzenie nowej większej tablicy
		CityList *l = cityHashTableGetCityList(*t);
		CityHashTable *temp = cityHashTableMake(l, (*t)->numberOfElements);
		if (temp == NULL) return; //nie udało się powiększyć tablicy
		//udało się utworzyć nową hash tabicę
		cityHashTableDelete2(*t);
		(*t) = temp;
		cityListDelete(l);
	}
}

/////////////////////////funkcje udostępniane////////////////////

//sprawdza czy napis może być poprawną nazwą miasta
bool cityCheckName(const char* str) {
	if (str == NULL) return false;
	if (*str == '\0') return false;
	for (int i=0;; ++i) {
		if (str[i] == '\0') return true; //koniec nazwy
		if ((int)str[i] < 32 && (int)str[i]>0) return false; //nazwa zawiera niepoprawny znak
		if (str[i] == ';') return false; //średniki w nazwach są niedozwolone
	}
}

//łączy dwa miasta drogą, zwraca true jeśli uda się dodać drogę lub false jeśli nie
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear) {

	if (roadListContain(c1->roads, c2)) return false; //droga już istnieje

	//tworzenie drogi
	Road *r = malloc(sizeof(*r));
	if (r == NULL) return false; //nie udało się zaalokować pamięci
	r->city1 = c1;
	r->city2 = c2;
	r->length = length;
	r->modificationDate = builtYear;

	//dodawanie drogi do miast
	if (roadListAdd(&(c1->roads), r)) {
		if (roadListAdd(&(c2->roads), r)) {
			return true; //udało się dodać drogę do obydwu miast
		}
		//udało się dodać drogę do miasta c1, ale nie do miasta c2
		roadListDeleteElement(&(c1->roads), r);
	}
	//nie udało się dodać drogi do żadnego z miast
	free(r);
	return false;
}

//tworzy nowe miasto o nazwie str
City *cityMake(const char *str) {
	City *c = malloc(sizeof(*c));
	if (c == NULL) return NULL; //błąd alokacji pamięci
	c->roads = NULL;
	c->name = malloc((strlen(str)+1) * sizeof(char));
	if (c->name == NULL) return NULL; //błąd alokacji pamięci
	strcpy(c->name, str);
	c->roads = NULL;
	c->temporaryData = NULL;
	return c;
}

//usuwa miasto oraz prowadzące do niego drogi
void cityDelete(City *c){
	RoadList *l = c->roads;
	//usuwanie odcinków dróg z list odcinków dróg w miastach do których prowadzą odcinki dróg z miasta c
	while (l != NULL) {
		City *temp= roadGetCity(l->r, c);
		roadListDeleteElement(&(temp->roads), l->r);
		l = l->next;
	}
	//usuwanie samych dróg
	roadListDeleteElements(c->roads);
	//usuwanie struktury listy pozostałej po drogach
	roadListDelete(c->roads);
	free(c->name);
	free(c->temporaryData);
	free(c);
}

//usuwa zmienne tymczasowe z miasta i wszystkich miast do których można z niego dojść drogami
void cityDeleteTemporaryData(City *c) {
	if (c->temporaryData == NULL) return;
	free(c->temporaryData);
	c->temporaryData = NULL;
	RoadList *l = c->roads;
	while (l != NULL) {
		cityDeleteTemporaryData(l->r->city1);
		cityDeleteTemporaryData(l->r->city2);
		l = l->next;
	}
}

unsigned cityHash(City *c) {
	return stringHash(c->name);
}

//usuwa listę miast, ale nie usuwa samych miast
void cityListDelete(CityList *l) {
	if (l == NULL)return;
	CityList *p = l->next;
	CityList *temp;
	while (p != NULL) {
		temp = p->next;
		free(p);
		p = temp;
	}
	free(l);
}

///usuwa element r z listy miast
void cityListDeleteElement(CityList **l, City *r) {
	if (l == NULL || r == NULL) return;
	if ((*l)->c == r) { //element r znajduje się na początku
		CityList *temp = *l;
		*l = (*l)->next;
		free(temp);
		return;
	}
	CityList *list = *l;
	while (list->next != NULL) {
		if (list->next->c == r) {
			CityList *temp = list->next;
			list->next = list->next->next;
			free(temp);
			return;
		}
		list = list->next;
	}
	return;
}

//zwalnia pamięć po elementach przechowywanych w liście
void cityListDeleteElements(CityList *l) {
	while (l != NULL) {
		cityDelete(l->c);
		l = l->next;
	}
}

//dodaje element do listy miast
bool cityListAdd(CityList **l, City *c) {
	CityList *temp;
	temp = malloc(sizeof(*temp));
	if (temp == NULL) return false;
	temp->next = *l;
	temp->c = c;
	*l = temp;
	return true;
}

//dodaje na początek listy l1 listę l2
void cityListAddList(CityList **l1, CityList *l2) {
	if (l2 == NULL) return; //nie ma nic do dodania
	CityList *last = l2;
	while (last->next != NULL) last = last->next;
	last->next = (*l1);
	(*l1) = l2;
}

//zwraca wskaźnik do szukanego miasta c lub NULL jeśli miasta nie ma na liście
City *cityListFind(CityList *l, City *c) {
	while (l != NULL) {
		if (!strcmp(l->c->name, c->name)) return l->c;
		l = l->next;
	}
	return NULL;
}

//zwraca wskaźnik do szukanego miasta c lub NULL jeśli miasta nie ma na liście
City *cityListFindStr(CityList *l, const char *str) {
	while (l != NULL) {
		if (!strcmp(l->c->name, str)) return l->c;
		l = l->next;
	}
	return NULL;
}

//tworzy hash table z listy miast, numberOfCities to liczba miast na jaką ma zostać przygotowana tablica
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities){
	if (numberOfCities == 0) return NULL;
	CityHashTable *table;
	table = malloc(sizeof(*table));
	if (table == NULL) return NULL; //brak pamięci
	CityList *list;
	int *temp;
	int size = numberOfCities;

	//w celu możliwości lepszego hashowania elementów większy rozmiar tablicy od liczby miast
	if (size > INT_MAX / 6 - 1) size = INT_MAX;
	else size = 6 * numberOfCities;

	//inicjalizacja ziarna dla funkcji rand czasem systemowym
	srand((unsigned)time(NULL));

	/*Losuje takie współczynniki funkcji liniowej a i b, które dają odchylenie kwadratowe ilości elementów w danym indeksie talicy <=4*size+1
	wykluczane są w ten sposób pesymistyczne przypadki, dla których otrzymamy nagromadzenie miast w jednym indeksie i co za tym idzie
	spowolnienie dostępu do miast w strukturze*/
	int a, b;
	temp = malloc(size * sizeof(*temp));
	if (temp == NULL) return NULL; //nie udało się zaalokować pamięci
	long squares;
	do {
		squares = 0;
		list = cities;
		for (int i = 0; i < size; ++i) {
			temp[i] = 0;
		}
		a = rand() % 100 + 1;
		b = rand() % 100 + 1;
		while (list != NULL) {
			++temp[tableHash(cityHash(list->c), a, b, size)];
			list = list->next;
		}
		for (int i = 0; i < size; ++i) {
			squares += temp[i] * temp[i];
		}
	} while (squares >= 6 * numberOfCities + 1); //w przypadku złego rozmieszczenia losujemy ponownie współczynniki

	free(temp);

	//dodawanie komórek do planszy dla optymalnych współczynników a i b
	table->size = size;
	table->tab = malloc(size * sizeof(*(table->tab)));
	if (table->tab == NULL) return NULL; //nie udało się zaalokować pamięci
	list = cities;
	for (int i = 0; i < size; ++i) {
		table->tab[i] = NULL;
	}
	while (list != NULL) {
		cityListAdd(&(table->tab[tableHash(cityHash(list->c), a, b, size)]),list->c);
		list = list->next;
	}
	if (table->size > 0) {
		table->a = a;
		table->b = b;
	}

	table->numberOfElements = numberOfCities;

	return table;
}

//usuwa strukturę i jej zawartość
void cityHashTableDelete(CityHashTable *t) {
	if (t == NULL) return;
	for (int i = 0; i < t->size; ++i) {
		cityListDeleteElements(t->tab[i]);
		cityListDelete(t->tab[i]);
	}
	free(t->tab);
	free(t);
}

//zwraca wskaźnik na miasto o nazwie str (dodaje je jeśli nie istnieje), w przypadku błędu zwraca NULL
City *cityHashTableAdd(CityHashTable **t, const char *str) {
	City *c = cityMake(str);
	if (c == NULL) return NULL; //nie udało się utworzyć miasta
	City *city = cityListFind((*t)->tab[tableHash(cityHash(c), (*t)->a, (*t)->b, (*t)->size)], c);
	if (city == NULL) {
		//miasto jeszcze nie istnieje i trzeba je dodać
		if(!cityListAdd(&((*t)->tab[tableHash(cityHash(c), (*t)->a, (*t)->b, (*t)->size)]), c)) return NULL;
		(*t)->numberOfElements++;
		//przy większej ilości elementów hashowanie może stać się nieoptymalne
		cityHashTableEnlarge(t); //w razie potrzeby powiększa tablicę na dane
		return c;
	}
	return city;
}

//zwraca wskaźnij do szukanego miasta lub NULL, gdy takie miasto nie istnieje
City *cityHashTableFind(CityHashTable *t, const char *str) {
	if (t == NULL) return NULL;
	if (str == NULL) return NULL;
	return cityListFindStr(t->tab[tableHash(stringHash(str), t->a, t->b, t->size)], str);
}

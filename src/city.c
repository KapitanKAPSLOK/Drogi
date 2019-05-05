#include "city.h"
#include "road.h"

#include<string.h>
#include <limits.h>
#include <time.h>

//////////////funkcje wewnêtrzne/////////////////

//liczy hash dla podanego parametru x przy ustalonych parametrach a i b i zwraca wynik modulo size
long tableHash(unsigned x, int a, int b, int size) {
	//2147483629 jest to najwiêksza liczba pierwsza mniejsza od INT_MAX, pozwala wróciæ z wiêkszych liczb na zakres inta bez utraty liniowoœci
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

//tworzy listê ze wszystkich miast przechowywanych w hash tabeli
CityList *cityHashTableGetCityList(CityHashTable *t) {
	CityList *l = NULL;
	for (int i = 0; i < t->size; ++i) {
		cityListAddList(&l, t->tab[i]);
	}
	return l;
}

//gdy liczby elementów hash tablicy jest wiêksza od po³owy jej rozmiaru tworzy now¹ wiêksz¹ tablicê
void cityHashTableEnlarge(CityHashTable **t) {
	if ((*t)->numberOfElements > (*t)->size / 2) {
		//tworzenie nowej wiêkszej tablicy
		CityList *l = cityHashTableGetCityList(*t);
		CityHashTable *temp = cityHashTableMake(l, (*t)->numberOfElements + 1);
		if (temp == NULL) return; //nie uda³o siê powiêkszyæ tablicy
		//uda³o siê utworzyæ now¹ hash tabicê
		cityHashTableDelete(*t);
		(*t) = temp;
		cityListDelete(l);
	}
}

/////////////////////////funkcje udostêpniane////////////////////

//³¹czy dwa miasta drog¹, zwraca true jeœli uda siê dodaæ drogê lub false jeœli nie
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear) {

	if (roadListContain(c1->roads, c2)) return false; //droga ju¿ istnieje

	//tworzenie drogi
	Road *r = malloc(sizeof(*r));
	if (r == NULL) return false; //nie uda³o siê zaalokowaæ pamiêci
	r->city1 = c1;
	r->city2 = c2;
	r->length = length;
	r->modificationDate = builtYear;

	//dodawanie drogi do miast
	if (roadListAdd(&(c1->roads), r)) {
		if (roadListAdd(&(c2->roads), r)) {
			return true; //uda³o siê dodaæ drogê do obydwu miast
		}
		//uda³o siê dodaæ drogê do miasta c1, ale nie do miasta c2
		roadListDeleteElement(&(c1->roads), r);
	}
	//nie uda³o siê dodaæ drogi do ¿adnego z miast
	free(r);
	return false;
}

//tworzy nowe miasto o nazwie str
City *cityMake(const char *str) {
	City *c = malloc(sizeof(*c));
	if (c == NULL) return NULL; //b³¹d alokacji pamiêci
	c->roads = NULL;
	c->name = malloc(strlen(str) * sizeof(*c->name));
	if (c->name == NULL) return NULL; //b³¹d alokacji pamiêci
	strcpy(c->name, str);
	c->roads = NULL;
	c->temporaryData = NULL;
	return c;
}

//usuwa miasto oraz prowadz¹ce do niego drogi
void cityDelete(City *c){
	free(c->name);
	free(c->temporaryData);
	RoadList *l = c->roads;
	//usuwanie dróg z list dróg w miastach do których prowadz¹ drogi z miasta c
	while (l != NULL) {
		City *temp= roadGetCity(l->r, c);
		roadListDeleteElement(&(temp->roads), l->r);
	}
	//usuwanie samych dróg
	roadListDeleteElements(c->roads);
	//usuwanie struktury listy pozosta³ej po drogach
	roadListDelete(c->roads);

	free(c);
}

//usuwa zmienne tymczasowe z miasta i wszystkich miast do których mo¿na z niego dojœæ drogami
void cityDeleteTemporaryData(City *c) {
	if (c->temporaryData == NULL) return;
	free(c->temporaryData);
	RoadList *l = c->roads;
	while (l != NULL) {
		cityDeleteTemporaryData(l->r->city1);
		cityDeleteTemporaryData(l->r->city2);
	}
}

unsigned cityHash(City *c) {
	return stringHash(c->name);
}

//usuwa listê miast, ale nie usuwa samych miast
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
	if ((*l)->c == r) { //element r znajduje siê na pocz¹tku
		CityList *temp = *l;
		*l = (*l)->next;
		free(temp);
	}
	while ((*l)->next != NULL) {
		if ((*l)->next->c == r) {
			CityList *temp = (*l)->next;
			(*l)->next = (*l)->next->next;
			free(temp);
			return;
		}
	}
	return;
}

//zwalnia pamiêæ po elementach przechowywanych w liœcie
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

//dodaje na pocz¹tek listy l1 listê l2
void cityListAddList(CityList **l1, CityList *l2) {
	if (l2 == NULL) return; //nie ma nic do dodania
	CityList *last = l2;
	while (last->next != NULL) last = last->next;
	last->next = (*l1);
	(*l1) = l2;
}

//zwraca wskaŸnik do szukanego miasta c lub NULL jeœli miasta nie ma na liœcie
City *cityListFind(CityList *l, City *c) {
	while (l != NULL) {
		if (!strcmp(l->c->name, c->name)) return l->c;
		l = l->next;
	}
	return NULL;
}

//zwraca wskaŸnik do szukanego miasta c lub NULL jeœli miasta nie ma na liœcie
City *cityListFindStr(CityList *l, const char *str) {
	while (l != NULL) {
		if (!strcmp(l->c->name, str)) return l->c;
		l = l->next;
	}
	return NULL;
}

//tworzy hash table z listy miast, numberOfCities to liczba miast na jak¹ ma zostaæ przygotowana tablica
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities){
	if (numberOfCities == 0) return NULL;
	CityHashTable *table;
	table = malloc(sizeof(*table));
	if (table == NULL) return NULL; //brak pamiêci
	CityList *list;
	int *temp;
	int size = numberOfCities;

	//w celu mo¿liwoœci lepszego hashowania elementów wiêkszy rozmiar tablicy od liczby miast
	if (size > INT_MAX / 6 - 1) size = INT_MAX;
	else size = 6 * numberOfCities;

	//inicjalizacja ziarna dla funkcji rand czasem systemowym
	srand((unsigned)time(NULL));

	/*Losuje takie wspó³czynniki funkcji liniowej a i b, które daj¹ odchylenie kwadratowe iloœci elementów w danym indeksie talicy <=4*size+1
	wykluczane s¹ w ten sposób pesymistyczne przypadki, dla których otrzymamy nagromadzenie miast w jednym indeksie i co za tym idzie
	spowolnienie dostêpu do miast w strukturze*/
	int a, b;
	temp = malloc(size * sizeof(*temp));
	if (temp == NULL) return NULL; //nie uda³o siê zaalokowaæ pamiêci
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
	} while (squares >= 6 * numberOfCities + 1); //w przypadku z³ego rozmieszczenia losujemy ponownie wspó³czynniki

	free(temp);

	//dodawanie komórek do planszy dla optymalnych wspó³czynników a i b
	table->size = size;
	table->tab = malloc(size * sizeof(*(table->tab)));
	if (table->tab == NULL) return NULL; //nie uda³o siê zaalokowaæ pamiêci
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

//usuwa strukturê
void cityHashTableDelete(CityHashTable *t) {
	if (t == NULL) return;
	for (int i = 0; i < t->size; ++i) {
		cityListDeleteElements(t->tab[i]);
		cityListDelete(t->tab[i]);
	}
	free(t->tab);
	free(t);
}

//zwraca wskaŸnik na miasto o nazwie str (dodaje je jeœli nie istnieje), w przypadku b³êdu zwraca NULL
City *cityHashTableAdd(CityHashTable **t, const char *str) {
	City *c = cityMake(str);
	if (c == NULL) return NULL; //nie uda³o siê utworzyæ miasta
	City *city = cityListFind((*t)->tab[tableHash(cityHash(c), (*t)->a, (*t)->b, (*t)->size)], c);
	if (city == NULL) {
		//miasto jeszcze nie istnieje i trzeba je dodaæ
		if(!cityListAdd(&((*t)->tab[tableHash(cityHash(c), (*t)->a, (*t)->b, (*t)->size)]), c)) return NULL;
		(*t)->numberOfElements++;
		//przy wiêkszej iloœci elementów hashowanie mo¿e staæ siê nieoptymalne
		cityHashTableEnlarge(t); //w razie potrzeby powiêksza tablicê na dane
		return c;
	}
	return city;
}

//zwraca wskaŸnij do szukanego miasta lub NULL, gdy takie miasto nie istnieje
City *cityHashTableFind(CityHashTable *t, const char *str) {
	if (t == NULL) return NULL;
	if (str == NULL) return NULL;
	return cityListFindStr(t->tab[tableHash(stringHash(str), t->a, t->b, t->size)], str);
}

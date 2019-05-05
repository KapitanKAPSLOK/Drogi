#include "city.h"
#include "road.h"

#include<string.h>
#include <limits.h>
#include <time.h>

//////////////funkcje wewn�trzne/////////////////

//liczy hash dla podanego parametru x przy ustalonych parametrach a i b i zwraca wynik modulo size
long tableHash(unsigned x, int a, int b, int size) {
	//2147483629 jest to najwi�ksza liczba pierwsza mniejsza od INT_MAX, pozwala wr�ci� z wi�kszych liczb na zakres inta bez utraty liniowo�ci
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

//tworzy list� ze wszystkich miast przechowywanych w hash tabeli
CityList *cityHashTableGetCityList(CityHashTable *t) {
	CityList *l = NULL;
	for (int i = 0; i < t->size; ++i) {
		cityListAddList(&l, t->tab[i]);
	}
	return l;
}

//gdy liczby element�w hash tablicy jest wi�ksza od po�owy jej rozmiaru tworzy now� wi�ksz� tablic�
void cityHashTableEnlarge(CityHashTable **t) {
	if ((*t)->numberOfElements > (*t)->size / 2) {
		//tworzenie nowej wi�kszej tablicy
		CityList *l = cityHashTableGetCityList(*t);
		CityHashTable *temp = cityHashTableMake(l, (*t)->numberOfElements + 1);
		if (temp == NULL) return; //nie uda�o si� powi�kszy� tablicy
		//uda�o si� utworzy� now� hash tabic�
		cityHashTableDelete(*t);
		(*t) = temp;
		cityListDelete(l);
	}
}

/////////////////////////funkcje udost�pniane////////////////////

//��czy dwa miasta drog�, zwraca true je�li uda si� doda� drog� lub false je�li nie
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear) {

	if (roadListContain(c1->roads, c2)) return false; //droga ju� istnieje

	//tworzenie drogi
	Road *r = malloc(sizeof(*r));
	if (r == NULL) return false; //nie uda�o si� zaalokowa� pami�ci
	r->city1 = c1;
	r->city2 = c2;
	r->length = length;
	r->modificationDate = builtYear;

	//dodawanie drogi do miast
	if (roadListAdd(&(c1->roads), r)) {
		if (roadListAdd(&(c2->roads), r)) {
			return true; //uda�o si� doda� drog� do obydwu miast
		}
		//uda�o si� doda� drog� do miasta c1, ale nie do miasta c2
		roadListDeleteElement(&(c1->roads), r);
	}
	//nie uda�o si� doda� drogi do �adnego z miast
	free(r);
	return false;
}

//tworzy nowe miasto o nazwie str
City *cityMake(const char *str) {
	City *c = malloc(sizeof(*c));
	if (c == NULL) return NULL; //b��d alokacji pami�ci
	c->roads = NULL;
	c->name = malloc(strlen(str) * sizeof(*c->name));
	if (c->name == NULL) return NULL; //b��d alokacji pami�ci
	strcpy(c->name, str);
	c->roads = NULL;
	c->temporaryData = NULL;
	return c;
}

//usuwa miasto oraz prowadz�ce do niego drogi
void cityDelete(City *c){
	free(c->name);
	free(c->temporaryData);
	RoadList *l = c->roads;
	//usuwanie dr�g z list dr�g w miastach do kt�rych prowadz� drogi z miasta c
	while (l != NULL) {
		City *temp= roadGetCity(l->r, c);
		roadListDeleteElement(&(temp->roads), l->r);
	}
	//usuwanie samych dr�g
	roadListDeleteElements(c->roads);
	//usuwanie struktury listy pozosta�ej po drogach
	roadListDelete(c->roads);

	free(c);
}

//usuwa zmienne tymczasowe z miasta i wszystkich miast do kt�rych mo�na z niego doj�� drogami
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

//usuwa list� miast, ale nie usuwa samych miast
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
	if ((*l)->c == r) { //element r znajduje si� na pocz�tku
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

//zwalnia pami�� po elementach przechowywanych w li�cie
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

//dodaje na pocz�tek listy l1 list� l2
void cityListAddList(CityList **l1, CityList *l2) {
	if (l2 == NULL) return; //nie ma nic do dodania
	CityList *last = l2;
	while (last->next != NULL) last = last->next;
	last->next = (*l1);
	(*l1) = l2;
}

//zwraca wska�nik do szukanego miasta c lub NULL je�li miasta nie ma na li�cie
City *cityListFind(CityList *l, City *c) {
	while (l != NULL) {
		if (!strcmp(l->c->name, c->name)) return l->c;
		l = l->next;
	}
	return NULL;
}

//zwraca wska�nik do szukanego miasta c lub NULL je�li miasta nie ma na li�cie
City *cityListFindStr(CityList *l, const char *str) {
	while (l != NULL) {
		if (!strcmp(l->c->name, str)) return l->c;
		l = l->next;
	}
	return NULL;
}

//tworzy hash table z listy miast, numberOfCities to liczba miast na jak� ma zosta� przygotowana tablica
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities){
	if (numberOfCities == 0) return NULL;
	CityHashTable *table;
	table = malloc(sizeof(*table));
	if (table == NULL) return NULL; //brak pami�ci
	CityList *list;
	int *temp;
	int size = numberOfCities;

	//w celu mo�liwo�ci lepszego hashowania element�w wi�kszy rozmiar tablicy od liczby miast
	if (size > INT_MAX / 6 - 1) size = INT_MAX;
	else size = 6 * numberOfCities;

	//inicjalizacja ziarna dla funkcji rand czasem systemowym
	srand((unsigned)time(NULL));

	/*Losuje takie wsp�czynniki funkcji liniowej a i b, kt�re daj� odchylenie kwadratowe ilo�ci element�w w danym indeksie talicy <=4*size+1
	wykluczane s� w ten spos�b pesymistyczne przypadki, dla kt�rych otrzymamy nagromadzenie miast w jednym indeksie i co za tym idzie
	spowolnienie dost�pu do miast w strukturze*/
	int a, b;
	temp = malloc(size * sizeof(*temp));
	if (temp == NULL) return NULL; //nie uda�o si� zaalokowa� pami�ci
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
	} while (squares >= 6 * numberOfCities + 1); //w przypadku z�ego rozmieszczenia losujemy ponownie wsp�czynniki

	free(temp);

	//dodawanie kom�rek do planszy dla optymalnych wsp�czynnik�w a i b
	table->size = size;
	table->tab = malloc(size * sizeof(*(table->tab)));
	if (table->tab == NULL) return NULL; //nie uda�o si� zaalokowa� pami�ci
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

//usuwa struktur�
void cityHashTableDelete(CityHashTable *t) {
	if (t == NULL) return;
	for (int i = 0; i < t->size; ++i) {
		cityListDeleteElements(t->tab[i]);
		cityListDelete(t->tab[i]);
	}
	free(t->tab);
	free(t);
}

//zwraca wska�nik na miasto o nazwie str (dodaje je je�li nie istnieje), w przypadku b��du zwraca NULL
City *cityHashTableAdd(CityHashTable **t, const char *str) {
	City *c = cityMake(str);
	if (c == NULL) return NULL; //nie uda�o si� utworzy� miasta
	City *city = cityListFind((*t)->tab[tableHash(cityHash(c), (*t)->a, (*t)->b, (*t)->size)], c);
	if (city == NULL) {
		//miasto jeszcze nie istnieje i trzeba je doda�
		if(!cityListAdd(&((*t)->tab[tableHash(cityHash(c), (*t)->a, (*t)->b, (*t)->size)]), c)) return NULL;
		(*t)->numberOfElements++;
		//przy wi�kszej ilo�ci element�w hashowanie mo�e sta� si� nieoptymalne
		cityHashTableEnlarge(t); //w razie potrzeby powi�ksza tablic� na dane
		return c;
	}
	return city;
}

//zwraca wska�nij do szukanego miasta lub NULL, gdy takie miasto nie istnieje
City *cityHashTableFind(CityHashTable *t, const char *str) {
	if (t == NULL) return NULL;
	if (str == NULL) return NULL;
	return cityListFindStr(t->tab[tableHash(stringHash(str), t->a, t->b, t->size)], str);
}

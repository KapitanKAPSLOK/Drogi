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

unsigned stringHash(char *str) {
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

/////////////////////////funkcje udost�pniane////////////////////

unsigned cityHash(City *c) {
	return stringHash(c->name);
}


//usuwa list� miast
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

//dodaje element do listy miast
bool cityListAdd(CityList **l, City *c) {
	CityList *temp;
	temp = malloc(sizeof(*temp));
	if (temp == NULL) return false;
	if (*l != NULL) temp->next = *l;
	else temp->next = NULL;
	temp->c = c;
	*l = temp;
	return true;
}

//tworzy hash table z listy miast
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities){
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
	long squares;
	do {
		squares = 0;
		list = cities->next;
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

	cityListDelete(list);

	return table;
}

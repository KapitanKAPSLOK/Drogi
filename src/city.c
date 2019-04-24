#include "city.h"
#include "road.h"

#include<string.h>

struct CityBST {
	City *c;
	CityBST *lChild,*rChild;
};

unsigned cityHash(char *str) {
	int len = strlen(str);
	unsigned hash = 1;
	const unsigned prime = 97; //prime number approximately equal to possible characters used in [str]
	unsigned pow = 1; //next powers of [prime]
	for (int i = 0; i < len; ++i) {
		hash = pow * ((int)str[i]-31) + hash;
		pow *= prime;
	}
}

typedef struct CityList {
	City* c;
	struct CityList *next;
	struct CityList *prev;
}CityList;

typedef struct CityHashTable {
	int a, b;
	int size;
	CityList **tab;
}CityHashTable;


long tableHash(unsigned x, int a, int b, int size) {
	//2147483629 jest to najwiêksza liczba pierwsza mniejsza od INT_MAX, pozwala wróciæ z wiêkszych liczb na zakres inta bez utraty liniowoœci
	return (a*x + b) % 2147483629 % size);
}

void cityListDelete(CityList *l) {
	if (l == NULL)return;
	CityList *p = l->next;
	CityList *temp;
	while (p != NULL) {
		temp = p->nast;
		free(p);
		p = temp;
	}
	free(l);
}

void cityListAdd(CityList **l, City *c) {
	CityList *temp;
	temp = malloc(sizeof(*temp));
	if (*l != NULL) temp->nast = *l;
	else temp->nast = NULL;
	temp->c = c;

	*l = temp;
}

CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities){
	CityHashTable *table;
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
			++temp[tableHash(cityHash(list->c, a, b, size)];
			list = list->next;
		}
		for (int i = 0; i < size; ++i) {
			squares += temp[i] * temp[i];
		}
	} while (squares >= 4 * numberOfCities + 1);

	free(temp);

	//dodawanie komórek do planszy dla optymalnych wspó³czynników a i b
	table->size = size;
	table->tab = malloc(size * sizeof(*(table->tab)));
	list = cities;
	for (int i = 0; i < size; ++i) {
		table->tab[i] = NULL;
	}
	while (list != NULL) {
		cityListAdd(&(table->tab[tableHash(CityHash(list->c), a, b, size)]),list->c);
		list = list->next;
	}
	if (table->size > 0) {
		table->a = a;
		table->b = b;
	}

	cityListDelete(list);

	return table;
}


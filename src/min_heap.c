#include "min_heap.h"
#include <limits.h>

//zamienia dwa indeksy o podanych indeksach
void swapInMinHeap(MinHeap *t, int index1, int index2) {
	City *temp = t->tab[index1];
	t->tab[index1] = t->tab[index2];
	t->tab[index2] = temp;
}

//przenosi ostatni element kopca do góry dopóki kopiec nie bêdzie znów posortowany
void repairUpMinHeap(MinHeap *h) {
	int i = h->size;
	while (i != 1 && h->tab[i]->distance > h->tab[i / 2]->distance) {
		swapInMinHeap(h, i, i / 2);
		i /= 2;
	}
}

//przesuwa pierwszy element w dó³ kopca dopóki jest on wiêkszy od swoich dzieci
void repairDownMinHeap(MinHeap *h){
	int i = 1; //nr naprawianego elementu, czyli jego pozycja w tablicy + 1
	while (i < (h->size + 1) / 2) {
		int act = h->tab[i-1]->distance;
		int child1 = h->tab[2*i-1]->distance;
		int child2 = h->tab[2*i]->distance;
		if (act > child1) {
			//zamiana  z mniejszym z dzieci
			if (child2 < child1) {
				swapInMinHeap(h, i-1, 2 * i);
				i = 2 * i;
			}
			else {
				swapInMinHeap(h, i-1, 2 * i - 1);
				i = 2 * i - 1;
			}
		}
		else if(act>child1){ //act>cild1, ale act<child2
			swapInMinHeap(h, i-1, 2 * i);
			i = 2 * i;
		}
		else { //element jest na swojej pozycji
			break;
		}
	}
	if (2 * i == h->size&&h->tab[i]->distance > h->tab[2 * i - 1]->distance) {
		swapInMinHeap(h, i - 1, 2 * i - 1);
	}
}

//tworzy nowy kopiec
MinHeap* createMinHeap() {
	MinHeap* t = malloc(sizeof(MinHeap));
	if (t == NULL) return NULL; //nie uda³o siê zaalokowac pamiêci na strukturê
	t->size = 0;
	t->tab = malloc(10*sizeof(*(t->tab))); //domyœlny pocz¹tkowy rozmiar tablicy to 1
	if (t->tab == NULL) { //za ma³o pamiêci na tablicê dziesiêcio elementow¹
		t->tab = malloc(sizeof(*(t->tab)));
		if (t->tab == NULL) return NULL; //brak pamiêci
		t->maxSize = 1;
		return t;
	}
	//uda³o siê zaalokowaæ pamiêæ na dziesiêcio elementow¹ tablicê
	t->maxSize = 10;
	return t;
}

//dodaje element @p c do kopca
bool addToMinHeap(MinHeap *t, City *c) {
	if (t->size >= t->maxSize) {
		if (t->size == INT_MAX) return false; //osi¹gniêto maksymaln¹ iloœæ elementów
		if (t->maxSize < INT_MAX / 2) {
			realloc(t->tab, 2 * t->maxSize);
		}
		else {
			realloc(t->tab, INT_MAX);
		}
	}
	if (t->tab == NULL) return false; //nie uda³o siê zaalokowaæ pamiêci
	t->tab[t->size++] = c;
	repairUpMinHeap(t);
	return true;
}

//zdejmuje pierwszy element z kopca
City* peakFromMinHeap(MinHeap *t) {
	if (t->size <= 0) return NULL;
	City *temp = t->tab[0];
	t->tab[0] = t->tab[--(t->size)];
	return temp;
}

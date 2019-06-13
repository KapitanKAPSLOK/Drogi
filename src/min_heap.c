#include "min_heap.h"

#include <limits.h>

//zamienia dwa indeksy o podanych indeksach
void minHeapSwap(MinHeap *h, int index1, int index2) {
	City *temp = h->tab[index1];
	h->tab[index1]->temporaryData[1] = index2;
	h->tab[index2]->temporaryData[1] = index1;
	h->tab[index1] = h->tab[index2];
	h->tab[index2] = temp;
}

//przenosi element o numerze index do góry kopca dopóki kopiec nie będzie znów posortowany
void minHeapRepairUp(MinHeap *h,int index) {
	int i = index;
	while (i != 1 && h->tab[i-1]->temporaryData[0] < h->tab[i / 2 - 1]->temporaryData[0]) {
		minHeapSwap(h, i-1, i / 2-1);
		i /= 2;
	}
}

//przesuwa pierwszy element w dół kopca dopóki jest on większy od swoich dzieci
void minHeapRepairDown(MinHeap *h){
	int i = 1; //nr naprawianego elementu, czyli jego pozycja w tablicy + 1
	while (i < (h->size + 1) / 2) {
		int act = h->tab[i-1]->temporaryData[0];
		int child1 = h->tab[2*i-1]->temporaryData[0];
		int child2 = h->tab[2*i]->temporaryData[0];
		if (act > child1) {
			//zamiana  z mniejszym z dzieci
			if (child2 < child1) {
				minHeapSwap(h, i-1, 2 * i);
				i = 2 * i;
			}
			else {
				minHeapSwap(h, i-1, 2 * i - 1);
				i = 2 * i - 1;
			}
		}
		else if(act>child1){ //act>cild1, ale act<child2
			minHeapSwap(h, i-1, 2 * i);
			i = 2 * i;
		}
		else { //element jest na swojej pozycji
			break;
		}
	}
	if (2 * i == h->size&&h->tab[i]->temporaryData[0] > h->tab[2 * i - 1]->temporaryData[0]) {
		minHeapSwap(h, i - 1, 2 * i - 1);
	}
}

//tworzy nowy kopiec
MinHeap* minHeapCreate(void) {
	MinHeap* h = malloc(sizeof(MinHeap));
	if (h == NULL) return NULL; //nie udało się zaalokowac pamięci na strukturę
	h->size = 0;
	h->tab = malloc(10*sizeof(*(h->tab))); //domyślny początkowy rozmiar tablicy to 10
	if (h->tab == NULL) { //za mało pamięci na tablicę dziesięcio elementową
		h->tab = malloc(sizeof(*(h->tab)));
		if (h->tab == NULL) {//brak pamięci
			free(h);
			return NULL; 
		}
		h->maxSize = 1;
		return h;
	}
	//udało się zaalokować pamięć na dziesięcio elementową tablicę
	h->maxSize = 10;
	return h;
}

//zwalnia pamieć po strukturze kopca
void minHeapDelete(MinHeap *h) {
	if (h == NULL) return;
	free(h->tab);
	free(h);
}

//dodaje element @p c do kopca
bool minHeapAdd(MinHeap *h, City *c) {
	if (h->size >= h->maxSize) {
		if (h->size == INT_MAX) return false; //osiągnięto maksymalną ilość elementów
		if (h->maxSize * sizeof(*(h->tab)) < INT_MAX / 2) {
			City **temp = realloc(h->tab, 2 * h->maxSize * sizeof(*(h->tab)));
			if (temp == NULL) return false; //brak pamięci
			h->tab = temp;
			h->maxSize *= 2;
		}
		else {
			City **temp = realloc(h->tab, INT_MAX);
			if (temp == NULL) return false; //brak pamięci
			h->tab = temp;
			h->maxSize = INT_MAX;
		}
	}
	c->temporaryData[1] = h->size; //miasto pamięta swój indeks w tablicy
	h->tab[h->size++] = c;

	minHeapRepairUp(h,h->size);

	return true;
}

//zdejmuje pierwszy element z kopca
City* minHeapPeak(MinHeap *h) {
	if (h->size <= 0) return NULL;
	City *temp = h->tab[0];
	h->tab[0] = h->tab[--(h->size)];
	minHeapRepairDown(h);
	return temp;
}


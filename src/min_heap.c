#include "min_heap.h"
#include <limits.h>

//zamienia dwa indeksy o podanych indeksach
void minHeapSwap(MinHeap *h, int index1, int index2) {
	City *temp = h->tab[index1];
	h->tab[index1] = h->tab[index2];
	h->tab[index2] = temp;
}

//przenosi ostatni element kopca do góry dopóki kopiec nie bêdzie znów posortowany
void minHeapRepairUp(MinHeap *h) {
	int i = h->size;
	while (i != 1 && h->tab[i-1]->temporaryData[0] < h->tab[i / 2 - 1]->temporaryData[0]) {
		minHeapSwap(h, i-1, i / 2-1);
		i /= 2;
	}
}

//przesuwa pierwszy element w dó³ kopca dopóki jest on wiêkszy od swoich dzieci
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
MinHeap* minHeapCreate() {
	MinHeap* h = malloc(sizeof(MinHeap));
	if (h == NULL) return NULL; //nie uda³o siê zaalokowac pamiêci na strukturê
	h->size = 0;
	h->tab = malloc(10*sizeof(*(h->tab))); //domyœlny pocz¹tkowy rozmiar tablicy to 1
	if (h->tab == NULL) { //za ma³o pamiêci na tablicê dziesiêcio elementow¹
		h->tab = malloc(sizeof(*(h->tab)));
		if (h->tab == NULL) return NULL; //brak pamiêci
		h->maxSize = 1;
		return h;
	}
	//uda³o siê zaalokowaæ pamiêæ na dziesiêcio elementow¹ tablicê
	h->maxSize = 10;
	return h;
}

//dodaje element @p c do kopca
bool minHeapAdd(MinHeap *h, City *c) {
	if (h->size >= h->maxSize) {
		if (h->size == INT_MAX) return false; //osi¹gniêto maksymaln¹ iloœæ elementów
		if (h->maxSize < INT_MAX / 2) {
			realloc(h->tab, 2 * h->maxSize);
		}
		else {
			realloc(h->tab, INT_MAX);
		}
	}
	if (h->tab == NULL) return false; //nie uda³o siê zaalokowaæ pamiêci
	h->tab[h->size++] = c;
	minHeapRepairUp(h);
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

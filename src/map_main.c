#include "map.h"
#include "min_heap.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#undef NDEBUG

#include <assert.h>

//////testy//////////

void showMinHeap(MinHeap *h) {
	printf("%d %d\n ", h->size, h->maxSize);
	for (int i = 0; i < h->size; ++i) {
		printf("%d ", h->tab[i]->distance);
	}
}


int main() {
	City *c1 = malloc(sizeof(City));
	City *c2 = malloc(sizeof(City));
	City *c3 = malloc(sizeof(City));
	City *c4 = malloc(sizeof(City));
	City *c5 = malloc(sizeof(City));
	City *c6 = malloc(sizeof(City));
	c1->distance = 2;
	c2->distance = 3;
	c3->distance = 7;
	c4->distance = 5;
	c5->distance = 3;
	c6->distance = 1;
	MinHeap *h = minHeapCreate();
	showMinHeap(h);
	minHeapAdd(h, c1);
	showMinHeap(h);
	printf("\n");
	minHeapAdd(h, c2);
	showMinHeap(h);
	printf("\n");
	minHeapPeak(h);
	minHeapAdd(h, c3);
	showMinHeap(h);
	printf("\n");
	minHeapAdd(h, c4);
	showMinHeap(h);
	minHeapPeak(h);
	printf("\n");
	minHeapAdd(h, c5);
	showMinHeap(h);
	printf("\n");
	minHeapAdd(h, c6);
	showMinHeap(h);



  /*
  char const* str;
  
  Map* m = newMap();
  assert(m);

  assert(addRoad(m, "Alinów", "Bór", 1, 2020));
  assert(addRoad(m, "Bór", "Cielińsk-Niekłańsk", 2, 2020));
  assert(addRoad(m, "Bór", "Dąb Stary", 3, 2020));
  assert(addRoad(m, "Cielińsk-Niekłańsk", "Emiliew", 4, 2020));
  assert(addRoad(m, "Dąb Stary", "Emiliew", 5, 2020));
  assert(addRoad(m, "Emiliew", "Bór", 8, 2020));
  assert(addRoad(m, "Emiliew", "Fraźnik Nowy", 3, 2020));
  assert(!repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2019));
  assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
  assert(!repairRoad(m, "Emiliew", "Alinów", 2020));
  assert(addRoad(m, "Fraźnik Nowy", "Grzegrzewo", 4, 2020));
  assert(addRoad(m, "Alinów", "Grzegrzewo", 10, 2020));
  assert(addRoad(m, "Homilcin", "Grzegrzewo", 5, 2020));
  assert(addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
  assert(!addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
  assert(!addRoad(m, "Cielińsk-Niekłańsk", "Fraźnik Nowy", 2, 2020));
  assert(!repairRoad(m, "Emiliew", "Bór", 2018));
  assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
  assert(repairRoad(m, "Emiliew", "Fraźnik Nowy", 2023));
  assert(addRoad(m, "Homilcin", "Cielińsk-Niekłańsk", 3, 2020));
  assert(newRoute(m, 10, "Alinów", "Emiliew"));

  str = getRouteDescription(m, 10);
  assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew") == 0);
  free((void *)str);

  assert(extendRoute(m, 10, "Homilcin"));

  str = getRouteDescription(m, 10);
  assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew"
                     ";3;2023;Fraźnik Nowy;4;2020;Grzegrzewo;5;2020;Homilcin") == 0);
  free((void *)str);
  
  deleteMap(m);
  */
 
  return 0;
}

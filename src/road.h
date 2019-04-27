#pragma once

#include "city.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct City City;
///struktura przechowuj¹ca informacje o drogach
typedef struct Road {
	unsigned length;
	int modificationDate;
	City *city1, *city2;
}Road;

///struktura przechowuj¹ca listê dróg
typedef struct RoadList {
	Road* r;
	struct RoadList *next;
}RoadList;

//dodaje element na pocz¹tek listy dróg
bool roadListAdd(RoadList **l, Road *r);

//usuwa listê dróg
void roadListDelete(RoadList *l);

///usuwa element r z listy dróg
void roadListDeleteElement(RoadList **l, Road *r);

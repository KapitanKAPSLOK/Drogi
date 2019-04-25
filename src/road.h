#pragma once

///struktura przechowuj¹ca informacje o drogach
struct Road {
	unsigned length;
	int modificationDate;
	char *City1, *City2;
};

///struktura przechowuj¹ca listê dróg
typedef struct RoadList {
	Road* r;
	struct RoadList *next;
}RoadList;
#pragma once

///struktura przechowuj�ca informacje o drogach
struct Road {
	unsigned length;
	int modificationDate;
	char *City1, *City2;
};

///struktura przechowuj�ca list� dr�g
typedef struct RoadList {
	Road* r;
	struct RoadList *next;
}RoadList;
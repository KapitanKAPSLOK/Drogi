#include "road.h"

struct Road {
	unsigned length;
	int builtYear;
	char *City1,*City2;
};

struct RoadList {
	Road r;
	RoadList *next;
};
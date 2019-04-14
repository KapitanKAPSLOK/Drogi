#include "city.h"
#include "road.h"

struct City {
	int id;
	RoadList *roads;
};

struct CityBST {
	City *c;
	CityBST *lChild,*rChild;
};

int cityHash(char *str) {
	//TODO:
}
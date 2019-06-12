#include "map.h"
#include "min_heap.h"
#include "io.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#undef NDEBUG
#include <assert.h>


int main() {
  Map* map = newMap();
  while (ioSkipToCommand()) {
	  if (ioIsCommand("addRoad"))
		  ioAddRoad(map);
	  else if (ioIsCommand("repairRoad"))
		  ioRepairRoad(map);
	  else if (ioIsCommand("getRouteDescription"))
		  ioGetRouteDescription(map);
	  else
		  ioMakeRoute(map);
	  ioSkipLine();
  }

  deleteMap(map);

  return 0;
}

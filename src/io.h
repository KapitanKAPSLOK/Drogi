#pragma once

#include "map.h"

#include <stdbool.h>

///wczytuje dane i wykonuje polecenie addRoad
void ioAddRoad(Map *m);

///wczytuje dane i wykonuje polecenie repairRoad
void ioRepairRoad(Map *m);

///wczytuje dane i wykonuje polecenie getRouteDescription
void ioGetRouteDescription(Map *m);
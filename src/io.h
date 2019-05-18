#pragma once

#include "map.h"

#include <stdbool.h>

//chceck if on stdin is given command containing [size] chars
bool ioIsCommand(const char *command);

///skip input data to first possible command, return 0 if end of file
bool ioSkipToCommand();

///wczytuje dane i wykonuje polecenie addRoad
void ioAddRoad(Map *m);

///wczytuje dane i wykonuje polecenie repairRoad
void ioRepairRoad(Map *m);

///wczytuje dane i wykonuje polecenie getRouteDescription
void ioGetRouteDescription(Map *m);

///wczytuje dane ze standardowego wejœcia i tworzy na ich podstawie now¹ drogê krajow¹
void ioMakeRoute(Map *m);
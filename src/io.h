/** @file
 * Interfejs klasy odpowiedzialnej za interakcję z użytkownikiem.
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 13.06.2019
 */

#pragma once

#include "map.h"

#include <stdbool.h>

 ///przewija dane wejściowe do końca linii
void ioSkipLine();

///sprawdza czy na standardowym wejściu znajduje się napis @p command
bool ioIsCommand(const char *command);

///przewija dane wejściowe do pierwszej możliwej komendy, jeśli skończył się plik zwraca @p false
bool ioSkipToCommand();

///wczytuje dane i wykonuje polecenie addRoad
void ioAddRoad(Map *m);

///wczytuje dane i wykonuje polecenie repairRoad
void ioRepairRoad(Map *m);

///wczytuje dane i wykonuje polecenie getRouteDescription
void ioGetRouteDescription(Map *m);

///wczytuje dane ze standardowego wejścia i tworzy na ich podstawie nową drogę krajową
void ioMakeRoute(Map *m);

///wczytuje potrzebne dane i wykonuje polecenie addRoute
void ioNewRoute(Map *m);

///wczytuje potrzebne dane i wykonuje polecenie extendRoute
void ioExtendRoute(Map *m);

///wczytuje potrzebne dane i wykonuje polecenie removeRoad
void ioRemoveRoad(Map *m);

///wczytuje potrzebne dane i wykonuje polecenie removeRoute
void ioRemoveRoute(Map *m);

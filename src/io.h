/** @file
 * Interfejs klasy odpowiedzialnej za interakcję z użytkownikiem.
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 18.05.2019
 */

#pragma once

#include "map.h"

#include <stdbool.h>

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
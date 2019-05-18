/** @file
 * Interfejs klasy odcinków dróg.
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 03.05.2019
 */
#pragma once

#include "city.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct City City;
///struktura przechowująca informacje o drogach
typedef struct Road {
	unsigned length; ///< długość odcinka drogi
	int modificationDate; ///< data ostatniej modyfikacji odcinka (data budowy lub remontu)
	City *city1, *city2; ///< miasta, które łączy odcinek drogi
}Road;

///struktura przechowująca listę dróg
typedef struct RoadList {
	Road* r; ///< wskaźnik na drogę
	struct RoadList *next; ///< wskaźnik do następnego elementu listy
}RoadList;

/** @brief Dodaje drogę @p r na początek listy odcinków dróg.
* @param[in, out] l  - wskaźnik na pierwszy element listy odcinków dróg
* @param[in] r       - wskaźnik na odcinek drogi, który ma zostać dodany
* @return @p true jeśli udało się dodać odcinek drogi lub @p false w przeciwnym wypadku
*/
bool roadListAdd(RoadList **l, Road *r);

/** @brief dodaje na początek listy pod wskaźnikiem @p l1 listę pod wskaźnikiem @p l2.
* @param[in, out] l1  - wskaźnik na listę, na której początek zostanie dodana druga lista
* @param[in] l2       - wskaźnik na listę, kktóra ma zostać dodana
*/
void roadListAddList(RoadList **l1, RoadList *l2);

/** @brief Usuwa listę dróg krajowych
* Zwalnia pamięć po strukturze listy, ale nie zwalnia pamięć po przechowywanych w niej odcinkach dróg.
* @param[in] l - wskaźnik na usuwaną listę odcinków dróg.
*/
void roadListDelete(RoadList *l);

/** @brief Usuwa element @p r z listy odcinków dróg.
* @param[in, out] l  - wskaźnik na listę, z której ma zostać usunięty element
* @param[in] r       - element do usunięcia z listy
*/
void roadListDeleteElement(RoadList **l, Road *r);

/** @brief Zwalnia pamięć po odcinkach dróg przechowywanych w liście
* @param[in] r - lista, w której ma zostać zwolniona pamięć po przechowywanych przez nią elementach
*/
void roadListDeleteElements(RoadList *r);

/** @brief Szuka w liście odcinka drogi z lub do miasta o nazwie @p str.
* Wyszukuje pierwsze miasto spełniające warunki.
* @param[in] l    - przeszukiwana lista odcinków dróg
* @param[in] str  - wskaźnik na napis reprezentujący nazwę miasta
* @return Wskaźnik do znalezionej drogi lub @p NULL jeśli nie znaleziono drogi spełniającej warunki
*/
Road *roadListFindStr(RoadList *l, const char *str);

///Odwraca podaną listę.
/// @return Odwrócona lista lub NULL, jeśli nie udało się tego zrobić
RoadList *roadListReverse(RoadList *l);

/** @brief Wyszukuje w liście drogę prowadzącą do miasta @p c.
* Znajduje pierwsze wystąpienie odcinka drogi spełniającego warunki.
* @param[in] l    - przeszukiwana lista odcinków dróg
* @param[in] c  - wskaźnik na miasto, do którego szukana jest droga
* @return wskaźnik do znalezionej drogi lub @p NULL jeśli nie znaleziono takiej
*/
Road *roadListContain(RoadList *l, City *c);

///zmienia rok ostatniej modyfikacji drogi
/** @brief Zmienia rok ostatniej modyfikacji odcinka drogi.
* Jeśli nowa data jest późniejsza od aktualnej daty ostatniej modyfikacji @p r nic nie robi.
* @param[in, out] r  - wskaźnik na modyfikowaną drogę
* @param[in] year    - rok nowej daty ostatniej modyfikacji
* @return @p true jeśli uda się zmodyfikować datę, @p false w przeciwnym wypadku
*/
bool roadRepair(Road *r, int year);

/** @brief Znajduje miasto, które łączy odcinek drogi, ale różne od miasta @p c
* Jeśli obydwa miasta są różne zwraca pierwsze miasto zapisane w strukrurze drogi @p r (@p r->city1).
* @param[in] r - wskaźnik na rozpatrywany odcinek drogi
* @param[in] c - wskaźnik na miasto, które nie zostanie zwrócone
* @return wskaźnik na znalezione miasto.
*/
City *roadGetCity(Road *r, City *c);

/** @brief Sprawdza czy podany odcinek drogi prowadzi do miasta @p c
* @param[in] r - wskaźnik na rozpatrywaną drogę
* @param[in] c - wskaźnik na miasto, do którego połaćzenie jest sprawdzane
* @return @p true jeśli odcinek drogi jest połączony bezpośrednio z @p c, @p false w przeciwnym razie
*/
bool roadConnectCity(Road *r, City *c);

/** Wypisuje informacje o drodze krajowej.
* Dane są postaci ";długość;data modyfikacji;miasto", 
* gdzie miasto jest jedno z miast, które łączy odcinek drogi, ale różne od @p c
* @param[in] r - wskaźnik na odcinek drogim którego informacje są zwracane
* @param[in] c - wskaźnik na miasto, którego nazwa nie jest wypisywana
* @return wskaźnik na napis lub @p NULL jeśli nie udało się zaalokować pamięci
*/
char *roadGetDescription(Road *r, City *c);
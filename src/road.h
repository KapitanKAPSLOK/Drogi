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
///struktura przechowuj¹ca informacje o drogach
typedef struct Road {
	unsigned length; ///< d³ugoœæ odcinka drogi
	int modificationDate; ///< data ostatniej modyfikacji odcinka (data budowy lub remontu)
	City *city1, *city2; ///< miasta, które ³¹czy odcinek drogi
}Road;

///struktura przechowuj¹ca listê dróg
typedef struct RoadList {
	Road* r; ///< wskaŸnik na drogê
	struct RoadList *next; ///< wskaŸnik do nastêpnego elementu listy
}RoadList;

/** @brief Dodaje drogê @p r na pocz¹tek listy odcinków dróg.
* @param[in, out] l  - wskaŸnik na pierwszy element listy odcinków dróg
* @param[in] r       - wskaŸnik na odcinek drogi, który ma zostaæ dodany
* @return @p true jeœli uda³o siê dodaæ odcinek drogi lub @p false w przeciwnym wypadku
*/
bool roadListAdd(RoadList **l, Road *r);

/** @brief dodaje na pocz¹tek listy pod wskaŸnikiem @p l1 listê pod wskaŸnikiem @p l2.
* @param[in, out] l1  - wskaŸnik na listê, na której pocz¹tek zostanie dodana druga lista
* @param[in] l2       - wskaŸnik na listê, kktóra ma zostaæ dodana
*/
void roadListAddList(RoadList **l1, RoadList *l2);

/** @brief Usuwa listê dróg krajowych
* Zwalnia pamiêæ po strukturze listy, ale nie zwalnia pamiêæ po przechowywanych w niej odcinkach dróg.
* @param[in] l - wskaŸnik na usuwan¹ listê odcinków dróg.
*/
void roadListDelete(RoadList *l);

/** @brief Usuwa element @p r z listy odcinków dróg.
* @param[in, out] l  - wskaŸnik na listê, z której ma zostaæ usuniêty element
* @param[in] r       - element do usuniêcia z listy
*/
void roadListDeleteElement(RoadList **l, Road *r);

/** @brief Zwalnia pamiêæ po odcinkach dróg przechowywanych w liœcie
* @param[in] r - lista, w której ma zostaæ zwolniona pamiêæ po przechowywanych przez ni¹ elementach
*/
void roadListDeleteElements(RoadList *r);

/** @brief Szuka w liœcie odcinka drogi z lub do miasta o nazwie @p str.
* Wyszukuje pierwsze miasto spe³niaj¹ce warunki.
* @param[in] l    - przeszukiwana lista odcinków dróg
* @param[in] str  - wskaŸnik na napis reprezentuj¹cy nazwê miasta
* @return WskaŸnik do znalezionej drogi lub @p NULL jeœli nie znaleziono drogi spe³niaj¹cej warunki
*/
Road *roadListFindStr(RoadList *l, const char *str);

///Odwraca podan¹ listê.
/// @return Odwrócona lista lub NULL, jeœli nie uda³o siê tego zrobiæ
RoadList *roadListReverse(RoadList *l);

/** @brief Wyszukuje w liœcie drogê prowadz¹c¹ do miasta @p c.
* Znajduje pierwsze wyst¹pienie odcinka drogi spe³niaj¹cego warunki.
* @param[in] l    - przeszukiwana lista odcinków dróg
* @param[in] c  - wskaŸnik na miasto, do którego szukana jest droga
* @return wskaŸnik do znalezionej drogi lub @p NULL jeœli nie znaleziono takiej
*/
Road *roadListContain(RoadList *l, City *c);

///zmienia rok ostatniej modyfikacji drogi
/** @brief Zmienia rok ostatniej modyfikacji odcinka drogi.
* Jeœli nowa data jest póŸniejsza od aktualnej daty ostatniej modyfikacji @p r nic nie robi.
* @param[in, out] r  - wskaŸnik na modyfikowan¹ drogê
* @param[in] year    - rok nowej daty ostatniej modyfikacji
* @return @p true jeœli uda siê zmodyfikowaæ datê, @p false w przeciwnym wypadku
*/
bool roadRepair(Road *r, int year);

/** @brief Znajduje miasto, które ³¹czy odcinek drogi, ale ró¿ne od miasta @p c
* Jeœli obydwa miasta s¹ ró¿ne zwraca pierwsze miasto zapisane w strukrurze drogi @p r (@p r->city1).
* @param[in] r - wskaŸnik na rozpatrywany odcinek drogi
* @param[in] c - wskaŸnik na miasto, które nie zostanie zwrócone
* @return wskaŸnik na znalezione miasto.
*/
City *roadGetCity(Road *r, City *c);

/** @brief Sprawdza czy podany odcinek drogi prowadzi do miasta @p c
* @param[in] r - wskaŸnik na rozpatrywan¹ drogê
* @param[in] c - wskaŸnik na miasto, do którego po³aæzenie jest sprawdzane
* @return @p true jeœli odcinek drogi jest po³¹czony bezpoœrednio z @p c, @p false w przeciwnym razie
*/
bool roadConnectCity(Road *r, City *c);

/** Wypisuje informacje o drodze krajowej.
* Dane s¹ postaci ";d³ugoœæ;data modyfikacji;miasto", 
* gdzie miasto jest jedno z miast, które ³¹czy odcinek drogi, ale ró¿ne od @p c
* @param[in] r - wskaŸnik na odcinek drogim którego informacje s¹ zwracane
* @param[in] c - wskaŸnik na miasto, którego nazwa nie jest wypisywana
* @return wskaŸnik na napis lub @p NULL jeœli nie uda³o siê zaalokowaæ pamiêci
*/
char *roadGetDescription(Road *r, City *c);
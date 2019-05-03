/** @file
 * Interfejs klasy odcink�w dr�g.
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 03.05.2019
 */
#pragma once

#include "city.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct City City;
///struktura przechowuj�ca informacje o drogach
typedef struct Road {
	unsigned length; ///< d�ugo�� odcinka drogi
	int modificationDate; ///< data ostatniej modyfikacji odcinka (data budowy lub remontu)
	City *city1, *city2; ///< miasta, kt�re ��czy odcinek drogi
}Road;

///struktura przechowuj�ca list� dr�g
typedef struct RoadList {
	Road* r; ///< wska�nik na drog�
	struct RoadList *next; ///< wska�nik do nast�pnego elementu listy
}RoadList;

/** @brief Dodaje drog� @p r na pocz�tek listy odcink�w dr�g.
* @param[in, out] l  - wska�nik na pierwszy element listy odcink�w dr�g
* @param[in] r       - wska�nik na odcinek drogi, kt�ry ma zosta� dodany
* @return @p true je�li uda�o si� doda� odcinek drogi lub @p false w przeciwnym wypadku
*/
bool roadListAdd(RoadList **l, Road *r);

/** @brief dodaje na pocz�tek listy pod wska�nikiem @p l1 list� pod wska�nikiem @p l2.
* @param[in, out] l1  - wska�nik na list�, na kt�rej pocz�tek zostanie dodana druga lista
* @param[in] l2       - wska�nik na list�, kkt�ra ma zosta� dodana
*/
void roadListAddList(RoadList **l1, RoadList *l2);

/** @brief Usuwa list� dr�g krajowych
* Zwalnia pami�� po strukturze listy, ale nie zwalnia pami�� po przechowywanych w niej odcinkach dr�g.
* @param[in] l - wska�nik na usuwan� list� odcink�w dr�g.
*/
void roadListDelete(RoadList *l);

/** @brief Usuwa element @p r z listy odcink�w dr�g.
* @param[in, out] l  - wska�nik na list�, z kt�rej ma zosta� usuni�ty element
* @param[in] r       - element do usuni�cia z listy
*/
void roadListDeleteElement(RoadList **l, Road *r);

/** @brief Zwalnia pami�� po odcinkach dr�g przechowywanych w li�cie
* @param[in] r - lista, w kt�rej ma zosta� zwolniona pami�� po przechowywanych przez ni� elementach
*/
void roadListDeleteElements(RoadList *r);

/** @brief Szuka w li�cie odcinka drogi z lub do miasta o nazwie @p str.
* Wyszukuje pierwsze miasto spe�niaj�ce warunki.
* @param[in] l    - przeszukiwana lista odcink�w dr�g
* @param[in] str  - wska�nik na napis reprezentuj�cy nazw� miasta
* @return Wska�nik do znalezionej drogi lub @p NULL je�li nie znaleziono drogi spe�niaj�cej warunki
*/
Road *roadListFindStr(RoadList *l, const char *str);

///Odwraca podan� list�.
/// @return Odwr�cona lista lub NULL, je�li nie uda�o si� tego zrobi�
RoadList *roadListReverse(RoadList *l);

/** @brief Wyszukuje w li�cie drog� prowadz�c� do miasta @p c.
* Znajduje pierwsze wyst�pienie odcinka drogi spe�niaj�cego warunki.
* @param[in] l    - przeszukiwana lista odcink�w dr�g
* @param[in] c  - wska�nik na miasto, do kt�rego szukana jest droga
* @return wska�nik do znalezionej drogi lub @p NULL je�li nie znaleziono takiej
*/
Road *roadListContain(RoadList *l, City *c);

///zmienia rok ostatniej modyfikacji drogi
/** @brief Zmienia rok ostatniej modyfikacji odcinka drogi.
* Je�li nowa data jest p�niejsza od aktualnej daty ostatniej modyfikacji @p r nic nie robi.
* @param[in, out] r  - wska�nik na modyfikowan� drog�
* @param[in] year    - rok nowej daty ostatniej modyfikacji
* @return @p true je�li uda si� zmodyfikowa� dat�, @p false w przeciwnym wypadku
*/
bool roadRepair(Road *r, int year);

/** @brief Znajduje miasto, kt�re ��czy odcinek drogi, ale r�ne od miasta @p c
* Je�li obydwa miasta s� r�ne zwraca pierwsze miasto zapisane w strukrurze drogi @p r (@p r->city1).
* @param[in] r - wska�nik na rozpatrywany odcinek drogi
* @param[in] c - wska�nik na miasto, kt�re nie zostanie zwr�cone
* @return wska�nik na znalezione miasto.
*/
City *roadGetCity(Road *r, City *c);

/** @brief Sprawdza czy podany odcinek drogi prowadzi do miasta @p c
* @param[in] r - wska�nik na rozpatrywan� drog�
* @param[in] c - wska�nik na miasto, do kt�rego po�a�zenie jest sprawdzane
* @return @p true je�li odcinek drogi jest po��czony bezpo�rednio z @p c, @p false w przeciwnym razie
*/
bool roadConnectCity(Road *r, City *c);

/** Wypisuje informacje o drodze krajowej.
* Dane s� postaci ";d�ugo��;data modyfikacji;miasto", 
* gdzie miasto jest jedno z miast, kt�re ��czy odcinek drogi, ale r�ne od @p c
* @param[in] r - wska�nik na odcinek drogim kt�rego informacje s� zwracane
* @param[in] c - wska�nik na miasto, kt�rego nazwa nie jest wypisywana
* @return wska�nik na napis lub @p NULL je�li nie uda�o si� zaalokowa� pami�ci
*/
char *roadGetDescription(Road *r, City *c);
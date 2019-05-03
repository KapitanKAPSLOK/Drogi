/** @file
 * Interfejs klasy dr�g krajowych
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 03.04.2019
 */
#pragma once

#include "road.h"

///struktura przechowuj�ca informacj� o drogach krajowych
typedef struct Route {
	City *start,*end;  ///< miasta mi�dzy kt�rymi poprowadzona jest droga
	unsigned id; ///< identyfikator drogi krajowej
	RoadList *roads; ///< lista odcink�w dr�g wchodz�cej w sk�ad drogi krajowej
}Route;

///struktura przechowuj�ca list� dr�g
typedef struct RouteList {
	Route* r;
	struct RouteList *next;
}RouteList;


/** @brief Tworzy now� drog� krajow�.
* Tworzy now� drog� krajow� o numerze @p nr, pomi�dzy miastami @p start i @p end 
*z�o�on� z odcink�w dr�g podanych przez @roads
* @param[in] nr        - identyfikator drogi krajowej
* @param[in] roads     - lista odcink�w dr�g sk�adaj�cych si� na tworzon� drog� krajow�
* @param[in] start     - wska�nik na miasto, w kt�rym zaczyna si� droga
* @param[in] end       - wska�nik na miasto, na kt�rym ko�czy si� droga krajowa
* @return Wska�nik na utworzon� drog� krajow� lub NULL w przypadku braku pami�ci
*/
Route *routeMake(unsigned nr, RoadList *roads, City *start, City *end);

/** @brief Dodaje drog� @p r na pocz�tek listy dr�g krajowych.
* @param[in, out] l  - wska�nik na pierwszy element listy dr�g
* @param[in] r       - wska�nik na drog�, kt�ra ma zosta� dodana
* @return @p true je�li uda�o si� doda� drog� lub @p false w przeciwnym wypadku
*/
bool routeListAdd(RouteList **l, Route *r);

/** @brief Usuwa list� dr�g krajowych
* Zwalnia pami�� po strukturze listy oraz pami�� po przechowywanych w niej drogach krajowych.
* @param[in] l - wska�nik na usuwan� list� dr�g krajowych
*/
void routeListDelete(RouteList *l);

/** @brief Usuwa drog� z listy dr�g krajowych.
* Usuwa drog� wskazan� przez wska�nik @p r z listy pod wska�nikiem @p l 
* lub nic nie robi je�li podanej drogi nie ma na li�cie
* @param[in, out] l  - wska�nik na list�, z kt�rej ma zostac usuni�ty element
* @param[in] r       - wska�nik na drog�, kt�ra ma zosta� usuni�ta
*/
void routeListDeleteElement(RouteList **l, Route *r);

/** @brief Szuka w li�cie dr�g krajowych drogi o podanym numerze
* @param[in] l  - wska�nik na list� dr�g krajowych
* @param[in] nr - numer szukanej drogi
* @return Wska�nik na drog� krajow� o podanym numerze lub NULL je�li takiej drogi nie ma na li�cie
*/
Route *routeListFind(RouteList *l, unsigned nr);

/** @brief Tworzy list� z miast, przez kt�re przechodzi droga krajowa.
* @param[in] r - wska�nik na drog� krajow�
* @return List� miast, przez kt�re przechodzi droga krajowa @p r lub NULL je�li zabraknie pami�ci
*/
CityList *routeMakeCityList(Route *r);

/** @brief Znajduje najkr�tsz� drog� mi�dzy miastami.
* Wyszukuje ci�g odcink�w dr�g ��cz�cy miasta @p start i @p end,
* kt�rego suma d�ugo�ci poszczeg�lnych odcink�w jest najkr�tsza. Je�li istnieje wi�cej jeden taki ci�g dr�g to
* dla ka�dego wariantu wyznacza najstarszy odcinek drogi i wybiera wariant, w kt�rym ten odcinek jest najm�odszy.
* Brane podd uwag� s� tylko te odcinki dr�g, kt�re nie przebiegaj� przez miasta podane w li�cie @p cities.
* Dodatkowo je�li zmienna @p norDirectly jest r�wna @p ture pomijany jest odcinek drogi ��cz�cy podane miasta bezpo�rednio
* @param[in] start        - wska�nik na pocz�tkowe miasto
* @param[in] end          - wska�nik na ko�cowe miasto
* @param[in] cities       - lista miast, przez kt�re nie mo�e przebiega� szukana droga
* @param[in] notDirectly  - m�wi czy nie uwzgl�dnia� odcinka drogi ��cz�cego miasta bezpo�rednio
* @return List� odcink�w dr�g stanowi�ce najkr�tsz� tras� lub @p NULL je�li wyst�pi� b��d: 
* kt�re� z miast nie istnieje (jest @p NULL), obie nazwy s� identyczne, 
*nie uda�o si� jednoznacznie wyznaczy� najkr�tszej trasy, nie uda�o si� zaalokowa� pami�ci
*/
RoadList *connectCities(City *start, City *end, CityList *cities, bool notDirectly);

/** @brief Sprawdza czy mo�na naprawi� drog� krajow� po usuni�cia odcinka drogi mi�dzy podanymi miastami.
* @param[in] r      - droga krajowa, z kt�rej ma zosta� usuni�ty odcinek drogi
* @param[in] c1, c2 - miasta, mi�dzy kt�rymi odcinek drogi ma zosta� usuni�ty
* @return @p true je�i to mo�liwe, @p false w przeciwnym razie
*/
bool routeCanChange(Route *r, City *c1, City *c2);

/** @brief Dodaje do drogi krajowej najkr�tsz� �cie�k� ��cz�c� podane miasta, ale nie bezpo�rednio.
* �eby mie� pewno��, �e operacja si� uda nale�y sprawdzi� to funkcj� @ref routeCanChange.
* @param[in]  r      - droga krajowa, do kt�rej zostanie dodany odcinek mi�dzy miastami @p c1, @p c2
* @param[in] c1, c2  - miasta, mi�dzy kt�rymi ma zosta� dodana nowa droga
*/
void routeFix(Route *r, City *c1, City *c2);
/** @file
 * Interfejs klasy dróg krajowych
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 03.04.2019
 */
#pragma once

#include "road.h"

///struktura przechowująca informację o drogach krajowych
typedef struct Route {
	City *start,*end;  ///< miasta między którymi poprowadzona jest droga
	unsigned id; ///< identyfikator drogi krajowej
	RoadList *roads; ///< lista odcinków dróg wchodzącej w skład drogi krajowej
}Route;

///struktura przechowująca listę dróg
typedef struct RouteList {
	Route* r;
	struct RouteList *next;
}RouteList;


/** @brief Tworzy nową drogę krajową.
* Tworzy nową drogę krajową o numerze @p nr, pomiędzy miastami @p start i @p end 
*złożoną z odcinków dróg podanych przez @roads
* @param[in] nr        - identyfikator drogi krajowej
* @param[in] roads     - lista odcinków dróg składających się na tworzoną drogę krajową
* @param[in] start     - wskaźnik na miasto, w którym zaczyna się droga
* @param[in] end       - wskaźnik na miasto, na którym kończy się droga krajowa
* @return Wskaźnik na utworzoną drogę krajową lub NULL w przypadku braku pamięci
*/
Route *routeMake(unsigned nr, RoadList *roads, City *start, City *end);

/** @brief Dodaje drogę @p r na początek listy dróg krajowych.
* @param[in, out] l  - wskaźnik na pierwszy element listy dróg
* @param[in] r       - wskaźnik na drogę, która ma zostać dodana
* @return @p true jeśli udało się dodać drogę lub @p false w przeciwnym wypadku
*/
bool routeListAdd(RouteList **l, Route *r);

/** @brief Usuwa listę dróg krajowych
* Zwalnia pamięć po strukturze listy oraz pamięć po przechowywanych w niej drogach krajowych.
* @param[in] l - wskaźnik na usuwaną listę dróg krajowych
*/
void routeListDelete(RouteList *l);

/** @brief Usuwa drogę z listy dróg krajowych.
* Usuwa drogę wskazaną przez wskaźnik @p r z listy pod wskaźnikiem @p l 
* lub nic nie robi jeśli podanej drogi nie ma na liście
* @param[in, out] l  - wskaźnik na listę, z której ma zostac usunięty element
* @param[in] r       - wskaźnik na drogę, która ma zostać usunięta
*/
void routeListDeleteElement(RouteList **l, Route *r);

/** @brief Szuka w liście dróg krajowych drogi o podanym numerze
* @param[in] l  - wskaźnik na listę dróg krajowych
* @param[in] nr - numer szukanej drogi
* @return Wskaźnik na drogę krajową o podanym numerze lub NULL jeśli takiej drogi nie ma na liście
*/
Route *routeListFind(RouteList *l, unsigned nr);

/** @brief Tworzy listę z miast, przez które przechodzi droga krajowa.
* @param[in] r - wskaźnik na drogę krajową
* @return Listę miast, przez które przechodzi droga krajowa @p r lub NULL jeśli zabraknie pamięci
*/
CityList *routeMakeCityList(Route *r);

/** @brief Znajduje najkrótszą drogę między miastami.
* Wyszukuje ciąg odcinków dróg łączący miasta @p start i @p end,
* którego suma długości poszczególnych odcinków jest najkrótsza. Jeśli istnieje więcej jeden taki ciąg dróg to
* dla każdego wariantu wyznacza najstarszy odcinek drogi i wybiera wariant, w którym ten odcinek jest najmłodszy.
* Brane podd uwagę są tylko te odcinki dróg, które nie przebiegają przez miasta podane w liście @p cities.
* Dodatkowo jeśli zmienna @p norDirectly jest równa @p ture pomijany jest odcinek drogi łączący podane miasta bezpośrednio
* @param[in] start        - wskaźnik na początkowe miasto
* @param[in] end          - wskaźnik na końcowe miasto
* @param[in] cities       - lista miast, przez które nie może przebiegać szukana droga
* @param[in] notDirectly  - mówi czy nie uwzględniać odcinka drogi łączącego miasta bezpośrednio
* @return Listę odcinków dróg stanowiące najkrótszą trasę lub @p NULL jeśli wystąpił błąd: 
* któreś z miast nie istnieje (jest @p NULL), obie nazwy są identyczne, 
*nie udało się jednoznacznie wyznaczyć najkrótszej trasy, nie udało się zaalokować pamięci
*/
RoadList *connectCities(City *start, City *end, CityList *cities, bool notDirectly);

/** @brief Sprawdza czy można naprawić drogę krajową po usunięcia odcinka drogi między podanymi miastami.
* @param[in] r      - droga krajowa, z której ma zostać usunięty odcinek drogi
* @param[in] c1, c2 - miasta, między którymi odcinek drogi ma zostać usunięty
* @return @p true jeśi to możliwe, @p false w przeciwnym razie
*/
bool routeCanChange(Route *r, City *c1, City *c2);

/** @brief Dodaje do drogi krajowej najkrótszą ścieżkę łączącą podane miasta, ale nie bezpośrednio.
* Żeby mieć pewność, że operacja się uda należy sprawdzić to funkcją @ref routeCanChange.
* @param[in]  r      - droga krajowa, do której zostanie dodany odcinek między miastami @p c1, @p c2
* @param[in] c1, c2  - miasta, między którymi ma zostać dodana nowa droga
*/
void routeFix(Route *r, City *c1, City *c2);
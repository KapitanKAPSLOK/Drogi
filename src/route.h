/** @file
 * Interfejs klasy dróg krajowych
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 03.04.2019
 */
#pragma once

#include "road.h"

///struktura przechowuj¹ca informacjê o drogach krajowych
typedef struct Route {
	City *start,*end;  ///< miasta miêdzy którymi poprowadzona jest droga
	unsigned id; ///< identyfikator drogi krajowej
	RoadList *roads; ///< lista odcinków dróg wchodz¹cej w sk³ad drogi krajowej
}Route;

///struktura przechowuj¹ca listê dróg
typedef struct RouteList {
	Route* r;
	struct RouteList *next;
}RouteList;


/** @brief Tworzy now¹ drogê krajow¹.
* Tworzy now¹ drogê krajow¹ o numerze @p nr, pomiêdzy miastami @p start i @p end 
*z³o¿on¹ z odcinków dróg podanych przez @roads
* @param[in] nr        - identyfikator drogi krajowej
* @param[in] roads     - lista odcinków dróg sk³adaj¹cych siê na tworzon¹ drogê krajow¹
* @param[in] start     - wskaŸnik na miasto, w którym zaczyna siê droga
* @param[in] end       - wskaŸnik na miasto, na którym koñczy siê droga krajowa
* @return WskaŸnik na utworzon¹ drogê krajow¹ lub NULL w przypadku braku pamiêci
*/
Route *routeMake(unsigned nr, RoadList *roads, City *start, City *end);

/** @brief Dodaje drogê @p r na pocz¹tek listy dróg krajowych.
* @param[in, out] l  - wskaŸnik na pierwszy element listy dróg
* @param[in] r       - wskaŸnik na drogê, która ma zostaæ dodana
* @return @p true jeœli uda³o siê dodaæ drogê lub @p false w przeciwnym wypadku
*/
bool routeListAdd(RouteList **l, Route *r);

/** @brief Usuwa listê dróg krajowych
* Zwalnia pamiêæ po strukturze listy oraz pamiêæ po przechowywanych w niej drogach krajowych.
* @param[in] l - wskaŸnik na usuwan¹ listê dróg krajowych
*/
void routeListDelete(RouteList *l);

/** @brief Usuwa drogê z listy dróg krajowych.
* Usuwa drogê wskazan¹ przez wskaŸnik @p r z listy pod wskaŸnikiem @p l 
* lub nic nie robi jeœli podanej drogi nie ma na liœcie
* @param[in, out] l  - wskaŸnik na listê, z której ma zostac usuniêty element
* @param[in] r       - wskaŸnik na drogê, która ma zostaæ usuniêta
*/
void routeListDeleteElement(RouteList **l, Route *r);

/** @brief Szuka w liœcie dróg krajowych drogi o podanym numerze
* @param[in] l  - wskaŸnik na listê dróg krajowych
* @param[in] nr - numer szukanej drogi
* @return WskaŸnik na drogê krajow¹ o podanym numerze lub NULL jeœli takiej drogi nie ma na liœcie
*/
Route *routeListFind(RouteList *l, unsigned nr);

/** @brief Tworzy listê z miast, przez które przechodzi droga krajowa.
* @param[in] r - wskaŸnik na drogê krajow¹
* @return Listê miast, przez które przechodzi droga krajowa @p r lub NULL jeœli zabraknie pamiêci
*/
CityList *routeMakeCityList(Route *r);

/** @brief Znajduje najkrótsz¹ drogê miêdzy miastami.
* Wyszukuje ci¹g odcinków dróg ³¹cz¹cy miasta @p start i @p end,
* którego suma d³ugoœci poszczególnych odcinków jest najkrótsza. Jeœli istnieje wiêcej jeden taki ci¹g dróg to
* dla ka¿dego wariantu wyznacza najstarszy odcinek drogi i wybiera wariant, w którym ten odcinek jest najm³odszy.
* Brane podd uwagê s¹ tylko te odcinki dróg, które nie przebiegaj¹ przez miasta podane w liœcie @p cities.
* Dodatkowo jeœli zmienna @p norDirectly jest równa @p ture pomijany jest odcinek drogi ³¹cz¹cy podane miasta bezpoœrednio
* @param[in] start        - wskaŸnik na pocz¹tkowe miasto
* @param[in] end          - wskaŸnik na koñcowe miasto
* @param[in] cities       - lista miast, przez które nie mo¿e przebiegaæ szukana droga
* @param[in] notDirectly  - mówi czy nie uwzglêdniaæ odcinka drogi ³¹cz¹cego miasta bezpoœrednio
* @return Listê odcinków dróg stanowi¹ce najkrótsz¹ trasê lub @p NULL jeœli wyst¹pi³ b³¹d: 
* któreœ z miast nie istnieje (jest @p NULL), obie nazwy s¹ identyczne, 
*nie uda³o siê jednoznacznie wyznaczyæ najkrótszej trasy, nie uda³o siê zaalokowaæ pamiêci
*/
RoadList *connectCities(City *start, City *end, CityList *cities, bool notDirectly);

/** @brief Sprawdza czy mo¿na naprawiæ drogê krajow¹ po usuniêcia odcinka drogi miêdzy podanymi miastami.
* @param[in] r      - droga krajowa, z której ma zostaæ usuniêty odcinek drogi
* @param[in] c1, c2 - miasta, miêdzy którymi odcinek drogi ma zostaæ usuniêty
* @return @p true jeœi to mo¿liwe, @p false w przeciwnym razie
*/
bool routeCanChange(Route *r, City *c1, City *c2);

/** @brief Dodaje do drogi krajowej najkrótsz¹ œcie¿kê ³¹cz¹c¹ podane miasta, ale nie bezpoœrednio.
* ¯eby mieæ pewnoœæ, ¿e operacja siê uda nale¿y sprawdziæ to funkcj¹ @ref routeCanChange.
* @param[in]  r      - droga krajowa, do której zostanie dodany odcinek miêdzy miastami @p c1, @p c2
* @param[in] c1, c2  - miasta, miêdzy którymi ma zostaæ dodana nowa droga
*/
void routeFix(Route *r, City *c1, City *c2);
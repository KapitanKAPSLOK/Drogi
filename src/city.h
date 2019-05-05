/** @file
 * Interfejs klasy miast.
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 03.04.2019
 */
#pragma once

#include "road.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct RoadList RoadList;

///struktura przechowuj¹ca informacje o miastach
typedef struct City {
	char* name; ///< nazwa miasta
	int *temporaryData; ///< miejsce, gdzie algorytmy korzystaj¹ce ze struktury mog¹ umieszczaæ dane tymczasowe
	RoadList *roads;  ///< lista odcinków dróg jakie wychodz¹ z miasta
}City;

///struktura przechowuj¹ca listê miast
typedef struct CityList {
	City* c; ///< wskaŸnik na przechowywane w strukturze miasto
	struct CityList *next; ///< wskaŸnik na nastêpny element
}CityList;

///struktura hash tablicy dla miast
typedef struct CityHashTable {
	int a, b;  ///< wspó³czynniki wykorzystywane przy hashowaniu
	int size;  ///< aktualny rozmiar tablicy tab na dane
	int numberOfElements; ///< iloœæ elementów przechowywana w tablicy
	CityList **tab; ///< tablica rozmiaru size przechowuj¹ca listy elementów
}CityHashTable;

//////////////////cityList/////////////////

/** Usuwa listê miast, ale nie usuwa samych miast.
@param[in] l - lista miast
*/
void cityListDelete(CityList *l);

/** Usuwa element z listy miast
@param[in, out] l  - wskaŸnik na listê miast
@param[in] r       - element, który ma byc usuniêty
*/
void cityListDeleteElement(CityList **l, City *r);

/** Zwalnia pamiêæ po elementach przechowywanych w liœcie.
Usuwa zarówno przechowywane miasta jak i nale¿¹ce do nich odcinki dróg
@param[in] l - lista miast
*/
void cityListDeleteElements(CityList *l);

/** Dodaje element na pocz¹tek listy.
@param[in, out] l  - wskaŸnik na listê miast
@param[in] c       - wskaŸnik na miasto, które ma zostaæ dodane
@return @p true jeœli uda³o siê dodaæ element lub @p false jeœli zabrak³o pamiêci
*/
bool cityListAdd(CityList **l, City *c);

/** Dodaje na pocz¹tek listy @p l1 listê @p l2
@param[in, out] - wskaŸnik na listê, do której maj¹ zostaæ dodane elementy
@param[in] - lista miast, która ma zostaæ dodana
*/
void cityListAddList(CityList **l1, CityList *l2);

/** Szuka miasta @p c w liœcie @p l.
Szuka miasta na liœcie o takiej samej nazwie jak podane miasto @p c.
@param[in] l - przeszukiwana lista miast
@param[in] c - wskaŸnik na szukane miasto
@return wskaŸnik do szukanego miasta znajdujacego siê na liœcie lub @p NULL jeœli nie uda³o siê go znaleŸæ
*/
City *cityListFind(CityList *l, City *c);

/** Szuka miasta o nazwie @p str na liœcie.
@param[in] l    - lista miast
@param[in] str  - wskaŸnik na napis reprezentuj¹cy nazwê miasta
@return wskaŸnik na znalezione miasto lub @p NULL jeœli miasta o podanej nazwie nie by³o na liœcie
*/
City *cityListFindStr(CityList *l, const char *str);

////////////////////cityHashTable/////////////////////////

/** Tworzy now¹ hash tablicê z podanej listy miast.
@param[in] cities          - lista miast, z których ma zostaæ utworzona hash tablica
@param[in] numberOfCities  - liczba miast znajduj¹cych siê na liœcie @p cities
@return wskaŸnik na utworzon¹ hash tablicê lub @p NULL jesli nie uda³o siê zaalokowaæ pamiêci
*/
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities);

/** Usuwa strukturê.
Usuwa zarówno strukturê jak i zwalnia pamiêæ po przechowywanych w niej elementach.
@param[in] t - wskaŸnik na strukturê hash tablicy
*/
void cityHashTableDelete(CityHashTable *t);

/** Dodaje element do hash tablicy.
Dodaje do tablicy miasto o nazwie reprezentowanej przez napis @p str jeœli jeszcze nie istnieje.
@param[in, out] t  - wskaŸnik na hash tablicê
@param[in] str     - wskaŸnik na napis reprezentuj¹cy nazwê miasta
@return wskaŸnik na dodane miasto lub @p NULL jeœli nie uda³o siê go dodaæ
*/
City *cityHashTableAdd(CityHashTable **t, const char *str);

/** Szuka w hash tablicy miasta o nazwie reprezentowanej przez napis @p str.
@param[in] t   - wskaŸnik na hash table
@param[in] str - wskaŸnik na napis reprezentuj¹cy nazwê miasta
@return wskaŸnik na znalezione miasto lub @p NULL jeœli go nie znaleziono
*/
City *cityHashTableFind(CityHashTable *t, const char *str);

///////////////////////inne///////////////////////////

/** Zamienia podane miasto na liczbê.
@param[in] c - wskaŸnik na miasto
@return liczba odpowiadaj¹ca danemu miastu
*/
unsigned cityHash(City *c);

/** Dodaje odcinek drogi miêdzy podanymi miastami.
Odcinek drogi jest dodawany tylko wtedy jeœli nie istnieje bezpoœrednie po³¹czenie miêdzy tymi miastami.
@param[in] c1, c2      - wskaŸnik na miasta do po³¹czenia odcinkiem drogi
@param[in] length      - d³ugoœæ nowego odcinka drogi
@param[in] builtYear   - rok budowy drogi (ostatniej modyfikacji)
@return @p true jeœli uda³o siê dodaæ odcinek drogi lub @p false w przeciwnym razie
*/
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear);

/** Usuwa miasto.
Usuwane jest zarówno miasto jak i wszystkie wychodz¹ce z niego odcinki dróg.
@param[in] c - wskaŸnik na miasto do usuniêcia
*/
void cityDelete(City *c);

/** Uwsuwa dane tymczasowe miasta i wszystkich miast, do których da siê z niego dojœæ drogami
@param[in] c - wskaŸnik na miasto
*/
void cityDeleteTemporaryData(City *c);
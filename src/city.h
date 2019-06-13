/** @file
 * Interfejs klasy miast.
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 13.06.2019
 */
#pragma once

#include "road.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct RoadList RoadList;

///struktura przechowująca informacje o miastach
typedef struct City {
	char* name; ///< nazwa miasta
	int *temporaryData; ///< miejsce, gdzie algorytmy korzystające ze struktury mogą umieszczać dane tymczasowe
	RoadList *roads;  ///< lista odcinków dróg jakie wychodzą z miasta
}City;

///struktura przechowująca listę miast
typedef struct CityList {
	City* c; ///< wskaźnik na przechowywane w strukturze miasto
	struct CityList *next; ///< wskaźnik na następny element
}CityList;

///struktura hash tablicy dla miast
typedef struct CityHashTable {
	int a, b;  ///< współczynniki wykorzystywane przy hashowaniu
	int size;  ///< aktualny rozmiar tablicy tab na dane
	int numberOfElements; ///< ilość elementów przechowywana w tablicy
	CityList **tab; ///< tablica rozmiaru size przechowująca listy elementów
}CityHashTable;

//////////////////cityList/////////////////

/** Usuwa listę miast, ale nie usuwa samych miast.
@param[in] l - lista miast
*/
void cityListDelete(CityList *l);

/** Usuwa element z listy miast
@param[in, out] l  - wskaźnik na listę miast
@param[in] r       - element, który ma byc usunięty
*/
void cityListDeleteElement(CityList **l, City *r);

/** Zwalnia pamięć po elementach przechowywanych w liście.
Usuwa zarówno przechowywane miasta jak i należące do nich odcinki dróg
@param[in] l - lista miast
*/
void cityListDeleteElements(CityList *l);

/** Dodaje element na początek listy.
@param[in, out] l  - wskaźnik na listę miast
@param[in] c       - wskaźnik na miasto, które ma zostać dodane
@return @p true jeśli udało się dodać element lub @p false jeśli zabrakło pamięci
*/
bool cityListAdd(CityList **l, City *c);

/** Dodaje na początek listy @p l1 listę @p l2
@param[in, out] - wskaźnik na listę, do której mają zostać dodane elementy
@param[in] - lista miast, która ma zostać dodana
*/
void cityListAddList(CityList **l1, CityList *l2);

/** Szuka miasta @p c w liście @p l.
Szuka miasta na liście o takiej samej nazwie jak podane miasto @p c.
@param[in] l - przeszukiwana lista miast
@param[in] c - wskaźnik na szukane miasto
@return wskaźnik do szukanego miasta znajdujacego się na liście lub @p NULL jeśli nie udało się go znaleźć
*/
City *cityListFind(CityList *l, City *c);

/** Szuka miasta o nazwie @p str na liście.
@param[in] l    - lista miast
@param[in] str  - wskaźnik na napis reprezentujący nazwę miasta
@return wskaźnik na znalezione miasto lub @p NULL jeśli miasta o podanej nazwie nie było na liście
*/
City *cityListFindStr(CityList *l, const char *str);

////////////////////cityHashTable/////////////////////////

/** Tworzy nową hash tablicę z podanej listy miast.
@param[in] cities          - lista miast, z których ma zostać utworzona hash tablica
@param[in] numberOfCities  - liczba miast znajdujących się na liście @p cities
@return wskaźnik na utworzoną hash tablicę lub @p NULL jesli nie udało się zaalokować pamięci
*/
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities);

/** Usuwa strukturę.
Usuwa zarówno strukturę jak i zwalnia pamięć po przechowywanych w niej elementach.
@param[in] t - wskaźnik na strukturę hash tablicy
*/
void cityHashTableDelete(CityHashTable *t);

/** Dodaje element do hash tablicy.
Dodaje do tablicy miasto o nazwie reprezentowanej przez napis @p str jeśli jeszcze nie istnieje.
@param[in, out] t  - wskaźnik na hash tablicę
@param[in] str     - wskaźnik na napis reprezentujący nazwę miasta
@return wskaźnik na dodane miasto lub @p NULL jeśli nie udało się go dodać
*/
City *cityHashTableAdd(CityHashTable **t, const char *str);

/** Szuka w hash tablicy miasta o nazwie reprezentowanej przez napis @p str.
@param[in] t   - wskaźnik na hash table
@param[in] str - wskaźnik na napis reprezentujący nazwę miasta
@return wskaźnik na znalezione miasto lub @p NULL jeśli go nie znaleziono
*/
City *cityHashTableFind(CityHashTable *t, const char *str);

///////////////////////inne///////////////////////////

/** Zamienia podane miasto na liczbę.
@param[in] c - wskaźnik na miasto
@return liczba odpowiadająca danemu miastu
*/
unsigned cityHash(City *c);

/** Dodaje odcinek drogi między podanymi miastami.
Odcinek drogi jest dodawany tylko wtedy jeśli nie istnieje bezpośrednie połączenie między tymi miastami.
@param[in] c1, c2      - wskaźnik na miasta do połączenia odcinkiem drogi
@param[in] length      - długość nowego odcinka drogi
@param[in] builtYear   - rok budowy drogi (ostatniej modyfikacji)
@return @p true jeśli udało się dodać odcinek drogi lub @p false w przeciwnym razie
*/
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear);

/** Usuwa miasto.
Usuwane jest zarówno miasto jak i wszystkie wychodzące z niego odcinki dróg.
@param[in] c - wskaźnik na miasto do usunięcia
*/
void cityDelete(City *c);

/** Uwsuwa dane tymczasowe miasta i wszystkich miast, do których da się z niego dojść drogami
@param[in] c - wskaźnik na miasto
*/
void cityDeleteTemporaryData(City *c);

///sprawdza czy napis może być poprawną nazwą miasta
/// @return @p true jeśli nazwa jest poprawna lub @p false jeśli nie
bool cityCheckName(const char* str);
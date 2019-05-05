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

///struktura przechowuj�ca informacje o miastach
typedef struct City {
	char* name; ///< nazwa miasta
	int *temporaryData; ///< miejsce, gdzie algorytmy korzystaj�ce ze struktury mog� umieszcza� dane tymczasowe
	RoadList *roads;  ///< lista odcink�w dr�g jakie wychodz� z miasta
}City;

///struktura przechowuj�ca list� miast
typedef struct CityList {
	City* c; ///< wska�nik na przechowywane w strukturze miasto
	struct CityList *next; ///< wska�nik na nast�pny element
}CityList;

///struktura hash tablicy dla miast
typedef struct CityHashTable {
	int a, b;  ///< wsp�czynniki wykorzystywane przy hashowaniu
	int size;  ///< aktualny rozmiar tablicy tab na dane
	int numberOfElements; ///< ilo�� element�w przechowywana w tablicy
	CityList **tab; ///< tablica rozmiaru size przechowuj�ca listy element�w
}CityHashTable;

//////////////////cityList/////////////////

/** Usuwa list� miast, ale nie usuwa samych miast.
@param[in] l - lista miast
*/
void cityListDelete(CityList *l);

/** Usuwa element z listy miast
@param[in, out] l  - wska�nik na list� miast
@param[in] r       - element, kt�ry ma byc usuni�ty
*/
void cityListDeleteElement(CityList **l, City *r);

/** Zwalnia pami�� po elementach przechowywanych w li�cie.
Usuwa zar�wno przechowywane miasta jak i nale��ce do nich odcinki dr�g
@param[in] l - lista miast
*/
void cityListDeleteElements(CityList *l);

/** Dodaje element na pocz�tek listy.
@param[in, out] l  - wska�nik na list� miast
@param[in] c       - wska�nik na miasto, kt�re ma zosta� dodane
@return @p true je�li uda�o si� doda� element lub @p false je�li zabrak�o pami�ci
*/
bool cityListAdd(CityList **l, City *c);

/** Dodaje na pocz�tek listy @p l1 list� @p l2
@param[in, out] - wska�nik na list�, do kt�rej maj� zosta� dodane elementy
@param[in] - lista miast, kt�ra ma zosta� dodana
*/
void cityListAddList(CityList **l1, CityList *l2);

/** Szuka miasta @p c w li�cie @p l.
Szuka miasta na li�cie o takiej samej nazwie jak podane miasto @p c.
@param[in] l - przeszukiwana lista miast
@param[in] c - wska�nik na szukane miasto
@return wska�nik do szukanego miasta znajdujacego si� na li�cie lub @p NULL je�li nie uda�o si� go znale��
*/
City *cityListFind(CityList *l, City *c);

/** Szuka miasta o nazwie @p str na li�cie.
@param[in] l    - lista miast
@param[in] str  - wska�nik na napis reprezentuj�cy nazw� miasta
@return wska�nik na znalezione miasto lub @p NULL je�li miasta o podanej nazwie nie by�o na li�cie
*/
City *cityListFindStr(CityList *l, const char *str);

////////////////////cityHashTable/////////////////////////

/** Tworzy now� hash tablic� z podanej listy miast.
@param[in] cities          - lista miast, z kt�rych ma zosta� utworzona hash tablica
@param[in] numberOfCities  - liczba miast znajduj�cych si� na li�cie @p cities
@return wska�nik na utworzon� hash tablic� lub @p NULL jesli nie uda�o si� zaalokowa� pami�ci
*/
CityHashTable *cityHashTableMake(CityList *cities, int numberOfCities);

/** Usuwa struktur�.
Usuwa zar�wno struktur� jak i zwalnia pami�� po przechowywanych w niej elementach.
@param[in] t - wska�nik na struktur� hash tablicy
*/
void cityHashTableDelete(CityHashTable *t);

/** Dodaje element do hash tablicy.
Dodaje do tablicy miasto o nazwie reprezentowanej przez napis @p str je�li jeszcze nie istnieje.
@param[in, out] t  - wska�nik na hash tablic�
@param[in] str     - wska�nik na napis reprezentuj�cy nazw� miasta
@return wska�nik na dodane miasto lub @p NULL je�li nie uda�o si� go doda�
*/
City *cityHashTableAdd(CityHashTable **t, const char *str);

/** Szuka w hash tablicy miasta o nazwie reprezentowanej przez napis @p str.
@param[in] t   - wska�nik na hash table
@param[in] str - wska�nik na napis reprezentuj�cy nazw� miasta
@return wska�nik na znalezione miasto lub @p NULL je�li go nie znaleziono
*/
City *cityHashTableFind(CityHashTable *t, const char *str);

///////////////////////inne///////////////////////////

/** Zamienia podane miasto na liczb�.
@param[in] c - wska�nik na miasto
@return liczba odpowiadaj�ca danemu miastu
*/
unsigned cityHash(City *c);

/** Dodaje odcinek drogi mi�dzy podanymi miastami.
Odcinek drogi jest dodawany tylko wtedy je�li nie istnieje bezpo�rednie po��czenie mi�dzy tymi miastami.
@param[in] c1, c2      - wska�nik na miasta do po��czenia odcinkiem drogi
@param[in] length      - d�ugo�� nowego odcinka drogi
@param[in] builtYear   - rok budowy drogi (ostatniej modyfikacji)
@return @p true je�li uda�o si� doda� odcinek drogi lub @p false w przeciwnym razie
*/
bool cityAddRoad(City *c1, City *c2, unsigned length, int builtYear);

/** Usuwa miasto.
Usuwane jest zar�wno miasto jak i wszystkie wychodz�ce z niego odcinki dr�g.
@param[in] c - wska�nik na miasto do usuni�cia
*/
void cityDelete(City *c);

/** Uwsuwa dane tymczasowe miasta i wszystkich miast, do kt�rych da si� z niego doj�� drogami
@param[in] c - wska�nik na miasto
*/
void cityDeleteTemporaryData(City *c);
/** @file
 * Interfejs struktury kopca minimalnego.
 * W kopcu przechowywane s� wska�niki na miasta. Miasta por�wnywane s� wzgl�dem warto�ci 
 * przechowywanych pod zerowym indeksem w @p temporaryData (miasto->temporaryData[0]).
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 22.04.2019
 */

#pragma once
#include "city.h"
#include <stdbool.h>
#include <stdlib.h>

///struktura kopca minimalnego
typedef struct MinHeap {
	int size; ///< aktualna ilo�� element�w na kopcu
	int maxSize; ///< rozmiar zaalokowanej tablicy
	City **tab; ///< tablica przechowuj�ca elementy kopca
}MinHeap;

/** Tworzy nowy pusty kopiec.
* Domy�lny rozmiar tablicy na kopiec wynosi 10 (lub 1 w przypadku ograniczonej pami�ci).
* @return wska�nik na nowy kopiec
*/
MinHeap* minHeapCreate(void);

/** Naprawia struktur� kopca przenosz�c element do g�ry.
* Por�wnuje element o numerze @p indeks z jego rodzicem i zamienia te 2 elementy miejscami
* je�li element jest wi�kszy od rodzica. Post�powanie jest powtarzane a� kopiec ponownie bedzie
* spe�nia� warunki kopca minimalnego.
* @param[in, out] h  - wska�nik na kopiec minimalnyn, na kt�rym dokonywane s� operacje
* @param[in] index   - numer elementu poddawanego naprawianiu
*/
void minHeapRepairUp(MinHeap *h,int index);

/** Dodaje miasto do kopca.
* Element jest dodawany na koniec kopca.
* @param[in, out] h   - wska�nik na kopiec, do kt�rego dodawany jest element
* @param[in] c        - wska�nik na miasto, kt�re ma byc dodane
* @return @p true je�li uda si� doda� element, @false je�li zabraknie pami�ci
*/
bool minHeapAdd(MinHeap *h, City *c);

/** Zdejmuje pierwszy element z kopca.
Zdj�ty element jest elementem minimalnym.
* @param[in, out] h - wska�nik na struktur� kopca minimalnego
* @return wska�nik na zdj�ty element, czyli miasto
*/
City* minHeapPeak(MinHeap *h);

/** @file
 * Interfejs struktury kopca minimalnego.
 * W kopcu przechowywane są wskaźniki na miasta. Miasta porównywane są względem wartości 
 * przechowywanych pod zerowym indeksem w @p temporaryData (miasto->temporaryData[0]).
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 03.05.2019
 */

#pragma once
#include "city.h"
#include <stdbool.h>
#include <stdlib.h>

///struktura kopca minimalnego
typedef struct MinHeap {
	int size; ///< aktualna ilość elementów na kopcu
	int maxSize; ///< rozmiar zaalokowanej tablicy
	City **tab; ///< tablica przechowująca elementy kopca
}MinHeap;

/** Tworzy nowy pusty kopiec.
* Domyślny rozmiar tablicy na kopiec wynosi 10 (lub 1 w przypadku ograniczonej pamięci).
* @return wskaźnik na nowy kopiec
*/
MinHeap* minHeapCreate(void);

/** Naprawia strukturę kopca przenosząc element do góry.
* Porównuje element o numerze @p indeks z jego rodzicem i zamienia te 2 elementy miejscami
* jeśli element jest większy od rodzica. Postępowanie jest powtarzane aż kopiec ponownie bedzie
* spełniał warunki kopca minimalnego.
* @param[in, out] h  - wskaźnik na kopiec minimalnyn, na którym dokonywane są operacje
* @param[in] index   - numer elementu poddawanego naprawianiu
*/
void minHeapRepairUp(MinHeap *h,int index);

/** Dodaje miasto do kopca.
* Element jest dodawany na koniec kopca.
* @param[in, out] h   - wskaźnik na kopiec, do którego dodawany jest element
* @param[in] c        - wskaźnik na miasto, które ma byc dodane
* @return @p true jeśli uda się dodać element, @false jeśli zabraknie pamięci
*/
bool minHeapAdd(MinHeap *h, City *c);

/** Zdejmuje pierwszy element z kopca.
Zdjęty element jest elementem minimalnym.
* @param[in, out] h - wskaźnik na strukturę kopca minimalnego
* @return wskaźnik na zdjęty element, czyli miasto
*/
City* minHeapPeak(MinHeap *h);

///zwalnia pamieć po strukturze kopca
void minHeapDelete(MinHeap *h);
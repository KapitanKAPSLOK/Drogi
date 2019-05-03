/** @file
 * Interfejs struktury kopca minimalnego.
 * W kopcu przechowywane s¹ wskaŸniki na miasta. Miasta porównywane s¹ wzglêdem wartoœci 
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
	int size; ///< aktualna iloœæ elementów na kopcu
	int maxSize; ///< rozmiar zaalokowanej tablicy
	City **tab; ///< tablica przechowuj¹ca elementy kopca
}MinHeap;

/** Tworzy nowy pusty kopiec.
* Domyœlny rozmiar tablicy na kopiec wynosi 10 (lub 1 w przypadku ograniczonej pamiêci).
* @return wskaŸnik na nowy kopiec
*/
MinHeap* minHeapCreate(void);

/** Naprawia strukturê kopca przenosz¹c element do góry.
* Porównuje element o numerze @p indeks z jego rodzicem i zamienia te 2 elementy miejscami
* jeœli element jest wiêkszy od rodzica. Postêpowanie jest powtarzane a¿ kopiec ponownie bedzie
* spe³nia³ warunki kopca minimalnego.
* @param[in, out] h  - wskaŸnik na kopiec minimalnyn, na którym dokonywane s¹ operacje
* @param[in] index   - numer elementu poddawanego naprawianiu
*/
void minHeapRepairUp(MinHeap *h,int index);

/** Dodaje miasto do kopca.
* Element jest dodawany na koniec kopca.
* @param[in, out] h   - wskaŸnik na kopiec, do którego dodawany jest element
* @param[in] c        - wskaŸnik na miasto, które ma byc dodane
* @return @p true jeœli uda siê dodaæ element, @false jeœli zabraknie pamiêci
*/
bool minHeapAdd(MinHeap *h, City *c);

/** Zdejmuje pierwszy element z kopca.
Zdjêty element jest elementem minimalnym.
* @param[in, out] h - wskaŸnik na strukturê kopca minimalnego
* @return wskaŸnik na zdjêty element, czyli miasto
*/
City* minHeapPeak(MinHeap *h);

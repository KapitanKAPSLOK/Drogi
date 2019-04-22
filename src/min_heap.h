/** @file
 * Interfejs struktury kopca minimalnego
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
	int size; //< aktualna ilo�� element�w na kopcu
	int maxSize; //< rozmiar zaalokowanej tablicy
	City **tab;
}MinHeap;

///tworzy nowy kopiec
MinHeap* createMinHeap();

///przenosi ostatni element kopca do g�ry dop�ki kopiec nie b�dzie zn�w posortowany
void repairUpMinHeap(MinHeap *h);

///dodaje element @p c do kopca
bool addToMinHeap(MinHeap *t, City *c);

///zdejmuje pierwszy element z kopca
City* peakFromMinHeap(MinHeap *t);
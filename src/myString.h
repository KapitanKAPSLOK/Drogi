/** @file
 * Interfejs klasy przechowującej funkcje działające na napisach.
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 13.06.2019
 */
#pragma once

/** Dodaje napis @p addition na koniec napisu @p str
* @param[in, out] str - wskaźnik na modyfikowany napis
* @param[in] addition - wskaźnik na napis, który ma zostać doklejony
* @return wskaźnik na napis wynikowy lub @p NULL w przypadku braku pamięci
*/
char *myStringAppendString(char *str, char *addition);

/** Zamienia liczbę na napis i dokleja ją na koniec napisu @p str
* @param[in] str -       wskaźnik na modyfikowany napis
* @param[in] addition  - doklejana liczba
* @return wskaźnik na napis wynikowy lub @p NULL w przypadku braku pamięci
*/
char *myStringAppendInt(char *str, int addition);
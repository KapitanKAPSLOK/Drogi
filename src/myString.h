/** @file
 * Interfejs klasy przechowuj¹cej funkcje dzia³aj¹ce na napisach.
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 03.04.2019
 */
#pragma once

/** Dodaje napis @p addition na koniec napisu @p str
* @param[in, out] str - wskaŸnik na modyfikowany napis
* @param[in] addition - wskaŸnik na napis, który ma zostaæ doklejony
* @return wskaŸnik na napis wynikowy lub @p NULL w przypadku braku pamiêci
*/
char *myStringAppendString(char *str, char *addition);

/** Zamienia liczbê na napis i dokleja j¹ na koniec napisu @p str
* @param[in] str -       wskaŸnik na modyfikowany napis
* @param[in] addition  - doklejana liczba
* @return wskaŸnik na napis wynikowy lub @p NULL w przypadku braku pamiêci
*/
char *myStringAppendInt(char *str, int addition);
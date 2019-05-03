/** @file
 * Interfejs klasy przechowuj�cej funkcje dzia�aj�ce na napisach.
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 03.04.2019
 */
#pragma once

/** Dodaje napis @p addition na koniec napisu @p str
* @param[in, out] str - wska�nik na modyfikowany napis
* @param[in] addition - wska�nik na napis, kt�ry ma zosta� doklejony
* @return wska�nik na napis wynikowy lub @p NULL w przypadku braku pami�ci
*/
char *myStringAppendString(char *str, char *addition);

/** Zamienia liczb� na napis i dokleja j� na koniec napisu @p str
* @param[in] str -       wska�nik na modyfikowany napis
* @param[in] addition  - doklejana liczba
* @return wska�nik na napis wynikowy lub @p NULL w przypadku braku pami�ci
*/
char *myStringAppendInt(char *str, int addition);
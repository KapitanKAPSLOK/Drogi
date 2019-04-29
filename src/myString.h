/** @file
 * Interfejs klasy przechowuj¹cej funkcje dzia³aj¹ce na napisach
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 29.04.2019
 */
#pragma once

///dodaje napis addition na koniec napisu str
char *myStringAppendString(char *str, char *addition);

///zmienia liczbê addition na napis i dodaje na koniec napisu str
char *myStringAppendInt(char *str, int addition);
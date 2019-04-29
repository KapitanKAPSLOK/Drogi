/** @file
 * Interfejs klasy przechowującej funkcje działające na napisach
 *
 * @author Mateusz Turowski <mj.turowski@student.uw.edu.pl>
 * @date 29.04.2019
 */
#pragma once

///dodaje napis addition na koniec napisu str
char *myStringAppendString(char *str, char *addition);

///zmienia liczbę addition na napis i dodaje na koniec napisu str
char *myStringAppendInt(char *str, int addition);
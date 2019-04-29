#include "myString.h"

#include <stdio.h>

//dodaje napis addition na koniec napisu str
char *myStringAppendString(char *str, char *addition) {
	char *newStr;
	str = realloc(str, strlen(str) + strlen(addition) + sizeof(char));
	if (str == NULL) return NULL;
	strcat(str, addition);
	return str;
}

//zmienia liczbê addition na napis i dodaje na koniec napisu str
char *myStringAppendInt(char *str, int addition) {
	char *integer = malloc(12 * sizeof(*str));
	if (integer == NULL) return NULL;
	sprintf(integer, "%d", addition);
	char *newStr;
	str = realloc(str, strlen(str) + strlen(addition) + sizeof(char));
	if (str == NULL) return NULL;
	strcat(str, addition);
	return str;
}
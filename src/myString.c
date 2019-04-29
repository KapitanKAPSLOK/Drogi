#include "myString.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//dodaje napis addition na koniec napisu str
char *myStringAppendString(char *str, char *addition) {
	str = realloc(str, strlen(str) + strlen(addition) + sizeof(char));
	if (str == NULL) return NULL;
	strcat(str, addition);
	return str;
}

//zmienia liczb� addition na napis i dodaje na koniec napisu str
char *myStringAppendInt(char *str, int addition) {
	char *integer = malloc(12 * sizeof(*str));
	if (integer == NULL) return NULL;
	sprintf(integer, "%d", addition);
	str = realloc(str, strlen(str) + strlen(integer) + sizeof(char));
	if (str == NULL) return NULL;
	strcat(str, integer);
	return str;
}
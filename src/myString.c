#include "myString.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//dodaje napis addition na koniec napisu str
char *myStringAppendString(char *str, char *addition) {
	int i=strlen(str);
 	i=strlen(addition);
	i=sizeof(char);
	char *temp = realloc(str, strlen(str) + strlen(addition) + sizeof(char));
	if (temp == NULL) {
		free(str);
		return NULL;
	}
	str = temp;
	if (str == NULL) return NULL;
	strcat(str, addition);
	return str;
}

//zmienia liczbê addition na napis i dodaje na koniec napisu str
char *myStringAppendInt(char *str, int addition) {
	char *integer = malloc(12 * sizeof(*str));
	if (integer == NULL) {
		free(str);
		return NULL;
	}
	sprintf(integer, "%d", addition);
	char *temp = realloc(str, strlen(str) + strlen(integer) + sizeof(char));
	if (temp == NULL) {
		free(str);
		return NULL;
	}
	str = temp;
	strcat(str, integer);
	return str;
}
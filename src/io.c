#include "io.h"
#include "myString.h"

#include <stdio.h>
#include <stdlib.h>

//skip input data to end of line
void ioSkipLine() {
	char c = getchar();
	while (c != '\n' && c != EOF) {
		c = getchar();
	}
	return;
}

//return 1 if succesfully read a char or 0 otherwise
bool ioReadChar(char* c) {//TODO
	*c = getchar();
	if (*c != '0' && *c != '1' && *c != '2' && *c != '3' && *c != '\n') {
		fputs("ERROR\n", stderr);
		ioSkipLine();
		return false;
	}
	return true;
}

//skip input data to first possible command, return 0 if end of file
bool ioSkipToCommand() {
	char c = getchar();
	while (c == '\n' || c == EOF || c == '#') {
		if (c == EOF) return false;
		else if (c == '#') ioSkipLine();
		c = getchar();
	}
	ungetc(c, stdin);
	return true;
}

//printf error message and skip to the end of the line
void ioError() {
	fputs("ERROR\n", stderr);
	ioSkipLine();
}

//chceck if on stdin is given command containing [size] chars
bool ioIsCommand(const char *command) {
	//liczenie długości polecenia
	int size = -1;
	while (command[++size] != '\0');

	char c = ' ';
	int i = 0;
	for (; i < size; ++i) {
		if ((c = getchar()) != command[i]) break;
	}
	if (i == size) return true; //sprawdza czy wszystkie znaki były zgodne z komendą
	//jeśli wczytane wiersze nie odpowiadały komendzie trzeba zwrócić pobrane znaki
	ungetc(c, stdin);
	for (--i; i >= 0; --i) {
		ungetc(command[i], stdin);
	}
	return false;
}

//if command is empty return 1 and gives error, else return 0
bool ioEmptyCommand() {
	char c = getchar();
	if (c == '\n' || c == ' ') {
		fputs("ERROR\n", stderr);
		ungetc(c, stdin);
		ioSkipLine();
		return true;
	}
	ungetc(c, stdin);
	return false;
}

//czyta nazwę miasta ze standardowego wejścia
char* ioGetCity() {
	if (ioEmptyCommand()) return NULL;
	char *str=malloc(sizeof(*str)); //wynikowa nazwa miasta
	if (str == NULL) {
		ioError();
		return NULL;
	}
	*str = '\0';
	char *buffer = malloc(21 * sizeof(*buffer));
	if (buffer == NULL) {
		free(str);
		ioError();
		return NULL;
	}
	int i = 0;
	buffer[i] = getchar();
	while (buffer[i] != EOF && buffer[i++] != '\n') {
		//sprawdzanie czy bufor się nie napełnił
		if (i >= 20) {
			buffer[i] = '\0';
			str = myStringAppendString(str, buffer);
			if (str == NULL) {
				free(buffer);
				ioError();
				return NULL;
			}
			i = 0;
		}
		buffer[i] = getchar();
		//sprawdzanie czy to już koniec nazwy miasta
		if (buffer[i] == ';' || buffer[i]=='\n') {
			ungetc(buffer[i], stdin);
			buffer[i] = '\0';
			str = myStringAppendString(str, buffer);
			free(buffer);
			if (str == NULL) ioError();
			return str;
		}
	}
	//jeśli program doszedł do tego miejsca znaczy wystąpiło \n lub EOF
	ungetc(buffer[i], stdin);
	ioError();
	return NULL;
}

//wczytuje znak ze standardowego wejścia i sprawdza czy jest to średnik
bool ioIsSemicolon() {
	char c = getchar();
	if (c != ';') {
		ungetc(c, stdin);
		ioError();
		return false;
	}
	return true;
}

//wczytuje dane i wykonuje polecenie addRoad
void ioAddRoad(Map *m) {
	if (!ioIsSemicolon()) return; //sprawdzanie czy jest średnik
	//wczytywanie miast
	char *c1 = ioGetCity();
	if (c1 == NULL) return; //wystąpił błąd podczas czytania nazwy miasta
	if (!ioIsSemicolon()) return;
	char *c2 = ioGetCity();
	if (c2 == NULL) {
		free(c1);
		return;
	}
	if (!ioIsSemicolon() || ioEmptyCommand()) {
		free(c1);
		free(c2);
		return;
	}
	//wczytywanie długości odcinka drogi
	unsigned length;
	if (scanf("%u", &length) != 1) {
		free(c1);
		free(c2);
		return;
	}
	if (!ioIsSemicolon() || ioEmptyCommand()) {
		free(c1);
		free(c2);
		return;
	}
	//wczytywanie roku remontu
	int year;
	if (scanf("%d", &year) != 1) {
		free(c1);
		free(c2);
		return;
	}
	char c = getchar();
	if (c != '\n' && c != EOF) { //sprawdzanie czy po poleceniu nie ma jeszcze czegoś
		ungetc(c, stdin);
		ioError();
		return;
	}
	//dodawanie odcinka drogi o wczytanych parametrach do mapy
	if (!addRoad(m, c1, c2, length, year)) ioError();
	free(c1);
	free(c2);
	return;
}

//wczytuje dane i wykonuje polecenie repairRoad
void ioRepairRoad(Map *m) {
	char c = getchar();
	if (c != ';') {
		ungetc(c, stdin);
		ioError();
		return;
	}
	char *c1 = ioGetCity();
	if (c1 == NULL) return; //wystąpił błąd podczas czytania nazwy miasta
	if (!ioIsSemicolon()) return;
	char *c2 = ioGetCity();
	if (c2 == NULL) {
		free(c1);
		return;
	}
	if (ioEmptyCommand()) { //polecenie skończyło się za wcześnie
		free(c1);
		free(c2);
		return; 
	}
	if (!ioIsSemicolon()) return;
	int year;
	if (scanf("%d", &year) != 1) {
		//nie udało się wczytać roku
		free(c1);
		free(c2);
		return;
	}
	c = getchar();
	if (c != '\n' && c != EOF) { //sprawdzanie czy po poleceniu nie ma jeszcze czegoś
		ungetc(c, stdin);
		ioError();
		return;
	}
	if (!repairRoad(m, c1, c2, year)) {
		ungetc(c, stdin);
		ioError();
	}
	free(c1);
	free(c2);
	return;
}

//wczytuje dane i wykonuje polecenie getRouteDescription
void ioGetRouteDescription(Map *m) {
	char c = getchar();
	if (c != ';') {
		ungetc(c, stdin);
		ioError();
		return;
	}
	unsigned nr;
	if (scanf("%u", &nr) != 1) {
		ioError();
		return;
	}
	c = getchar();
	if (c != '\n' && c != EOF) { //sprawdzanie czy po poleceniu nie ma jeszcze czegoś
		ungetc(c, stdin);
		ioError(); 
		return;
	}
	const char *str = getRouteDescription(m, nr);
	if (str == NULL) {
		//nie udało się uzyskać opisu podanej drogi krajowej
		ioError();
		return;
	}
	printf(str);
	printf("\n");
	free(str);
	return;
}

//wczytuje dane ze standardowego wejścia i tworzy na ich podstawie nową drogę krajową
void ioMakeRoute(Map *m) {
	if (ioEmptyCommand()) return;
	//wczytywanie numeru drogi
	unsigned nr;
	if (scanf("%u", &nr) != 1) {
		ioError();
		return;
	}
	if (!ioIsSemicolon()) return;
	//wczytywanie miasta startwego
	const char *city1 = ioGetCity();
	if (city1 == NULL) return; //nie udało się wczytać nazwy miasta
	if (!ioIsSemicolon()) return;
	if (ioEmptyCommand()) return;
	//wczytywanie długości odcinka drogi
	unsigned length;
	if (scanf("%u", &length) != 1) {
		free(city1);
		ioError();
		return;
	}
	if (!ioIsSemicolon()) return;
	//wczytywanie roku ostatniej modyfikacji odcinka drogi
	int year;
	if (scanf("%d", &year) != 1) {
		free(city1);
		ioError();
		return;
	}
	if (!ioIsSemicolon()) return;
	//wczytywanie drugiego miasta
	const char *city2 = ioGetCity();
	if (city2 == NULL) {
		free(city1);
		return;
	}
	if (ioEmptyCommand()) return;

	//wczytano pierwszy odcinek drogi można utworzyć drogę krajową z niego
	if(!makeRoute(m, nr, city1, city2, length, year)){
		//nie udało się utworzyć drogi krajowej o podanych parametrach
		free(city1);
		free(city2);
		ioError();
		return;
	}
	free(city1);
	free(city2);
	char c = getchar();
	//wczytywanie kolejnych odcinków dróg, które mają zostać dodane do drogi krajowej
	while (c != '\n' && c != EOF) {
		ungetc(c, stdin);

		if (!ioIsSemicolon()) return;
		//wczytywanie długości odcinka drogi
		unsigned length;
		if (scanf("%u", &length) != 1) {
			free(city1);
			ioError();
			return;
		}
		if (!ioIsSemicolon()) return;
		//wczytywanie roku ostatniej modyfikacji odcinka drogi
		int year;
		if (scanf("%d", &year) != 1) {
			free(city1);
			ioError();
			return;
		}
		if (!ioIsSemicolon()) return;
		//wczytywanie kolejnego miasta
		const char *city2 = ioGetCity();
		if (city2 == NULL) return;

		if (!addToRoute(m, nr, city2, length, year)) {
			free(city2);
			ioError();
			return;
		}
		free(city2);
		c = getchar();
	}
	ungetc(c, stdin);
	return;
}

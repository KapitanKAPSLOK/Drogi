#include "io.h"
#include "myString.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

//zwiększa numer wiersza o add i zwraca go
unsigned rowNumber(int add) {
	static unsigned line = 1;//zmienna pamiętająca aktualny numer wiersza we wczytywanych danych
	line = line + add;
	return line;
}

//skip input data to end of line
void ioSkipLine() {
	char c = getchar();
	while (c != '\n' && c != EOF) {
		c = getchar();
	}
	rowNumber(1);
	return;
}

//return 1 if succesfully read a char or 0 otherwise
//bool ioReadChar(char* c) {//TODO
//	*c = getchar();
//	if (*c != '0' && *c != '1' && *c != '2' && *c != '3' && *c != '\n') {
//		fputs("ERROR\n", stderr);
//		ioSkipLine();
//		return false;
//	}
//	return true;
//}

//skip input data to first possible command, return 0 if end of file
bool ioSkipToCommand() {
	char c = getchar();
	while (c == '\n' || c == EOF || c == '#') {
		if (c == EOF) return false;
		else if (c == '#') ioSkipLine();
		else rowNumber(1); //zwiększanie numeru aktualnie czytanej linii o 1
		c = getchar();
	}
	ungetc(c, stdin);
	return true;
}

//printf error message and skip to the end of the line
void ioError() {
	fprintf(stderr, "ERROR %u\n", rowNumber(0));
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
		ungetc(c, stdin);
		ioError();
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
	int bufferSize = 101;
	char *buffer = malloc(bufferSize * sizeof(*buffer));
	if (buffer == NULL) {
		free(str);
		ioError();
		return NULL;
	}
	int i = 0;
	buffer[i] = getchar();
	buffer[bufferSize-1] = '\0';
	int howLong = 0;//liczy aktualną długość nazwy miasta

	while (buffer[i] != EOF && buffer[i++] != '\n') {
		//sprawdzanie czy bufor się nie napełnił
		if (i >= bufferSize-1) {
			str = myStringAppendString(str, buffer);
			if (str == NULL) {
				free(buffer);
				ioError();
				return NULL;
			}
			i = 0;

			if (++howLong == 100) {
				//nazwa miasta jest bardzo długa i bardziej optymalny jest większy bufor
				char* temp = realloc(buffer, bufferSize * 10 * sizeof(*buffer));
				if (temp == NULL) free(str);
				else {
					buffer = temp;
					bufferSize *= 10;
					buffer[bufferSize - 1] = '\0';
					howLong = 0;
				}

			}
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

//czyta inta ze standardowego wejścia, zwraca true jeśli się uda, false w innym wypadku
bool ioReadInteger(int *v) {
	char buffer[11];
	for (int i = 0; i < 11; ++i) {
		buffer[i] = getchar();
		if (buffer[i] == ';' || buffer[i] == '\n' || buffer[i] == EOF) {
			//osiągnięto koniec liczby
			ungetc(buffer[i], stdin);
			if (i == 0) return false; //nie wczytano, żadnego poprawnego znaku
			break;
		}
	}
	char *index;
	//w tym miejscu wczytano liczbę do bufora
	long temp = strtol(buffer, &index, 10);
	if (temp == 0 && errno == EINVAL) return false;
	if (errno == ERANGE) return false;
	if (temp > INT_MAX || temp < INT_MIN) return false;
	*v = temp;
	return true;
}

//czyta unsigned inta ze standardowego wejścia, zwraca true jeśli się uda, false w innym wypadku
bool ioReadUnsigned(unsigned *v) {
	char sign = getchar();
	if (sign == '-') { //podana liczba jest ujemna, nie można jej zinterpretować jako unsigned int
		ungetc(sign, stdin);
		return false;
	}
	ungetc(sign, stdin);
	char buffer[10];
	for (int i = 0; i < 10; ++i) {
		buffer[i] = getchar();
		if (buffer[i] == ';' || buffer[i] == '\n' || buffer[i] == EOF) {
			//osiągnięto koniec liczby
			ungetc(buffer[i], stdin);
			if (i == 0) return false; //nie wczytano, żadnego poprawnego znaku
			break;
		}
		if (buffer[i] < 48 || buffer[i] > 57) {
			//wczytany znak nie jest cyfrą
			ungetc(buffer[i], stdin);
			return false;
		}
	}
	char *index;
	//w tym miejscu wczytano liczbę do bufora
	unsigned long temp = strtoul(buffer, &index, 10);
	if (errno == ERANGE) return false;
	if (temp > UINT_MAX) return false;
	*v = temp;
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
	if (!ioReadUnsigned(&length)) {
		ioError();
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
	if (!ioReadInteger(&year)) {
		ioError();
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
	if (!addRoad(m, c1, c2, (unsigned)length, year)) ioError();
	free(c1);
	free(c2);
	ungetc(c, stdin);
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
	if (!ioReadInteger(&year)) {
		ioError();
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
		ioError();
	}
	free(c1);
	free(c2);
	ungetc(c, stdin);
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
	free((void *)str);
	ungetc(c, stdin);
	return;
}

//wczytuje dane ze standardowego wejścia i tworzy na ich podstawie nową drogę krajową
void ioMakeRoute(Map *m) {
	if (ioEmptyCommand()) return;
	//wczytywanie numeru drogi
	unsigned nr;
	if (!ioReadUnsigned(&nr)) {
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
	if (!ioReadUnsigned(&length)) {
		free((void *)city1);
		ioError();
		return;
	}
	if (!ioIsSemicolon()) return;
	//wczytywanie roku ostatniej modyfikacji odcinka drogi
	int year;
	if (!ioReadInteger(&year)) {
		free((void *)city1);
		ioError();
		return;
	}
	if (!ioIsSemicolon()) return;
	//wczytywanie drugiego miasta
	const char *city2 = ioGetCity();
	if (city2 == NULL) {
		free((void *)city1);
		return;
	}

	//wczytano pierwszy odcinek drogi można utworzyć drogę krajową z niego
	if(!makeRoute(m, nr, city1, city2, length, year)){
		//nie udało się utworzyć drogi krajowej o podanych parametrach
		free((void *)city1);
		free((void *)city2);
		ioError();
		return;
	}
	free((void *)city1);
	free((void *)city2);
	char c = getchar();
	//wczytywanie kolejnych odcinków dróg, które mają zostać dodane do drogi krajowej
	while (c != '\n' && c != EOF) {
		ungetc(c, stdin);

		if (!ioIsSemicolon()) return;
		//wczytywanie długości odcinka drogi
		unsigned length;
		if (!ioReadUnsigned(&length)) {
			free((void *)city1);
			ioError();
			return;
		}
		if (!ioIsSemicolon()) return;
		//wczytywanie roku ostatniej modyfikacji odcinka drogi
		int year;
		if (!ioReadInteger(&year)) {
			free((void *)city1);
			ioError();
			return;
		}
		if (!ioIsSemicolon()) return;
		//wczytywanie kolejnego miasta
		const char *city2 = ioGetCity();
		if (city2 == NULL) return;

		if (!addToRoute(m, nr, city2, length, year)) {
			free((void *)city2);
			ioError();
			return;
		}
		free((void *)city2);
		c = getchar();
	}
	ungetc(c, stdin);
	return;
}

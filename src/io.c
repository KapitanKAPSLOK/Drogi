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
				if (temp != NULL){
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
	free(buffer);
	ioError();
	free(str);
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
	if (!ioIsSemicolon()) {
		free(c1);
		return;
	}
	char *c2 = ioGetCity();
	if (c2 == NULL) {
		free(c1);
		return;
	}
	if (!ioIsSemicolon()) {
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
	if (!ioIsSemicolon()) {
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
		free(c1);
		free(c2);
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
	if (!ioIsSemicolon()) return;
	//wczytywanie nazwy miasta
	char *c1 = ioGetCity();
	if (c1 == NULL) return; //wystąpił błąd podczas czytania nazwy miasta
	if (!ioIsSemicolon()) {
		free(c1);
		return;
	}
	char *c2 = ioGetCity();
	if (c2 == NULL) {
		free(c1);
		return;
	}
	if (!ioIsSemicolon()) {
		free(c1);
		free(c2);
		return;
	}
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
		free(c1);
		free(c2);
		return;
	}
	ungetc(c, stdin);
	if (!repairRoad(m, c1, c2, year)) {
		ioError();
	}
	free(c1);
	free(c2);
	return;
}

//wczytuje dane i wykonuje polecenie getRouteDescription
void ioGetRouteDescription(Map *m) {
	if (!ioIsSemicolon()) return;
	//wczytywanie numeru drogi
	unsigned nr;
	if (!ioReadUnsigned(&nr)) {
		ioError();
		return;
	}
	//sprawdzanie czy po poleceniu nie ma jeszcze czegoś
	char c = getchar();
	if (c != '\n' && c != EOF) { 
		ungetc(c, stdin);
		ioError(); 
		return;
	}
	ungetc(c, stdin);
	const char *str = getRouteDescription(m, nr);
	if (str == NULL) {
		//nie udało się uzyskać opisu podanej drogi krajowej
		ioError();
		return;
	}
	printf(str);
	printf("\n");
	free((void *)str);
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
	char *city1 = ioGetCity();
	if (city1 == NULL) return; //nie udało się wczytać nazwy miasta
	if (!ioIsSemicolon()) {
		free(city1);
		return;
	}
	//wczytywanie długości odcinka drogi
	unsigned length;
	if (!ioReadUnsigned(&length)) {
		free(city1);
		ioError();
		return;
	}
	if (!ioIsSemicolon()) {
		free(city1);
		return;
	}
	//wczytywanie roku ostatniej modyfikacji odcinka drogi
	int year;
	if (!ioReadInteger(&year)) {
		free(city1);
		ioError();
		return;
	}
	if (!ioIsSemicolon()) {
		free(city1);
		return;
	}
	//wczytywanie drugiego miasta
	char *city2 = ioGetCity();
	if (city2 == NULL) {
		free(city1);
		return;
	}

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
	city2 = NULL;
	char c = getchar();
	//wczytywanie kolejnych odcinków dróg, które mają zostać dodane do drogi krajowej
	while (c != '\n' && c != EOF) {
		ungetc(c, stdin);

		if (!ioIsSemicolon()) return;
		//wczytywanie długości odcinka drogi
		unsigned length;
		if (!ioReadUnsigned(&length)) {
			ioError();
			return;
		}
		if (!ioIsSemicolon()) return;
		//wczytywanie roku ostatniej modyfikacji odcinka drogi
		int year;
		if (!ioReadInteger(&year)) {
			ioError();
			return;
		}
		if (!ioIsSemicolon()) return;
		//wczytywanie kolejnego miasta
		city2 = ioGetCity();
		if (city2 == NULL) return;
		//udało się wczytać dane, próba wykonywania polecenia
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

//wczytuje potrzebne dane i wykonuje polecenie addRoute
void ioNewRoute(Map *m) {
	if (ioEmptyCommand()) return;
	if (!ioIsSemicolon()) return;
	//wczytywanie numeru drogi
	unsigned id;
	if (!ioReadUnsigned(&id)) {
		ioError();
		return;
	}
	if (!ioIsSemicolon()) return;
	//wczytywanie nazwy pierwszego miasta
	const char *city1 = ioGetCity();
	if (city1 == NULL) return;
	if (!ioIsSemicolon()) {
		free((void *)city1);
		return;
	}
	//wczytywanie nazwy drugiego miasta
	const char *city2 = ioGetCity();
	if (city2 == NULL) {
		free((void *)city1);
		return;
	}
	//sprawdzanie czy po poleceniu nie ma jeszcze czegoś
	char c = getchar();
	if (c != '\n' && c != EOF) {
		ungetc(c, stdin);
		ioError();
		free((void *)city1);
		free((void *)city2);
		return;
	}
	ungetc(c, stdin);
	//udało się wczytać dane, próba wykonywania polecenia
	if (!newRoute(m, id, city1, city2)) ioError();
	free((void *)city1);
	free((void *)city2);
	return;
}

//wczytuje potrzebne dane i wykonuje polecenie extendRoute
void ioExtendRoute(Map *m) {
	if (ioEmptyCommand()) return;
	if (!ioIsSemicolon()) return;
	//wczytywanie numeru drogi
	unsigned id;
	if (!ioReadUnsigned(&id)) {
		ioError();
		return;
	}
	if (!ioIsSemicolon()) return;
	//wczytywanie nazwy pierwszego miasta
	const char *city = ioGetCity();
	if (city == NULL) return;
	//sprawdzanie czy po poleceniu nie ma jeszcze czegoś
	char c = getchar();
	if (c != '\n' && c != EOF) {
		ungetc(c, stdin);
		ioError();
		free((void *)city);
		return;
	}
	ungetc(c, stdin);
	//udało się wczytać dane, próba wykonywania polecenia
	if (!extendRoute(m, id, city)) ioError();
	free((void *)city);
	return;
}

//wczytuje potrzebne dane i wykonuje polecenie extendRoute
void ioRemoveRoad(Map *m) {
	if (ioEmptyCommand()) return;
	if (!ioIsSemicolon()) return;
	//wczytywanie nazwy pierwszego miasta
	const char *city1 = ioGetCity();
	if (city1 == NULL) return;
	if (!ioIsSemicolon()) {
		free((void *)city1);
		return;
	}
	//wczytywanie nazwy drugiego miasta
	const char *city2 = ioGetCity();
	if (city2 == NULL) {
		free((void *)city1);
		return;
	}
	//sprawdzanie czy po poleceniu nie ma jeszcze czegoś
	char c = getchar();
	if (c != '\n' && c != EOF) {
		ungetc(c, stdin);
		ioError();
		free((void *)city1);
		free((void *)city2);
		return;
	}
	ungetc(c, stdin);
	//udało się wczytać dane, próba wykonywania polecenia
	if (!removeRoad(m, city1, city2)) ioError();
	free((void *)city1);
	free((void *)city2);
	return;
}

//wczytuje potrzebne dane i wykonuje polecenie removeRoute
void ioRemoveRoute(Map *m) {
	if (ioEmptyCommand()) return;
	if (!ioIsSemicolon()) return;
	//wczytywanie numeru drogi
	unsigned id;
	if (!ioReadUnsigned(&id)) {
		ioError();
		return;
	}
	//sprawdzanie czy po poleceniu nie ma jeszcze czegoś
	char c = getchar();
	if (c != '\n' && c != EOF) {
		ungetc(c, stdin);
		ioError();
		return;
	}
	ungetc(c, stdin);
	//udało się wczytać dane, próba wykonywania polecenia
	if (!removeRoute(m, id)) ioError();
	return;
}
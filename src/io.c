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
	//liczenie d³ugoœci polecenia
	int size = -1;
	while (command[++size] != '\0');

	char c = ' ';
	int i = 0;
	for (; i < size; ++i) {
		if ((c = getchar()) != command[i]) break;
	}
	if (i == size) return true; //sprawdza czy wszystkie znaki by³y zgodne z komend¹
	//jeœli wczytane wiersze nie odpowiada³y komendzie trzeba zwróciæ pobrane znaki
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

//czyta nazwê miasta ze standardowego wejœcia
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
		//sprawdzanie czy bufor siê nie nape³ni³
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
		//sprawdzanie czy to ju¿ koniec nazwy miasta
		if (buffer[i] == ';' || buffer[i]=='\n') {
			ungetc(buffer[i], stdin);
			buffer[i] = '\0';
			str = myStringAppendString(str, buffer);
			free(buffer);
			if (str == NULL) ioError();
			return str;
		}
	}
	//jeœli program doszed³ do tego miejsca znaczy wyst¹pi³o \n lub EOF
	ungetc(buffer[i], stdin);
	ioError();
	return NULL;
}

//wczytuje znak ze standardowego wejœcia i sprawdza czy jest to œrednik
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
	if (!ioIsSemicolon()) return; //sprawdzanie czy jest œrednik
	//wczytywanie miast
	char *c1 = ioGetCity();
	if (c1 == NULL) return; //wyst¹pi³ b³¹d podczas czytania nazwy miasta
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
	//wczytywanie d³ugoœci odcinka drogi
	int length;
	if (scanf("%d", &length) != 1) {
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
	if (ioEmptyCommand()) {
		free(c1);
		free(c2);
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
	if (c1 == NULL) return; //wyst¹pi³ b³¹d podczas czytania nazwy miasta
	char *c2 = ioGetCity();
	if (c2 == NULL) {
		free(c1);
		return;
	}
	if (ioEmptyCommand()) { //polecenie skoñczy³o siê za wczeœnie
		free(c1);
		free(c2);
		return; 
	}
	int year;
	if (scanf("%d", &year) != 1) {
		//nie uda³o siê wczytaæ roku
		free(c1);
		free(c2);
		return;
	}
	c = getchar();
	if (c != '\n' && c != EOF) { //sprawdzanie czy po poleceniu nie ma jeszcze czegoœ
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
	if (c != '\n' && c != EOF) { //sprawdzanie czy po poleceniu nie ma jeszcze czegoœ
		ungetc(c, stdin);
		ioError(); 
		return;
	}
	const char *str = getRouteDescription(m, nr);
	if (str == NULL) {
		//nie uda³o siê uzyskaæ opisu podanej drogi krajowej
		ioError();
		return;
	}
	printf(str);
	free(str);
	return;
}

//wczytuje dane ze standardowego wejœcia i tworzy na ich podstawie now¹ drogê krajow¹
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
	if (city1 == NULL) return; //nie uda³o siê wczytaæ nazwy miasta
	if (!ioIsSemicolon()) return;
	if (ioEmptyCommand()) return;
	//wczytywanie d³ugoœci odcinka drogi
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

	//wczytano pierwszy odcinek drogi mo¿na utworzyæ drogê krajow¹ z niego
	if(!makeRoute(m, nr, city1, city2, length, year)){
		//nie uda³o siê utworzyæ drogi krajowej o podanych parametrach
		free(city1);
		free(city2);
		ioError();
		return;
	}
	free(city1);
	free(city2);
	char c = getchar();
	//wczytywanie kolejnych odcinków dróg, które maj¹ zostaæ dodane do drogi krajowej
	while (c != '\n' && c != EOF) {
		ungetc(c, stdin);

		if (!ioIsSemicolon()) return;
		//wczytywanie d³ugoœci odcinka drogi
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

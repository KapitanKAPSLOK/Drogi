#include "road.h"
#include "myString.h"

#include<string.h>


///////////////////////roadList/////////////////////////////

//usuwa listę dróg
void roadListDelete(RoadList *l) {
	if (l == NULL)return;
	RoadList *p = l->next;
	RoadList *temp;
	while (p != NULL) {
		temp = p->next;
		free(p);
		p = temp;
	}
	free(l);
}

//dodaje element na początek listy dróg
bool roadListAdd(RoadList **l, Road *r) {
	RoadList *temp;
	temp = malloc(sizeof(*temp));
	if (temp == NULL) return false;
	if (*l != NULL) temp->next = *l;
	else temp->next = NULL;
	temp->r = r;
	*l = temp;
	return true;
}

//dodaje na początek listy l1 listę l2
void roadListAddList(RoadList **l1, RoadList *l2) {
	if (l2 == NULL) return; //nie ma nic do dodania
	RoadList *last = l2;
	while (last->next != NULL) last = last->next;
	last->next = (*l1);
	(*l1) = l2;
}

///usuwa element r z listy dróg
void roadListDeleteElement(RoadList **l, Road *r) {
	if (l == NULL || r == NULL || (*l)==NULL) return;
	if ((*l)->r == r) { //element r znajduje się na początku
		RoadList *temp = *l;
		*l = (*l)->next;
		free(temp);
		return;
	}
	RoadList *list = *l;
	while (list->next != NULL) {
		if (list->next->r == r) {
			RoadList *temp = list->next;
			list->next= list->next->next;
			free(temp);
			return;
		}
		list = list->next;
	}
	return;
}

//zwalnia pamięć po elementach przechowywanych w liście
void roadListDeleteElements(RoadList *l) {
	while (l != NULL) {
		free(l->r);
		l = l->next;
	}
}

//zwraca wskaźnik do drogi zawierającej z miasta o nazwie str lub NULL jeśli nie ma takiej drogi nie ma na liście
Road *roadListFindStr(RoadList *l, const char *str) {
	while (l != NULL) {
		if (!strcmp(l->r->city1->name, str) || !strcmp(l->r->city2->name, str)) return l->r;
		l = l->next;
	}
	return NULL;
}

//odwraca listę
RoadList *roadListReverse(RoadList *l) {
	if (l == NULL) return NULL;
	RoadList *prev = NULL;
	while (l != NULL) {
		RoadList *temp = l->next;
		l->next = prev;
		prev = l;
		l = temp;
	}
	return prev;
}

//jeśli któraś z dróg w liście l dochodzi do miasta c zwraca wskaźnik do niej w przeciwnym razie NULL
Road *roadListContain(RoadList *l, City *c) {
	while (l != NULL) {
		if (roadConnectCity(l->r,c)) return l->r;
		l = l->next;
	}
	return NULL;
}


///////////////////////////////road//////////////////////////////

//zmienia rok ostatniej modyfikacji drogi
bool roadRepair(Road *r, int year) {
	if (year < r->modificationDate) return false;
	if (year == 0) return false; //nie było roku 0
	r->modificationDate = year;
	return true;
}

//zwraca jedno z miast, które łączy droga, ale różne od miasta c
City *roadGetCity(Road *r, City *c) {
	if (r->city1 != c) return r->city1;
	return r->city2;
}

//sprawdza czy dana droga dochodzi do podanego miasta
bool roadConnectCity(Road *r, City *c) {
	if (c == NULL || r == NULL) return false;
	if (!strcmp(r->city1->name, c->name) || !strcmp(r->city2->name, c->name)) return true;
	return false;
}

//zwraca informacje o drodze w formacie ";długość;data modyfikacji;miasto inne niż c"
char *roadGetDescription(Road *r, City *c) {
	char *str = malloc(sizeof(*str));
	if (str == NULL) return NULL; //brak pamięci
	*str = '\0'; //napis w C kończy się zerem
	//jeśli przy dołączaniu napisów zostanie zwrócony NULL znaczy, że zabrakło pamięci, wtedy zwracany jest NULL
	if (!(str = myStringAppendString(str, ";"))) return NULL;
	if (!(str = myStringAppendInt(str, r->length))) return NULL;
	if (!(str = myStringAppendString(str, ";"))) return NULL;
	if (!(str = myStringAppendInt(str, r->modificationDate))) return NULL;
	if (!(str = myStringAppendString(str, ";"))) return NULL;
	if (!(str = myStringAppendString(str, roadGetCity(r,c)->name))) return NULL;
	return str;
}


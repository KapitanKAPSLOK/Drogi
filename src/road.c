#include "road.h"
#include "myString.h"

#include<string.h>

//usuwa listê dróg
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

//dodaje element na pocz¹tek listy dróg
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

//dodaje na pocz¹tek listy l1 listê l2
void roadListAddList(RoadList **l1, RoadList *l2) {
	if (l2 == NULL) return; //nie ma nic do dodania
	RoadList *last = l2;
	while (last->next != NULL) last = last->next;
	last->next = (*l1);
	(*l1) = l2;
}

///usuwa element r z listy dróg
void roadListDeleteElement(RoadList **l, Road *r) {
	if (l == NULL || r == NULL) return;
	if ((*l)->r == r) { //element r znajduje siê na pocz¹tku
		RoadList *temp = *l;
		*l = (*l)->next;
		free(temp);
	}
	while ((*l)->next != NULL) {
		if ((*l)->next->r == r) {
			RoadList *temp = (*l)->next;
			(*l)->next= (*l)->next->next;
			free(temp);
			return;
		}
	}
	return;
}

//zwalnia pamiêæ po elementach przechowywanych w liœcie
void roadListDeleteElements(RoadList *l) {
	while (l != NULL) {
		free(l->r);
		l = l->next;
	}
}

//zwraca wskaŸnik do drogi zawieraj¹cej z miasta o nazwie str lub NULL jeœli nie ma takiej drogi nie ma na liœcie
Road *roadListFindStr(RoadList *l, const char *str) {
	while (l != NULL) {
		if (!strcmp(l->r->city1->name, str) || !strcmp(l->r->city2->name, str)) return l->r;
		l = l->next;
	}
	return NULL;
}

//odwraca listê
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

//jeœli któraœ z dróg w liœcie l dochodzi do miasta c zwraca wskaŸnik do niej w przeciwnym razie NULL
Road *roadListContain(RoadList *l, City *c) {
	while (l != NULL) {
		if (roadConnectCity(l->r,c)) return l->r;
		l = l->next;
	}
	return NULL;
}
///////////////////

//zmienia rok ostatniej modyfikacji drogi
bool roadRepair(Road *r, int year) {
	if (year < r->modificationDate) return false;
	if (year == 0) return false; //nie by³o roku 0
	r->modificationDate = year;
	return true;
}

//zwraca jedno z miast, które ³¹czy droga, ale ró¿ne od miasta c
City *roadGetCity(Road *r, City *c) {
	if (r->city1 != c) return r->city1;
	return r->city2;
}

//sprawdza czy dana droga dochodzi do podanego miasta
bool roadConnectCity(Road *r, City *c) {
	if (!strcmp(r->city1->name, c->name) || !strcmp(r->city2->name, c->name)) return true;
	return false;
}

//zwraca informacje o drodze w formacie ";d³ugoœæ;data modyfikacji;miasto inne ni¿ c"
char *roadGetDescription(Road *r, City *c) {
	City *temp=roadGetCity(r, c);
	char *str = ";";
	//jeœli przy do³¹czaniu napisów zostanie zwrócony NULL znaczy, ¿e zabrak³o pamiêci, wtedy zwracany jest NULL
	if (!(str = myStringAppendInt(str, r->length))) return NULL;
	if (!(str = myStringAppendInt(str, ";"))) return NULL;
	if (!(str = myStringAppendInt(str, r->modificationDate))) return NULL;
	if (!(str = myStringAppendInt(str, ";"))) return NULL;
	if (!(str = myStringAppendInt(str, roadGetCity(r,c)))) return NULL;
	return str;
}


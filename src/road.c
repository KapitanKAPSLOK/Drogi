#include "road.h"

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
void roadListDeleteElements(RoadList *r) {
	while (r != NULL) {
		free(r->c);
		r = r->next;
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
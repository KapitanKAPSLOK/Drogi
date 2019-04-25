#include "road.h"


//usuwa list� dr�g
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

//dodaje element na pocz�tek listy dr�g
bool RoadListAdd(RoadList **l, Road *c) {
	RoadList *temp;
	temp = malloc(sizeof(*temp));
	if (temp == NULL) return false;
	if (*l != NULL) temp->next = *l;
	else temp->next = NULL;
	temp->c = c;
	*l = temp;
	return true;
}

///usuwa element r z listy dr�g
void roadListDeleteElement(RoadList **l, Road *r) {
	if (l == NULL || r == NULL) return;
	if (*l == r) { //element r znajduje si� na pocz�tku
		Road *temp = *l;
		*l = (*l)->next;
		free(temp);
	}
	while (l->next != NULL) {
		if ((*l)->next->r == r) {
			Road *temp = (*l)->next;
			(*l)->next= (*l)->next->next;
			free(temp);
			return;
		}
	}
	return;
}

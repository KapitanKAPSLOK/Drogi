#include "road.h"


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

///usuwa element r z listy dróg
void roadListDeleteElement(RoadList **l, Road *r) {
	if (l == NULL || r == NULL) return;
	if (*l == r) { //element r znajduje siê na pocz¹tku
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

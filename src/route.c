#include "route.h"
#include "road.h"
#include "city.h"


//usuwa list� dr�g krajowych
void routeListDelete(RouteList *l) {
	if (l == NULL)return;
	RouteList *p = l->next;
	RouteList *temp;
	while (p != NULL) {
		temp = p->next;
		free(p);
		p = temp;
	}
	free(l);
}

//dodaje element na pocz�tek listy dr�g krajowych
bool routeListAdd(RouteList **l, Route *r) {
	RouteList *temp;
	temp = malloc(sizeof(*temp));
	if (temp == NULL) return false;
	if (*l != NULL) temp->next = *l;
	else temp->next = NULL;
	temp->r = r;
	*l = temp;
	return true;
}

///usuwa element r z listy dr�g krajowych
void routeListDeleteElement(RouteList **l, Route *r) {
	if (l == NULL || r == NULL) return;
	if ((*l)->r == r) { //element r znajduje si� na pocz�tku
		RouteList *temp = *l;
		*l = (*l)->next;
		free(temp);
	}
	while ((*l)->next != NULL) {
		if ((*l)->next->r == r) {
			RouteList *temp = (*l)->next;
			(*l)->next = (*l)->next->next;
			free(temp);
			return;
		}
	}
	return;
}
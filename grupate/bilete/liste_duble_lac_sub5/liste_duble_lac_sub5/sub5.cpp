#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct Lac {
	int cod_lac;
	char* denumire;
	float suprafata;
	int areInsule;
};

struct nodD {
	Lac info;
	nodD* next;
	nodD* prev;
};

struct ListaDubla {
	nodD* prim;
	nodD* ultim;
};

Lac creareLac(int cod, const char* nume, float suprafata, int insule) {
	Lac l;
	l.cod_lac = cod;
	l.denumire = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(l.denumire, nume);
	l.suprafata = suprafata;
	l.areInsule = insule;

	return l;
}

void afisareLac(Lac l) {
	printf("\nLacul %s (cod: %d) are o suprafata de %5.1f si %d insule", l.denumire, l.cod_lac, l.suprafata, l.areInsule);
}

void afisareListaIncSf(ListaDubla lista) {
	nodD* aux = lista.prim;
	while (aux) {
		afisareLac(aux->info);
		aux = aux->next;
	}
}

void afisareListaSfInc(ListaDubla lista) {
	nodD* aux = lista.ultim;
	while (aux) {
		afisareLac(aux->info);
		aux = aux->prev;
	}
}

nodD* creareNod(Lac l, nodD* next, nodD* prev) {
	nodD* nou = (nodD*)malloc(sizeof(nodD));
	nou->info = creareLac(l.cod_lac, l.denumire, l.suprafata, l.areInsule);
	nou->next = next;
	nou->prev = prev;

	return nou;
}

ListaDubla inserareInceput(ListaDubla lista, Lac l) {
	nodD* nou = creareNod(l, lista.prim, NULL);
	if (lista.prim) {
		lista.prim->prev = nou;
		lista.prim = nou;
	}
	else {
		lista.prim = lista.ultim = nou;
	}
	return lista;
}

ListaDubla stergereLista(ListaDubla lista) {
	nodD* temp = lista.prim;
	while (temp) {
		free(temp->info.denumire);
		nodD* aux = temp;
		temp = temp->next;
		free(aux);
	}
	lista.prim = lista.ultim = NULL;
	return lista;
}

float suprafataTotalaLacuriFaraInsule(ListaDubla lista) {
	nodD* temp = lista.prim;
	float suma = 0;
	while (temp) {
		if (temp->info.areInsule == 0) {
			suma += temp->info.suprafata;
		}
		temp = temp->next;
	}
	return suma;
}

Lac stergereDupaCod(ListaDubla* lista, int cod) {
	nodD* temp = lista->prim;
	while (temp) {
		if (temp->info.cod_lac == cod) {
			Lac rezultat = creareLac(temp->info.cod_lac, temp->info.denumire, temp->info.suprafata, temp->info.areInsule);
			if (temp->prev) { 
				temp->prev->next = temp->next;
				if (temp->next) { //suntem intre 2 noduri
					temp->next->prev = temp->prev;
				}
				else { //suntem pe ultimul nod
					lista->ultim = temp->prev;
					if (lista->ultim == NULL) { //am sters singurul nod din lista
						lista->prim = NULL;
					}
				}
			}
			else { //suntem pe primul nod
				if (temp->next) {
					temp->next = lista->prim;
					temp->next->prev = NULL;
				}
				else {
					lista->prim = lista->ultim = NULL;
				}
			}
			free(temp->info.denumire);
			free(temp);
			return rezultat;
		}
		temp = temp->next;
	}
	if (temp->info.cod_lac != cod) {
		return creareLac(0, "", 0, 0);
	}
}

void main() {
	ListaDubla lista;
	lista.prim = lista.ultim = NULL;
	lista = inserareInceput(lista, creareLac(258, "Balea", 200, 0));
	lista = inserareInceput(lista, creareLac(842, "Sf. Ana", 840, 3));
	lista = inserareInceput(lista, creareLac(185, "Frumos", 220, 5));
	lista = inserareInceput(lista, creareLac(208, "Albastru", 690, 0));
	lista = inserareInceput(lista, creareLac(957, "Verde", 750, 1));

	printf("\nLista de la inceput la sfarsit este: ");
	afisareListaIncSf(lista);

	printf("\n*************************************");
	printf("\nLista de la sfarsit la inceput este: ");
	afisareListaSfInc(lista);

	printf("\n*************************************");
	float suprafata = suprafataTotalaLacuriFaraInsule(lista);
	printf("\nSuprafata totala a lacurilor fara insule este de %5.2f", suprafata);

	Lac cautat = stergereDupaCod(&lista, 185);
	printf("\n*************************************");
	printf("\nLacul cautat este: ");
	afisareLac(cautat);

	printf("\n*************************************");
	printf("\nDupa extragere lista este: ");
	afisareListaIncSf(lista);


	lista = stergereLista(lista);

	system("pause");
}
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Aeroport {
	char* nume;
	int nrZboruri;
};

struct nodD {
	nodD* next;
	nodD* prev;
	Aeroport info;
};

struct LDI {
	nodD* prim;
	nodD* ultim;
};

struct nod {
	Aeroport info;
	nod* next;
};

Aeroport creareAeroport(const char* nume, int nrZboruri) {
	Aeroport a;
	a.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(a.nume, nume);
	a.nrZboruri = nrZboruri;

	return a;
}

nod* inserareInceputListaSimpla(nod* cap, Aeroport a) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareAeroport(a.nume, a.nrZboruri);
	nou->next = cap;

	return nou;
}

nodD* creareNod(Aeroport a, nodD* prev, nodD* next) {
	nodD* nou = (nodD*)malloc(sizeof(nodD));
	nou->info = creareAeroport(a.nume, a.nrZboruri);
	nou->next = next;
	nou->prev = prev;

	return nou;
}

LDI inserareInceput(Aeroport info, LDI lista) {
	nodD* nou = creareNod(info, NULL,lista.prim);
	if (lista.prim) { //daca avem deja lista
		lista.prim->prev = nou;
		lista.prim = nou;
	}
	else {
		lista.prim = lista.ultim = nou;
	}
	return lista;
}

LDI inserareSfarsit(Aeroport a, LDI lista) {
	nodD* nou = creareNod(a, lista.ultim, NULL);
	if (lista.ultim) {
		lista.ultim->next = nou;
		lista.ultim = nou;
	}
	else {
		lista.prim = lista.ultim = nou;
	}
	return lista;
}

LDI inserareSortata(LDI lista, Aeroport info) {
	if (lista.prim) { //daca am lista
		nodD* temp = lista.prim; //temporar pt primul nod
		while (temp&&temp->info.nrZboruri < info.nrZboruri) {
			temp = temp->next;
		}
		if (temp) { //am gasit deci inserez inaintea lui temp care poate sa fie primul nod sau altul
			nodD* nou = creareNod(info, temp->prev, temp);
			if (temp->prev) { //daca exista predecesor
				temp->prev->next = nou; //succesorul predecesorului va fi nodul de inserat
			}
			else { //nu exista predecesor, deci e primul nod
				lista.prim = nou;
			}
			temp->prev = nou; 
		}
		else { //e dupa ultimul nod
			nodD* nou = creareNod(info, lista.ultim, NULL);
			lista.ultim->next = nou;
			lista.ultim = nou;
		}
	}
	else {
		nodD* nou = creareNod(info, NULL, NULL);
		lista.prim = lista.ultim = nou;
	}
	return lista;
}

void afisareAeroport(Aeroport a) {
	printf("\nAeroportul %s are %d zboruri", a.nume, a.nrZboruri);
}

void afisareIncSf(LDI lista) {
	nodD* temp = lista.prim;
	while (temp) {
		afisareAeroport(temp->info);
		temp = temp->next;
	}
}

void afisareSfInc(LDI lista) {
	nodD* temp = lista.ultim;
	while (temp) {
		afisareAeroport(temp->info);
		temp = temp->prev;
	}
}

void afisareListaSimpla(nod* cap) {
	while (cap) {
		afisareAeroport(cap->info);
		cap = cap->next;
	}
}

//extragere dupa nr si stergere aeroport
Aeroport extragereAeroport(LDI* lista, int nrZboruriCautat) {
	nodD* p = lista->prim;
	while (p&&p->info.nrZboruri != nrZboruriCautat) {
		p = p->next;
	}
	if (p) {
		Aeroport rezultat = creareAeroport(p->info.nume, p->info.nrZboruri);
		if (p->prev) {
			p->prev->next = p->next;
			if (p->next) { //suntem intre 2 noduri
				p->next->prev = p->prev;
			}
			else { //suntem pe ultimul nod
				lista->ultim = p->prev;
				if (lista->ultim == NULL) { //care poate fi si singurul
					lista->prim == NULL;
				}
			}
		}
		else {//suntem pe primul nod
			if (p->next) {
				p->next->prev = NULL;
				lista->prim = p->next;
			}
			else{ //e singurul nod
				lista->prim = lista->ultim = NULL;
			}
		}
		free(p->info.nume);
		free(p);
		return rezultat;
	}
	else { //nu l-am gasit
		Aeroport a;
		a.nume = NULL;
		a.nrZboruri = -1;
		return a;
	}
}

//filtrare cu lista simpla
nod* filtrareAeroporturi(LDI lista, int nrLimita) {
	nod* cap = NULL;//initializez o lista goala in care o sa tin nodurile
	if (lista.prim) { //daca am lista
		nodD* temp = lista.prim;
		while (temp) {
			if (temp->info.nrZboruri <= nrLimita) {
				cap = inserareInceputListaSimpla(cap, temp->info);
			}
			temp = temp->next;
		}
	}
	return cap;
}

LDI stergereLista(LDI lista) {
	nodD* p = lista.prim;
	while (p) {
		free(p->info.nume);
		nodD* temp = p;
		p = p->next;
		free(temp);
	}
	lista.prim = NULL;
	lista.ultim = NULL;
	return lista;
}

void main() {
	LDI lista;
	lista.prim = lista.ultim = NULL;
	lista = inserareSortata(lista, creareAeroport("CDG", 200));
	lista = inserareSortata(lista, creareAeroport("Henri Coanda", 100));
	lista = inserareSortata(lista, creareAeroport("BXL", 150));
	lista = inserareSortata(lista, creareAeroport("VIE", 170));

	printf("\nLista de la inceput la sfarsit este:");
	afisareIncSf(lista);

	printf("\nExtragerea aeroportului cu 150 de zboruri");
	Aeroport extras = extragereAeroport(&lista, 150);
	afisareAeroport(extras);

	free(extras.nume);

	printf("\nLista dupa extragere este:");
	afisareIncSf(lista);

	nod* cap = NULL;
	cap = filtrareAeroporturi(lista, 170);
	printf("\nLista filtrata pentru a nu avea peste 170 de zboruri/aeroport este:");
	afisareListaSimpla(cap);

	lista = stergereLista(lista);
}

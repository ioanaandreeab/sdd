#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

struct Aeroport {
	char* nume;
	int nrZboruri;
};

struct nodD {
	nodD* next;
	nodD* prev;
	Aeroport info;
};

struct nod {
	nod* next;
	Aeroport info;
};

struct LDI{
	nodD* prim;
	nodD* ultim;
};

Aeroport creareAeroport(const char* nume, int nrZboruri) {
	Aeroport a;
	a.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(a.nume, nume);
	a.nrZboruri = nrZboruri;

	return a;
}

nodD* creareNod(Aeroport a, nodD* next, nodD* prev) {
	nodD* nou = (nodD*)malloc(sizeof(nodD));
	nou->next = next;
	nou->prev = prev;
	nou->info = creareAeroport(a.nume, a.nrZboruri);
	return nou;
}

nod* inserareInceput(nod* cap, Aeroport info) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareAeroport(info.nume, info.nrZboruri);
	nou->next = cap;
	return nou;
}

LDI inserareInceput(Aeroport info, LDI lista) {
	nodD* nodNou = creareNod(info, lista.prim, NULL);
	if (lista.prim) {
		lista.prim->prev = nodNou;
		lista.prim = nodNou;
	}
	else {
		lista.prim = nodNou;
		lista.ultim = nodNou;
	}
	return lista;
}

LDI inserareSfarsit(Aeroport info, LDI lista) {
	nodD* nodNou = creareNod(info, NULL, lista.ultim);
	if (lista.ultim) {
		lista.ultim->next = nodNou;
		lista.ultim = nodNou;
	}
	else {
		lista.prim = nodNou;
		lista.ultim = nodNou;
	}
	return lista;
}

LDI inserareSortata(LDI lista, Aeroport info) {
	if (lista.prim) {
		nodD* p = lista.prim;
		while (p&&p->info.nrZboruri < info.nrZboruri) {
			p = p->next;
		}
		if (p) {
			nodD* nou = creareNod(info, p, p->prev);
			if (p->prev) {
				p->prev->next = nou;
			}
			else {
				lista.prim = nou;
			}
			p->prev = nou;
		}
		else {
			nodD* nou = creareNod(info, NULL, lista.ultim);
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
	printf("Aeroportul %s are %d zboruri\n", a.nume, a.nrZboruri);
}

//afisare lista de la inceput la sfarsit
void afisareListaIncSf(LDI lista) {
	nodD* temp = lista.prim;
	while (temp) {
		afisareAeroport(temp->info);
		temp = temp->next;
	}
}

//afisare lista de la sfarsit la inceput
void afisareListaSfInc(LDI lista) {
	nodD* temp = lista.ultim;
	while (temp) {
		afisareAeroport(temp->info);
		temp = temp->prev;
	}
}

LDI stergereLista(LDI lista) {
	nodD* p = lista.prim;
	while (p) {
		free(p->info.nume);
		nodD* temp = p; //ca sa nu pierd info
		p = p->next;//ma mut pe urmatorul nod
		free(temp);//sterg nodul cu totul
	}
	lista.prim = NULL;
	lista.ultim = NULL;
	return lista;
}

LDI inserarePePoz(Aeroport info, LDI lista, int poz) {
	if (poz <= 1 || lista.prim == NULL) {
		lista = inserareInceput(info, lista);
		return lista;
	}
	else {
		int contor = 1;
		nodD* p = lista.prim;
		while (p&&contor < poz) {
			p = p->next;
			contor++;
		}
		if (p) {
			nodD* nodNou = creareNod(info, p, p->prev);
			p->prev->next = nodNou;
			p->prev = nodNou;
			return lista;
		}
		else {
			lista = inserareSfarsit(info, lista);
			return lista;
		}
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
			if (p->next) { //aici suntem intre doua noduri
				p->next->prev = p->prev;
			}
			else { //suntem pe ultimul nod
				lista->ultim = p->prev;
				if (lista->ultim == NULL) {
					lista->prim == NULL;
				}
			}
		}
		else { //suntem pe primul nod
			if (p->next) {
				p->next->prev = NULL;
				lista->prim = p->next;
			}
			else {
				lista->prim = NULL;
				lista->ultim = NULL;
			}
		}
		free(p->info.nume);
		free(p);
		return rezultat;
	}
	else {
		Aeroport a;
		a.nume = NULL;
		a.nrZboruri = -1;
		return a;
	}
}

//filtrare cu lista simpla in care sunt afisate nodurile
nod* filtrareAeroporturi(LDI lista, int nrLimita) {
	nod* cap = NULL;//initializez o lista in care o sa tin nodurile
	if (lista.prim) {
		nodD* p = lista.prim;
		while (p) {
			if (p->info.nrZboruri >= nrLimita) {
				cap = inserareInceput(cap, p->info);
			}
			p = p->next;
		}
	}
	return cap;
}


void main() {
	LDI lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	lista = inserareSortata(lista, creareAeroport("CDG", 200));
	lista = inserareSortata(lista, creareAeroport("Henri Coanda", 100));
	lista = inserareSortata(lista, creareAeroport("BXL", 150));

	printf("Lista afisata de la inceput la sfarsit este:\n");
	afisareListaIncSf(lista);
	printf("\nLista afisata de la sfarsit la inceput este:\n");
	afisareListaSfInc(lista);

	int nr;
	printf("\nIntroduceti nr de zboruri al aeroportului pe care vreti sa il stergeti: ");
	scanf("%d", &nr);
	Aeroport a = extragereAeroport(&lista, nr);
	printf("\nAeroportul:\n");
	afisareAeroport(a);

	printf("\nLista dupa extragere este:\n");
	afisareListaIncSf(lista);

	int limita;
	printf("\nIntroduceti nr de zboruri limita: ");
	scanf("%d", &limita);
	nod* cap = filtrareAeroporturi(lista, limita);

	printf("\n\n");
	while (cap) {
		afisareAeroport(cap->info);

		nod* temp = cap;
		cap = cap->next;
		free(temp->info.nume);
		free(temp);
	}
	printf("\nDupa filtrare, lista este:\n");
	afisareListaIncSf(lista);

	lista = stergereLista(lista);

	printf("\nDupa stergere, lista este:\n");
	afisareListaIncSf(lista);
	system("pause");
}
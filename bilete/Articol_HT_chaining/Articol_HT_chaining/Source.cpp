#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Articol {
	int id;
	char* titlu;
	char* categorie; //cheie
	int nrPagini;
	int nrAutori;
};

Articol initArticol(int id, const char* titlu, const char* categorie, int nrPagini, int nrAutori) {
	Articol a;
	a.id = id;
	a.titlu = (char*)malloc(sizeof(char)*(strlen(titlu) + 1));
	strcpy(a.titlu, titlu);
	a.categorie = (char*)malloc(sizeof(char)*(strlen(categorie) + 1));
	strcpy(a.categorie, categorie);
	a.nrPagini = nrPagini;
	a.nrAutori = nrAutori;

	return a;
}

void afisareArticol(Articol a) {
	printf("\nArticolul %d, cu titlul %s, categorie %s, are %d pagini si %d autori.", a.id, a.titlu, a.categorie, a.nrPagini, a.nrAutori);
}

struct nod {
	nod* next;
	Articol info;
};

struct HT {
	nod* *vector;
	int dim;
};

HT initHT(int dim) {
	HT h;
	h.dim = dim;
	h.vector = (nod**)malloc(sizeof(nod*)*dim);
	for (int i = 0; i < dim; i++) {
		h.vector[i] = NULL;
	}

	return h;
}

nod* inserareInceput(nod* cap, Articol a) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = cap;
	nou->info = initArticol(a.id,a.titlu,a.categorie,a.nrPagini,a.nrAutori);
	
	return nou;
}

int hashFunction(int dim, const char* categorie) {
	int suma = 0;
	for (int i = 0; i < strlen(categorie); i++) {
		suma += categorie[i];
	}
	return suma % dim;
}

int inserareHT(HT h, Articol a) {
	if (h.dim > 0) {
		int hashCode = hashFunction(h.dim, a.categorie);
		h.vector[hashCode] = inserareInceput(h.vector[hashCode], a);
		return hashCode;
	}
	else {
		return -2; //nu exista tabela
	}
}

void afisareLista(nod* cap) {
	if (cap) {
		nod* temp = cap;
		while (temp) {
			afisareArticol(temp->info);
			temp = temp->next;
		}
	}
}

int nrTotalAutori(HT h) {
	if (h.dim > 0) {
		int nrAutori = 0;
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i] != NULL) {
				nod* temp = h.vector[i];
				while (temp) {
					nrAutori+=temp->info.nrAutori;
					temp = temp->next;
				}
			}
		}
		return nrAutori;
	}
}

void afisareHT(HT h) {
	if (h.dim > 0) {
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i] != NULL) {
				afisareLista(h.vector[i]);
			}
		}
	}
}

Articol* articoleDinCategorie(HT h, const char* categorie, int &nrArticole) {
	if (h.dim > 0) {
		int hashCode = hashFunction(h.dim, categorie);
		if (h.vector[hashCode]) {
			nod* temp = h.vector[hashCode];
			while (temp) {
				if (strcmp(temp->info.categorie, categorie)==0) {
					nrArticole++;
				}
				temp = temp->next;
			}
		}
		Articol* articole = (Articol*)malloc(sizeof(Articol)*nrArticole);
		int index = 0;
		if (h.vector[hashCode]) {
			nod* temp = h.vector[hashCode];
			while (temp) {
				if (strcmp(temp->info.categorie, categorie)==0) {
					articole[index++] = temp->info;
				}
				temp = temp->next;
			}
		}
		return articole;
	}
	else {
		return NULL;
	}
}

void marireNrPagini(HT &h) {
	if (h.dim>0) {
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i] != NULL) {
				nod* temp = h.vector[i];
				while (temp) {
					temp->info.nrPagini++;
					temp = temp->next;
				}
			}
		}
	}
}

void stergereArticol(HT &h, const char* categorie, int id) {
	if (h.dim > 0) {
		int hashCode = hashFunction(h.dim, categorie);
		if (h.vector[hashCode]) {
			nod* temp = h.vector[hashCode];
			if (temp->info.id == id) {
				nod* aux = temp;
				temp = temp->next;
				free(aux->info.categorie);
				free(aux->info.titlu);
				free(aux);
			}
			else {
				nod* aux = temp;
				while (aux->next && aux->next->info.id != id) {
					aux = aux->next;
				}
				if (aux->next) {
					nod* p = aux->next;
					aux->next = aux->next->next;
					free(p->info.categorie);
					free(p->info.titlu);
					free(p);
				}
			}
		}
	}
}

void main() {
	HT h = initHT(6);
	inserareHT(h, initArticol(1, "IoT Security", "Security", 12, 2));
	inserareHT(h, initArticol(2, "Blockchain", "Finance", 7, 1));
	inserareHT(h, initArticol(3, "Frontend Development", "Web development", 20, 2));
	inserareHT(h, initArticol(4, "Backend Development", "Web development", 15, 3));
	inserareHT(h, initArticol(5, "Cooking with Jamie", "Culinary", 30, 1));
	afisareHT(h);

	int nrAutori = nrTotalAutori(h);
	printf("\n\nNr total de autori este: %d", nrAutori);

	printf("\n\nArticolele din categoria data sunt: ");
	int nrArticole = 0;
	Articol* articole = articoleDinCategorie(h, "Web development", nrArticole);
	for (int i = 0; i < nrArticole; i++) {
		afisareArticol(articole[i]);
	}

	printf("\n\nDupa marirea nr. paginilor:");
	marireNrPagini(h);
	afisareHT(h);

	printf("\n\nDupa stergerea articolului:");
	stergereArticol(h, "Web development", 4);
	afisareHT(h);
}
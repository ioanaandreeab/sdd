#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct Telefon {
	char* serie;
	float pret;
	int memorie;
};

struct nod {
	Telefon info;
	nod* next;
};

struct HashTable {
	nod* *vector;
	int dim;
};

Telefon creareTelefon(const char* serie, float pret, int mem) {
	Telefon t;
	t.serie = (char*)malloc(sizeof(char)*(strlen(serie) + 1));
	strcpy(t.serie, serie);
	t.pret = pret;
	t.memorie = mem;

	return t;
}

void afisareTelefon(Telefon t) {
	printf("\nTelefonul seria %s are pretul de %5.2f si o memorie de %d GB", t.serie, t.pret, t.memorie);
}

int hashFunction(const char* serie, int dim) {
	int suma = 0;
	for (int i = 0; i < strlen(serie); i++) {
		suma += serie[i];
	}
	return suma % dim;
}

nod* inserareInceput(nod* cap, Telefon info) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareTelefon(info.serie, info.pret, info.memorie);
	nou->next = cap;

	return nou;	
}

HashTable creareHashTable(int dim) {
	HashTable h;
	h.dim = dim;
	h.vector = (nod**)malloc(sizeof(nod*)*dim);
	for (int i = 0; i < dim; i++) {
		h.vector[i] = NULL;
	}
	return h;
}

int inserareTabela(Telefon t, HashTable h) {
	if (h.dim > 0) {
		int hash = hashFunction(t.serie, h.dim);
		if (h.vector[hash]) { //avem coliziune
			h.vector[hash] = inserareInceput(h.vector[hash], t);
		}
		else {
			h.vector[hash] = inserareInceput(h.vector[hash], t);
		}
		return hash;
	}
	else {
		return -1;
	}
}

void afisareTabela(HashTable h) {
	for (int i = 0; i < h.dim; i++) {
		if (h.vector[i]) {
			nod* p = h.vector[i];
			while (p) {
				afisareTelefon(p->info);
				p = p->next;
			}
		}
	}
}

Telefon cautareDupaSerie(const char* serie, HashTable ht) {
	if (ht.vector) {
		int pozitie = hashFunction(serie, ht.dim);
		nod*p = ht.vector[pozitie];
		while (p&&strcmp(serie, p->info.serie) != 0) {
			p = p->next;
		}
		if (p) {
			return p->info;
		}
	}
	else {
		return creareTelefon("", 0, 0);
	}
}

void stergereTabela(HashTable *ht) {
	for (int i = 0; i < ht->dim; i++) {
		nod* p = ht->vector[i];
		while (p) {
			free(p->info.serie);
			nod* aux = p;
			p = p->next;
			free(aux);
		}
	}
	free(ht->vector);
	ht->dim = 0;
	ht->vector = NULL;
}

void main() {
	HashTable ht = creareHashTable(5);
	inserareTabela(creareTelefon("RX23", 400, 10),ht);
	inserareTabela(creareTelefon("UQU25", 250, 6),ht);
	inserareTabela(creareTelefon("OPW54", 360, 8),ht);
	inserareTabela(creareTelefon("ASD41", 450, 2),ht);
	inserareTabela(creareTelefon("RX25", 4000, 100),ht);

	afisareTabela(ht);
	
	printf("\n");

	Telefon cautat = cautareDupaSerie("RX25", ht);
	if (cautat.serie) {
		afisareTelefon(cautat);
	}

	stergereTabela(&ht);

	system("pause");
}
#include <stdio.h>
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

Telefon creareTelefon(const char* serie, float pret, int memorie) {
	Telefon t;
	t.serie = (char*)malloc(sizeof(char)*(strlen(serie) + 1));
	strcpy(t.serie, serie);
	t.pret = pret;
	t.memorie = memorie;

	return t;
}

void afisareTelefon(Telefon t) {
	printf("\nTelefonul cu seria %s si memoria de %d GB are pretul de %5.2f", t.serie, t.memorie, t.pret);
}

HashTable createHashTable(int dim) {
	HashTable h;
	h.dim = dim;
	h.vector = (nod**)malloc(sizeof(nod*)*dim);
	for (int i = 0; i < dim; i++) {
		h.vector[i] = NULL;
	}
	return h;

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

int inserareInTabela(Telefon t, HashTable h) {
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
		return -2; //nu exista tabela
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
		nod* p = ht.vector[pozitie];
		while (p && strcmp(serie, p->info.serie) != 0) {
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

HashTable stergereTabela(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		nod* p = ht.vector[i];
		while (p) {
			free(p->info.serie);
			nod* aux = p;
			p = p->next;
			free(aux);
		}
	}
	free(ht.vector);
	ht.dim = 0;
	ht.vector = NULL;
	return ht;
}

void main() {
	HashTable ht = createHashTable(5);
	inserareInTabela(creareTelefon("RX23", 400, 10), ht);
	inserareInTabela(creareTelefon("UQU25", 250, 6), ht);
	inserareInTabela(creareTelefon("OPW54", 360, 8), ht);
	inserareInTabela(creareTelefon("ASD41", 450, 2), ht);
	inserareInTabela(creareTelefon("RX25", 4000, 100), ht);

	afisareTabela(ht);

	Telefon cautat = cautareDupaSerie("RX25",ht);
	printf("\n\nTelefonul cautat este: ");
	afisareTelefon(cautat);

	ht = stergereTabela(ht);

}
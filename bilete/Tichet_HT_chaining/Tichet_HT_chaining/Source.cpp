#include <stdio.h>
#include <malloc.h>
#include <string.h>

enum stare { New, Open, Progress, Fixed, Closed };

struct Tichet {
	char* codTichet;
	char* descriere;
	char* numeEmitent; //cheie
	char* dataDeschidere;
	stare stare;
};

Tichet initTichet(const char* codTichet, const char* descriere, const char* numeEmitent, const char* dataDeschidere, stare stare) {
	Tichet t;
	t.codTichet = (char*)malloc(sizeof(char)*(strlen(codTichet) + 1));
	strcpy(t.codTichet, codTichet);
	t.descriere = (char*)malloc(sizeof(char)*(strlen(descriere) + 1));
	strcpy(t.descriere, descriere);
	t.numeEmitent = (char*)malloc(sizeof(char)*(strlen(numeEmitent) + 1));
	strcpy(t.numeEmitent, numeEmitent);
	t.dataDeschidere = (char*)malloc(sizeof(char)*(strlen(dataDeschidere) + 1));
	strcpy(t.dataDeschidere, dataDeschidere);
	t.stare = stare;
	
	return t;
}

void afisareTichet(Tichet t){
	printf("\nTichet %s", t.codTichet);
	printf("\nDescriere: %s", t.descriere);
	printf("\nNume emitent: %s", t.numeEmitent);
	printf("\nData deschidere: %s", t.dataDeschidere);
	switch (t.stare) {
	case New: printf("\nStare: new\n"); break;
	case Open: printf("\nStare: open\n"); break;
	case Progress: printf("\nStare: progress\n"); break;
	case Fixed: printf("\nStare: fixed\n"); break;
	case Closed: printf("\nStare: closed\n"); break;
	}
}

struct nod {
	nod* next;
	Tichet info;
};

struct HT {
	int dim;
	nod* *vector;
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

int hashFunction(const char* numeEmitent, int dim) {
	int s = 0;
	for (int i = 0; i < strlen(numeEmitent); i++) {
		s += numeEmitent[i];
	}
	return s % dim;
}

nod* inserareLaInceput(nod* cap, Tichet t) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = cap;
	nou->info = initTichet(t.codTichet, t.descriere, t.numeEmitent, t.dataDeschidere, t.stare);
	return nou;
}

nod* creareNod(nod* next, Tichet t) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = next;
	nou->info = initTichet(t.codTichet, t.descriere, t.numeEmitent, t.dataDeschidere, t.stare);
	return nou;
}

int inserareHT(HT h, Tichet t) {
	if (h.dim > 0) {
		int hashCode = hashFunction(t.numeEmitent, h.dim);
		h.vector[hashCode] = inserareLaInceput(h.vector[hashCode],t);
		return hashCode;
	}
	else {
		return -2; //nu exista tabela
	}
}

void afisareLista(nod* cap) {
	if (cap) {
		while (cap) {
			afisareTichet(cap->info);
			cap = cap->next;
		}
	}
}

void afisareHt(HT h) {
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i]) {
				afisareLista(h.vector[i]);
			}
		}
}

int ticheteDupaNume(HT h, const char* numeEmitent) {
	int suma = 0;
	if (h.dim > 0)
	{
		int hashCode = hashFunction(numeEmitent,h.dim);
		while (h.vector[hashCode])
		{
			if (strcmp(h.vector[hashCode]->info.numeEmitent, numeEmitent) == 0)
				suma++;
			h.vector[hashCode] = h.vector[hashCode]->next;
		}
		return suma;
	}
}

void main() {
	HT h = initHT(5);
	inserareHT(h, initTichet("25B", "Vacanta in Hawaii", "Ioana", "25.02.2019", New));
	inserareHT(h, initTichet("XW35B", "Vacanta in Viena", "Andrei", "26.06.2019", Fixed));
	inserareHT(h, initTichet("OF64", "Vacanta in Paris", "Denisa", "25.07.2019", Closed));
	inserareHT(h, initTichet("68GB", "Vacanta in Munchen", "Ioana", "25.02.2019", New));
	inserareHT(h, initTichet("4676S", "Vacanta in Bruxelles", "Stefan", "25.02.2019", New));

	afisareHt(h);
	int nrTichete = ticheteDupaNume(h, "Ioana");
	printf("\n\nAngajatul Ioana a emis %d tichete.", nrTichete);

	printf("\n\nTichete emise de Ioana in data de 25.02.2019:");
	afisareLista(tichete);
	
}
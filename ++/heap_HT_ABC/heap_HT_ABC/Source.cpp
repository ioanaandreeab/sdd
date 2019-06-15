#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct HEAP {
	int* vector;
	int dim;
};

HEAP initHeap(int dim) {
	HEAP h;
	h.vector = (int*)malloc(sizeof(int)*dim);
	h.dim = dim;

	return h;
}

//filtreaza vectorul a.i. sa fie in forma de maxim, adica maximul va fi pe prima pozitie, de restul nu stim exact dar nu ne intereseaza
void filtrare(HEAP heap, int index) {
	if (index < heap.dim) {
		int pozSt = 2 * index + 1;
		int pozDr = 2 * index + 2;
		int maxPoz = index;

		if (pozSt < heap.dim && heap.vector[maxPoz] < heap.vector[pozSt]) {
			maxPoz = pozSt;
		}
		
		if (pozDr < heap.dim && heap.vector[maxPoz] < heap.vector[pozDr]) {
			maxPoz = pozDr;
		}

		if (maxPoz != index) {
			int aux = heap.vector[index];
			heap.vector[index] = heap.vector[maxPoz];
			heap.vector[maxPoz] = aux;
			if (2 * maxPoz + 1 < heap.dim) { //daca elem curent mai are fiu(in stanga)
				filtrare(heap, maxPoz);
			}
		}
	}
}

//trebuie sa mutam elem din heap in ABC
struct nodABC {
	nodABC* st;
	nodABC* dr;
	int info;
};

nodABC* inserareInABC(nodABC* rad, int info) {
	if (rad) {
		if (info < rad->info) {
			rad->st = inserareInABC(rad->st, info);
		}
		else {
			rad->dr = inserareInABC(rad->dr, info);
		}
		return rad;
	}
	else {
		nodABC* nou = (nodABC*)malloc(sizeof(nodABC));
		nou->dr = nou->st = NULL;
		nou->info = info;
		return nou;
	}
}

//afisare inordine
void afisareABCInordine(nodABC* rad) {
	if (rad) {
		afisareABCInordine(rad->st);
		printf("%d ", rad->info);
		afisareABCInordine(rad->dr);
	}
}

//elementele din arbore le vom pune acum intr-un HT; fol chaining cu lista simpla
struct nod {
	nod* next;
	int info;
};

struct HT {
	nod* *vector;
	int dim;
};

HT initHT(int dim) {
	HT h;
	h.vector = (nod**)malloc(sizeof(nod*)*dim);
	h.dim = dim;
	for (int i = 0; i < dim; i++) {
		h.vector[i] = NULL;
	}

	return h;
}

int hashFunction(int dim, int info) {
	return info % dim;
}

//inserare la inceput de lista
nod* inserareLaInceput(nod* cap, int info) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = cap;
	nou->info = info;
	cap = nou;

	return cap;
}

HT insertInHT(HT h, int info) {
	if (h.dim > 0) {
		int index = hashFunction(h.dim, info);
		h.vector[index] = inserareLaInceput(h.vector[index], info);
	}
	return h;
}

//functie care mula elem din arbore in HT; seamana ft mult cu functia de
//daca voiam sa inserez doar elem care trec de un anumit prag
//pun aici conditie la root & primesc param
void transformaABCInHT(nodABC* rad, HT h) {
	if (rad) {
		transformaABCInHT(rad->st, h);
		insertInHT(h, rad->info);
		transformaABCInHT(rad->dr, h);
	}
}

//afisare a listei
void afisareLista(nod* cap) {
	while (cap) {
		printf("%d ", cap->info);
		cap = cap->next;
	}
}

//afisare HT
void afisareHT(HT h){
	for (int i = 0; i < h.dim; i++) {
		printf("%d. ", (i + 1));//linia pe care suntem
		if (h.vector[i]) {
			afisareLista(h.vector[i]);
		}
		printf("\n");
	}
}

void main() {
	HEAP heap = initHeap(6);
	for (int i = 0; i < heap.dim; i++) {
		heap.vector[i] = i + 1;
	}

	//filtrare de jos in sus, de la ultimul parinte
	for (int i = (heap.dim - 2) / 2; i >= 0; i--) {
		filtrare(heap, i);
	}

	//afisez heapul
	for (int i = 0; i < heap.dim; i++) {
		printf("%d ", heap.vector[i]);
	}

	nodABC* arbore = NULL;
	//inserez elem din heap in arbore
	for (int i = 0; i < heap.dim; i++) {
		arbore = inserareInABC(arbore, heap.vector[i]);
	}

	//afisez arborele
	printf("\nArbore:\n");
	afisareABCInordine(arbore);

	//pun elem din arbore in HT
	HT hashTable = initHT(7);
	transformaABCInHT(arbore, hashTable);
	//afisare hash
	printf("\nHT:\n");
	afisareHT(hashTable);
}
#include <stdio.h>
#include <malloc.h>
#include <string.h>


struct Proiect {
	int id;
	char* titlu;
	int nrPagini;
};

struct nodArbore {
	Proiect info;
	nodArbore* st;
	nodArbore* dr;
};

nodArbore* inserareInArbore(nodArbore* rad, Proiect p) {
	if (rad) {
		if (rad->info.id > p.id) {
			rad->st = inserareInArbore(rad->st, p);
		}
		else {
			rad->dr = inserareInArbore(rad->dr, p);
		}
		return rad;
	}
	else {
		nodArbore* nou = (nodArbore*)malloc(sizeof(nodArbore));
		nou->dr = nou->st = NULL;
		nou->info = p;
		return nou;
	}
}

Proiect initProiect(int id, const char* titlu, int nrPagini) {
	Proiect p;
	p.id = id;
	p.titlu = (char*)malloc(sizeof(char)*(strlen(titlu) + 1));
	strcpy(p.titlu, titlu);
	p.nrPagini = nrPagini;
	
	return p;
}

void afisareProiect(Proiect p) {
	printf("\nProiectul %d, cu titlul %s are %d pagini", p.id, p.titlu, p.nrPagini);
}

void afisareInordine(nodArbore* rad) {
	if (rad) {
		afisareInordine(rad->st);
		afisareProiect(rad->info);
		afisareInordine(rad->dr);
	}
}

struct nodLista {
	nodLista* next;
	Proiect info;
};

nodLista* inserareInLista(nodLista* cap, Proiect p) {
	nodLista* nou = (nodLista*)malloc(sizeof(nodLista));
	nou->info = initProiect(p.id, p.titlu, p.nrPagini);
	nou->next = NULL;
	if (cap) {
		nodLista* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		cap = nou;
	}
	return cap;
}

void transformaArboreInLista(nodLista* &cap, nodArbore* rad) {
	if (rad) {
		transformaArboreInLista(cap,rad->st);
		cap = inserareInLista(cap, initProiect(rad->info.id,rad->info.titlu,rad->info.nrPagini));
		transformaArboreInLista(cap, rad->dr);
	}
}

void transformaArboreInListaConditie(nodLista* &cap, nodArbore* rad, int prag) {
	if (rad) {
		transformaArboreInListaConditie(cap, rad->st, prag);
		if (rad->info.nrPagini > 30) {
			cap = inserareInLista(cap, initProiect(rad->info.id, rad->info.titlu, rad->info.nrPagini));
		}
		transformaArboreInListaConditie(cap, rad->dr, prag);
	}
}

nodArbore* minimArbore(nodArbore* rad) {
	if (rad) {
		if (rad->st) {
			return minimArbore(rad->st);
		}
		else {
			return rad;
		}
	}
}

nodArbore* maximArbore(nodArbore* rad) {
	if (rad) {
		if (rad->dr) {
			return maximArbore(rad->dr);
		}
		else {
			return rad;
		}
	}
}

nodArbore* maximArboreIterativ(nodArbore* rad) {
	if (rad) {
		while (rad->dr) {
			rad = rad->dr;
		}
		return rad;
	}
}

void afisareLista(nodLista* cap) {
	while (cap) {
		afisareProiect(cap->info);
		cap = cap->next;
	}
}

void main() {
	nodArbore* arbore = NULL;
	arbore = inserareInArbore(arbore, initProiect(3, "Ceva", 20));
	arbore = inserareInArbore(arbore, initProiect(7, "Altceva", 40));
	arbore = inserareInArbore(arbore, initProiect(2, "Inca altceva", 10));
	arbore = inserareInArbore(arbore, initProiect(1, "Ink Ceva", 50));
	arbore = inserareInArbore(arbore, initProiect(5, "Cevaa", 20));

	printf("\nArborele este:");
	afisareInordine(arbore);

	nodLista* cap = NULL;
	transformaArboreInLista(cap, arbore);
	printf("\n\nLista este:");
	afisareLista(cap);
	
	printf("\n\nAfisare lista cu proiecte ce au peste 30 de pagini:");
	nodLista* cond = NULL;
	transformaArboreInListaConditie(cond, arbore, 30);
	afisareLista(cond);

}
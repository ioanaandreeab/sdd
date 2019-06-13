#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

struct Carte {
	char* titlu;
	int nrPagini;
};

struct nod {
	Carte info;
	nod* next;
};

Carte creareCarte(const char* titlu, int nrPag) {
	Carte c;
	c.titlu = (char*)malloc(sizeof(char)*(strlen(titlu) + 1));
	strcpy(c.titlu, titlu);
	c.nrPagini = nrPag;

	return c;
}

void afisareCarte(Carte c) {
	printf("\nCartea cu titlul %s are %d pagini",c.titlu,c.nrPagini);
}

void afisareLista(nod* cap) {
	while (cap) {
		afisareCarte(cap->info);
		cap = cap->next;
	}
}

nod* stergereLista(nod* cap) {
	while (cap)
	{
		free(cap->info.titlu);
		nod* temp = cap;
		cap = cap->next;
		free(temp);
	}
	return cap;
}

nod* inserareInceput(nod* cap, Carte c) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = cap;
	nou->info = creareCarte(c.titlu, c.nrPagini);

	return nou;
}

nod* inserareSfarsit(nod* cap, Carte c) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareCarte(c.titlu, c.nrPagini);
	nou->next = NULL;
	if (cap) {
		nod* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou; //am ajuns la final si introduc nodul
	}
	else {
		cap = nou;
	}
	return cap;
}

nod* inserareSortata(nod* cap, Carte c) {
	if (cap) {
		if (c.nrPagini < cap->info.nrPagini) { //daca ce vreau sa introduc e mai mic decat ce se afla in cap
			cap = inserareInceput(cap,c); //inserez
		}
		else { //verific cu urmatorul nod de dupa cap
			nod* temp = cap;
			while (temp->next && c.nrPagini > temp->next->info.nrPagini) {
				temp = temp->next;
			}
			temp->next = inserareInceput(temp->next, c);
		}
	}
	else {
		cap = inserareInceput(NULL, c);
	}
	return cap;
}

nod* pushStack(nod* stiva, Carte c) {
	stiva = inserareInceput(stiva, c);
	return stiva;
}

Carte popStack(nod* *stiva) {
	if (*stiva) {
		Carte rezultat = creareCarte((*stiva)->info.titlu, (*stiva)->info.nrPagini);
		nod* temp = *stiva;
		*stiva = (*stiva)->next;
		temp->next = NULL;
		free(temp->info.titlu);
		free(temp);
		return rezultat;
	}
	else {
		Carte c = creareCarte("", -1);
		return c;
	}
}

nod* pushQueue(nod* cap, Carte info) {
	cap = inserareSfarsit(cap, info);
	return cap;
}

Carte popQueue(nod* *cap) {
	return popStack(cap);
}

int existaStiva(nod* stiva) {
	return stiva ? 1 : 0;
}

nod* filtrareCarti(nod* stiva, int limita) {
	nod* stivaTemp = NULL;
	while (existaStiva(stiva)) { //cat timp am elemente in stiva
		Carte carteTemp = popStack(&stiva); //scot cate un element din stiva
		if (carteTemp.nrPagini >= limita) { //daca nr. de pag e mai mare decat limita
			afisareCarte(carteTemp); //afisez cartea
		}
		else {
			stivaTemp = pushStack(stivaTemp, carteTemp); //daca e mai mic, pun cartea in stiva temporara
		}
		free(carteTemp.titlu); //dezaloc char*
	}
	while (stivaTemp) { //cat timp am elemente in stiva temporara
		Carte temp = popStack(&stivaTemp); //le iau din stiva temporara
		stiva = pushStack(stiva, temp); //si le pun inapoi in stiva doar pe cele mai mici decat limita!!
		free(temp.titlu); //dezaloc char*
	}
	return stiva;
}

nod* filtrareCartiInCoada(nod* *stack, int limit) {
	nod* tempStack = NULL;
	nod* resultQueue = NULL;

	while (existaStiva(*stack)) {
		Carte tempBook = popStack(stack);
		if (tempBook.nrPagini >= limit) {
			resultQueue = pushQueue(resultQueue, tempBook);
		}
		else {
			tempStack = pushStack(tempStack, tempBook);
		}
		free(tempBook.titlu);
	}
	while (tempStack) {
		Carte temp = popStack(&tempStack);
		*stack = pushStack(*stack, temp);
		free(temp.titlu);
	}

	return resultQueue;
}

void main() {
	nod* stiva = NULL;
	stiva = pushStack(stiva, creareCarte("A Clockwork Orange", 250));
	stiva = pushStack(stiva, creareCarte("Ferma Animalelor", 120));
	stiva = pushStack(stiva, creareCarte("1984", 300));
	stiva = pushStack(stiva, creareCarte("Lolita", 150));
	stiva = pushStack(stiva, creareCarte("The Notebook", 550));
	afisareLista(stiva);

	int limita;
	printf("\nIntroduceti limita de pagini: ");
	scanf("%d", &limita);

	//stiva = filtrareCarti(stiva, limita);
	nod* rezultat = filtrareCartiInCoada(&stiva, limita);

	printf("\nStiva dupa filtrare: ");
	while (existaStiva(stiva)) {
		Carte c = popStack(&stiva);
		afisareCarte(c);
		free(c.titlu);
	}

	printf("\nCoada dupa filtrare: ");
	while (rezultat) {
		Carte c = popQueue(&rezultat);
		afisareCarte(c);
		free(c.titlu);
	}



	system("pause");
}
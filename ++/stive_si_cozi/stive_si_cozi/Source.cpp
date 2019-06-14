#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Carte {
	char* titlu;
	int nrPagini;
};

struct nod {
	Carte info;
	nod* next;
};

Carte creareCarte(const char* titlu, int nrPagini) {
	Carte c;
	c.titlu = (char*)malloc(sizeof(char)*(strlen(titlu) + 1));
	strcpy(c.titlu, titlu);
	c.nrPagini = nrPagini;

	return c;
}

void afisareCarte(Carte c) {
	printf("\nCartea cu titlul %s are %d pagini", c.titlu, c.nrPagini);
}

void afisareLista(nod* cap) {
	while (cap) {
		afisareCarte(cap->info);
		cap = cap->next;
	}
}

nod* stergereLista(nod* cap) {
	while (cap) {
		free(cap->info.titlu);
		nod* temp = cap;
		cap = cap->next;
		free(temp);
	}
	return cap;
}
//inserare inceput pentru stiva
nod* inserareInceput(nod* cap, Carte c) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareCarte(c.titlu, c.nrPagini);
	nou->next = cap;

	return nou;
}

nod* pushStack(nod* stiva, Carte c) {
	stiva = inserareInceput(stiva, c);
	return stiva;
}

//inserare sfarsit pentru coada
nod* inserareSfarsit(nod* cap, Carte c) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareCarte(c.titlu, c.nrPagini);
	nou->next = NULL;
	if (cap) {
		nod* temp = cap;
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

nod* pushQueue(nod* coada, Carte c) {
	coada = inserareSfarsit(coada, c);
	return coada;
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

Carte popQueue(nod* *coada) {
	return popStack(coada);
}

int existaStiva(nod* stiva) {
	return stiva ? 1 : 0;
}

nod* filtrareCarti(nod* stiva, int limita) {
	nod* stivaTemp = NULL;
	while (existaStiva(stiva)) { //cat timp am elem in stiva
		Carte carteTemp = popStack(&stiva);//scot cate un elem din stiva
		if (carteTemp.nrPagini >= limita) { //daca nr de pag e mai mare decat limita
			afisareCarte(carteTemp); //afisez
		}
		else {
			stivaTemp = pushStack(stivaTemp, carteTemp);//daca e mai mic, pun cartea in stiva temporara
		}
		free(carteTemp.titlu);
	}
	while (stivaTemp) { //cat timp am elem in stiva temporara
		Carte temp = popStack(&stivaTemp); //le iau din stiva temporara
		stiva = pushStack(stiva, temp); //si le pun inapoi in stiva initiala
		free(temp.titlu);
	}
	return stiva;
}

nod* filtrareCartiInCoada(nod* *stiva, int limita) {
	nod* stivaTemp = NULL;
	nod* coadaRez = NULL;

	while (existaStiva(*stiva)) {
		Carte temp = popStack(stiva);
		if (temp.nrPagini >= limita) {
			coadaRez = pushQueue(coadaRez, temp);
		}
		else {
			stivaTemp = pushStack(stivaTemp, temp);
		}
		free(temp.titlu);
	}
	while (stivaTemp) {
		Carte temp = popStack(&stivaTemp);
		*stiva = pushStack(*stiva, temp);
		free(temp.titlu);
	}
	return coadaRez;
}

void main() {
	nod* stiva = NULL;
	stiva = pushStack(stiva, creareCarte("A Clockwork Orange", 250));
	stiva = pushStack(stiva, creareCarte("Ferma Animalelor", 120));
	stiva = pushStack(stiva, creareCarte("1984", 300));
	stiva = pushStack(stiva, creareCarte("Lolita", 150));
	stiva = pushStack(stiva, creareCarte("The Notebook", 550));
	afisareLista(stiva);

	//printf("\n\nCartile care au fost eliminate din stiva");
	//stiva = filtrareCarti(stiva, 270);
	//printf("\n\nStiva dupa ce a fost filtrata sa nu depaseasca 270 pagini");
	//afisareLista(stiva);

	nod* rezultat = filtrareCartiInCoada(&stiva, 270);
	printf("\n\nCartile care au fost eliminate din stiva si introduse in coada");
	afisareLista(rezultat);
	printf("\n\nStiva dupa ce a fost filtrata sa nu depaseasca 270 pagini");
	afisareLista(stiva);

	stiva = stergereLista(stiva);
	rezultat = stergereLista(rezultat);
}


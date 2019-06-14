#include <stdio.h>
#include <string.h>
#include <malloc.h>

struct Ferma {
	char* nume;
	int nrAnimale;
};

struct nod {
	Ferma info;
	nod* next;
};

Ferma creareFerma(const char* nume, int nrAnimale) {
	Ferma f;
	f.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(f.nume, nume);
	f.nrAnimale = nrAnimale;

	return f;
}

nod* creareNod(Ferma f, nod* urm) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = f;
	nou->next = urm;

	return nou;
}

nod* inserareInceput(nod* cap, Ferma f) {
	nod* nou = creareNod(f, cap);
	return nou;
}

nod* inserareSfarsit(Ferma f, nod* cap) {
	nod* nou = creareNod(f, NULL);
	if (cap) { //avem lista
		nod* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {//nu avem lista
		cap = nou;
	}
	return cap;
}

void afisareFerma(Ferma f) {
	printf("\nFerma %s are %d animale.", f.nume, f.nrAnimale);
}

void afisareLista(nod* cap) {
	while (cap) {
		afisareFerma(cap->info);
		cap = cap->next;
	}
}

float medieAnimale(nod* cap) {
	if (cap) {
		int nrAnimale = 0;
		int suma = 0;
		while (cap) {
			nrAnimale++;
			suma += cap->info.nrAnimale;
			cap = cap->next;
		}
		return suma / nrAnimale;
	}
	else
		return 0;
}

//inserare a.i fermele sa fie sortate crescator dupa nr animalelor
nod* inserare_crescatoare(nod* cap, Ferma info) {
	nod* nou = creareNod(info, NULL);
	if (cap) {
		if (cap->info.nrAnimale > info.nrAnimale) { //comparam cu primul nod
			cap = inserareInceput(cap, info); //inseram la inceput, inaintea primului nod
			return cap;
		}
		else {
			nod* temp = cap; //ia valoarea primului nod
			while (temp->next && temp->next->info.nrAnimale < info.nrAnimale) { //compara urm noduri cu valoarea de introdus
				temp = temp->next; //pana ii gaseste locul
			}
			nou->next = temp->next; //inseram nodul
			temp->next = nou;
			return cap;
		}
	}
	else { //nu exista lista
		return nou;
	}
}

nod* stergereElement(nod* cap, int nrAnimale) {
	if (cap) {
		if (cap->info.nrAnimale == nrAnimale) { //daca val cautata e pe primul nod
			nod* temp = cap;
			cap = cap->next;
			free(temp->info.nume);
			free(temp);
		}
		else { //nu e prima pozitie
			nod* temp = cap;
			while (temp->next && temp->next->info.nrAnimale != nrAnimale) {
				temp = temp->next; //cautam elem ala
			}
			if (temp->next) { //am gasit valoarea
				nod* aux = temp->next;
				temp->next = temp->next->next;
				free(aux->info.nume);
				free(aux);
			}
		}
	}
	return cap;
}

nod* cautareValoare(nod* cap, int nrAnimale) {
	while (cap != NULL && cap->info.nrAnimale != nrAnimale) {
		cap = cap->next;
	}
	if (cap->info.nrAnimale==nrAnimale)
		return cap;
	else return NULL;
}

nod* stergereLista(nod* cap) {
	while (cap) {
		nod* aux = cap;
		cap = cap->next;
		free(aux->info.nume);
		free(aux);
	}
	return cap;
}

void main() {
	nod* cap = NULL;
	cap = inserare_crescatoare(cap, creareFerma("Cocosul vesel", 200));
	cap = inserare_crescatoare(cap, creareFerma("Ferma", 100));
	cap = inserare_crescatoare(cap, creareFerma("Avicola", 400));
	cap = inserare_crescatoare(cap, creareFerma("Crevedia", 1200));

	afisareLista(cap);

	printf("\n\n");
	printf("*******Stergere ferma cu 400 de animale*************");
	cap = stergereElement(cap, 400);
	printf("\n\n");
	printf("*******Dupa stergere lista este:*************");
	afisareLista(cap);
	printf("\n\n");
	printf("*******Se cauta ferma cu 200 de animale*************");
	nod* cautat = cautareValoare(cap, 200);
	afisareFerma(cautat->info);
	free(cautat->info.nume);
	free(cautat);

	cap = stergereLista(cap);

}
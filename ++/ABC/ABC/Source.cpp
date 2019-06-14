#include <stdio.h>
#include <malloc.h>
#include <string.h>

//mereu la functiile de lucru cu arbori trebuie sa verificam daca avem radacina

struct Avion {
	char* model;
	int nrLocuri; //asta o sa fie cheia
	int nrLocuriOcupate;
	float* preturiBilete;//are lungimea = nrLocuriOcupate
};

//functie de creat avioane
Avion initAvion(const char* model, int nrLocuri, int nrLocuriOcupate, float* preturiBilete) {
	Avion a;
	a.model = (char*)malloc(sizeof(char)*(strlen(model) + 1));
	strcpy(a.model, model);
	a.nrLocuri = nrLocuri;
	a.nrLocuriOcupate = nrLocuriOcupate;
	a.preturiBilete = (float*)malloc(sizeof(float)*nrLocuriOcupate);
	for (int i = 0; i < nrLocuriOcupate; i++) {
		a.preturiBilete[i] = preturiBilete[i];
	}

	return a;
}

void afisareAvion(Avion avion) {
	printf("\nAvionul %s are %d locuri, dar au fost ocupate doar %d: ", avion.model, avion.nrLocuri, avion.nrLocuriOcupate);
	for (int i = 0; i < avion.nrLocuriOcupate; i++) {
		printf(" %5.2f", avion.preturiBilete[i]);
	}
}

//functie de citit din fisier
Avion citesteAvionDinFisier(FILE* f) {
	Avion a;
	//citesc in buffer pentru ca nu stiu cat spatiu trebuie sa aloc
	char buffer[20];
	fscanf(f, "%s", buffer);
	a.model = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(a.model, buffer);
	fscanf(f, "%d", &a.nrLocuri);
	fscanf(f, "%d", &a.nrLocuriOcupate);
	a.preturiBilete = (float*)malloc(sizeof(float)*a.nrLocuriOcupate);
	for (int i = 0; i < a.nrLocuriOcupate; i++) {
		fscanf(f, "%f", &a.preturiBilete[i]); //fscanf citeste pana la spatiu
	}

	return a;
}

struct nod {
	Avion info;
	nod* st;
	nod* dr;
};

nod* inserareAvionInArbore(nod* rad, Avion a) {
	if (rad) { //verific daca am arbore - asta e si conditia de oprire din recursivitate
		if (rad->info.nrLocuri > a.nrLocuri) {
			rad->st = inserareAvionInArbore(rad->st, a);
		}
		else {
			rad->dr = inserareAvionInArbore(rad->dr, a);
		}
		return rad; //prin asta refac legaturile pana la inserarea respectiva
	}
	else { //nu avem arbore
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->info = a;//shallow copy
		nou->dr = nou->st = NULL; //=>nod frunza
		return nou;
	}
}

//=> elementele sunt afisate crescator
void afisareArboreInordine(nod* rad) {
	if (rad) {
		afisareArboreInordine(rad->st);
		afisareAvion(rad->info);
		afisareArboreInordine(rad->dr);
	}
}

//cautare avion, doar returneaza, nu si extrage
Avion cautareAvionInArboreDupaNrLocuri(nod* rad, int nrLocuri) {
	if (rad) {
		if (rad->info.nrLocuri == nrLocuri) {
			Avion a = initAvion(rad->info.model, rad->info.nrLocuri, rad->info.nrLocuriOcupate, rad->info.preturiBilete);
			return a;
		}
		else if (rad->info.nrLocuri > nrLocuri) {
			return cautareAvionInArboreDupaNrLocuri(rad->st, nrLocuri);
		}
		else {
			return cautareAvionInArboreDupaNrLocuri(rad->dr, nrLocuri);
		}
	}
	else {
		return initAvion("", -1, -1, NULL);
	}
}

void main() {
	nod* rad = NULL; //nu avem niciun nod -> arbore gol
	FILE* fisier = fopen("avioane.txt", "r");
	if (fisier) {
		int nrAvioane = 0;
		fscanf(fisier, "%d", &nrAvioane);
		for (int i = 0; i < nrAvioane; i++) {
			rad = inserareAvionInArbore(rad, citesteAvionDinFisier(fisier));
		}
	}
	fclose(fisier);
	afisareArboreInordine(rad);

	printf("\n***************\n");
	Avion avionCautat = cautareAvionInArboreDupaNrLocuri(rad, 30);
	printf("Avion cautat:\n");
	afisareAvion(avionCautat);
	//pentru ca am facut deep copy
	free(avionCautat.model);
	free(avionCautat.preturiBilete);
}
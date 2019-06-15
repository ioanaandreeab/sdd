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

//functie pentru stergerea completa a arborelui - utilizeaza parcurgerea in postordine
nod* stergereArboreBinarDeCautare(nod* rad) {
	if (rad) {
		stergereArboreBinarDeCautare(rad->st);
		stergereArboreBinarDeCautare(rad->dr);
		free(rad->info.model);
		free(rad->info.preturiBilete);
		free(rad);
		rad = NULL;
		return rad;
	}
}

//functie pt determinarea nivelelor arborelui
int nivele(nod *rad) {
	if (rad) {
		int niveleS = nivele(rad->st);
		int niveleD = nivele(rad->dr);
		return 1 + (niveleS > niveleD ? niveleS : niveleD);
	}
	else
		return 0;
}

//afisare pe un nivel dat
void afisareNivel(nod* rad, int nivelDorit, int nivelCurent) {
	if (rad) {
		if (nivelDorit == nivelCurent) {
			afisareAvion(rad->info);
		}
		else {
			afisareNivel(rad->st, nivelDorit, nivelCurent + 1);
			afisareNivel(rad->dr, nivelDorit, nivelCurent + 1);
		}
	}
}

nod* stergereABC(nod* rad, int nrLocuriCautat) {
	if (rad) {
		if (rad->info.nrLocuri > nrLocuriCautat)
			rad->st = stergereABC(rad->st, nrLocuriCautat);
		else
			if (rad->info.nrLocuri < nrLocuriCautat) {
				rad->dr = stergereABC(rad->dr, nrLocuriCautat);
			}
			else {
				//am gasit nodul de sters
				if (rad->st == NULL) { //nu are descendent in stanga
					nod* tmp = rad->dr; //poate fi null sau nu, adica poate fi frunza sau nu
					//dezaloc nodul curent
					free(rad->info.model);
					free(rad->info.preturiBilete);
					free(rad);

					rad = tmp;//refac legatura
				}
				else
					if (rad->dr == NULL) { //nu are descendent in dreapta
						nod* tmp = rad->st; //poate fi null sau nu, adica poate fi frunza sau nu
						//dezaloc nodul curent
						free(rad->info.model);
						free(rad->info.preturiBilete);
						free(rad);

						rad = tmp; //refac legatura
					}
					else {
						//cazul cu 2 descendenti
						nod* tmp = rad->st;
						while (tmp->dr)
							tmp = tmp->dr; //cautarea celui mai din dreapta nod din subarborele stang al lui rad
						//tmp e nodul cu cheia maxima din subarborele stang
						//dezalocare
						free(rad->info.model);
						free(rad->info.preturiBilete);
						//mutarea datelor din tmp in rad
						rad->info.model = (char*)malloc(sizeof(char)*(strlen(tmp->info.model) + 1));
						strcpy(rad->info.model, tmp->info.model);
						rad->info.nrLocuri = tmp->info.nrLocuri;
						rad->info.nrLocuriOcupate = tmp->info.nrLocuriOcupate;
						rad->info.preturiBilete = (float*)malloc(sizeof(float)*rad->info.nrLocuriOcupate);
						for (int i = 0; i < rad->info.nrLocuriOcupate; i++) {
							rad->info.preturiBilete[i] = tmp->info.preturiBilete[i];
						}
						//sterg ce aveam in tmp(recursiv)
						rad->st = stergereABC(rad->st, tmp->info.nrLocuri);
					}
			}
	}
	return rad;
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
	printf("\n****************");
	int niveleArbore = nivele(rad);
	printf("\nNumarul de nivele din arbore este: %d", niveleArbore);

	printf("\n***************");
	printf("\nPe nivelul 2 se afla:\n");
	afisareNivel(rad, 2, 1);

	printf("\n***************");
	printf("\nStergerea nodului radacina");
	rad = stergereABC(rad, 30);
	printf("\nDupa stergere:");
	afisareArboreInordine(rad);
	stergereArboreBinarDeCautare(rad);
	printf("\nArborele a fost sters.");
}
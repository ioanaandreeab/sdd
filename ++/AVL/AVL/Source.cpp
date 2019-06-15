#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Avion {
	char* model;
	int nrLocuri; //asta o sa fie cheia 
	int nrLocuriOcupate;
	float* preturiBilete; //are lungimea = nrLocuriOcupate
};

//functie de creat avioane
Avion initAvion(const char* model, int nrLocuri, int nrLocuriOcupate, float* preturiBilete) {
	Avion avion;
	avion.model = (char*)malloc(sizeof(char)*(strlen(model) + 1));
	strcpy(avion.model, model);
	avion.nrLocuri = nrLocuri;
	avion.nrLocuriOcupate = nrLocuriOcupate;
	avion.preturiBilete = (float*)malloc(sizeof(float)*nrLocuriOcupate);
	for (int i = 0; i < nrLocuriOcupate; i++) {
		avion.preturiBilete[i] = preturiBilete[i];
	}

	return avion;
}

void afisareAvion(Avion avion) {
	printf("Avionul %s are %d locuri, dar au fost ocupate doar %d: ", avion.model, avion.nrLocuri, avion.nrLocuriOcupate);
	for (int i = 0; i < avion.nrLocuriOcupate; i++) {
		printf("%5.2f, ", avion.preturiBilete[i]);
	}
	printf("\n");
}

//functie de citit din fisier
Avion citesteAvionDinFisier(FILE* f) {
	Avion a;
	//nu stiu cat spatiu trb sa aloc si imi declar un buffer
	char buffer[20];
	fscanf(f, "%s", buffer);
	a.model = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(a.model, buffer);
	fscanf(f, "%d", &a.nrLocuri);
	fscanf(f, "%d", &a.nrLocuriOcupate);
	a.preturiBilete = (float*)malloc(sizeof(float)*a.nrLocuriOcupate);
	for (int i = 0; i < a.nrLocuriOcupate; i++) {
		fscanf(f, "%f", &a.preturiBilete[i]);//fscanf citeste pana la spatiu
	}

	return a;
}

struct nod {
	Avion info;
	nod* st;
	nod* dr;
};

nod* inserareAvionInArbore(Avion a, nod* rad) {
	//verificam daca avem unde sa inseram
	if (rad) {
		if (rad->info.nrLocuri > a.nrLocuri) {
			rad->st = inserareAvionInArbore(a, rad->st);
		}
		else {
			rad->dr = inserareAvionInArbore(a, rad->dr);
		}
		return rad;
	}
	else {
		//nu avem nod deci il cream
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->info = a; //shallow copy
		nou->dr = NULL;//pentru ca e un nod
		nou->st = NULL;//frunza
		return nou;
	}
}

//rotirea la dreapta - returneaza noua radacina
nod* rotireLaDreapta(nod* rad) {
	if (rad) {
		nod* aux = rad->st; //nodul pe care il ridic - adica noua radacina
		rad->st = aux->dr; //nodul pe care o sa-l ia radacina
		aux->dr = rad; //vechea radacina va fi subarborele dr al noii radacini
		return aux; //noua radacina
	}
	else {
		return rad;
	}
}

//rotire la stanga
nod* rotireLaStanga(nod* rad) {
	if (rad) {
		nod* aux = rad->dr;//nodul pe care il ridic - sigur am ceva in dreapta
		rad->dr = aux->st;//nodul pe care il ia radacina
		aux->st = rad; //vechea radacina va fi subarborele st al noii radacini
		return aux;//noua radacina
	}
	else {
		return rad;
	}
}

//functie pentru determinarea inaltimii arborelui
int inaltimeArbore(nod* rad) {
	if (rad) {
		int inaltimeSt = inaltimeArbore(rad->st);
		int inaltimeDr = inaltimeArbore(rad->dr);
		return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
	}
	else {
		return 0;
	}
}

int calculareGradEchilibru(nod* rad) {
	if (rad) {
		int inaltimeSt = inaltimeArbore(rad->st);
		int inaltimeDr = inaltimeArbore(rad->dr);
		return inaltimeSt - inaltimeDr;
	}
	else {
		return 0;
	}
}

nod* inserareAvionInArboreAVL(Avion a, nod* rad) {
	//verificam daca avem unde sa inseram
	if (rad) {
		if (rad->info.nrLocuri > a.nrLocuri) {
			rad->st = inserareAvionInArboreAVL(a, rad->st);
		}
		else {
			rad->dr = inserareAvionInArboreAVL(a, rad->dr);
		}
		//inainte de a returna arborele trb sa verificam daca acesta a fost dezechilibrat

		if (calculareGradEchilibru(rad) == 2) {//dezechilibru pe stanga
			 //calculam si gradul de echilibru pt subarborele st al rad curente
			if (calculareGradEchilibru(rad->st) != 1) {
				//nu e dezechilibru total -> e dezechilibru in stanga si un mic dezechilibru pe dreapta
				//in subarborele st care prezinta dezechilibru dreapta rotim la stanga (cum am face pentru un dezechilibru total dreapta)
				rad->st = rotireLaStanga(rad->st);
			}
			//daca avem dezechilibru total (ge(rad)=2 & ge(rad->st)=1) sau daca nu e total si am aplicat rotirea pentru a corecta dezechilibrul dreapta
			//rotim la dreapta
			rad = rotireLaDreapta(rad);
		}
		if (calculareGradEchilibru(rad) == -2) {//dezechilibru pe dreapta
			//calculeaza si gradul de echilibru pt subarborele dreapta
			if (calculareGradEchilibru(rad->dr) != -1) {
				//cazul usor
				rad->dr = rotireLaDreapta(rad->dr);
			}
			rad = rotireLaStanga(rad);
		}
		return rad;
	}
	else {
		//nu avem nod deci il cream
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->info = a; //shallow copy
		nou->dr = NULL;//pentru ca e un nod
		nou->st = NULL;//frunza
		return nou;
	}
}

//afisare in inordine => elementele vor fi afisate crescator
void afisareArbore(nod* rad) {
	if (rad) {
		afisareArbore(rad->st);
		afisareAvion(rad->info);
		afisareArbore(rad->dr);
	}
}

//cautare avion, doar returneaza, nu si extrage
Avion cautareAvionInArboreDupaNrLocuri(nod* rad, int nrLocuri) {
	if (rad) {
		if (rad->info.nrLocuri == nrLocuri) {
			Avion a = initAvion(rad->info.model, rad->info.nrLocuri, rad->info.nrLocuriOcupate, rad->info.preturiBilete);//deep copy
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

//functie pentru stergerea completa a arborelui
nod* stergereAVL(nod* rad) {
	if (rad) {
		stergereAVL(rad->st);
		stergereAVL(rad->dr);
		free(rad->info.model);
		free(rad->info.preturiBilete);
		free(rad);
		rad = NULL;
		return rad;
	}
}

nod* stergereNodAVL(nod* rad, int nrLocuriCautat) {
	if (rad) {
		if (rad->info.nrLocuri > nrLocuriCautat)
			rad->st = stergereNodAVL(rad->st, nrLocuriCautat);
		else
			if (rad->info.nrLocuri < nrLocuriCautat) {
				rad->dr = stergereNodAVL(rad->dr, nrLocuriCautat);
			}
			else {
				// am gasit nodul de sters
				if (rad->st == NULL) { //nu are descendent in stanga
					nod* tmp = rad->dr; // poate fi NULL sau nu, adica poate fi frunza sau nu
					// dezaloc nodul curent rad
					free(rad->info.model);
					free(rad->info.preturiBilete);
					free(rad);

					rad = tmp; //refac legatura
				}
				else {
					if (rad->dr == NULL) { //nu exista descendent pe dreapta
						// exista un sg descendent pe stanga nodului curent rad
						nod* tmp = rad->st;

						// dezalocare nod curent r
						free(rad->info.model);
						free(rad->info.preturiBilete);
						free(rad);
						rad = tmp; //refac legatura
					}
					else {
						// cazul cu 2 descendenti
						nod* tmp = rad->st;
						while (tmp->dr)
							tmp = tmp->dr; // cautarea celui mai din dreapta nod din subarborele stanga al lui rad
						// tmp este nodul cu cheia maxima din subarborele stanga
						// dezalocare nume student pentru nodul de sters
						free(rad->info.model);
						free(rad->info.preturiBilete);
						// mutarea datelor din tmp in rad
						rad->info.model = (char*)malloc(sizeof(char)*(strlen(tmp->info.model) + 1));
						strcpy(rad->info.model, tmp->info.model);
						rad->info.nrLocuri = tmp->info.nrLocuri;
						rad->info.nrLocuriOcupate = tmp->info.nrLocuriOcupate;
						rad->info.preturiBilete = (float*)malloc(sizeof(float)*rad->info.nrLocuriOcupate);
						for (int i = 0; i < rad->info.nrLocuriOcupate; i++) {
							rad->info.preturiBilete[i] = tmp->info.preturiBilete[i];
						}

						rad->st = stergereNodAVL(rad->st, tmp->info.nrLocuri);

					}
				}
			}
	}
	//dupa ce stergem verificam daca trebuie sa facem rotiri
	if (rad) {
		if (calculareGradEchilibru(rad) == 2) {//dezechilibru pe stanga
			 //calculam si gradul de echilibru pt subarborele st al rad curente
			if (calculareGradEchilibru(rad->st) != 1) {
				//nu e dezechilibru total -> e dezechilibru in stanga si un mic dezechilibru pe dreapta
				//in subarborele st care prezinta dezechilibru dreapta rotim la stanga (cum am face pentru un dezechilibru total dreapta)
				rad->st = rotireLaStanga(rad->st);
			}
			//daca avem dezechilibru total (ge(rad)=2 & ge(rad->st)=1) sau daca nu e total si am aplicat rotirea pentru a corecta dezechilibrul dreapta
			//rotim la dreapta
			rad = rotireLaDreapta(rad);
		}
		if (calculareGradEchilibru(rad) == -2) {//dezechilibru pe dreapta
			//calculeaza si gradul de echilibru pt subarborele dreapta
			if (calculareGradEchilibru(rad->dr) != -1) {
				//cazul usor
				rad->dr = rotireLaDreapta(rad->dr);
			}
			rad = rotireLaStanga(rad);
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
			rad = inserareAvionInArboreAVL(citesteAvionDinFisier(fisier), rad);
		}
	}
	fclose(fisier);
	afisareArbore(rad);

	printf("\n***************\n");
	Avion avionCautat = cautareAvionInArboreDupaNrLocuri(rad, 30);
	printf("Avion cautat:\n");
	afisareAvion(avionCautat);
	//pentru ca am facut deep copy
	free(avionCautat.model);
	free(avionCautat.preturiBilete);
	printf("\n**************\n");
	printf("Inaltime arbore: %d\n", inaltimeArbore(rad));
	rad = stergereNodAVL(rad, 30);
	rad = stergereNodAVL(rad, 40);
	rad = stergereNodAVL(rad, 45);
	afisareArbore(rad);
	rad = stergereAVL(rad);


}

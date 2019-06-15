#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Comanda {
	int id;
	char* denumire;
};

//aici doar anunt
struct nodListaPrincipala;
//pentru nodurile adiacente - nu salvam toata info, doar adresa nodurilor din lsita principala pentru ca altfel as fi ocupat mai multa memorie
struct nodListaSimpla {
	nodListaPrincipala* info;
	nodListaSimpla* next;
};

struct nodListaPrincipala {
	Comanda info; //informatia
	nodListaPrincipala* next; //urmatorul nod
	nodListaSimpla* adiacente; //nodurile adiacente coresp nodului curent
};

nodListaSimpla* inserareListaSimpla(nodListaSimpla* cap, nodListaPrincipala* nod) {
	nodListaSimpla* nou = (nodListaSimpla*)malloc(sizeof(nodListaSimpla));
	nou->info = nod; //shallow copy
	nou->next = NULL;

	if (cap) {
		nodListaSimpla* temp = cap;
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

nodListaPrincipala* inserareListaPrincipala(nodListaPrincipala* cap, Comanda c) {
	nodListaPrincipala* nou = (nodListaPrincipala*)malloc(sizeof(nodListaPrincipala));
	nou->adiacente = NULL; //cand introduc un nod nu stiu ce noduri adiacente are
	nou->info = c;
	nou->next = NULL; //pentru ca fac inserare la sf

	if (cap) {
		nodListaPrincipala* temp = cap;
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

void afisareListaSimpla(nodListaSimpla* cap) {
	while (cap) {
		printf("      %d. %s", cap->info->info.id, cap->info->info.denumire);
		cap = cap->next;
	}
}

void afisareGraf(nodListaPrincipala* graf) {
	while (graf) {
		printf("\n%d. are nodurile adiacente: ", graf->info.id);
		afisareListaSimpla(graf->adiacente);
		graf = graf->next;
	}
}

nodListaPrincipala* cautareDupaId(int id, nodListaPrincipala* cap) {
	while (cap&&cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}

void adaugareArcInGrafNeorientat(int de_la, int pana_la, nodListaPrincipala* graf) {
	//trebuie sa cautam de_la si pana_la
	nodListaPrincipala* DE = cautareDupaId(de_la, graf);
	nodListaPrincipala* PANA = cautareDupaId(pana_la, graf);
	//si inserez in lista simpla si trebuie sa ma asigur ca legatura e facuta in ambele liste de noduri adiacente pentru ca am legatura si intre x si y si intre y si x
	if (DE && PANA) {
		DE->adiacente = inserareListaSimpla(DE->adiacente, PANA);
		PANA->adiacente = inserareListaSimpla(PANA->adiacente, DE);
	}
}

//parcurgere in adancime
//stiva - structura auxiliara
struct nodStiva {
	int id;
	nodStiva* next;
};

nodStiva* push(nodStiva* cap, int id) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->id = id;
	nou->next = NULL; //inserare la sf
	if (cap) {
		nodStiva* temp = cap;
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

int pop(nodStiva* &cap) {
	if (cap) {
		nodStiva* temp = cap;
		while (temp->next && temp->next->next){ //trb sa-l pastrez si pe penultimul
			temp = temp->next;
		}
		if (temp->next) {
			int rezultat = temp->next->id;
			//stergem nodul
			free(temp->next);
			temp->next = NULL;
			return rezultat;
		}
		else {
			int rezultat = temp->id; //deoarece temp->next nu exista, adica am un singur nod in stiva
			free(temp);
			cap = NULL;
			return rezultat;
		}
	}
	else {
		return -1; //stiva goala
	}
}

int calculareNrNoduri(nodListaPrincipala* cap) {
	int nr = 0;
	while (cap) {
		nr++;
		cap = cap->next;
	}
	return nr;
}

//parcurgerea in adancime a grafului
void parcurgereInAdancime(int nodStart, nodListaPrincipala* graf) {
	//1. ne luam o stiva
	//2. adaugam nodul de start in stiva
	//3. ne luam un vector de lungimea egala cu nr de noduri, il umplem cu val de 0, atunci cand un nod e modificat valoarea coresp va deveni 1
	//4. vizitam nodul de start
	//5. extragem elem din stiva
	//6. il afisam/procesam
	//7. adaugam in stiva nodurile adiacente nevizitate
	//8. ne intoarcem la pasul 5
	//9. facem bucla 5-8 pana nu mai avem noduri in lista

	nodStiva* stiva = NULL;
	stiva = push(stiva, nodStart);
	int nrNoduri = calculareNrNoduri(graf);
	int* vizitate = (int*)malloc(sizeof(int)* nrNoduri);
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}
	vizitate[nodStart] = 1;

	while (stiva) {
		int nodCurent = pop(stiva);
		nodListaPrincipala* nodC = cautareDupaId(nodCurent, graf);
		printf("\n%. %s", nodC->info.id, nodC->info.denumire);

		nodListaSimpla* temp = nodC->adiacente;
		while (temp) {
			if (vizitate[temp->info->info.id] == 0) { //daca e nevizitat
				stiva = push(stiva, temp->info->info.id);
				vizitate[temp->info->info.id] = 1;
			}
			temp = temp->next;
		}
	}
}

void main() {
	nodListaPrincipala* graf = NULL;
	printf("Cate noduri avem?");
	int nrNoduri = 0;
	scanf("%d", &nrNoduri);

	//citesc de la tastatura info, id-ul il setez eu si preiau doar denumirea
	for (int i = 0; i < nrNoduri; i++) {
		Comanda c;
		c.id = i;
		char buffer[20];
		printf("Denumirea comenzii:");
		scanf("%s", buffer);
		c.denumire = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
		strcpy(c.denumire, buffer);
		graf = inserareListaPrincipala(graf, c);
	}
	
	//intreb utilizatorul cate arce are graful
	printf("Cate arce avem?");
	int nrArce = 0;
	scanf("%d", &nrArce);

	for (int i = 0; i < nrArce; i++) {
		printf("De la: ");
		int de_la = 0;
		scanf("%d", &de_la);
		printf("Pana la: ");
		int pana_la = 0;
		scanf("%d", &pana_la);

		adaugareArcInGrafNeorientat(de_la, pana_la, graf);
	}

	printf("\nGraful este: ");
	afisareGraf(graf);
	printf("\nParcurgere in adancime: ");
	parcurgereInAdancime(0, graf);
}
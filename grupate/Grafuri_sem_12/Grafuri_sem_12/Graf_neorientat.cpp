#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string>

//GRAFURI

struct Comanda {
	int id;
	char* denumire;
};

//aici doar il anunt
struct nodListaPrincipala;
//pentru nodurile adiacente - nu salvam toata informatia, doar adresa nodurilor din lista principala pentru ca altfel as fi ocupat mai multa memorie
struct nodListaSimpla {
	nodListaPrincipala* info;
	nodListaSimpla* next;
};

struct nodListaPrincipala {
	Comanda info;
	nodListaPrincipala* next;
	nodListaSimpla* adiacente;
};

nodListaSimpla* inserareListaSimpla(nodListaSimpla* cap, nodListaPrincipala* nod) {
	nodListaSimpla* nodNou = (nodListaSimpla*)malloc(sizeof(nodListaSimpla));
	nodNou->next = NULL;
	nodNou->info = nod; //shallow copy

	if (cap) {
		nodListaSimpla* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nodNou;
	}
	
	else {
		cap = nodNou;
	}

	return cap;
}

nodListaPrincipala* inserareListaPrincipala(nodListaPrincipala* cap, Comanda c) {
	nodListaPrincipala* nodNou = (nodListaPrincipala*)malloc(sizeof(nodListaPrincipala));
	nodNou->adiacente = NULL; //nu stiu inca ce noduri adiacente are
	nodNou->info = c;
	nodNou->next = NULL; //pentru ca fac inserare la sfarsit

	if (cap) {
		nodListaPrincipala* p = cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nodNou;
	}
	else {
		cap = nodNou;
	}

	return cap;
}

nodListaPrincipala* cautareDupaId(int id, nodListaPrincipala* cap) {
	while (cap && cap->info.id != id) {
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

void afisareListaSimpla(nodListaSimpla* cap) {
	while (cap) {
		printf("      %d. %s", cap->info->info.id, cap->info->info.denumire);
		cap = cap->next;
	}
}

void afisareGraf(nodListaPrincipala* graf) {
	while (graf) {
		printf("\n%d. %s are nodurile adiacente: ", graf->info.id, graf->info.denumire);
		afisareListaSimpla(graf->adiacente);
		graf = graf->next;
	}
}

//parcurgere in adancime
//trebuie sa facem o stiva
struct nodStiva {
	int id;
	nodStiva* next;
};

nodStiva* push(nodStiva* cap, int id) {
	nodStiva* nodNou = (nodStiva*)malloc(sizeof(nodStiva));
	nodNou->id = id;
	nodNou->next = NULL; //la final
	if (cap) {
		nodStiva* p = cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nodNou;
	}
	else {
		cap = nodNou;
	}
	return cap;
}

int pop (nodStiva* &cap) {
	if (cap) {
		nodStiva* p = cap;
		while (p->next && p->next->next) { //trb sa-l pastrez si pe penultimul
			p = p->next;
		}
		if (p->next) {
			int rezultat = p->next->id;
			//stergem nodul
			free(p->next); //mereu inainte de return pentru ca dupa return nu se mai executa nimic
			p->next = NULL;
			return rezultat;
		}
		else {
			int rezultat = p->id;//deoarece p->next nu exista
			free(p);
			cap = NULL;
			return rezultat;
		}
	}
	else {
		return -1; //nu exista id-ul
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
	//3. ne luam un vector de lungimea egala cu nr de noduri, il facem tot 0 si atunci cand vizitam nodul schimbam valoarea coresp nodului in 1
	//4. vizitam nodul de start
	//5. extragem element din stiva
	//6. il afisam/procesam
	//7. adaugam in stiva nodurile adiacente nevizitate
	//8. ne intoarcem la pasul 5
	//9. facem bucla 5-8 pana cand nu mai avem noduri in lista

	nodStiva* stiva = NULL;
	stiva = push(stiva, nodStart);
	int nrNoduri = calculareNrNoduri(graf);
	int* vizitate = (int*)malloc(sizeof(int)*nrNoduri);
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}
	vizitate[nodStart] = 1;
	while (stiva) {
		int nodCurent = pop(stiva);
		nodListaPrincipala* nodC = cautareDupaId(nodCurent, graf);
		printf("\n%d. %s", nodC->info.id, nodC->info.denumire);

		nodListaSimpla* temp = nodC->adiacente;
		while (temp) {
			if (vizitate[temp->info->info.id] == 0) { //daca este nevizitat 
				stiva = push(stiva, temp->info->info.id);
				vizitate[temp->info->info.id] = 1;
			}
			temp = temp->next;
		}
	}
}

void main() {

	nodListaPrincipala* graf = NULL;
	printf("Cate noduri avem? ");
	int nrNoduri = 0;
	scanf("%d", &nrNoduri);
	
	//citesc de la tastatura informatia, id-ul il setez eu si preiau doar denumirea
	for (int i = 0; i < nrNoduri; i++) {
		Comanda c;
		c.id = i;
		char buffer[20];
		printf("Denumirea comenzii: ");
		scanf("%s", buffer);
		c.denumire = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
		strcpy(c.denumire, buffer);
		graf = inserareListaPrincipala(graf, c);
	}
	
	//intrebam utilizatorul cate arce are
	printf("Cate arce avem? ");
	int nrArce = 0;
	scanf("%d", &nrArce);

	for (int i = 0; i < nrArce; i++) {
		//citesc cele doua noduri adica de la nodul x->pana la nodul y
		printf("De la: ");
		int de_la = 0;
		scanf("%d", &de_la);
		printf("Pana la: ");
		int pana_la = 0;
		scanf("%d", &pana_la);

		adaugareArcInGrafNeorientat(de_la, pana_la, graf);
	}
	
	printf("\nGraful este:");
	afisareGraf(graf);
	printf("\nParcurgere in adancime:");
	parcurgereInAdancime(0, graf);
	system("pause");
}
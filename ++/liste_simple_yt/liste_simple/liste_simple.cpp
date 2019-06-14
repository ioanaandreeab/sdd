#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Muzeu {
	char* denumire;
	float pret;
	int nrVizitatori;
};

struct nod {
	Muzeu info;
	nod* next; //adresa urmatorului nod
};

void afisareMuzeu(Muzeu muzeu) {
	printf("\nMuzeul %s are %d vizitatori, iar biletul costa %5.2f lei", muzeu.denumire, muzeu.nrVizitatori, muzeu.pret);
}

Muzeu citireMuzeu() {
	Muzeu muzeu;
	//citim in buffer ca sa aflam dimensiunea a ce am citit ca sa putem sa alocam memorie cum trb
	char buffer[20];
	printf("Nume muzeu: ");
	scanf("%s", buffer);
	muzeu.denumire = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(muzeu.denumire, buffer);
	printf("Pret bilet intrare: ");
	scanf("%f", &muzeu.pret);
	printf("Nr vizitatori: ");
	scanf("%d", &muzeu.nrVizitatori);

	return muzeu;
}

nod* creareNod(Muzeu m, nod* urmator) {
	nod* nou;
	nou = (nod*)malloc(sizeof(nod));
	nou->info = m; //shallow copy daca ar fi doar asta; pt primitive se copiaza continutul direct
	nou->info.denumire = (char*)malloc(sizeof(char)*(strlen(m.denumire) + 1)); //astea doua pt deep copy
	strcpy(nou->info.denumire, m.denumire);
	nou->next = urmator;

	return nou;
}

nod* inserareSfarsit(nod* cap, Muzeu m) {
	if (cap) {
		nod* p = cap;
		while (p->next) { //ma opresc pe ultimul nod
			p = p->next;
		}
		//aici sunt pe ultimul nod
		p->next = creareNod(m, NULL);
		return cap;
	}
	else { //nu exista lista
		return creareNod(m, NULL);
	}
}

void afisareLista(nod* cap) {
	while (cap) {
		afisareMuzeu(cap->info);
		cap = cap->next;
	}
}

nod* stergereLista(nod* cap) {
	while (cap) {
		free(cap->info.denumire);
		nod* temp = cap; //ramane la nodul pe care vreau sa-l sterg
		cap = cap->next;
		free(temp);
	}
	return NULL; //sau cap, pentru ca si cap o sa fie null
}

void main() {
	nod* lista = NULL;//o lista goala
	//lista = creareNod(citireMuzeu(), lista); //aici lista e null
	//lista = creareNod(citireMuzeu(), lista); //aici insereaza la inceput
	//lista = creareNod(citireMuzeu(), lista); //si aici insereaza la inceput
	//
	//afisareLista(lista);

	////dezalocare lista
	//lista = stergereLista(lista);
	lista = inserareSfarsit(lista, citireMuzeu());
	lista = inserareSfarsit(lista, citireMuzeu());
	lista = inserareSfarsit(lista, citireMuzeu());
	afisareLista(lista);
	lista = stergereLista(lista);
}
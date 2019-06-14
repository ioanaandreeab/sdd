#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

struct Muzeu {
	char* denumire;
	float pret;
	int nrVizitatori;
};

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

void afisareMuzeu(Muzeu muzeu) {
	printf("\nMuzeul %s are %d vizitatori, iar biletul costa %5.2f lei", muzeu.denumire, muzeu.nrVizitatori, muzeu.pret);
}

void main() {
	//declar pointer 
	Muzeu* pVectorMuzee = NULL;
	int nrMuzee;
	//citesc de la tastatura
	printf("Numar muzee: ");
	scanf("%d", &nrMuzee);
	pVectorMuzee = (Muzeu*)malloc(sizeof(Muzeu)*nrMuzee);
	//citesc elem
	for (int i = 0; i < nrMuzee; i++) {
		pVectorMuzee[i] = citireMuzeu();
	}

	for (int i = 0; i < nrMuzee; i++) {
		afisareMuzeu(pVectorMuzee[i]);
	}

	//dezaloc memoria
	for (int i = 0; i < nrMuzee; i++) {
		free(pVectorMuzee[i].denumire);
	}
	free(pVectorMuzee);



	system("pause");
}
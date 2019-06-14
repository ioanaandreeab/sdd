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

//primesc pointeri
Muzeu** citireMatrice(int *n, int *m) {
	printf("Numarul de linii: ");
	//nu mai pun cu & deoarece n e deja o adresa
	scanf("%d", n);
	printf("Numarul de coloane: ");
	scanf("%d", m);
	Muzeu** matrice;
	//n e o adresa si eu trb sa folosesc ce e la adresa respectiva deci referentiez cu * pe n
	matrice = (Muzeu**)malloc(sizeof(Muzeu*)*(*n));
	for (int i = 0; i < *n; i++) {
		matrice[i] = (Muzeu*)malloc(sizeof(Muzeu)*(*m));
	}

	for (int i = 0; i < *n; i++) {
		for (int j = 0; j < *m; j++) {
			matrice[i][j] = citireMuzeu();
		}
	}

	return matrice;
}

void afisareMuzeu(Muzeu muzeu) {
	printf("\nMuzeul %s are %d vizitatori, iar biletul costa %5.2f lei", muzeu.denumire, muzeu.nrVizitatori, muzeu.pret);
}

void main() {
	//declar un vector de pointeri;la fiecare pointer de acolo am cate un vector de elemente de tip muzeu
	Muzeu** matrice;
	int nrLinii;
	int nrColoane;

	//transmit cei doi param prin adresa deoarece vreau sa se modifice
	matrice = citireMatrice(&nrLinii, &nrColoane);


	for (int i = 0; i < nrLinii; i++) {
		for (int j = 0; j < nrColoane; j++) {
			afisareMuzeu(matrice[i][j]);
		}
		printf("\n");
	}

	//dezaloc spatiul
	for (int i = 0; i < nrLinii; i++) {
		for (int j = 0; j < nrColoane; j++) {
			//dezaloc numele
			free(matrice[i][j].denumire);
		}
		//pt fiecare linie dezaloc pointerul unde tineam linia
		free(matrice[i]);
	}
	free(matrice);
	system("pause");
}
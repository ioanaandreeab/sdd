#include <stdio.h>
#include <malloc.h>
#include <string.h>


struct Rezervare {
	int idRezervare;
	char* numeClient;
	char* nrTelefon;
	int canal; //0 - telefon, 1 - aplicatie
	int durata; //asta e pt prioritate
};

struct HEAP {
	int dim;
	Rezervare* vector;
};

Rezervare initRezervare(int idRezervare, const char* numeClient, const char* nrTelefon, int canal, int durata) {
	Rezervare r;
	r.idRezervare = idRezervare;
	r.numeClient = (char*)malloc(sizeof(char)*(strlen(numeClient) + 1));
	strcpy(r.numeClient, numeClient);
	r.nrTelefon = (char*)malloc(sizeof(char)*(strlen(nrTelefon) + 1));
	strcpy(r.nrTelefon, nrTelefon);
	r.canal = canal;
	r.durata = durata;

	return r;
}

void afisareRezervare(Rezervare r) {
	printf("\nRezervarea %d, facuta de %s - %s, prin intermediul canalului %d, are durata de %d ore", r.idRezervare, r.numeClient, r.nrTelefon, r.canal, r.durata);
}

void traversareCoada(HEAP h) {
	if (h.dim > 0) {
		for (int i = 0; i < h.dim; i++) {
			afisareRezervare(h.vector[i]);
		}
	}
}

void filtrare(HEAP h, int pozitie) {
	int fiuS = 2 * pozitie + 1;
	int fiuD = 2 * pozitie + 2;
	int max = pozitie;

	if (fiuS < h.dim && h.vector[max].durata < h.vector[fiuS].durata) {
		max = fiuS;
	}
	if (fiuD < h.dim && h.vector[max].durata < h.vector[fiuD].durata) {
		max = fiuD;
	}
	if (max != pozitie) {
		//e nevoie de interschimb
		Rezervare aux = h.vector[max];
		h.vector[max] = h.vector[pozitie];
		h.vector[pozitie] = aux;
		if (2 * max + 1 < h.dim) {//daca noul maxim mai are descendenti
			filtrare(h, max); //se aplica filtrarea din nou
		}
	}
}

void inserare(HEAP &h, Rezervare r) {
	if (h.dim > 0) {
		//aloc un vector cu un spatiu in plus
		Rezervare* temp = (Rezervare*)malloc(sizeof(Rezervare)*(h.dim + 1));
		//copiez in vectorul temporar ce am in vectorul din heap
		for (int i = 0; i < h.dim; i++) {
			temp[i] = h.vector[i];
		}
		temp[h.dim] = r;
		free(h.vector);
		h.vector = temp;
		h.dim++;
		for (int i = (h.dim - 2) / 2; i >= 0; i--) {
			filtrare(h, i);
		}
	}
}

void extragere(HEAP &h) {
	if (h.dim > 0) {
		//luam un vector care are cu un elem mai putin
		Rezervare* temp = (Rezervare*)malloc(sizeof(Rezervare)*(h.dim - 1));
		//copiem elem
		for (int i = 0; i < h.dim-1; i++) {
			temp[i] = h.vector[i + 1];
		}
		free(h.vector);
		h.vector = temp;
		h.dim--;
		for (int i = (h.dim - 2) / 2; i >= 0; i--) {
			filtrare(h, i);
		}
	}
}

Rezervare* rezervariPestePrag(HEAP h, int &dim) {
	if (h.dim > 0) {
		int nrRez = 0;
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i].durata > 3) {
				nrRez++;
			}
		}
		Rezervare* rezervari = (Rezervare*)malloc(sizeof(Rezervare)*nrRez);
		dim = nrRez;
		nrRez = 0;
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i].durata > 3) {
				rezervari[nrRez++] = h.vector[i];
			}
		}
		return rezervari;
	}
}

void main() {
	HEAP h;
	h.dim = 5;
	h.vector = (Rezervare*)malloc(sizeof(Rezervare)*h.dim);
	h.vector[0] = initRezervare(1, "Ioana", "0766723468", 1, 2);
	h.vector[1] = initRezervare(2, "Denisa", "0766723468", 1, 5);
	h.vector[2] = initRezervare(3, "Andrei", "0766723468", 0, 4);
	h.vector[3] = initRezervare(4, "Stefan", "0766723468", 1, 1);
	h.vector[4] = initRezervare(5, "Teo", "0766723468", 0, 3);

	for (int i = (h.dim - 2) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	traversareCoada(h);

	printf("\n\nDupa inserare:");
	inserare(h, initRezervare(6, "Cristian", "0723474758", 1, 6));
	traversareCoada(h);

	extragere(h);
	printf("\n\nDupa extragere:");
	traversareCoada(h);

	printf("\n\nRezervari ce au peste 3 ore:");
	int dim = 0;
	Rezervare* rezervari = rezervariPestePrag(h,dim);
	for (int i = 0; i < dim; i++) {
		afisareRezervare(rezervari[i]);
	}
}
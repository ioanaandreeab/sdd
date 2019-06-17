#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Imobil {
	unsigned int idImobil; //cheie
	char* numeStrada;
	unsigned int nrStrada;
	unsigned int nrApartamente;
	int* nrLocatari;
	float valTotalaIntretinere;
};

Imobil initImobil(unsigned int idImobil, const char* numeStrada, unsigned int nrStrada, unsigned int nrApartamente, int* nrLocatari, float valTotalaIntretinere) {
	Imobil i;
	i.idImobil = idImobil;
	i.numeStrada = (char*)malloc(sizeof(char)*(strlen(numeStrada) + 1));
	strcpy(i.numeStrada, numeStrada);
	i.nrStrada = nrStrada;
	i.nrApartamente = nrApartamente;
	i.nrLocatari = (int*)malloc(sizeof(int)*nrApartamente);
	for (int j = 0; j < nrApartamente; j++) {
		i.nrLocatari[j] = nrLocatari[j];
	}
	i.valTotalaIntretinere = valTotalaIntretinere;

	return i;
}

void afisareImobil(Imobil imobil) {
	printf("\nImobilul %d de pe %s, nr. %d, are %d apartamente, la care locuiesc: ", imobil.idImobil, imobil.numeStrada, imobil.nrStrada, imobil.nrApartamente);
	for (int i = 0; i < imobil.nrApartamente; i++) {
		printf("%d ", imobil.nrLocatari[i]);
	}
	printf(" persoane, cu suma totala de plata de %5.2f lei", imobil.valTotalaIntretinere);
}

struct nodArbore {
	nodArbore* st;
	nodArbore* dr;
	Imobil info;
};

int calculInaltime(nodArbore* rad) {
	if (rad) {
		int inaltimeS = calculInaltime(rad->st);
		int inaltimeD = calculInaltime(rad->dr);
		
		return 1 + (inaltimeS > inaltimeD ? inaltimeS : inaltimeD);
	}
	else {
		return 0;
	}
}

int gradEchilibru(nodArbore* rad) {
	if (rad) {
		return calculInaltime(rad->st) - calculInaltime(rad->dr);
	}
	else
		return 0;
}

nodArbore* rotireLaDreapta(nodArbore* rad) {
	if (rad) {
		nodArbore* aux = rad->st;
		rad->st = aux->dr;
		aux->dr = rad;
		return aux;
	}
	else {
		return rad;
	}
}

nodArbore* rotireLaStanga(nodArbore* rad) {
	if (rad) {
		nodArbore* aux = rad->dr;
		rad->dr = aux->st;
		aux->st = rad;
		return aux;
	}
	else {
		return rad;
	}
}

nodArbore* inserareAVL(nodArbore* rad, Imobil imobil) {
	if (rad) {
		if (rad->info.idImobil < imobil.idImobil) {
			rad->dr = inserareAVL(rad->dr, imobil);
		}
		else {
			rad->st = inserareAVL(rad->st, imobil);
		}
		
		//inainte de a returna, verificari
		if (gradEchilibru(rad) == 2) { //dezechilibru total pe partea stanga
			if (gradEchilibru(rad->st) != 1) { //rotire extra
				rad->st = rotireLaStanga(rad->st);
			}
			rad = rotireLaDreapta(rad);
		}
		if (gradEchilibru(rad) == -2) { //dezechilibru total pe partea dreapta
			if (gradEchilibru(rad->dr) != -1) {
				rad->dr = rotireLaDreapta(rad->dr);
			}
			rad= rotireLaStanga(rad);
		}
		return rad;
	}
	else {
		nodArbore* nou = (nodArbore*)malloc(sizeof(nodArbore));
		nou->dr = nou->st = NULL;
		nou->info = initImobil(imobil.idImobil, imobil.numeStrada, imobil.nrStrada, imobil.nrApartamente, imobil.nrLocatari, imobil.valTotalaIntretinere);
		return nou;
	}
}

void imobilePeStrada(nodArbore* rad, const char* strada, int &nrImobile) {
	if (rad) {
		if (strcmp(rad->info.numeStrada, strada) == 0) {
			nrImobile++;
		}
		imobilePeStrada(rad->st, strada, nrImobile);
		imobilePeStrada(rad->dr, strada, nrImobile);
	}
}

void traversareInordine(nodArbore* rad) {
	if (rad) {
		traversareInordine(rad->st);
		afisareImobil(rad->info);
		traversareInordine(rad->dr);
	}
}

float plataPersoana(nodArbore* rad, unsigned int idImobil) {
	if (rad) {
		if (rad->info.idImobil == idImobil) {
			int nrLocatariTotal = 0;
			for (int i = 0; i < rad->info.nrApartamente; i++) {
				nrLocatariTotal += rad->info.nrLocatari[i];
			}
			return rad->info.valTotalaIntretinere / nrLocatariTotal;
		}
		plataPersoana(rad->st, idImobil);
		plataPersoana(rad->dr, idImobil);
	}
}

void main() {
	nodArbore* rad = NULL;
	
	int* pers1 = (int*)malloc(sizeof(int) * 5);
	pers1[0] = 2;
	pers1[1] = 3;
	pers1[2] = 4;
	pers1[3] = 5;
	pers1[4] = 2;

	int* pers2 = (int*)malloc(sizeof(int) * 3);
	pers2[0] = 2;
	pers2[1] = 3;
	pers2[2] = 4;

	int* pers3 = (int*)malloc(sizeof(int) * 5);
	pers3[0] = 2;
	pers3[1] = 3;
	pers3[2] = 1;
	pers3[3] = 5;
	pers3[4] = 2;

	int* pers4 = (int*)malloc(sizeof(int) * 2);
	pers4[0] = 2;
	pers4[1] = 3;

	int* pers5 = (int*)malloc(sizeof(int) * 6);
	pers5[0] = 2;
	pers5[1] = 3;
	pers5[2] = 4;
	pers5[3] = 5;
	pers5[4] = 2;
	pers5[5] = 3;

	rad = inserareAVL(rad, initImobil(23, "Strada Fizicienilor", 18, 5, pers1, 2500));
	rad = inserareAVL(rad, initImobil(15, "Strada Panselutelor", 47, 3, pers2, 1700));
	rad = inserareAVL(rad, initImobil(68, "Strada Panselutelor", 20, 5,pers3, 9500));
	rad = inserareAVL(rad, initImobil(42, "Strada Plutonier", 8, 2, pers4, 5500));
	rad = inserareAVL(rad, initImobil(53, "Strada Garoafelor", 11, 6, pers5, 4000));

	traversareInordine(rad);

	int nrImobile = 0;
	imobilePeStrada(rad, "Strada Panselutelor", nrImobile);
	printf("\n\nPe strada data sunt %d imobie", nrImobile);

	float plata = plataPersoana(rad, 23);
	printf("\n\nPlata pentru o persoana din imobilul cerut este: %5.2f lei.", plata);


}
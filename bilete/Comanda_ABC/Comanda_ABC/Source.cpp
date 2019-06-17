#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Produs {
	char* denumireProdus;
	int cantitate;
	char* unitateMasura;
};

struct Comanda {
	int idComanda;
	char* denumireClient;
	int nrProduse;
	Produs* produse;
	char* dataLivrare;
};

struct nodArbore{
	nodArbore* st;
	nodArbore* dr;
	Comanda info;
};

Produs initProdus(const char* denumireProdus, int cantitate, const char* unitateMasura) {
	Produs p;
	p.denumireProdus = (char*)malloc(sizeof(char)*(strlen(denumireProdus) + 1));
	strcpy(p.denumireProdus, denumireProdus);
	p.cantitate = cantitate;
	p.unitateMasura = (char*)malloc(sizeof(char)*(strlen(unitateMasura) + 1));
	strcpy(p.unitateMasura, unitateMasura);

	return p;
}

void afisareProdus(Produs p) {
	printf("\nProdusul %s, cantitate %d %s", p.denumireProdus, p.cantitate, p.unitateMasura);
}

Comanda initComanda(int idComanda, const char* denumireClient, int nrProduse, Produs* produse, const char* dataLivrare) {
	Comanda c;
	c.idComanda = idComanda;
	c.denumireClient = (char*)malloc(sizeof(char)*(strlen(denumireClient) + 1));
	strcpy(c.denumireClient, denumireClient);
	c.nrProduse = nrProduse;
	c.produse = (Produs*)malloc(sizeof(Produs)*nrProduse);
	for (int i = 0; i < nrProduse; i++) {
		c.produse[i] = initProdus(produse[i].denumireProdus, produse[i].cantitate, produse[i].unitateMasura);
	}
	c.dataLivrare = (char*)malloc(sizeof(char)*(strlen(dataLivrare) + 1));
	strcpy(c.dataLivrare, dataLivrare);

	return c;
}

void afisareComanda(Comanda c) {
	printf("\nComanda cu id %d, plasata de %s va fi livrata pe %s si are %d produse: ", c.idComanda, c.denumireClient, c.dataLivrare, c.nrProduse);
	for (int i = 0; i < c.nrProduse; i++) {
		afisareProdus(c.produse[i]);
	}
	printf("\n");
}

nodArbore* inserareArbore(nodArbore* rad, Comanda c) {
	if (rad) {
		if (rad->info.idComanda > c.idComanda) {
			rad->st = inserareArbore(rad->st, c);
		}
		else {
			rad->dr = inserareArbore(rad->dr, c);
		}
		return rad;
	}
	else {
		nodArbore* nou = (nodArbore*)malloc(sizeof(nodArbore));
		nou->dr = nou->st = NULL;
		nou->info = c;
		return nou;
	}
}

void traversareInordine(nodArbore* rad) {
	if (rad) {
		traversareInordine(rad->st);
		afisareComanda(rad->info);
		traversareInordine(rad->dr);
	}
}

int* idDupaData(int* &vector, int &dim, nodArbore* rad, const char* data) {
	if (rad) {
		idDupaData(vector, dim, rad->st, data);
		if (strcmp(rad->info.dataLivrare, data) == 0) {
			int* provizoriu = (int*)malloc(sizeof(int)*(dim + 1));
			for (int i = 0; i < dim; i++) {
				provizoriu[i] = vector[i];
			}
			provizoriu[dim] = rad->info.idComanda;
			dim++;
			vector = provizoriu;
			return vector;
		}
		idDupaData(vector, dim, rad->dr, data);
	}
}

int nrFrunzeClient(nodArbore* rad, const char* numeClient, int &nrFrunze) {
	if (rad) {
		nrFrunzeClient(rad->st, numeClient, nrFrunze);
		if (strcmp(rad->info.denumireClient, numeClient) == 0 && rad->dr == NULL && rad->st == NULL) {
			nrFrunze++;
			return nrFrunze;
		}
		nrFrunzeClient(rad->dr, numeClient, nrFrunze);
	}
}

void modificaProduse(nodArbore* &rad, int idComanda, Produs* produse, int nrProduse) {
	if (rad) {
		if (rad->info.idComanda == idComanda) {
			free(rad->info.produse);
			rad->info.nrProduse = nrProduse;
			rad->info.produse = (Produs*)malloc(sizeof(Produs)*nrProduse);
			for (int i = 0; i < nrProduse; i++) {
				rad->info.produse[i] = initProdus(produse[i].denumireProdus, produse[i].cantitate, produse[i].unitateMasura);
			}
		}
		else if (rad->info.idComanda > idComanda) {
			modificaProduse(rad->st, idComanda, produse, nrProduse);
		}
		else {
			modificaProduse(rad->dr, idComanda, produse, nrProduse);
		}
	}
}

Comanda cautaComandaDupaId(nodArbore* rad, int idComanda) {
	if (rad) {
		if (rad->info.idComanda == idComanda) {
			return rad->info;
		}
		else if (rad->info.idComanda > idComanda) {
			return cautaComandaDupaId(rad->st, idComanda);
		}
		else {
			return cautaComandaDupaId(rad->dr, idComanda);
		}
	}
}

void main() {
	nodArbore* rad = NULL;
	Produs* produse1 = (Produs*)malloc(sizeof(Produs) * 2);
	Produs* produse2 = (Produs*)malloc(sizeof(Produs) * 3);
	Produs* produse3 = (Produs*)malloc(sizeof(Produs));
	Produs* produse4 = (Produs*)malloc(sizeof(Produs) * 4);
	Produs* produse5 = (Produs*)malloc(sizeof(Produs) * 2);
	produse1[0] = initProdus("Huawei P30", 1, "unitate");
	produse1[1] = initProdus("Arduino UNO", 2, "placi");

	produse2[0] = initProdus("Rosii", 6, "kg");
	produse2[1] = initProdus("Castraveti", 3, "kg");
	produse2[2] = initProdus("Vinete", 2, "kg");

	produse3[0] = initProdus("Masina", 1, "unitate");

	produse4[0] = initProdus("Cana", 2, "seturi");
	produse4[1] = initProdus("Farfurii", 3, "seturi");
	produse4[2] = initProdus("Linguri", 1, "seturi");
	produse4[3] = initProdus("Tigai", 5, "seturi");

	produse5[0] = initProdus("Apa", 20, "litri");
	produse5[1] = initProdus("Suc", 2, "litri");

	Comanda c1 = initComanda(25, "Ioana", 2, produse1, "25.02.2019");
	Comanda c2 = initComanda(41, "Denisa", 3, produse2, "12.03.2019");
	Comanda c3 = initComanda(12, "Teo", 1, produse3, "25.02.2019");
	Comanda c4 = initComanda(32, "Andrei", 4, produse4, "20.04.2019");
	Comanda c5 = initComanda(50, "Stefan", 2, produse5, "25.03.2019");

	rad = inserareArbore(rad, c1);
	rad = inserareArbore(rad, c2);
	rad = inserareArbore(rad, c3);
	rad = inserareArbore(rad, c4);
	rad = inserareArbore(rad, c5);

	traversareInordine(rad);
	int* vector;
	int dim = 0;
	int* rezultat = idDupaData(vector, dim, rad, "25.02.2019");
	printf("\n\nPe data de 25.02.2019 vor fi livrate comenzile: ");
	for (int i = 0; i < dim; i++) {
		printf("%d ", rezultat[i]);
	}

	int nrFrunze = 0;
	int rezultatFrunze = nrFrunzeClient(rad, "Andrei", nrFrunze);
	printf("\n\nClientul Andrei are %d noduri frunza cu comenzi plasate", nrFrunze);

	printf("\n\nComanda inainte de modificare:");
	Comanda c = cautaComandaDupaId(rad, 50);
	afisareComanda(c);

	printf("\n\nComanda dupa modificare:");
	modificaProduse(rad, 50, produse3, 1);
	c = cautaComandaDupaId(rad, 50);
	afisareComanda(c);
}
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Adresa {
	char* strada;
	int nr;
};

Adresa initAdresa(const char* strada, int nr) {
	Adresa a;
	a.strada = (char*)malloc(sizeof(char)*(strlen(strada) + 1));
	strcpy(a.strada, strada);
	a.nr = nr;

	return a;
}

void afisareAdresa(Adresa a) {
	printf(" strada %s, nr. %d", a.strada, a.nr);
}

struct CotaIntretinere {
	Adresa adresa;
	int nrApartament; //cheia pt HT
	int nrPersoane;
	int an;
	int luna;
	float intretinere;
};

CotaIntretinere initCota(Adresa adresa, int nrApartament, int nrPersoane, int an, int luna, float intretinere) {
	CotaIntretinere c;
	c.adresa = initAdresa(adresa.strada, adresa.nr);
	c.nrApartament = nrApartament;
	c.nrPersoane = nrPersoane;
	c.an = an;
	c.luna = luna;
	c.intretinere = intretinere;

	return c;
}

void afisareCotaIntretinere(CotaIntretinere c) {
	printf("\nApartamentul nr. %d, de la adresa:", c.nrApartament);
	afisareAdresa(c.adresa);
	printf(", in care locuiesc %d persoane are de plata pentru luna %d a anului %d %5.2f lei", c.nrPersoane, c.luna, c.an, c.intretinere);
}

struct nod {
	nod* next;
	CotaIntretinere info;
};

struct HT {
	int dim;
	nod* *vector;
};

HT initHT(int dim) {
	HT hash;
	hash.dim = dim;
	hash.vector = (nod**)malloc(sizeof(nod*)*dim);
	for (int i = 0; i < dim; i++) {
		hash.vector[i] = NULL;
	}

	return hash;
}

int hashFunction(int dim, int nrApartament) {
	return nrApartament % dim;
}

nod* inserareLaInceput(nod* cap, CotaIntretinere c) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = cap;
	nou->info = initCota(c.adresa,c.nrApartament,c.nrPersoane,c.an,c.luna,c.intretinere);

	return nou;
}

int inserareInHT(HT hashTable, CotaIntretinere c) {
	if (hashTable.dim > 0) {
		int hash = hashFunction(hashTable.dim, c.nrApartament);
		hashTable.vector[hash] = inserareLaInceput(hashTable.vector[hash], c);
		return hash;		
	}
	else {
		return -2; //nu exista tabela
	}
}

void afisareHT(HT hash) {
	if (hash.dim > 0) {
		for (int i = 0; i < hash.dim; i++) {
			if (hash.vector[i]) {
				nod* temp = hash.vector[i];
				while (temp) {
					afisareCotaIntretinere(temp->info);
					temp = temp->next;
				}
			}
		}
	}
}

float valoareAnualaDupaNr(HT h, int nrApartament, int an, Adresa adresa) {
	if (h.dim > 0) {
		int suma = 0;
		int pozitie = hashFunction(h.dim, nrApartament);
		if (h.vector[pozitie]) {
			nod* temp = h.vector[pozitie];
			while (temp) {
				if (temp->info.adresa.nr == adresa.nr && strcmp(temp->info.adresa.strada, adresa.strada) == 0 && temp->info.an == an) {
					suma += temp->info.intretinere * 12;
				}
				temp = temp->next;
			}
		}
		return suma;
	}
}

int cotePestePrag(HT h, float prag) {
	if (h.dim > 0) {
		int nrCote = 0;
		for (int i = 0; i < h.dim; i++) {
			nod* temp = h.vector[i];
			while (temp) {
				if (temp->info.intretinere > prag) {
					nrCote++;
				}
				temp = temp->next;
			}
		}
		return nrCote;
	}
}

void stergereDupaNrSiAdresa(HT &h, int nrApartament, Adresa a) {
	if (h.dim > 0) {
		int hash = hashFunction(h.dim, nrApartament);
		if (h.vector[hash]) {
			nod* cap = h.vector[hash];
			//daca e primul nod
			if (cap->info.adresa.nr == a.nr && strcmp(cap->info.adresa.strada, a.strada) == 0) {
				nod* temp = cap;
				cap = cap->next;
				free(temp->info.adresa.strada);
				free(temp);
			}
			else {
				nod* temp = cap;
				while (temp->next && temp->next->info.adresa.nr != a.nr && strcmp(temp->next->info.adresa.strada, a.strada) != 0) {
					temp = temp->next;
				}
				if (temp->next) {//am gasit valoarea
					nod* aux = temp->next;
					temp->next = temp->next->next;
					free(aux->info.adresa.strada);
					free(aux);
				}
			}
		}
	}
}

void main() {
	HT hashTable = initHT(5);
	inserareInHT(hashTable, initCota(initAdresa("Fizicienilor", 18), 27, 3, 2019, 6, 250.50));
	inserareInHT(hashTable, initCota(initAdresa("Panselutelor", 54), 24, 1, 2019, 5, 100.00));
	inserareInHT(hashTable, initCota(initAdresa("Mihai Bravu", 12), 30, 5, 2019, 2, 750.50));
	inserareInHT(hashTable, initCota(initAdresa("Dorobanti", 21), 183, 4, 2019, 6, 1250.50));
	inserareInHT(hashTable, initCota(initAdresa("Craiul", 18), 10, 3, 2019, 6, 250.50));
	
	afisareHT(hashTable);

	float dePlata = valoareAnualaDupaNr(hashTable, 27, 2019, initAdresa("Fizicienilor", 18));
	printf("\n\nApartamentul 27 are de plata intr-un an suma de %5.2f lei", dePlata);

	int nrCote = cotePestePrag(hashTable, 500);
	printf("\n\nNr cotelor care depasesc 500 de lei este de %d.",nrCote);

	stergereDupaNrSiAdresa(hashTable, 30, initAdresa("Mihai Bravu", 12));
	printf("\n\nHT dupa stergere:");
	afisareHT(hashTable);
}
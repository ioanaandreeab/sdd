#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Cofetarie {
	char* nume;
	int nrPrajituri;
};

struct HashTable {
	Cofetarie* *vector;
	int dim;
};

Cofetarie initCofetarie(const char* nume, int nrPrajituri) {
	Cofetarie c;
	c.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(c.nume, nume);
	c.nrPrajituri = nrPrajituri;

	return c;
}

void afisareCofetarie(Cofetarie c) {
	printf("\nCofetaria %s are %d prajituri", c.nume, c.nrPrajituri);
}

HashTable initHashTable(int dim) {
	HashTable h;
	h.dim = dim;
	h.vector = (Cofetarie**)malloc(sizeof(Cofetarie*)*dim);
	for (int i = 0; i < dim; i++) {
		h.vector[i] = NULL;
	}
	return h;
}

void afisareTabela(HashTable h) {
	for (int i = 0; i < h.dim; i++) {
		if (h.vector[i]) {
			afisareCofetarie(*(h.vector[i]));
		}
	}
}

int hashFunction(Cofetarie c, HashTable h) {
	int suma = 0;
	for (int i = 0; i < strlen(c.nume); i++) {
		suma += c.nume[i];
	}
	return suma % h.dim;
}

int inserareCofetarie(HashTable h, Cofetarie c) {
	if (h.dim > 0) {//daca exista ht 
		int hashCode = hashFunction(c, h);
		if (h.vector[hashCode]) { //avem coliziune, adica exista deja elem pe pozitia det de hashfunction
			int index = (hashCode + 1) % h.dim;//calculeaza un nou hash code
			while (index != hashCode) { //daca avem un hashcode diferit fata de prima val obtinuta prin hashfunction
				if (h.vector[index] == NULL) {//daca nu e nimic la indexul calculat
					h.vector[index] = (Cofetarie*)malloc(sizeof(Cofetarie));
					*(h.vector[index]) = initCofetarie(c.nume, c.nrPrajituri);
				}
				else {
					index = (index + 1) % h.dim;//calculez din nou hashcode
				}
			}
			if (index == hashCode) {
				return -1; //codul de eroare pt tabela full
			}
			return index;
		}
		else {//n-avem coliziune
			h.vector[hashCode] = (Cofetarie*)malloc(sizeof(Cofetarie));
			*(h.vector[hashCode]) = initCofetarie(c.nume, c.nrPrajituri);
			return hashCode;
		}
	}
	else {
		return -2; //nu exista tabela
	}
}
 
HashTable stergereTabela(HashTable h) {
	for (int i = 0; i < h.dim; i++) {
		if (h.vector[i]) {
			free(h.vector[i]->nume);
			free(h.vector[i]);
		}
	}
	free(h.vector);
	h.vector = NULL;
	return h;
}

Cofetarie stergereByIndex(HashTable h, int index) {
	if (h.dim > 0 && h.vector[index] != NULL) {
		Cofetarie rezultat = *(h.vector[index]);
		free(h.vector[index]);
		h.vector[index] = NULL;
		return rezultat;
	}
	else {
		return initCofetarie("", 0);
	}
}

int cautareCofetarie(HashTable h, Cofetarie c) {
	if (h.dim > 0) {
		int pozitie = hashFunction(c, h);
		if (h.vector[pozitie] && strcmp(c.nume, h.vector[pozitie]->nume) == 0) {//h.vector[pozitie] inseamna ca am gasit ceva pe pozitia aia si pt ca strcmp da 0 inseamna ca e fix ce caut
			return pozitie;
		}
		else {
			int index = (pozitie + 1) % h.dim;
			while (index != pozitie) {
				if (h.vector[index] && strcmp(c.nume, h.vector[index]->nume) == 0) {
					return index;
				}
				index = (index + 1) % h.dim;
			}
		}
	}
	else return -2;
}


void main() {
	HashTable h = initHashTable(5);
	Cofetarie c = initCofetarie("Ana", 6);
	inserareCofetarie(h, c);
	Cofetarie d = initCofetarie("Iepurasul", 8);
	inserareCofetarie(h, d);
	Cofetarie e = initCofetarie("Alice", 9);
	inserareCofetarie(h, e);
	Cofetarie f = initCofetarie("Paris", 5);
	inserareCofetarie(h, f);
	Cofetarie g = initCofetarie("Cofetarie", 3);
	inserareCofetarie(h, g);

	afisareTabela(h);

	printf("\n\nCofetaria cautata este: ");
	int pozitie = cautareCofetarie(h, e);
	afisareCofetarie(*(h.vector[pozitie]));

	printf("\n\nCofetaria stearsa este: ");
	Cofetarie stearsa = stergereByIndex(h, 2);
	afisareCofetarie(stearsa);

	printf("\n\nDupa stergerea cofetariei tabela este: ");
	afisareTabela(h);

	h = stergereTabela(h);
}
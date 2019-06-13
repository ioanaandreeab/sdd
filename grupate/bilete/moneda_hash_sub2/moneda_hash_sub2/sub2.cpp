#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct Moneda {
	char cod[3];
	char* denumire;
	char* emitent;
	int sold_valutar;
};

struct HashTable {
	Moneda* *vector;
	int dim;
};

struct nod {
	Moneda info;
	nod* next;
};

HashTable initHashTable(int dim) {
	HashTable ht;
	ht.dim = dim;
	ht.vector = (Moneda**)malloc(sizeof(Moneda*)*dim);
	for (int i = 0; i < dim; i++)
	{
		ht.vector[i] = NULL;
	}
	
	return ht;
}

int hashFunction(HashTable ht, Moneda m) {
	int suma = 0;
	for (int i = 0; i < 3; i++)
	{
		suma += m.cod[i];
	}
	return suma % ht.dim;
}

Moneda creareMoneda(const char cod[3], const char* denumire, const char* emitent, int sold) {
	Moneda m;
	strcpy(m.cod, cod);
	m.denumire = (char*)malloc(sizeof(char)*(strlen(denumire) + 1));
	strcpy(m.denumire, denumire);
	m.emitent = (char*)malloc(sizeof(char)*strlen(emitent) + 1);
	strcpy(m.emitent, emitent);
	m.sold_valutar = sold;

	return m;
}

int inserareHash(HashTable ht, Moneda m) {
	if (ht.dim > 0) {
		int hashCode = hashFunction(ht, m);
		if (ht.vector[hashCode]) { //avem coliziune
			int index = (hashCode + 1) % ht.dim;
			while (index != hashCode) {
				if (ht.vector[index] == NULL) {
					ht.vector[index] = (Moneda*)malloc(sizeof(Moneda));
					*(ht.vector[index]) = creareMoneda(m.cod, m.denumire, m.emitent, m.sold_valutar);
				}
				else {
					index = (index + 1) % ht.dim;
				}
			}
			if (index == hashCode) {
				return -1; //nu mai exista loc in tabela
			}
			return index;
		}
		else {
			ht.vector[hashCode] = (Moneda*)malloc(sizeof(Moneda));
			*(ht.vector[hashCode]) = creareMoneda(m.cod, m.denumire, m.emitent, m.sold_valutar);
			return hashCode;
		}

	}
	else {
		return -2; //nu exista tabela
	}
}

void afisareMoneda(Moneda m) {
	printf("\nMoneda %s (%s) e emisa de %s si are soldul de %d", m.denumire, m.cod, m.emitent, m.sold_valutar);
}

void afisareTabela(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i]) {
			afisareMoneda(*(ht.vector[i]));
		}
	}
}

void afisareLista(nod* cap) {
	while (cap) {
		afisareMoneda(cap->info);
		cap = cap->next;
	}
}

nod* inserareInceput(nod* cap, Moneda m) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareMoneda(m.cod, m.denumire, m.emitent, m.sold_valutar);
	nou->next = cap;

	return nou;
}

nod* cautareMonede(HashTable ht, int val) {
	nod* rezultat = NULL; //lista in care imi tin monezile mai mari decat soldul
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i]->sold_valutar > val) {
			rezultat = inserareInceput(rezultat, *(ht.vector[i]));
		}
	}
	return rezultat;
}

HashTable modificareSold(HashTable ht, int sold_initial, int sold_final) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i]->sold_valutar == sold_initial)
			ht.vector[i]->sold_valutar = sold_final;
	}
	return ht;
}

HashTable stergereTabela(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i]) {
			free(ht.vector[i]->denumire);
			free(ht.vector[i]->emitent);
			free(ht.vector[i]);
		}
	}
	free(ht.vector);
	ht.vector = NULL;
	return ht;
}

void main() {
	HashTable ht = initHashTable(5);
	inserareHash(ht, creareMoneda("RON", "Leu romanesc", "BNR", 500000));
	inserareHash(ht, creareMoneda("ERO", "Euro", "BE", 15500000));
	inserareHash(ht, creareMoneda("USD", "Dolar american", "BA", 5500000));
	inserareHash(ht, creareMoneda("DLN", "Dolar canadian", "BC", 2500000));
	inserareHash(ht, creareMoneda("GLS", "Lira sterlina", "BGB", 1400000));

	afisareTabela(ht);
	
	printf("\n***********************");
	printf("\nMonezile cu sold mai mare de 500000 unitati sunt:");
	nod* rezultat;
	rezultat = cautareMonede(ht, 500000);
	afisareLista(rezultat);

	printf("\n***********************");
	printf("\nDupa modificarea stocului pentru RON, tabela este:");
	ht = modificareSold(ht, 500000, 300000);
	afisareTabela(ht);
	ht = stergereTabela(ht);


	system("pause");
}
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct Rezervare {
	unsigned int id_rezervare;
	char* denumire_hotel;
	unsigned char nr_camere;
	char* nume_client;
	char* perioada;
	float suma_plata;
};

struct HashTable {
	Rezervare* *vector;
	int dim;
};

Rezervare creareRezervare(unsigned int id, const char* nume_hotel, unsigned char camere, const char* client, const char* perioada, float plata) {
	Rezervare r;
	r.id_rezervare = id;
	r.denumire_hotel = (char*)malloc(sizeof(char)*(strlen(nume_hotel) + 1));
	strcpy(r.denumire_hotel, nume_hotel);
	r.nr_camere = camere;
	r.nume_client = (char*)malloc(sizeof(char)*(strlen(client) + 1));
	strcpy(r.nume_client, client);
	r.perioada = (char*)malloc(sizeof(char)*(strlen(perioada) + 1));
	strcpy(r.perioada, perioada);
	r.suma_plata = plata;

	return r;
}

HashTable creareHT(int dim) {
	HashTable ht;
	ht.dim = dim;
	ht.vector = (Rezervare**)malloc(sizeof(Rezervare*)*dim);
	for (int i = 0; i < dim; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int hashFunction(HashTable h, unsigned int id) {
	return id%h.dim;
}

int inserareInHT(HashTable h, Rezervare r) {
	if (h.dim > 0) {
		int hashCode = hashFunction(h, r.id_rezervare);
		if (h.vector[hashCode]) { //avem coliziune
			int index = (hashCode + 1) % h.dim;
			while (index != hashCode) {
				if (h.vector[index] == NULL) {
					h.vector[index] = (Rezervare*)malloc(sizeof(Rezervare)*h.dim);
					*(h.vector[index]) = creareRezervare(r.id_rezervare, r.denumire_hotel, r.nr_camere, r.nume_client, r.perioada, r.suma_plata);
				}
				else {
					index = (index + 1) % h.dim;
				}
			}
			if (index == hashCode) {
				return -1; //nu mai exista loc in tabela
			}
			return index;
		}
		else {
			h.vector[hashCode] = (Rezervare*)malloc(sizeof(Rezervare)*h.dim);
			*(h.vector[hashCode]) = creareRezervare(r.id_rezervare, r.denumire_hotel, r.nr_camere, r.nume_client, r.perioada, r.suma_plata);
			return hashCode;
		}

	}
	return -2; //nu exista tabela
}

void afisareRezervare(Rezervare r) {
	printf("\nRezervarea cu id-ul %d, la hotelul %s, pentru %d camere, a fost facuta de %s in perioada %s si are val de %5.2f", r.id_rezervare, r.denumire_hotel, r.nr_camere, r.nume_client, r.perioada, r.suma_plata);
}

void afisareHT(HashTable h) {
	for (int i = 0; i < h.dim; i++) {
		if (h.vector[i])
			afisareRezervare(*(h.vector[i]));
	}
}

int cautareRezervare(HashTable h, unsigned int id) {
	if (h.dim > 0) {
		int hashCode = hashFunction(h, id);
		if (h.vector[hashCode]&& (id ==h.vector[hashCode]->id_rezervare)) {
			return hashCode;
		}
		else {
			int index = (hashCode + 1) % h.dim;
			while (index != hashCode) {
				if (h.vector[index] && (id == h.vector[index]->id_rezervare)) {
					return index;
				}
				index = (index + 1) % h.dim;
			}
		}
	}
	else return -2;//nu exista tabela
}

HashTable modificarePerioada(HashTable h, unsigned int id_rez, const char* perioada) {
	int pozitie = cautareRezervare(h, id_rez);
	if (h.vector[pozitie]) {
		strcpy(h.vector[pozitie]->perioada, (char*)perioada);
		afisareRezervare(*(h.vector[pozitie]));
	}
	return h;
}

HashTable stergereTabela(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i]) {
			free(ht.vector[i]->denumire_hotel);
			free(ht.vector[i]->nume_client);
			free(ht.vector[i]->perioada);
			free(ht.vector[i]);
		}
	}
	free(ht.vector);
	ht.vector = NULL;
	return ht;
}

void main() {
	HashTable ht = creareHT(5);
	inserareInHT(ht, creareRezervare(1, "Splendid", 2, "Ioana Berdei", "05.02-06.02", 230));
	inserareInHT(ht, creareRezervare(2, "Floarea", 5, "Denisa Calota", "08.04-12.04", 1000));
	inserareInHT(ht, creareRezervare(3, "Fantana", 3, "Andrei Baicu", "08.04-12.04", 500));
	inserareInHT(ht, creareRezervare(4, "Gratios", 4, "Stefan Lazea", "04.02-07.02", 650));
	inserareInHT(ht, creareRezervare(5, "Wow", 1, "Cristian Puican", "21.02-24.02", 300));
	afisareHT(ht);

	printf("\n************");
	//int indexCautat = cautareRezervare(ht, 5);
	//afisareRezervare(*(ht.vector[indexCautat]));
	
	ht = modificarePerioada(ht, 5, "09.04-15.04");
	//afisareRezervare(*(ht.vector[indexModificat]));

	//afisareHT(ht);
	ht = stergereTabela(ht);

	system("pause");
}
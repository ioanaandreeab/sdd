#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct Factura {
	char* serie_factura;
	char data_emiterii[11];
	char denumire_client[20];
	float val_factura;
};

struct nod {
	Factura info;
	nod* next;
};

Factura creareFactura(const char* serie, const char data[], const char client[], float valoare) {
	Factura f;
	f.serie_factura = (char*)malloc(sizeof(char)*(strlen(serie) + 1));
	strcpy(f.serie_factura, serie);
	strcpy(f.data_emiterii, data);
	strcpy(f.denumire_client, client);
	f.val_factura = valoare;

	return f;
}

nod* inserareInceput(nod* cap, Factura f) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareFactura(f.serie_factura, f.data_emiterii, f.denumire_client, f.val_factura);
	nou->next = cap;

	return nou;
}

void afisareFactura(Factura f) {
	printf("\nFactura cu nr si seria %s, emisa pe data de %s de catre %s are o valoare de %5.2f lei.", f.serie_factura,f.data_emiterii,f.denumire_client,f.val_factura);
}

void afisareLista(nod* cap) {
	while (cap) {
		afisareFactura(cap->info);
		cap = cap->next;
	}
}

int facturiEmise(nod* cap, const char data[]) {
	int count = 0;
	nod* thank_u = cap;
	while (thank_u) {
		if (strcmp(thank_u->info.data_emiterii, data) == 0) {
			afisareFactura(thank_u->info);
			count++;
		}
		thank_u = thank_u->next;
	}
	return count;
}

nod* facturiDupaClient(nod* cap, const char client[]) {
	nod* rezultat = NULL;
	nod* temp = cap;
	while (temp) {
		if (strcmp(temp->info.denumire_client, client) == 0) {
			rezultat = inserareInceput(rezultat, temp->info);
		}
		temp = temp->next;
	}
	return rezultat;
}

nod* stergereLista(nod* cap) {
	while (cap) {
		nod* temp = cap;
		cap = cap->next;
		free(temp->info.serie_factura);
		free(temp);
	}
	return cap;
}

void main() {
	nod* lista = NULL;
	lista = inserareInceput(lista, creareFactura("RX 4485414", "25.02.2018", "EMAG SRL", 100000));
	lista = inserareInceput(lista, creareFactura("RM 4485014", "28.02.2018", "Dona SA", 100500));
	lista = inserareInceput(lista, creareFactura("MX 4444414", "27.02.2018", "Elefant SRL", 500000));
	lista = inserareInceput(lista, creareFactura("RA 4489414", "22.02.2018", "EMAG SRL", 178000));
	lista = inserareInceput(lista, creareFactura("RU 4458814", "25.02.2018", "Jeffree Star SRL", 80000000));

	afisareLista(lista);
	printf("\n**********************");

	printf("\nFacturile de pe data de 25.02.2018 sunt:");
	int facturi = facturiEmise(lista, "25.02.2018");

	printf("\n**********************");
	printf("\nFacturile emise de EMAG SRL sunt: ");
	nod* rezultat = facturiDupaClient(lista, "EMAG SRL");
	afisareLista(rezultat);
	lista = stergereLista(lista);
	rezultat = stergereLista(rezultat);
	system("pause");
}
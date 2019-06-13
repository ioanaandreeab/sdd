#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct Monitor {
	int cod;
	char* producator;
	float diagonala;
	int nrPorturi;
};

struct nod {
	nod* next;
	Monitor info;
};

void afisareMonitor(Monitor m) {
	printf("\nMonitorul cu codul %d, produs de %s, cu diagonala %f are %d porturi\n", m.cod, m.producator, m.diagonala, m.nrPorturi);
}

void afisareLista(nod* cap) {
	if (cap) {
		nod* temp = cap->next;
		afisareMonitor(cap->info);
		while (temp != cap) {
			afisareMonitor(temp->info);
			temp = temp->next;
		}
	}
}

Monitor creareMonitor(int cod, const char* prod, float diag, int porturi) {
	Monitor m;
	m.cod = cod;
	m.producator = (char*)malloc(sizeof(char)*(strlen(prod) + 1));
	strcpy(m.producator, prod);
	m.diagonala = diag;
	m.nrPorturi = porturi;

	return m;
}

nod* inserareNod(nod* cap, Monitor m) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareMonitor(m.cod, m.producator, m.diagonala, m.nrPorturi);
	if (cap) {
		nod* temp = cap;
		while (temp->next != cap) {
			temp = temp->next;
		}
		nou->next = cap;
		temp->next = nou;
	}
	else {
		nou->next = nou;
		cap = nou;
	}
	return cap;
}

nod* stergereLista(nod* cap) {
	if (cap) {
		nod* temp = cap->next;
		while (temp->next != cap) {
			free(temp->info.producator);
			nod*aux = temp;
			temp = temp->next;
			free(aux);
		}
		free(temp->info.producator);
		free(temp);
		cap = NULL;
	}
	return cap;
}

void stergereNod(nod* pred) {
	nod* victima = pred->next;
	pred->next = pred->next->next;

	free(victima->info.producator);
	free(victima);
}

void extragereSiStergereNod(nod* &cap, int cod) {
	if (cap) {
		nod* temp = cap;
		if (cap->info.cod == cod) {
			if (cap->next != cap) { //cat timp nu sunt la ultimul nod
				while (temp->next != cap) 
				{
					temp = temp->next; //parcurg lista
				} //iese din while la ultimul nod
				afisareMonitor(cap->info); //afisez nodul curent pe care il sterg
				nod* aux = cap;
				free(cap->info.producator);
				temp->next = cap->next; //a ajuns la final si modifica next-ul pentru ultimul element
				cap = cap->next; //noul cap de lista, adica nodul al doilea
				free(aux);
			}
		}
		else {
			while (temp->next != cap && temp->next !=NULL && temp->next->info.cod != cod)
				temp = temp->next;
			if (temp->next != cap && temp->next != NULL)
				afisareMonitor(temp->next->info);
				stergereNod(temp);
		}
	}
}

void filtrareMonitoare(nod*&cap, float diag) {
	if (cap)
	{
		nod* temp = cap;
		while (temp->next != cap) {
			if (temp->next->info.diagonala > diag) //tre sa te opresti inainte de nodul cautat gen
			{
				if (temp == cap)
				{
					while (temp->next != cap) {
						temp = temp->next;
					}
					nod* aux = cap;
					afisareMonitor(cap->info);
					free(cap->info.producator);
					temp->next = cap->next;
					cap = cap->next;
					free(aux);
					temp = cap;
				}
				else
				{
					
					nod* deSters = temp->next;
					afisareMonitor(deSters->info);
					stergereNod(temp);
				}
			}
			else {
				temp = temp->next;
			}
		}
		if (temp->next == cap && temp->next->info.diagonala > diag)
		{
			afisareMonitor(temp->next->info);
			free(temp->next->info.producator);
			free(temp);
			cap = NULL;
		}	
	}
}

void main() {
	nod* lista = NULL;
	lista = inserareNod(lista, creareMonitor(1, "LG", 15.6, 4));
	lista = inserareNod(lista, creareMonitor(2, "Panasonic", 16, 5));
	lista = inserareNod(lista, creareMonitor(3, "Siemens", 15.8, 2));
	lista = inserareNod(lista, creareMonitor(4, "Samsung", 19, 6));
	lista = inserareNod(lista, creareMonitor(5, "Apple", 17, 3));
	lista = inserareNod(lista, creareMonitor(6, "Microsoft", 20.2, 1));
	
	printf("\nLista de monitoare este: ");
	afisareLista(lista);

	//lista = stergereLista(lista);
	//printf("\nDupa stergere totala lista este: ");
	//afisareLista(lista);

	int cod;
	//printf("Introduceti codul monitorului ce doriti sa fie sters: ");
	//scanf("%d", &cod);
	//functia de extragere(cu stergere&returnare) dupa cod si apoi afisare lista dupa stergere
	//extragereSiStergereNod(lista, cod);
	//printf("\nDupa extragerea nodului mentionat, lista este: ");
	//afisareLista(lista);

	float diag;
	printf("Introduceti diagonala mai mare decat care nu doriti monitoare: ");
	scanf("%f", &diag);
	filtrareMonitoare(lista, diag);
	printf("\nDupa filtrare, lista este: ");
	afisareLista(lista);

	system("pause");
}
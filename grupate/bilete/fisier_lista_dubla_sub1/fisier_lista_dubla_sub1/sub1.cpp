#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct Fisier {
	char* denumire;
	char* cale_absoluta;
	char* extensie;
	unsigned int dimensiune;
};

struct nod {
	Fisier info;
	nod* next;
};

struct nodD {
	Fisier info;
	nodD* next;
	nodD* prev;
};

struct ListaDubla {
	nodD* prim;
	nodD* ultim;
};

Fisier creareFisier(const char* denumire, const char* cale, const char* ext, unsigned int dim) {
	Fisier f;
	f.denumire = (char*)malloc(sizeof(char)*(strlen(denumire) + 1));
	strcpy(f.denumire, denumire);
	f.cale_absoluta = (char*)malloc(sizeof(char)*(strlen(cale) + 1));
	strcpy(f.cale_absoluta, cale);
	f.extensie = (char*)malloc(sizeof(char)*(strlen(ext) + 1));
	strcpy(f.extensie, ext);
	f.dimensiune = dim;

	return f;
}

void afisareFisier(Fisier f) {
	printf("\nFisierul %s are calea absoluta %s, extensia %s si o dimensiune de %d bytes.", f.denumire, f.cale_absoluta, f.extensie, f.dimensiune);
}

void afisareIncSf(ListaDubla lista) {
	nodD* temp = lista.prim;
	while (temp) {
		afisareFisier(temp->info);
		temp = temp->next;
	}
}

void afisareSfInc(ListaDubla lista) {
	nodD* temp = lista.ultim;
	while (temp) {
		afisareFisier(temp->info);
		temp = temp->prev;
	}
}

void afisareListaSimpla(nod* cap) {
	while (cap) {
		afisareFisier(cap->info);
		cap = cap->next;
	}
}

nodD* creareNod(Fisier info, nodD* next, nodD* prev) {
	nodD* nou = (nodD*)malloc(sizeof(nodD));
	nou->info = creareFisier(info.denumire, info.cale_absoluta, info.extensie, info.dimensiune);
	nou->next = next;
	nou->prev = prev;

	return nou;
}

nod* inserareInceput(nod* cap, Fisier f) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareFisier(f.denumire,f.cale_absoluta,f.extensie,f.dimensiune);
	nou->next = cap;

	return nou;
}

ListaDubla inserareInceput(ListaDubla lista, Fisier f) {
	nodD* nou = creareNod(f, lista.prim, NULL);
	nou->info = f;
	if (lista.prim) {
		lista.prim->prev = nou;
		lista.prim = nou;
		return lista;
	}
	else {
		lista.prim = lista.ultim = nou;
	}
	return lista;
}

nod* cautareFisiereExtensie(ListaDubla lista, const char* extensie) {
	nod* listaRezultat = NULL;
	if (lista.prim) {
		nodD* temp = lista.prim;
		while (temp) {
			if (strcmp(temp->info.extensie, extensie) == 0)
			{
				listaRezultat = inserareInceput(listaRezultat, temp->info);
			}
			temp = temp->next;
		}
	}
	return listaRezultat;
}

Fisier cautareFisier(ListaDubla lista, const char* cale) {
	if (lista.prim) {
		nodD* temp = lista.prim;
		while (temp) {
			if (strcmp(temp->info.cale_absoluta, cale) == 0) {
				return temp->info;
			}
			temp = temp->next;
		}
	}
}

ListaDubla modificareCaleFisier(ListaDubla lista, const char* cale_curenta, const char* cale_deModificat) {
	if (lista.prim) {
		Fisier f;
		f = cautareFisier(lista, cale_curenta);
		strcpy(f.cale_absoluta, cale_deModificat);
	}
	return lista;
}

nod* stergereListaSimpla(nod* cap){
	while (cap) {
		free(cap->info.denumire);
		free(cap->info.cale_absoluta);
		free(cap->info.extensie);
		nod* temp = cap;
		cap = cap->next;
		free(temp);
	}
	return NULL;
}

ListaDubla stergereListaDubla(ListaDubla lista) {
	nodD* aux = lista.prim;
	while (aux) {
		free(aux->info.cale_absoluta);
		free(aux->info.denumire);
		free(aux->info.extensie);
		nodD* temp = aux;
		aux = aux->next;
		free(temp);
	}
	lista.prim = NULL;
	lista.ultim = NULL;
	return lista;
}

void main() {
	ListaDubla lista;
	lista.prim = lista.ultim = NULL;

	lista = inserareInceput(lista, creareFisier("Student", "C:\\Users\\Stud", ".txt", 10));
	lista = inserareInceput(lista, creareFisier("Poza", "C:\\Users\\Pics", ".png", 2000));
	lista = inserareInceput(lista, creareFisier("Retete", "C:\\Users\\Gastro", ".txt", 500));
	lista = inserareInceput(lista, creareFisier("Index", "C:\\Users\\Site", ".html", 105));
	lista = inserareInceput(lista, creareFisier("Script", "C:\\Users\\Scripts", ".js", 86));

	printf("\nLista afisata de la inceput la sfarsit este: ");
	afisareIncSf(lista);
	printf("\n***************************");

	printf("\nLista afisata de la sfarsit la inceput este: ");
	afisareSfInc(lista);

	nod* rezultat = cautareFisiereExtensie(lista,".txt");
	printf("\n***************************");
	printf("\nLista cu fisierele care au extensia cautata este: ");
	afisareListaSimpla(rezultat);

	printf("\n***************************");
	printf("\n Fisierul de la calea specificata este: ");
	Fisier cautat = cautareFisier(lista, "C:\\Users\\Site");
	afisareFisier(cautat);

	printf("\n***************************");
	printf("\n Dupa modificarea caii fisierului, lista este:");
	lista = modificareCaleFisier(lista, "C:\\Users\\Site", "T:\\Users\\Site");
	afisareIncSf(lista);
	rezultat = stergereListaSimpla(rezultat);
	lista = stergereListaDubla(lista);
	
	system("pause");
}
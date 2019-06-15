#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Facultate {
	int id;
	char* nume;
};

struct nodListaPrincipala;
struct nodListaSecundara {
	nodListaSecundara* next;
	nodListaPrincipala* info; //in loc sa am informatia propriu-zisa am un pointer catre nodul respectiv
};

struct nodListaPrincipala {
	nodListaPrincipala* next;
	nodListaSecundara* adiacente;
	Facultate info;
};

void citesteGrafMemoratInMatriceAdiacenta(int** &matrice, int &nrNoduri, Facultate* &facultati) {
	nrNoduri = 0;
	printf("Numarul de noduri:");
	scanf("%d", &nrNoduri);

	facultati = (Facultate*)malloc(sizeof(Facultate)*nrNoduri);
	for (int i = 0; i < nrNoduri; i++) {
		printf("ID-ul facultatii:");
		scanf("%d", &facultati[i].id);
		char buffer[20];
		printf("Numele facultatii:");
		scanf("%s", buffer);
		facultati[i].nume = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
		strcpy(facultati[i].nume, buffer);
	}

	matrice = (int**)malloc(sizeof(int*)*nrNoduri);
	for (int i = 0; i < nrNoduri; i++) {
		matrice[i] = (int*)malloc(sizeof(int)*nrNoduri);
		for (int j = 0; j < nrNoduri; j++) {
			matrice[i][j] = 0;
		}
	}
	int nrArce = 0;
	printf("Numarul de arce:");
	scanf("%d", &nrArce);

	for (int i = 0; i < nrArce; i++) {
		int idStart = 0;
		printf("Id-ul nodului de start:");
		scanf("%d", &idStart);
		int idStop = 0;
		printf("Id-ul nodului de stop:");
		scanf("%d", &idStop);
		matrice[idStart][idStop] = 1;
		matrice[idStop][idStart] = 1;
	}
}


nodListaPrincipala* inserareListaPrincipala(nodListaPrincipala* cap, Facultate f) {
	nodListaPrincipala* nou = (nodListaPrincipala*)malloc(sizeof(nodListaPrincipala));
	nou->adiacente = NULL;
	nou->info = f;
	nou->next = NULL;
	if (cap) {
		nodListaPrincipala* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		cap = nou;
	}
	return cap;
}

nodListaSecundara* inserareListaSecundara(nodListaSecundara* cap, nodListaPrincipala* info) {
	nodListaSecundara* nou = (nodListaSecundara*)malloc(sizeof(nodListaSecundara));
	nou->info = info;
	nou->next = NULL;
	if (cap) {
		nodListaSecundara* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		cap = nou;
	}
	return nou;
}

nodListaPrincipala* cautareDupaId(nodListaPrincipala* cap, int id) {
	while (cap && cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}

void inserareArcInGraf(nodListaPrincipala* graf, int idStart, int idStop) {
	if (graf) {
		nodListaPrincipala* nodStart = cautareDupaId(graf, idStart);
		nodListaPrincipala* nodStop = cautareDupaId(graf, idStop);
		if (nodStart && nodStop) {
			nodStart->adiacente = inserareListaSecundara(nodStart->adiacente, nodStop);
			nodStop->adiacente = inserareListaSecundara(nodStop->adiacente, nodStart);
		}
	}
}

void afisareListaSecundara(nodListaSecundara* cap) {
	while (cap) {
		printf("    %d. %s\n", cap->info->info.id, cap->info->info.nume);
		cap = cap->next;
	}
}

void afisareListaPrincipala(nodListaPrincipala* cap) {
	while (cap) {
		printf("%d. %s are urmatorii vecini: \n", cap->info.id, cap->info.nume);
		afisareListaSecundara(cap->adiacente);
		cap = cap->next;
	}
}

struct nodStiva {
	int id;
	nodStiva* next;
};

nodStiva* push(nodStiva* cap, int id) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->next = cap;
	nou->id = id;
	return nou;
}

int pop(nodStiva* &cap) {
	if (cap) {
		int id = cap->id;
		nodStiva* temp = cap;
		cap = cap->next;
		free(temp);
		return id;
	}
	else {
		return -1;
	}
}

int calculareNrNoduri(nodListaPrincipala* graf) {
	int nrNoduri = 0;
	while (graf) {
		nrNoduri++;
		graf = graf->next;
	}
	return nrNoduri;
}

void parcurgereAdancime(nodListaPrincipala* graf, int nodStart) {
	if (graf) {
		//luam o stiva
		nodStiva* stiva = NULL;
		//luam un vector vizitate - il initializez cu 0
		int nrNoduri = calculareNrNoduri(graf);
		int* vizitate = (int*)malloc(sizeof(int)*nrNoduri);
		for (int i = 0; i < nrNoduri; i++) {
			vizitate[i] = 0;
		}
		//adaug nodul de start in stiva
		stiva = push(stiva, nodStart);
		//marchez ca vizitat nodul de start
		vizitate[nodStart] = 1;
		//cat timp am stiva
		while (stiva) {
			//extrag nodul din stiva
			int idNodCurent = pop(stiva);
			nodListaPrincipala* nodCurent = cautareDupaId(graf, idNodCurent);
			//il afisez
			printf("%d . %s \n", nodCurent->info.id, nodCurent->info.nume);
			//afisez noduri adiacente
			nodListaSecundara* vecini = nodCurent->adiacente;
			while (vecini) {
				//daca nu l-am vizitat
				if (vizitate[vecini->info->info.id] == 0) {
					//il pun in stiva
					stiva = push(stiva, vecini->info->info.id);
					//il vizitez
					vizitate[vecini->info->info.id] = 1;
				}
				vecini = vecini->next; //trec la urmatorul vecin
			}
		}
	}
}

//parcurgere latime
struct nodCoada {
	nodCoada* next;
	int id;
};

nodCoada* inserareCoada(nodCoada* cap, int id) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->id = id;
	nou->next = NULL;
	if (cap) {
		nodCoada* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		cap = nou;
	}
	return cap;
}

int extragereCoada(nodCoada* &cap) {
	if (cap) {
		int rezultat = cap->id;
		nodCoada* temp = cap;
		cap = cap->next;
		free(temp);
		return rezultat;
	}
	else {
		return -1; //id inexistent/eroare
	}
}

void parcurgereInLatimeMatrice(int** graf, int nodStart, int nrNoduri) {
	//1. definim o coada
	//2. punem nodul de start in coada
	//3. luam un vector de vizitate
	//4. vizitam nodul de start
	//5. cat timp avem elemente in coada:
	//6. extragem nod din coada
	//7. cautam nodurile adiacente nevizitate si le inseram in coada
	//8. procesam nodul curent - cel extras
	nodCoada* coada = NULL;
	coada = inserareCoada(coada, nodStart);
	int* vizitate = (int*)malloc(sizeof(int)*nrNoduri);
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}
	vizitate[nodStart] = 1;
	while (coada) {
		int idNodCurent = extragereCoada(coada);
		printf("%d ", idNodCurent);
		for (int i = 0; i < nrNoduri; i++) {
			if (graf[idNodCurent][i] == 1 && vizitate[i] == 0) {
				inserareCoada(coada, i);
				vizitate[i] = 1;
			}
		}
	}
}

struct nodListaFacultate {
	Facultate info;
	nodListaFacultate* next;
};

nodListaFacultate* inserareFacultateInLista(nodListaFacultate* cap, Facultate f) {
	nodListaFacultate* nou = (nodListaFacultate*)malloc(sizeof(nodListaFacultate));
	nou->info = f;
	nou->next = cap;
	cap = nou;
	return cap;
}

nodListaFacultate* copiereGrafLatimeInLista(int** matrice, int nrNoduri, Facultate* facultati, int nodStart) {
	nodCoada* coada = NULL;
	int* vizitate = (int*)malloc(sizeof(vizitate)*nrNoduri);
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}
	coada = inserareCoada(coada, nodStart);
	vizitate[nodStart] = 1;
	nodListaFacultate* listaFacultati = NULL;
	while (coada) {
		int idCurent = extragereCoada(coada);
		Facultate f;
		f.id = facultati[idCurent].id;
		f.nume = (char*)malloc(sizeof(char)*(strlen(facultati[idCurent].nume) + 1));
		strcpy(f.nume, facultati[idCurent].nume);
		listaFacultati = inserareFacultateInLista(listaFacultati, f);
		for (int i = 0; i < nrNoduri; i++) {
			if (matrice[idCurent][i] == 1 && vizitate[i] == 0) {
				vizitate[i] = 1;
				coada = inserareCoada(coada, i);
			}
		}
	}
	return listaFacultati;
}

void afisareListaFacultati(nodListaFacultate* cap) {
	while (cap) {
		printf("%d. %s\n", cap->info.id, cap->info.nume);
		cap = cap->next;
	}
}

void main() {
	int** matrice;
	Facultate* facultati;
	int nrNoduri;
	citesteGrafMemoratInMatriceAdiacenta(matrice, nrNoduri, facultati);

	nodListaFacultate* lista = copiereGrafLatimeInLista(matrice, nrNoduri, facultati, 2);
	afisareListaFacultati(lista);
}
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct Muzeu {
	char* nume;
	float pret_bilet;
	int nr_vizitatori;
};

struct nod {
	Muzeu info; //informatia utila
	nod* next;  //ADRESA urmatorului nod
};

//creare nod --inserare la inceput
//I: informatia utila & adresa urmatorului nod
//E: adresa noului nod
nod* creareNod(Muzeu m, nod* urm) {
	nod* nodNou;
	nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = m; //problema - shallow copy pentru ca tipul e pointer (char*)
	nodNou->info.nume = (char*)malloc((sizeof(char)*strlen(m.nume) + 1)); //si copiez de la adresa veche la adresa noua
	strcpy_s(nodNou->info.nume, (strlen(m.nume) + 1), m.nume);
	nodNou->next = urm;
	return nodNou;
}
//citire muzeu
Muzeu citireMuzeu() {
	Muzeu muzeu;
	char buffer[20];
	printf("\nNume muzeu:");
	scanf("%s", buffer);
	muzeu.nume = (char*)malloc((sizeof(char)*strlen(buffer) + 1));
	strcpy_s(muzeu.nume, strlen(buffer) + 1, buffer);
	printf("Pret bilet intrare:");
	scanf("%f", &muzeu.pret_bilet);
	printf("Numar vizitatori: ");
	scanf("%d", &muzeu.nr_vizitatori);
	
	return muzeu;
}

//afisare muzeu
void afisareMuzeu(Muzeu m) {
	printf("Muzeul %s are pretul de %5.2f si %d vizitatori \n", m.nume, m.pret_bilet, m.nr_vizitatori);
}

//parsare lista
void parsareLista(nod* cap) {
	while (cap) {
		afisareMuzeu(cap->info);
		cap = cap->next; //ma duc la urmatorul nod
	}
}

//dezalocare lista
//I: lista, adica pointerul de la nod
//E: adresa lista
nod* stergereLista(nod* cap) {
	while (cap) {
		free(cap->info.nume);
		nod* temp = cap; //au aceeasi adresa, catre primul nod; altfel n-as mai avea acces la info
		cap = cap->next; //acum temp pointeaza catre primul elem, si cap catre al doilea
		free(temp); //sterg primul nod
	}
	return NULL;
}

//inserare la final
//I: adresa primului nod si info de inserat
//E: adresa primului nod din lista - pentru cazul de la inceput, cand am o lista goala
nod* inserareSfarsit(nod* cap, Muzeu m) {
	nod* nou = creareNod(m, NULL);
	if (cap) {
		nod* temp = cap; //pentru deplasare
		while (temp->next) { //ma opresc inainte de ultimul nod; ca atunci cand calc pe un pod si verific urmatoarea scandura
			temp = temp->next;
		}
		//acum sunt fix pe ultimul nod
		temp->next = nou;
	}
	else { //nu avem lista
		cap = nou; //lista va avea un singur nod
	}
	return cap; //adresa primului nod
}

//cautare pozitie
nod* cautarePozitie(nod* cap, int poz) {
	int i = 0;
	while (cap != NULL && i < poz)
	{
		cap = cap->next;
		i++;
	}
	if (i == poz)
		return cap;
	else return NULL;
}

//cautare dupa valoare - nr_vizitatori
nod* cautareValoare(nod* cap, int nrVizitatori) {
	int i = 0;
	while (cap != NULL && cap->info.nr_vizitatori != nrVizitatori)
	{
		cap = cap->next;
		i++;
	}
	if (cap->info.nr_vizitatori == nrVizitatori)
		return cap;
	else return NULL;
}

//stergere nod
void stergereNod(nod* pred) {
	nod* victima = pred->next;
	pred->next = pred->next->next;

	delete victima;
}

//stergere nod in functie de pozitia primita
void stergereNodPozitie(nod* &cap, int poz)
{
	//in cazul in care primul element este cel care trebuie sters
	if (poz == 0)
	{
		nod* victima = cap;
		cap = cap->next;
		delete victima;
	}
	else {
		nod* pred = cautarePozitie(cap, poz-1);
		stergereNod(pred);
	}
}

//stergere dupa valoarea nr_vizitatori
//I: primul nod al listei, adica lista & valoarea campului dupa care se sterge
void stergereNodVal(nod* &cap, int nrVizitatori) {
	// daca lista e vida nu facem nimic
	if (cap == NULL) return;
	if (cap->info.nr_vizitatori == nrVizitatori) {
		nod* victima = cap;
		cap = cap->next;
		delete victima;
	}
	// cautam predecesorul
	nod* temp = cap;
	while (temp->next != NULL && temp->next->info.nr_vizitatori != nrVizitatori)
		temp = temp->next;
	// daca a fost gasit, atunci il stergem
	if (temp->next != NULL)
		stergereNod(temp);
}

//sortare dupa nr_vizitatori
nod* sortareBubble(nod* l)
{
	if (!l || !l->next) //daca nu exista elemente in lista sau daca lista are un singur element
		return l;

	char vb = 1;
	nod *p, *q, *r;

	while (vb)
	{
		vb = 0; //nu are loc nici o interschimbare

		if (l->info.nr_vizitatori > l->next->info.nr_vizitatori)
		{
			//criteriul de sortare neindeplinit intre primul si al doilea nod
			p = l->next;
			q = p->next;

			//interschimbare
			l->next = q;
			p->next = l;
			l = p;
			vb = 1;
		}

		nod* t = l;
		while (t->next->next) //t->next->next corespunde nodului j
		{
			p = t->next; //p este nodul i
			q = p->next; //q este nodul j
			if (p->info.nr_vizitatori > q->info.nr_vizitatori)
			{
				r = q->next; //corespunde lui j+1;

				t->next = q;
				p->next = r;
				q->next = p;
				vb = 1;
			}
			t = t->next;
		}
	}
	return l;
}

void main() {
	nod* lista = NULL;
	lista = creareNod(citireMuzeu(), lista);
	lista = creareNod(citireMuzeu(), lista);
	lista = creareNod(citireMuzeu(), lista);

	printf("\nLista este: \n");
	parsareLista(lista);

	lista = sortareBubble(lista);

	parsareLista(lista);
	//lista = inserareSfarsit(lista, citireMuzeu());

	//parsareLista(lista);

	//stergereNodVal(lista, 100);
	//stergereNodPozitie(lista, 1);
	//lista = stergereLista(lista);
	//nod* elem;
	//elem = cautareValoare(lista, 100);

	//printf("\nDupa stergere, lista este: \n");
	//parsareLista(elem);

	system("pause");

}
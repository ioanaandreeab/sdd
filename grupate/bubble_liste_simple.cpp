#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct Muzeu {
	char* nume;
	float pret_bilet;
	int nr_vizitatori;
};

struct nod {
	Muzeu info;
	nod* next;
};

Muzeu creareMuzeu(const char* nume, float pret, int vizitatori) {
	Muzeu m;
	m.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(m.nume, nume);
	m.pret_bilet = pret;
	m.nr_vizitatori = vizitatori;

	return m;
}

nod* inserareInceput(nod* lista, Muzeu m){
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = m;
	nou->next = lista;

	return nou;
}

nod* sortareBubble(nod* head) {
	nod *prev, *curr, *temp;
	int count = 1;
	int i;

	if (head == NULL) { //lista e goala
		return NULL;
	}
	else if (head->next == NULL) { //lista are un singur nod
		return head;
	}

	curr = head;
	// numara nodurile
	while (curr->next != NULL) {
		count++;
		curr = curr->next;
	}
	// daca avem 2 noduri
	if (count == 2) {
		if (head->info.nr_vizitatori > head->next->info.nr_vizitatori) {
			temp = head->next; //temp e candidatul2
			temp->next = head; 
			head->next = NULL;
			return temp; //noul cap de lista dupa interschimb
		}
		else {
			return head;
		}
	}
	// bubble sort
	for (i = 1; i < count; i++) {
		curr = prev = head;//curent & prev pe primul nod
		while (curr != NULL && curr->next != NULL) { //cat timp am cate doua elemente
			if (curr->info.nr_vizitatori > curr->next->info.nr_vizitatori) { //daca elementul curent e mai mare decat urmatorul
				temp = curr->next;//temp e al doilea elem
				curr->next = curr->next->next;//next-ul primului element devine nextul celui de-al doilea
				temp->next = curr; //next-ul celui de-al doilea elem devine primul element
				if (head == curr) {
					head = prev = temp;
				}
				else {
					prev->next = temp;
					curr = temp;
				}
			}
			prev = curr;
			curr = curr->next;
		}
	}
	return head;
}

void afisareMuzeu(Muzeu m) {
	printf("\nMuzeul %s are un bilet de intrare de %5.2f lei si a avut %d vizitatori.", m.nume, m.pret_bilet, m.nr_vizitatori);
}

void afisareLista(nod* cap) {
	while (cap) {
		afisareMuzeu(cap->info);
		cap = cap->next;
	}
}

void main() {
	nod* lista = NULL;
	lista = inserareInceput(lista, creareMuzeu("Antipa", 20, 600));
	lista = inserareInceput(lista, creareMuzeu("MNAR", 5, 250));
	lista = inserareInceput(lista, creareMuzeu("MNAC", 10, 180));
	lista = inserareInceput(lista, creareMuzeu("Muzeul de istorie", 7, 150));
	lista = inserareInceput(lista, creareMuzeu("Casa experimentelor", 2, 500));

	lista = sortareBubble(lista);

	afisareLista(lista);
	system("pause");
}
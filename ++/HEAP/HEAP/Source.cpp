#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Mesaj {
	char* text;
	int prioritate;
};

struct HEAP {
	Mesaj* vector;
	int dim;
};

Mesaj initMesaj(const char* text, int prioritate) {
	Mesaj m;
	m.text = (char*)malloc(sizeof(char)*(strlen(text) + 1));
	strcpy(m.text, text);
	m.prioritate = prioritate;

	return m;
}

void afisareMesaj(Mesaj m) {
	printf("\nMesajul %s are prioritatea %d", m.text, m.prioritate);
}

void afisareHeap(HEAP heap) {
	for (int i = 0; i < heap.dim; i++) {
		afisareMesaj(heap.vector[i]);
	}
}

void filtrare(HEAP heap, int pozitie) {
	int fiuS = 2 * pozitie + 1; //calculeaza pozitia fiului din stanga
	int fiuD = 2 * pozitie + 2; //calculeaza pozitia fiului din dreapta
	int max = pozitie; //max ia val pozitiei curente analizate
	if (fiuS < heap.dim && heap.vector[max].prioritate < heap.vector[fiuS].prioritate) { //daca fiul exista & daca fiul e mai mare decat nodul la care ne aflam
		max = fiuS; //max ia pozitia fiului din stanga
	}
	if (fiuD < heap.dim && heap.vector[max].prioritate < heap.vector[fiuD].prioritate) { //valoarea fiului din dreapta e mai mare
		max = fiuD; //max ia pozitia fiului din dreapta
	}

	if (max != pozitie) { //daca max e diferit de pozitie, adica e nevoie de un interschimb
		Mesaj aux = heap.vector[max]; //se face interschimbul
		heap.vector[max] = heap.vector[pozitie];
		heap.vector[pozitie] = aux;
		if (2 * max + 1 < heap.dim) { //filtrarea se reapeleaza doar daca 2*max+1 < heap.dim => adica daca nodul curent mai are copii
			filtrare(heap, max); //si se apeleaza pentru max, care e noua pozitie curenta de analizat
		}
	}
}

Mesaj extragereMesaj(HEAP &heap) {
	if (heap.dim > 0) { //daca exista heap
		Mesaj rezultat = heap.vector[0]; //retin primul element - maximul
		Mesaj* temp = (Mesaj*)malloc(sizeof(Mesaj)*(heap.dim - 1)); //aloc memorie pentru un vector care e cu 1 elem mai mic
		for (int i = 0; i < heap.dim - 1; i++) { //copiez elem in vectorul nou
			temp[i] = heap.vector[i+1];
		}
		free(heap.vector); //sterg vectorul initial
		heap.vector = temp; //inlocuiesc cu vectorul obtinut
		heap.dim--; //scad dimensiunea
		for (int i = (heap.dim - 2) / 2; i >= 0; i--) { //de la parintele ultimei frunze pana la radacina
			filtrare(heap, i); //filtrez
		}
		return rezultat;
	}
	else {
		return initMesaj("", -1);
	}
}

//fff similar cu extragerea
void inserareHeap(HEAP &heap, Mesaj m) {
	Mesaj* temp = (Mesaj*)malloc(sizeof(Mesaj)*(heap.dim + 1));//aici maresc dimensiunea
	for (int i = 0; i < heap.dim; i++) {
		temp[i] = heap.vector[i];
	}
	temp[heap.dim] = m;
	free(heap.vector);
	heap.vector = temp;
	heap.dim++;
	for (int i = (heap.dim - 2) / 2; i >= 0; i--) { //de la parintele ultimei frunze pana la radacina
		filtrare(heap, i);
	}
}

void afisareCuPrioritateMare(HEAP &heap, int p) {
	while (heap.dim > 0 && heap.vector[0].prioritate >= p) { //cat timp mai am elemente in heap & prioritatea e mai mare sau egala cu ce am primit ca param
		Mesaj m = extragereMesaj(heap);
		afisareMesaj(m);
		free(m.text);
	}
}

void main() {
	HEAP heap;
	heap.dim = 6;
	heap.vector = (Mesaj*)malloc(sizeof(Mesaj)*heap.dim);
	heap.vector[0] = initMesaj("Salut", 4);
	heap.vector[1] = initMesaj("Ce faci?", 8);
	heap.vector[2] = initMesaj("Bine...tu?", 3);
	heap.vector[3] = initMesaj("Foarte bine", 6);
	heap.vector[4] = initMesaj("Cum asa?", 9);
	heap.vector[5] = initMesaj("pai, nimeni nu zice nimic", 2);
	
	afisareHeap(heap);

	for (int i = (heap.dim - 2) / 2; i >= 0; i--) {
		filtrare(heap, i);
	}
	printf("\n\nHeap dupa filtrare: ");
	afisareHeap(heap);

	printf("\n\nMesaj extras(prioritate max): ");
	Mesaj m = extragereMesaj(heap);
	afisareMesaj(m);
	free(m.text);

	printf("\n\nHeap dupa extragere: ");
	afisareHeap(heap);

	printf("\n\nAfisare cu prioritate mare");
	afisareCuPrioritateMare(heap, 6);
	printf("\n\n");
	afisareHeap(heap);

}
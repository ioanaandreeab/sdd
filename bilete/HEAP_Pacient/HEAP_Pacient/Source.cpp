#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Pacient {
	int cod;
	char* nume;
	char* descriereProblema;
	float varsta;
	int prioritate;
};

struct HEAP {
	Pacient* vector;
	int dim;
};

Pacient initPacient(int cod, const char* nume, const char* descriereProblema, float varsta, int prioritate) {
	Pacient p;
	p.cod = cod;
	p.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(p.nume, nume);
	p.descriereProblema = (char*)malloc(sizeof(char)*(strlen(descriereProblema) + 1));
	strcpy(p.descriereProblema, descriereProblema);
	p.varsta = varsta;
	p.prioritate = prioritate;

	return p;
}

void afisarePacient(Pacient p) {
	printf("\nPacientul %s (%d), in varsta de %5.2f ani, are problema %s cu prioritatea %d", p.nume, p.cod, p.varsta, p.descriereProblema, p.prioritate);
}

void afisareHeap(HEAP h) {
	for (int i = 0; i < h.dim; i++) {
		afisarePacient(h.vector[i]);
	}
}

void filtrare(HEAP heap, int pozitie) {
	int fiuS = 2 * pozitie + 1;
	int fiuD = 2 * pozitie + 2;
	int max = pozitie;
	if (fiuS < heap.dim && heap.vector[max].prioritate < heap.vector[fiuS].prioritate) {
		max = fiuS;
	}
	if (fiuD < heap.dim && heap.vector[max].prioritate < heap.vector[fiuD].prioritate) {
		max = fiuD;
	}

	if (max != pozitie) { //trebuie modificat maximul
		//interschimb
		Pacient aux = heap.vector[max];
		heap.vector[max] = heap.vector[pozitie];
		heap.vector[pozitie] = aux;
		if (2 * max + 1 < heap.dim) {
			filtrare(heap, max); //filtrare pentru noua prioritate max
		}
	}
}

void inserareHeap(HEAP &heap, Pacient p, const char* categorie) {
	//maresc dimensiunea vectorului
	Pacient* temp = (Pacient*)malloc(sizeof(Pacient)*(heap.dim + 1));
	for (int i = 0; i < heap.dim; i++) {
		temp[i] = initPacient(heap.vector[i].cod,heap.vector[i].nume,heap.vector[i].descriereProblema,heap.vector[i].varsta,heap.vector[i].prioritate);
		free(heap.vector[i].nume);
		free(heap.vector[i].descriereProblema);
	}
	if (strcmp("Categoria 1", categorie) == 0) {
		p.prioritate = 5;
	}
	else if (strcmp("Categoria 2", categorie) == 0) {
		p.prioritate = 4;
	}
	else{
		p.prioritate = 1;
	}
	temp[heap.dim] = initPacient(p.cod, p.nume, p.descriereProblema, p.varsta, p.prioritate);
	heap.dim++;
	free(heap.vector);
	heap.vector = temp;
	for (int i = (heap.dim - 2) / 2; i >= 0; i--) {//de la ultimul parinte la rad
		filtrare(heap, i);
	}
}

void modificarePrioritate(HEAP &heap) {
	if (heap.dim > 0) {
		for (int i = 0; i < heap.dim; i++) {
			if (heap.vector[i].prioritate != 5 && heap.vector[i].prioritate >= 2) {
				heap.vector[i].prioritate++;
			}
		}
	}
}

Pacient extragerePacient(HEAP &heap) {
	if (heap.dim > 0) {
		Pacient rezultat = heap.vector[0];
		Pacient* temp = (Pacient*)malloc(sizeof(Pacient)*(heap.dim-1));

		for (int i = 0; i < heap.dim - 1; i++) {
			temp[i] = heap.vector[i + 1];
		}
		free(heap.vector);
		heap.vector = temp;
		heap.dim--;
		for (int i = (heap.dim - 2) / 2; i >= 0; i--) {
			filtrare(heap, i);
		}
		return rezultat;
	}
	else {
		return initPacient(0, "", "", 0, 0);
	}

}

int determinareNrGravide(HEAP h) {
	int gravide = 0;
	if (h.dim > 0) {
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i].prioritate == 5)
				gravide++;
		}
	}
	return gravide;
}


void main() {
	HEAP h;
	h.dim = 5;
	h.vector = (Pacient*)malloc(sizeof(Pacient)*h.dim);
	h.vector[0] = initPacient(20, "Ioana", "Ecografie", 21, 5);
	h.vector[1] = initPacient(35, "Stefan", "Durere de cap", 22, 1);
	h.vector[2] = initPacient(24, "Cristiana", "Raceala", 12,4);
	h.vector[3] = initPacient(51, "Anabela", "Avort", 25, 5);
	h.vector[4] = initPacient(10, "Clarisa", "Deget umflat", 10, 3);

	for (int i = (h.dim - 2)/2; i >= 0; i--) {
		filtrare(h, i);
	}
	printf("\nHeap initial dupa filtrare: ");
	afisareHeap(h);
	Pacient p = initPacient(75, "Mariana", "Raceala", 11, 0);
	inserareHeap(h, p, "Categoria 2");

	for (int i = (h.dim - 2) / 2; i >= 0; i--) {
		filtrare(h, i);
	}
	printf("\n\nHeap dupa inserare:");
	afisareHeap(h);

	int nrGravide = determinareNrGravide(h);
	printf("\n\nGravide si copii de sub 1 an: %d",nrGravide);

	printf("\n\nHeap dupa ce au crescut prioritatile");
	modificarePrioritate(h);
	afisareHeap(h);
	extragerePacient(h);
	printf("\n\nDupa stergere heap-ul este:");
	afisareHeap(h);
}
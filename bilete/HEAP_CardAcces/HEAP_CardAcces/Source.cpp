#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct CardAcces {
	int prioritateProcesare;
	char* codIdentificare;
	int durataValabilitate;
	float creditConsum;
	int nrValidari;
	float* istoricValidari;
};

struct HEAP {
	CardAcces *vector;
	int dim;
};

void afisareCard(CardAcces c) {
	printf("\nCardul %s, cu prioritatea de procesare %d, valabil %d zile, mai are credit %5.2f si a fost validat de %d ori: ", c.codIdentificare, c.prioritateProcesare, c.durataValabilitate, c.creditConsum, c.nrValidari);
	for (int i = 0; i < c.nrValidari; i++) {
		printf("%5.2f ", c.istoricValidari[i]);
	}
}

void afisareHEAP(HEAP h) {
	if (h.dim > 0) {
		for (int i = 0; i < h.dim; i++) {
			afisareCard(h.vector[i]);
		}
	}
}

void filtrare(HEAP h, int pozitie) {
	int fiuS = 2 * pozitie + 1;
	int fiuD = 2 * pozitie + 2;
	int max = pozitie;
	if (fiuS < h.dim && h.vector[max].prioritateProcesare < h.vector[fiuS].prioritateProcesare) { 
		max = fiuS;
	}
	if (fiuD < h.dim && h.vector[max].prioritateProcesare < h.vector[fiuD].prioritateProcesare) { 
		max = fiuD; 
	}

	if (max != pozitie) { //e nevoie de interschimb
		CardAcces aux = h.vector[max];
		h.vector[max] = h.vector[pozitie];
		h.vector[pozitie] = aux;
		if (2 * max + 1 < h.dim) {
			filtrare(h, max);
		}
	}
}

void inserareHEAP(HEAP &heap, CardAcces c) {
	CardAcces* temp = (CardAcces*)malloc(sizeof(CardAcces)*(heap.dim + 1));
	for (int i = 0; i < heap.dim; i++) {
		temp[i] = heap.vector[i];
	}
	temp[heap.dim] = c;
	heap.dim++;
	free(heap.vector);
	heap.vector = temp;
	for (int i = (heap.dim - 2) / 2; i >= 0; i++) {
		filtrare(heap, i);
	}
}

CardAcces citireCardDinFisier(FILE* f) {
		CardAcces c;
		fscanf(f, "%d", &c.prioritateProcesare);
		char buffer[20];
		fscanf(f, "%s", buffer);
		c.codIdentificare = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
		strcpy(c.codIdentificare, buffer);
		fscanf(f, "%d", &c.durataValabilitate);
		fscanf(f, "%f", &c.creditConsum);
		fscanf(f, "%d", &c.nrValidari);
		c.istoricValidari = (float*)malloc(sizeof(float)*c.nrValidari);
		for (int i = 0; i < c.nrValidari; i++) {
			fscanf(f, "%f", &c.istoricValidari[i]);
		}
		return c;
}

float valTranzactiiCard(HEAP h, const char* cod) {
	if (h.dim > 0) {
		float valoare = 0;
		for (int i = 0; i < h.dim; i++) {
			if (strcmp(h.vector[i].codIdentificare, cod) == 0) {
				for (int j = 0; j < h.vector[i].nrValidari; j++) {
					valoare += h.vector[i].istoricValidari[j];
				}
			}
		}
		return valoare;
	}
}

CardAcces extragereSiDebitare(HEAP h, float valDebitare, const char* cod) {
	if (h.dim > 0) {
		for (int i = 0; i < h.dim; i++) {
			if (strcmp(h.vector[i].codIdentificare, cod) == 0) {
				CardAcces c = h.vector[i];
				c.creditConsum -= valDebitare;
				return c;
			}
		}
	}
}

void carduriSubNrDeZile(CardAcces* carduri, int nrZileLimita) {
	
}

void main() {
	HEAP h;
	h.dim = 5;
	h.vector = (CardAcces*)malloc(sizeof(CardAcces)*h.dim);
	int index = 0;
	FILE* f = fopen("carduri.txt", "r");
	if (f) {
		int nrCarduri = 0;
		fscanf(f, "%d", &nrCarduri);
		for (int i = 0; i < nrCarduri; i++) {
			h.vector[index++]=citireCardDinFisier(f);
		}
	}
	fclose(f);

	afisareHEAP(h);
	for (int i = (h.dim - 2) / 2; i >= 0; i--) {
		filtrare(h, i);
	}
	printf("\n\nHeap filtrat:");
	afisareHEAP(h);
	float val = valTranzactiiCard(h, "B27");
	printf("\n\nClientul cu codul dat are tranzactiile in valoare de %5.2f lei.", val);

	printf("\n\nAu fost validate cardurile: ");
	CardAcces c = extragereSiDebitare(h, 12, "B27");
	afisareCard(c);
	CardAcces d = extragereSiDebitare(h, 3, "B29");
	afisareCard(d);
	CardAcces e = extragereSiDebitare(h, 10, "B25");
	afisareCard(e);

	CardAcces* carduri = (CardAcces*)malloc(sizeof(CardAcces) * 3);
	carduri[0] = c;
	carduri[1] = d;
	carduri[2] = e;
	printf("\n\nCarduri sub nr de zile dat: ");
	carduriSubNrDeZile(carduri, 31);
}
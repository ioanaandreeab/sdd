//heap
//arbore binar sub forma de vector
//folosit pentru coada de prioritate
//valoarea dintr-un nod sa fie mai mare decat nodurile fiu
//max heap & min heap 
//vectorul dupa ce terminam nu va fi sortat
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct Mesaj {
	char* textMesaj;
	int prioritate;
};

Mesaj initMesaj(const char* text, int prioritate) {
	Mesaj m;
	m.prioritate = prioritate;
	m.textMesaj = (char*)malloc(sizeof(char)*(strlen(text) + 1));
	strcpy(m.textMesaj, text);

	return m;
}

void afisareMesaj(Mesaj m) {
	printf("%d. %s. \n", m.prioritate, m.textMesaj);
}

struct HEAP {
	Mesaj* vector;
	int dim;
};

void afisareVector(HEAP heap) {
	for (int i = 0; i < heap.dim; i++)
		afisareMesaj(heap.vector[i]);
}

//nu e nevoie sa fie transmisa prin referinta pentru ca doar modificam ordinea elementelor in interior
//are aceeasi dimensiune
//folosesc deja adresa pe care o are alocata vectorul, doar modific in interiorul lui
//transmit adresa, si punctul la care filtrez(pozitia nodului); o sa incep cu ultimul parinte, in cazul de fata e 4
void filtrare(HEAP heap, int pozitie) {
	int pozitieFiuStanga = 2 * pozitie + 1; //formulele pentru a determina pozitiile
	int pozitieFiuDreapta = 2 * pozitie + 2;
	//consideram ca nodul pe care l-am primit e maxim
	int max = pozitie;
	//pozitieFiuStanga < heap.dim --> daca nu am ajuns la sfarsitul vectorului
	//daca am min heap singurele modificari pe care le fac la tot programul sunt aici in if-urile astea 2 din < in >
	if (pozitieFiuStanga < heap.dim && heap.vector[max].prioritate < heap.vector[pozitieFiuStanga].prioritate)
	{
		max = pozitieFiuStanga;
	}
	if (pozitieFiuDreapta < heap.dim && heap.vector[max].prioritate < heap.vector[pozitieFiuDreapta].prioritate) {
		max = pozitieFiuDreapta;
	}

	//daca max s-a modificat in urma compararilor, interschimb elementele
	if (max != pozitie) {
		Mesaj aux = heap.vector[max];
		heap.vector[max] = heap.vector[pozitie];
		heap.vector[pozitie] = aux;

		//verific daca mai sunt copii pentru nodul respectiv; adica daca max e ultimul parinte
		//ultimul parinte este parintele ultimului element
		//max<= pentru a lua in calcul si ultimul parinte!!!
		if (max <= (heap.dim - 1) / 2) {
			//atunci reapelam metoda
			filtrare(heap, max);
		}
	}
}

//pentru inserare voi copia toate elem intr-un vector nou, introduc elem la final si apoi apelez filtrare iar
//e similara cu extragerea

//extrag elementul cu prioritatea cea mai mare pentru ca stiu ca e pe prima pozitie in coada de prioritati
//si realoc vectorul -> deci se schimba vectorul, adica il transmit prin referinta
Mesaj extragereDinHeap(HEAP &heap) {
	if (heap.dim > 0) { //daca avem HEAP
		Mesaj rezultat = heap.vector[0];
		Mesaj* temp = (Mesaj*)malloc(sizeof(Mesaj)*(heap.dim - 1));
		for (int i = 1; i < heap.dim; i++) {
			temp[i - 1] = heap.vector[i]; //copiez toate elementele de dupa elementul pe care il extrag intr-un nou vector care are dimensiunea cu 1 elem mai mica decat cel inital
		}
		heap.dim--;
		free(heap.vector);//dezaloc vectorul initial
		heap.vector = temp; //ii dau adresa lui vector catre temp

		//filtram elementele ramase in heap
		for (int i = (heap.dim - 1) / 2; i >= 0; i--)
		{
			filtrare(heap, i);
		}
		return rezultat;
	}
	else {
		return initMesaj("", -1);
	}

}
void main() {
	HEAP heap;
	heap.dim = 6;
	heap.vector = (Mesaj*)malloc(sizeof(Mesaj)*heap.dim);
	heap.vector[0] = initMesaj("Salut", 3);
	heap.vector[1] = initMesaj("Ce faci", 7);
	heap.vector[2] = initMesaj("Bine. Dar tu?", 8);
	heap.vector[3] = initMesaj("Foarte bine", 6);
	heap.vector[4] = initMesaj("Intelegem", 5);
	heap.vector[5] = initMesaj("Seen", 4);

	afisareVector(heap);

	//filtrez de jos catre radacina; pornesc de la ultimul parinte pentru ca ii stiu formula
	for (int i = (heap.dim - 1) / 2; i >= 0; i--)
	{
		filtrare(heap, i);
	}

	printf("\n");
	afisareVector(heap);

	printf("\n");
	//afisareMesaj(extragereDinHeap(heap));//daca apelez asa avem memory leak
	//deci fac asa:
	Mesaj m = extragereDinHeap(heap);
	afisareMesaj(m);
	free(m.textMesaj);

	system("pause");
}
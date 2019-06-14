#include<iostream>

struct LocAutocar
{
	char* locAutocar;
	int nrRand;
	float pretLoc;
	char* stareLoc; //ocupat,disponibil
};

struct CursaAutocar
{
	int capacitateLocuri;
	float distanta;
	
};

struct Heap
{
	LocAutocar* vector;
	int dim;
};

void filtrare(Heap heap, int pozitie)
{
	if (pozitie < heap.dim  && pozitie >= 0)
	{
		int pozS = 2 * pozitie + 1;
		int pozD = 2 * pozitie + 2;
		int pozMax = pozitie;

		if (pozS<heap.dim && heap.vector[pozS].locAutocar>heap.vector[pozMax].locAutocar)
			pozMax = pozS;
		if (pozD<heap.dim && heap.vector[pozD].locAutocar>heap.vector[pozMax].locAutocar)
			pozMax = pozD;
		if (pozMax != pozitie)
		{
			LocAutocar loc = heap.vector[pozMax];
			heap.vector[pozMax] = heap.vector[pozitie];
			heap.vector[pozitie] = loc;
			
			if (2 * pozMax + 1 < heap.dim)
				filtrare(heap, pozMax);
		}
		
	}
}

Heap initializare(int dim, LocAutocar *vect)
{
	Heap heap;
	heap.dim = dim;
	heap.vector = (LocAutocar*)malloc(sizeof(LocAutocar)*dim);
	for (int i = 0; i < dim; i++)
	{
		heap.vector[i] = vect[i];
	}
	for (int i = ((heap.dim - 1) - 1) / 2; i >= 0; i--)
		filtrare(heap, i);
	return heap;
}

Heap inserareLoc(Heap heap, LocAutocar l)
{
	LocAutocar* loc = (LocAutocar*)malloc(sizeof(LocAutocar)*(heap.dim + 1));
	for (int i = 0; i < heap.dim; i++)
	{
		loc[i] = heap.vector[i];
	}
	loc[heap.dim] = l;
	heap.dim++;
	free(heap.vector);
	heap.vector = loc;
	for (int i = ((heap.dim - 1) - 1) / 2; i >= 0; i--)
		filtrare(heap, i);
	return heap;
}

LocAutocar citireLoc(FILE* f)
{
	LocAutocar l;
	char buffer[30];
	fscanf(f, "%s", &buffer);
	l.locAutocar = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(l.locAutocar, buffer);
	fscanf(f, "%d", &l.nrRand);
	fscanf(f, "%f", &l.pretLoc);
	fscanf(f, "%s", &buffer);
	l.stareLoc = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(l.stareLoc, buffer);
	return l;
}

CursaAutocar citire(FILE* f)
{
	CursaAutocar c;
	fscanf(f, "%f", &c.distanta);
	fscanf(f, "%d", &c.capacitateLocuri);
	return c;
}

void afisareLoc(LocAutocar l)
{
	printf("Loc:%s  Rand:%d  Pret:%5.2f lei Stare:%s\n", l.locAutocar, l.nrRand, l.pretLoc, l.stareLoc);
}

void afisareCursa(CursaAutocar c)
{
	printf("Distanta: %5.2f km\nCapacitate Locuri: %d\n", c.distanta, c.capacitateLocuri);
}

void afisareHeap(Heap heap)
{
	if (heap.dim > 0)
	{
		for (int i = 0; i < heap.dim; i++)
			afisareLoc(heap.vector[i]);
	}
}

Heap prioritate(Heap heap, char* cod)
{
	if (heap.dim>0)
	{
		for (int i = 0; i < heap.dim; i++)
		{
			if (strcmp(heap.vector[i].locAutocar, cod) == 0)
			{
				int ajutor = atoi(heap.vector[i].locAutocar);
				int n = 0;
				n = ajutor*2;
				char buffer[30];
				strcpy(buffer, itoa(n, buffer, 10));
				free(heap.vector[i].locAutocar);
				heap.vector[i].locAutocar=(char*)malloc(sizeof(char)*(strlen(buffer)+1));
				strcpy(heap.vector[i].locAutocar, buffer);
			
			}
		}

		for (int i = ((heap.dim - 1) - 1) / 2; i >= 0; i--)
			filtrare(heap, i);

		return heap;
	}
	
}

void costMediu(Heap heap,CursaAutocar c)
{
	if (heap.dim > 0)
	{
		float suma = 0;
		for (int i = 0; i < heap.dim; i++)
		{
			suma = suma + heap.vector[i].pretLoc;
			printf("Pentru locul %s pretul pe km este de %5.2f.\n", heap.vector[i].locAutocar, suma/c.distanta);
		}
	
	}
}

float valoareCursa(Heap heap)
{
	if (heap.dim > 0)
	{   
		float suma = 0;
		for (int i = 0; i < heap.dim; i++)
		{
			if (strcmp(heap.vector[i].stareLoc, "ocupat") == 0)
			{
				suma = suma + heap.vector[i].pretLoc;
			}
		}
		return suma;
	}
}

void main()
{	
	LocAutocar* locuri = NULL;
	FILE* f = fopen("Fisier.txt", "r");
	CursaAutocar c = citire(f);
	locuri = (LocAutocar*)malloc(sizeof(LocAutocar)*c.capacitateLocuri);
	for (int i = 0; i < c.capacitateLocuri; i++)
		locuri[i] = citireLoc(f);
	Heap heap = initializare(c.capacitateLocuri, locuri);
	afisareCursa(c);
	for (int i = ((heap.dim - 1) - 1) / 2; i >= 0; i--)
		filtrare(heap, i);
	afisareHeap(heap);
	printf("\n");
	heap = prioritate(heap, "7");
	afisareHeap(heap);
	printf("\n\n");
	costMediu(heap, c);
	printf("\n\n");
	float a = valoareCursa(heap);
	printf("Suma incasata pt toate locurile rezervate este: %5.2f.", a);
	
}
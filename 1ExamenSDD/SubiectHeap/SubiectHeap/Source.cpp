#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
#include "iostream"
using namespace std;
struct Movie
{
	int cod;
	char* titlu;
	float buget;
	char* gen;
	int rating;
};
struct Heap
{
	Movie** elements;
	int heapSize;
	int index;
};

void InitHeap(Heap&, int);
Movie* CreateItem(int, int, char*, char*, int);
void Enqueue(Heap&, int, int, char*, char*, int);
Movie* Dequeue(Heap&);
void ReheapUp(Heap, int, int);
void ReheapDown(Heap, int, int);
void PrintHeap(Heap);

void InitHeap(Heap& heap, int size)
{
	heap.elements = (Movie**)malloc(sizeof(Movie*)*size);
	heap.heapSize = size;
	heap.index = 0;
	memset(heap.elements, NULL, sizeof(Movie*)*size);
}

Movie* CreateItem(int codFilm, int bugetFilm, char* titluFilm, char* genFilm, int ratingFilm)
{
	Movie* item = (Movie*)malloc(sizeof(Movie));
	item->cod = codFilm;
	item->buget = bugetFilm;
	item->rating = ratingFilm;
	item->titlu = (char*)malloc(strlen(titluFilm) + 1);
	strcpy(item->titlu, titluFilm);
	item->gen = (char*)malloc(strlen(genFilm) + 1);
	strcpy(item->gen, genFilm);
	return item;
}

void Enqueue(Heap& heap, int cod, int buget, char* titlu, char* gen, int rating)
{
	if (heap.index < heap.heapSize)
	{
		Movie* item = CreateItem(cod, buget, titlu, gen, rating);
		heap.elements[heap.index] = item;
		ReheapUp(heap, 0, heap.index);
		heap.index++;
		return;
	}
	printf("Heap structure is full!\n");
}

void ReheapUp(Heap heap, int first, int last)
{
	int parinte;
	Movie* temp = NULL;
	if (first < last)
	{
		parinte = (last - 1) / 2;
		if (heap.elements[parinte]->cod <
			heap.elements[last]->cod)
		{
			temp = heap.elements[parinte];
			heap.elements[parinte] = heap.elements[last];
			heap.elements[last] = temp;
			ReheapUp(heap, first, parinte);
		}
	}
}

void ReheapDown(Heap heap, int current, int last)
{
	int leftChild, rightChild, maxChild;
	Movie* temp = NULL;
	leftChild = 2 * current + 1;
	rightChild = 2 * current + 2;
	if (leftChild <= last)
	{
		if (leftChild == last)
			maxChild = leftChild;
		if (heap.elements[leftChild]->cod <
			heap.elements[rightChild]->cod)
			maxChild = leftChild;
		else
			maxChild = rightChild;

		if (heap.elements[current]->cod >
			heap.elements[maxChild]->cod)
		{
			temp = heap.elements[current];
			heap.elements[current] = heap.elements[maxChild];
			heap.elements[maxChild] = temp;
		}
		ReheapDown(heap, maxChild, last);
	}
}

Movie* Dequeue(Heap& heap)
{
	Movie* result = NULL;
	if (heap.index > 0)
	{
		result = heap.elements[0];
		heap.elements[0] = heap.elements[--heap.index];
		ReheapDown(heap, 0, heap.index - 1);
	}
	return result;
}

void afisareFilm(Movie* m)
{
	printf("%d - %s - %5.2f - %s - %d\n", (*m).cod, (*m).titlu, (*m).buget, (*m).gen, (*m).rating);
}

void afisareHeap(Heap* heap,char* categorie)
{
	if ((*heap).index>0)
	{
		for (int i = 0; i < (*heap).index; i++)
		{
			/*if (strcmp((*heap).elements[i]->gen, categorie) != 0)*/
				afisareFilm((*heap).elements[i]);
		}
	}

}

float* returnBuget(Heap heap)
{
	if (heap.index > 0)
	{
		float medie = 0;
		float suma = 0;
		float bugetMin = 0;
		float bugetMax = 0;
		float* vector = (float*)malloc(sizeof(float)*2);

		for (int i = 0; i < heap.index; i++)
			suma = suma + heap.elements[i]->rating;
		medie = suma / heap.index;

		for (int i = 0; i < heap.index; i++)
		{	
			if (heap.elements[i]->rating < medie)
			{
				bugetMin = bugetMin + heap.elements[i]->buget;
			}
			else
			{
				bugetMax = bugetMax + heap.elements[i]->buget;
			}
		}

		vector[0] = bugetMin;
		vector[1] = bugetMax;
		return vector;
	}
	
}

void afisareFilme(Heap heap)
{
	if (heap.index > 0)
	{
		char** categorii = (char**)malloc(sizeof(char*)*heap.index);
		int nrCategorii = 0;

		for (int i = 0; i < heap.index; i++)
		{
			int ok = 0;
			for (int j = 0; j < nrCategorii; j++)
			if (strcmp(categorii[j], heap.elements[i]->gen) == 0)
			{
				ok = 1;
			}

			if (ok == 0)
			{
				categorii[nrCategorii] = (char*)malloc(sizeof(char)*(strlen(heap.elements[i]->gen) + 1));
				strcpy(categorii[nrCategorii], heap.elements[i]->gen);
				nrCategorii++;
			}

		}
			for (int i = 0; i < nrCategorii; i++)
			{
				int contor = 0;
				for (int j = 0; j < heap.index; j++)
				{
					if (strcmp(categorii[i], heap.elements[j]->gen) == 0)
					{
						contor++;
					}

				}
				printf("Din categoria %s fac parte %d filme.\n", categorii[i], contor);
			}		
	}	
}

Heap schimbare(Heap heap,int codVechi, int codNou)
{
	if (heap.index > 0)
	{
		for (int i = 0; i < heap.index; i++)
		{
			if (heap.elements[i]->cod == codVechi)
			{
				heap.elements[i]->cod = codNou;
			}
		}

		for (int i = ((heap.index - 1) - 1) / 2; i >= 0; i--)
		{
			ReheapDown(heap, i, heap.index - 1);
		}

		return heap;
	}
}

void main()
{
	Heap heap;
	InitHeap(heap, 8);
	FILE* pFile = fopen("Filme.txt", "r");
	if (pFile)
	{
		int cod, rating; float buget; char titlu[50], gen[50];
		fscanf(pFile, "%d", &cod);
		while (!feof(pFile))
		{
			fscanf(pFile, "%f", &buget);
			fscanf(pFile, "%s", titlu);
			fscanf(pFile, "%s", gen);
			fscanf(pFile, "%d", &rating);
			Enqueue(heap, cod, buget, titlu, gen, rating);
			fscanf(pFile, "%d", &cod);
		}
	}
	for (int i = ((heap.index - 1) - 1) / 2; i >= 0; i--)
	{
		ReheapDown(heap, i, heap.index-1);
	}
	afisareHeap(&heap,"Kids");
	printf("\n\n");
	float* vect = returnBuget(heap);
	printf("Bugetul filmelor cu rating mai mic decat media este %5.2f.\n", vect[0]);
	printf("Bugetul filmelor cu rating mai mare decat media este %5.2f.\n", vect[1]);
	printf("\n\n");
	afisareFilme(heap);
	printf("\n\n");
	heap = schimbare(heap, 300, 800);
	afisareHeap(&heap,"Kids");
	/*Movie* item = NULL;
	while ((item = Dequeue(heap)) != NULL)
	printf("Heap cod:%d, buget:%f\n",
	item->cod, item->buget);*/

}
#include "iostream"
#include "stdio.h"
using namespace std;
enum TipHardware
{
	MONITOR = 10,
	HARD_DRIVE = 20,
	RAM = 30,
	PCU = 40
};
struct ComponentaHdw
{
	char* denumire;
	TipHardware tipHardware;
	float valoarePiesa;
	int luniGarantie;
};
struct PiesaSistem
{
	ComponentaHdw* ch;
	PiesaSistem* next;
};
struct SistemCalcul
{
	PiesaSistem* head = nullptr;
	PiesaSistem* tail = nullptr;
};

void initCoada(SistemCalcul& coada)
{
	coada.head = coada.tail = nullptr;
}
bool isQueueEmpty(const SistemCalcul& coada)
{
	return coada.head == nullptr ? true : false;
}
ComponentaHdw* creareHdw(char* den, TipHardware hdwType, float hdwPrice, int hdwWarranty)
{
	ComponentaHdw* result =
		(ComponentaHdw*)malloc(sizeof(ComponentaHdw));
	result->denumire = (char*)malloc(strlen(den) + 1);
	strcpy(result->denumire, den);
	result->tipHardware = hdwType;
	result->valoarePiesa = hdwPrice;
	result->luniGarantie = hdwWarranty;
	return result;
}
PiesaSistem* creareNodCoada(ComponentaHdw* hdw)
{
	PiesaSistem* nod = (PiesaSistem*)malloc(sizeof(PiesaSistem));
	nod->ch = hdw;
	nod->next = NULL;
	return nod;
}
void Enqueue(SistemCalcul& coada, PiesaSistem* nod)
{
	if (isQueueEmpty(coada))
		coada.head = coada.tail = nod;
	else
	{
		coada.tail->next = nod;
		coada.tail = nod;
	}
}

void afisareCoada(SistemCalcul coada)
{
	if (coada.head)
	{
		PiesaSistem* temp = coada.head;
		while (temp)
		{
			if (temp->ch->tipHardware == 10)
				printf("%s - %s - %5.2f - %d\n", temp->ch->denumire, "MONITOR", temp->ch->valoarePiesa, temp->ch->luniGarantie);
			else if (temp->ch->tipHardware == 20)
				printf("%s - %s - %5.2f - %d\n", temp->ch->denumire, "HARD_DRIVE", temp->ch->valoarePiesa, temp->ch->luniGarantie);
			else if (temp->ch->tipHardware == 30)
				printf("%s - %s - %5.2f - %d\n", temp->ch->denumire, "RAM", temp->ch->valoarePiesa, temp->ch->luniGarantie);
			else
				printf("%s - %s - %5.2f - %d\n", temp->ch->denumire, "PCU", temp->ch->valoarePiesa, temp->ch->luniGarantie);
			temp = temp->next;
		}
	}
}

SistemCalcul modificaPretul(SistemCalcul coada, int tip)
{
	if (coada.head)
	{
		PiesaSistem* temp = coada.head;
		{
			while (temp)
			{
				if (temp->ch->tipHardware == tip)
				{
					temp->ch->valoarePiesa = temp->ch->valoarePiesa*1.15;
				}
				temp = temp->next;
			}
		}
		
	}	
	return coada;		
}


int dimensiune(SistemCalcul coada,int n)
{
	if (coada.head)
	{
		int contor = 0;
		PiesaSistem* temp = coada.head;
		while (temp)
		{
			if (temp->ch->luniGarantie>n)
			{
				contor++;
			}
			temp = temp->next;
		}
		return contor;
	}
	else
	{
		return 0;
	}
}

ComponentaHdw** afisarePiese(SistemCalcul coada, int nr)
{
	if (coada.head)
	{
		int contor = dimensiune(coada, nr);	
		int n = 0;
		ComponentaHdw** vector = (ComponentaHdw**)malloc(sizeof(ComponentaHdw*)*contor);
		PiesaSistem* temp = coada.head;
		while (temp)
		{
			if (temp->ch->luniGarantie > nr)
			{				
				vector[n] = temp->ch;
				n++;
			}
			temp = temp->next;
		}
		return vector;
	}
	else
	{
		return NULL;
	}

}

PiesaSistem* inserareLista(PiesaSistem* lista, ComponentaHdw* comp)
{
	PiesaSistem* nou = (PiesaSistem*)malloc(sizeof(PiesaSistem));	
	nou->ch = comp;
	nou->next = lista;
	return nou;
}

PiesaSistem* inserarePiese(SistemCalcul& coada, int nr)
{
	PiesaSistem* lista = NULL;
	if (coada.head)
	{		
		PiesaSistem* temp = coada.head;
		while (temp)
		{
			if (temp->ch->luniGarantie > nr)
			{
				lista=inserareLista(lista, temp->ch);
			}
			temp = temp->next;
		}
		return lista;
	}


}

ComponentaHdw* Dequeue(SistemCalcul& coada)
{
	ComponentaHdw* piesa = nullptr;

	if (!isQueueEmpty(coada))
	{
		PiesaSistem* tmpq = coada.head;
		piesa = tmpq->ch;
		coada.head = tmpq->next;
		/*free(tmpq->ch->denumire);*/
		free(tmpq);

	}
	
	return piesa;
	
}

void main()
{
	SistemCalcul coada;
	initCoada(coada);
	FILE* pFile = fopen("Sistem.txt", "r");
	if (pFile)
	{
		char buffer[50]; int hdwType, hdwWar; float hdwPrice;
		fscanf(pFile, "%s", buffer);
		while (!feof(pFile))
		{
			fscanf(pFile, "%d", &hdwType);
			fscanf(pFile, "%f", &hdwPrice);
			fscanf(pFile, "%d", &hdwWar);
			ComponentaHdw* chdw = creareHdw(buffer, (TipHardware)hdwType, hdwPrice, hdwWar);
			PiesaSistem* nod = creareNodCoada(chdw);
			Enqueue(coada, nod);
			fscanf(pFile, "%s", buffer);
		}
		fclose(pFile);
	}
	afisareCoada(coada);
	printf("\n");

	coada=modificaPretul(coada, 10);
	afisareCoada(coada);

	printf("\n");
	int c = dimensiune(coada, 12);
	
	ComponentaHdw** componente = (ComponentaHdw**)malloc(sizeof(ComponentaHdw*)*c);
	componente = afisarePiese(coada, 12);
	for (int i = 0; i < c; i++)
	{
		if (componente[i]->tipHardware == 10)
			printf("%s - %s - %5.2f - %d\n", componente[i]->denumire, "MONITOR", componente[i]->valoarePiesa, componente[i]->luniGarantie);
		else if (componente[i]->tipHardware == 20)
			printf("%s - %s - %5.2f - %d\n", componente[i]->denumire, "HARD_DRIVE", componente[i]->valoarePiesa, componente[i]->luniGarantie);
		else if (componente[i]->tipHardware == 30)
			printf("%s - %s - %5.2f - %d\n", componente[i]->denumire, "RAM", componente[i]->valoarePiesa, componente[i]->luniGarantie);
		else
			printf("%s - %s - %5.2f - %d\n", componente[i]->denumire, "PCU", componente[i]->valoarePiesa, componente[i]->luniGarantie);
		
	}

	printf("\n\n-------------------------------------\n\n");
	PiesaSistem* lista = NULL;
	lista = inserarePiese(coada, 12);
	while (lista)
	{
		if (lista->ch->tipHardware == 10)
			printf("%s - %s - %5.2f - %d\n", lista->ch->denumire, "MONITOR", lista->ch->valoarePiesa, lista->ch->luniGarantie);
		else if (lista->ch->tipHardware == 20)
			printf("%s - %s - %5.2f - %d\n", lista->ch->denumire, "HARD_DRIVE", lista->ch->valoarePiesa, lista->ch->luniGarantie);
		else if (lista->ch->tipHardware == 30)
			printf("%s - %s - %5.2f - %d\n", lista->ch->denumire, "RAM", lista->ch->valoarePiesa, lista->ch->luniGarantie);
		else
			printf("%s - %s - %5.2f - %d\n", lista->ch->denumire, "PCU", lista->ch->valoarePiesa, lista->ch->luniGarantie);
		lista = lista->next;
	}
	printf("\n\n-------------------------------------\n\n");

	printf("\n");
		
	while (!isQueueEmpty(coada))
	{
	ComponentaHdw* hwd = Dequeue(coada);
	if (hwd)
	printf("Componenta: %s, tip: %d, price: %3.2f\n",
	hwd->denumire, hwd->tipHardware, hwd->valoarePiesa);
	}

	
}
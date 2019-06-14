#include<iostream>

struct PiesaDeTeatru
{
	char* numePiesa;
	int nrSpectatori;
};

struct nod
{
	PiesaDeTeatru info;
	nod* next;
	nod* prev;
};

struct LDI
{
	nod* prim;
	nod* ultim;
};

PiesaDeTeatru deepCopy(PiesaDeTeatru piesa)
{
	PiesaDeTeatru temp;
	temp.numePiesa = (char*)malloc(sizeof(char)*(strlen(piesa.numePiesa) + 1));
	strcpy(temp.numePiesa, piesa.numePiesa);
	temp.nrSpectatori = piesa.nrSpectatori;
	return temp;
}

LDI inserareInceput(LDI lista, PiesaDeTeatru piesa)
{
	nod* nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = deepCopy(piesa);
	nodNou->prev = NULL;
	nodNou->next = NULL;
	
	if (lista.prim)
	{
		lista.prim->prev = nodNou;
		lista.prim = nodNou;
	}
	else
	{
		lista.prim = nodNou;
		lista.ultim = nodNou;
	}
	return lista;
}

nod* inserareInceputCircualara(nod* inceput, PiesaDeTeatru piesa)
{
	nod* nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = deepCopy(piesa);
	nodNou->next = NULL;
	nodNou->prev = NULL;
	if (inceput)
	{
		nodNou->prev = inceput->prev;
		inceput->prev = nodNou;
		nodNou->prev->next = nodNou;
		nodNou->next = inceput;		
	}
	else
	{
		inceput = nodNou;
		nodNou->next = nodNou;
		nodNou->prev = nodNou;		
		
	}
	return nodNou;
}

nod* inserareSfarsitCirculara(nod* inceput,PiesaDeTeatru piesa)
{
	nod* nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = deepCopy(piesa);
	nodNou->next = NULL;
	nodNou->prev = NULL;

	if (inceput)
	{
		nodNou->prev = inceput->prev;
		inceput->prev = nodNou;
		nodNou->prev->next = nodNou;
		nodNou->next = inceput;
	}
	else
	{
		inceput = nodNou;
		nodNou->next = nodNou;
		nodNou->prev = nodNou;
	}
	return inceput;

}

nod* inserarePozitieCirculara(nod* inceput, PiesaDeTeatru piesa,int pozitie)
{
	if (inceput && pozitie>0)
	{
		nod* nodNou = (nod*)malloc(sizeof(nod));
		nodNou->info = deepCopy(piesa);

		if (pozitie == 1)
		{			
		   	nodNou->prev = inceput->prev;
			inceput->prev = nodNou;
			nodNou->prev->next = nodNou;
			nodNou->next = inceput;
			return nodNou;
		}
		else
		{
			nod* temp = inceput;
			int contor = 1;
			while (temp->next && contor < pozitie-1)
			{				
				temp = temp->next;			
				contor++;
			}

			if (temp->next != inceput)
			{
				nodNou->next = temp->next;
				temp->next = nodNou;
				nodNou->next->prev = nodNou;
				nodNou->prev = temp;			
				
				return inceput;
				
			}

			else
			{					
				nodNou->prev = inceput->prev;
				inceput->prev = nodNou;
				nodNou->prev->next = nodNou;
				nodNou->next = inceput;
				return inceput;					
			}			
		}
	}
}

void afisareCirculara(nod* inceput)
{
	if (inceput)
	{
		nod* temp = inceput;
		while (temp->next!=inceput)
		{			
			printf("%s - %d\n", temp->info.numePiesa, temp->info.nrSpectatori);
			temp = temp->next;
		}
		printf("%s - %d\n", temp->info.numePiesa, temp->info.nrSpectatori);	
	}
	else
	{
		printf("Lista este vida!");
	}
}

void afisareCicularaSfarsit(nod* inceput)
{
	if (inceput)
	{
		nod* temp = inceput;		
		do
		{
			temp = temp->prev;
			printf("%s - %d\n", temp->info.numePiesa, temp->info.nrSpectatori);
			

		} while (temp != inceput);
	}
	else
	{
		printf("Lista este vida!");
	}
}

nod* stergereListaCirculara(nod* inceput)
{	
	if (inceput)
	{
		nod*temp = inceput;
		do
		{
			temp = temp->next;
			free(inceput->info.numePiesa);			
			free(inceput);

		} while (temp != inceput);

		return inceput;
	}
	
}

int calculeazaNr(nod* inceput, char* nume)
{
	if (inceput)
	{
		int suma = 0;
		nod* temp = inceput;
		do
		{
			if (strcmp(temp->info.numePiesa, nume) == 0)
			{
				suma = suma + temp->info.nrSpectatori;
			}

			temp = temp->next;
		} while (temp != inceput);		

		return suma;
	}
	else
	{
		return 0;
	}
}

LDI inserareSfarsit(LDI lista, PiesaDeTeatru piesa)
{
	nod* nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = deepCopy(piesa);
	nodNou->next = NULL;
	nodNou->prev = NULL;

	if (lista.prim)
	{
		nodNou->prev = lista.ultim;
		lista.ultim->next = nodNou;
		lista.ultim = nodNou;
	}
	else
	{
		lista.prim = nodNou;
		lista.ultim = nodNou;
	}
	return lista;

}

LDI inserarePozitie(LDI lista, PiesaDeTeatru piesa, int pozitie)
{
	if (lista.prim && pozitie > 0)
	{				
		if (pozitie == 1)
		{			
			lista = inserareInceput(lista, piesa);
		}

		else
		{						
			int contor = 1;	
			nod* temp = lista.prim;
			while (temp!=lista.ultim && contor < pozitie)
			{
				temp = temp->next;
				contor++;
			}
			
				nod* nodNou = (nod*)malloc(sizeof(nod));
				nodNou->info = deepCopy(piesa);

				nodNou->next = temp;
				nodNou->prev = temp->prev;
				temp->prev = nodNou;
				nodNou->prev->next = nodNou;
			
		}
		
	}	
	return lista;
}

LDI stergere(LDI lista)
{
	while (lista.prim)
  {
		free(lista.prim->info.numePiesa);
		nod* temp = lista.prim;
		lista.prim = lista.prim->next;
		free(temp);		
  }
	lista.ultim = NULL;
	return lista;	
	
}

LDI stergereElement(LDI lista, int nr)
{
	if (lista.prim)
	{
		
		if (lista.prim->info.nrSpectatori == nr)
		{	
			nod* temp = lista.prim;
			lista.prim = lista.prim->next;	
			lista.prim->prev = NULL;
			free(temp->info.numePiesa);
			free(temp);			
		}
		else
		{
			nod* temp = lista.prim;
			while (temp->next && temp->info.nrSpectatori!=nr)
			{
				temp = temp->next;
			}
			if (temp->next)
			{
				temp->next->prev = temp->prev;
				temp->prev->next = temp->next;
				free(temp->info.numePiesa);
				free(temp);
			}
			else
			{
				temp->prev->next = NULL;
				lista.ultim = temp->prev;
				free(temp->info.numePiesa);
				free(temp);
			}

		}
		return lista;
	}
	else
	{
		printf("Lista este vida!");
	}
	
}

LDI stergerePozitie(LDI lista, int pozitie)
{
	if (lista.prim && pozitie>0)
	{
		if (pozitie == 1)
		{
			nod* temp = lista.prim;
			lista.prim = lista.prim->next;
			lista.prim->prev = NULL;
			free(temp->info.numePiesa);
			free(temp);
		}
		else
		{
			int contor = 1;
			nod* temp = lista.prim;
			while (temp->next&&contor < pozitie)
			{
				temp = temp->next;
				contor++;
			}

			if (temp->next)
			{
				temp->next->prev = temp->prev;
				temp->prev->next = temp->next;
				free(temp->info.numePiesa);
				free(temp);
			}
			else
			{
				temp->prev->next = NULL;
				lista.ultim = temp->prev;
				free(temp->info.numePiesa);
				free(temp);
			}
		}
		return lista;
	}
	else
	{
		printf("Lista este vida!");
	}
}


void afisareNr(LDI lista, char* nume)
{
	if (lista.prim)
	{
		int contor = 0;
		nod* temp = lista.prim;
		while (temp)
		{
			if (strcmp(temp->info.numePiesa, nume) == 0)
			{
				contor++;
			}
			temp = temp->next;
		}
		printf("Nr de piese cu numele %s este %d.", nume, contor);
	}
	else
	{
		printf("Lista este vida!");
	}
}

int count(LDI lista,int nr)
{
	if (lista.prim)
	{
		int contor = 0;
		nod* temp = lista.prim;
	    while (temp)
		{
			if (temp->info.nrSpectatori > nr)
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

PiesaDeTeatru* afisareVector(LDI lista, int n)
{
	if (lista.prim)
	{
		
		int contor = count(lista,n);
				
		int nr=0;
		PiesaDeTeatru* vector = (PiesaDeTeatru*)malloc(sizeof(PiesaDeTeatru)*contor);
		nod*temp = lista.prim;
		while (temp)
		{
			if (temp->info.nrSpectatori > n)
			{
				vector[nr] = temp->info;
				nr++;
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

PiesaDeTeatru citirePiesa(FILE* f)
{
	PiesaDeTeatru piesa;
	char buffer[30];
	fscanf(f, "%s", &buffer);
	piesa.numePiesa = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(piesa.numePiesa, buffer);
	fscanf(f, "%d", &piesa.nrSpectatori);
	return piesa;
}

PiesaDeTeatru citire()
{
	PiesaDeTeatru piesa;
	char buffer[30];
	scanf("%s", &buffer);
	piesa.numePiesa = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(piesa.numePiesa, buffer);
	scanf("%d", &piesa.nrSpectatori);
	return piesa;
}

LDI initializare()
{
	LDI lista;
	lista.prim = lista.ultim = NULL;
	return lista;
}

void afisareLista(LDI lista)
{
	if (lista.prim)
	{
		nod* temp = lista.prim;
		while (temp)
		{
			printf("%s - %d\n", temp->info.numePiesa, temp->info.nrSpectatori);
			temp = temp->next;
		}
	}
	else
	{
		printf("Lista este vida!");
	}
}

void afisareSfarsit(LDI lista)
{
	if (lista.prim)
	{
		nod* temp = lista.ultim;
		while (temp)
		{
			printf("%s - %d\n", temp->info.numePiesa, temp->info.nrSpectatori);
			temp = temp->prev;
		}
	}
	else
	{
		printf("Lista este vida!");
	}
}

void main()
{
	LDI lista = initializare();
	FILE* f = fopen("Fisier.txt", "r");
	int nr = 0;
	fscanf(f, "%d", &nr);
//	for (int i = 0; i < nr; i++)
//	{
//		/*lista = inserareInceput(lista, citirePiesa(f));*/
//		lista = inserareSfarsit(lista, citirePiesa(f));
//	}
//	afisareLista(lista);
//	/*printf("\n\n");
//	lista=inserarePozitie(lista, citire(),4);*/
//	afisareLista(lista);
//	printf("\n\n");
//	/*lista = stergereElement(lista, 23);*/
//	/*lista = stergerePozitie(lista, 3);
//	afisareSfarsit(lista);*/
//	/*lista=stergere(lista);
//	printf("\n\n");
//	afisareLista(lista);
//*/
//	/*afisareNr(lista, "Fredo");*/
//
//	
//	int n = count(lista,270);
//	PiesaDeTeatru* piesa = (PiesaDeTeatru*)malloc(sizeof(PiesaDeTeatru)*n);
//	piesa = afisareVector(lista, 270);
//	for (int i = 0; i < n; i++)
//		printf("%s - %d\n", piesa[i].numePiesa, piesa[i].nrSpectatori);
//	
	
	//lista dubla circulara
	nod* lista1 = NULL;
	for (int i = 0; i < nr; i++)
		{
			/*lista1 = inserareInceputCircualara(lista1, citirePiesa(f));*/
	       	lista1 = inserareSfarsitCirculara(lista1, citirePiesa(f));
		}
	afisareCirculara(lista1);
	lista1 = inserarePozitieCirculara(lista1, citire(), 4);
	afisareCirculara(lista1);
	//int c = calculeazaNr(lista1, "Fredo");
	//printf("\n%d", c);
	stergereListaCirculara(lista1);
	afisareCirculara(lista1);
	/*printf("\n\n");
	afisareCicularaSfarsit(lista1);*/
}
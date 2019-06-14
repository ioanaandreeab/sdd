#include<iostream>

struct Concert
{
	float pretBilet;
	char* locatie;
	int nrLocuri;
};

struct nod
{
	Concert info;
	nod* next;
};

Concert citireConcert()
{
	Concert c;
	printf("Dati locatia: ");
	char buffer[30];
	scanf("%s", &buffer);
	c.locatie = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(c.locatie, buffer);
	printf("Dati nr de locuri: ");
	scanf("%d", &c.nrLocuri);
	printf("Dati pretul biletului: ");
	scanf("%f", &c.pretBilet);
	return c;
}

Concert deepCopy(Concert c)
{
	Concert temp;
	temp.locatie = (char*)malloc(sizeof(char)*(strlen(c.locatie) + 1));
	strcpy(temp.locatie, c.locatie);
	temp.nrLocuri = c.nrLocuri;
	temp.pretBilet = c.pretBilet;
	return temp;
}

nod* inserareInceput(nod* inceput, Concert c)
{
	nod* nodNou = (nod*)malloc(sizeof(nod));
	nodNou->next = inceput;
	nodNou->info = deepCopy(c);
	return nodNou;
}

nod* inserareSfarsit(nod* inceput, Concert c)
{
	nod*nodNou = (nod*)malloc(sizeof(nod));
	nodNou->next = NULL;
	nodNou->info = deepCopy(c);
	nod* temp = inceput;
	if (inceput)
	{
		while (inceput->next)
		{
			inceput = inceput->next;
		}
		inceput->next = nodNou;
		return temp;

	}
	else
	{
		return nodNou;
	}
}

void afisareLista(nod* inceput)
{
	while (inceput)
	{
		printf("%s - %d - %5.2f\n", inceput->info.locatie, inceput->info.nrLocuri, inceput->info.pretBilet);
		inceput = inceput->next;
	}
}

nod* stergereLista(nod* inceput)
{
	while (inceput)
	{
		free(inceput->info.locatie);
		nod* temp = inceput->next;
		free(inceput);
		inceput = temp;
	}
	return NULL;
}

nod* inserarePePozitie(int pozitie, nod* inceput, Concert c)
{
	if (pozitie == 1)
	{
		inserareInceput(inceput, c);
		return inceput;
	}
	else
	{
		if (inceput)
		{
			int contor = 1;
			nod* temp = inceput;
			while (inceput->next&&contor < pozitie - 1)
			{
				contor++;
				inceput = inceput->next;
			}
			inceput->next = inserareInceput(inceput->next, c);
			return temp;

		}
		else
		{
			nod* nodNou = (nod*)malloc(sizeof(nod));
			nodNou->info = deepCopy(c);
			nodNou->next = NULL;
			return nodNou;
		}
	}
}


nod* stergereElement(nod* inceput, float pret)
{
	if (inceput)
	{
		if (inceput->info.pretBilet == pret)
		{
			nod* temp = inceput;
			inceput = inceput->next;
			free(temp->info.locatie);
			free(temp);
		}
		else
		{
			nod* temp = inceput;
			while (temp->next && temp->next->info.pretBilet != pret)
				temp = temp->next;
			
			if (temp->next)
			{
				nod* aux = temp->next;
				temp->next = temp->next->next;
				free(aux->info.locatie);
				free(aux);
			}
		}

	}
	return inceput;
}

nod* stergerePozitie(nod* inceput, int pozitie)
{
	if (inceput && pozitie>0) 
		
	{	
	    if (pozitie == 1)
	   {
		nod* temp = inceput;
		inceput = inceput->next;
		free(temp->info.locatie);
		free(temp);
		
	   }
	
		else
		{
			nod* temp = inceput;
			int contor = 1;
			while (temp->next && contor < pozitie - 1)
			{
				temp = temp->next;
				contor++;
			}
			if (temp->next)
			{
				nod* aux = temp->next;
				temp->next = temp->next->next;
				free(aux->info.locatie);
				free(aux);
			}
		}
	}
	return inceput;
	
}

void afisareInversa(nod* inceput)
{
	if (inceput)
	{
		afisareInversa(inceput->next);
		printf("%s - %d - %5.2f\n", inceput->info.locatie, inceput->info.nrLocuri, inceput->info.pretBilet);
	}

}

nod* cautare(nod* inceput, int nr)
{
	if (inceput)
	{		
		if (inceput->info.nrLocuri == nr)
		{
			printf("%s - %d - %5.2f\n", inceput->info.locatie, inceput->info.nrLocuri, inceput->info.pretBilet);
			
		}

		else
		{
			nod* temp = inceput;
			while (temp->next && temp->next->info.nrLocuri != nr)
			{
				temp = temp->next;
			}
			if (temp->next)
			{
				printf("%s - %d - %5.2f\n", temp->next->info.locatie, temp->next->info.nrLocuri, temp->next->info.pretBilet);
				
			}
			
		}
	}
	else
	{
		return NULL;
	}
}

void suma(nod* inceput)
{	
	if (inceput)
	{
		int s = 0;
		nod* temp = inceput;
		while (temp)
		{
			s += temp->info.nrLocuri;
			temp = temp->next;
		}		
		printf("Suma este %d.", s);
	}	
	printf("Lista este vida!");
}

void cautareConcert(nod* inceput, int nr)
{
	if (inceput)
	{
		int contor = 0;
		while (inceput)
		{
			if (inceput->info.nrLocuri == nr)
			{
				contor++;
			}
			inceput = inceput->next;
		}
		printf("Numarul de concerte cu %d locuri este %d.", nr, contor);
	}
}

void main()
{
	nod* lista = NULL;
	//lista = inserareInceput(lista, citireConcert());
	//lista = inserareInceput(lista, citireConcert());
	//afisareLista(lista);
	//stergereLista(lista);

	lista = inserareSfarsit(lista, citireConcert());
	lista = inserareSfarsit(lista, citireConcert());
	lista = inserarePePozitie(4, lista, citireConcert());
	
	printf("\n");
	afisareLista(lista);
	/*printf("\n");
	afisareInversa(lista);
	printf("\n");
	suma(lista);*/
	/*printf("\n");
	cautareConcert(lista, 2);
	printf("\n");*/
	
	/*afisareLista(lista);
	lista=stergereElement(lista, 23);
	lista = stergerePozitie(lista, 2);
	afisareLista(lista);
	stergereLista(lista);*/

}
#include<iostream>

struct Serial
{
	int nr_episoade;
	char titlu[30];
};

struct nod
{
	Serial info;
	nod* next;
};

Serial deepCopy(Serial serial)
{
	Serial temp;
	temp.nr_episoade = serial.nr_episoade;
	strcpy(temp.titlu, serial.titlu);
	return temp;
}

Serial citireFisier(FILE* f)
{
	Serial temp;
	char buffer[30];
	fscanf(f, "%s", &buffer);
	strcpy(temp.titlu, buffer);
	fscanf(f, "%d", &temp.nr_episoade);
	return temp;
}

nod* inserareCoada(nod* inceput, Serial serial)
{
	nod* nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = deepCopy(serial);
	nodNou->next = NULL;

	if (inceput)
	{
		nod* temp = inceput;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = nodNou;
		return inceput;
	}
	else
	{
		return nodNou;
	}
}

Serial* inserareCoadaVector(Serial* vector, int* dim, Serial serial)
{
	(*dim)++;
	Serial* temp = (Serial*)malloc(sizeof(Serial)*(*dim));
	for (int i = 0; i < (*dim) - 1; i++)
	{
		temp[i] = vector[i];
	}
	temp[(*dim)-1] = deepCopy(serial);
	free(vector);
	return temp;
}

Serial extragereCoadaVector(Serial** vector, int *dim)
{
	if ((*dim) > 0)
	{
		(*dim)--;
		Serial* temp = (Serial*)malloc(sizeof(Serial)*(*dim));
		for (int i = 0; i < (*dim); i++)
		{
			temp[i] = (*vector)[i + 1];
		}
		Serial rezultat = (*vector)[0];
		free(*vector);
		*vector = temp;
		return rezultat;
	}
	else
	{
		throw "Stiva este vida!";
	}
}

Serial extragereCoada(nod** inceput)
{
	if (*inceput)
	{
		Serial rezultat=(*inceput)->info;
		nod* temp = *inceput;
		(*inceput) = (*inceput)->next;
		free(temp);
		return rezultat;
	}
	else
	{
		printf("Coada este vida!");
	}
}

void afisareSerial(Serial serial)
{	
	printf("%s - %d\n", serial.titlu, serial.nr_episoade);
}

void afisareCoadaVector(Serial** seriale, int*dim)
{
	while ((*dim) > 0)
	{
		Serial serial = extragereCoadaVector(seriale, dim);
		afisareSerial(serial);
	}
}

void afisareCoada(nod** inceput)
{
	if (*inceput)
	{
		while (*inceput)
		{
			afisareSerial(extragereCoada(inceput));
		}
	}
	else
	{
		printf("Coada este nula!");
	}
}

void main()
{
	/*nod* coada = NULL;
	FILE* f = fopen("Fisier.txt", "r");
	int nr = 0;
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++)
	{
		coada = inserareCoada(coada, citireFisier(f));
	}
	afisareCoada(&coada);*/

	Serial* coada1 = NULL;
	int dim = 0;
	FILE* f = fopen("Fisier.txt", "r");
	int nr = 0;
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++)
	{
		coada1 = inserareCoadaVector(coada1, &dim, citireFisier(f));
	}
	afisareCoadaVector(&coada1,&dim);
}
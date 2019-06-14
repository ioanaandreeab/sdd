#include<iostream>

struct Serial
{
	int nr_episoade;
	char titlu[10];
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

nod* inserareStiva(nod* inceput, Serial serial)
{
	nod* nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = deepCopy(serial);
	nodNou->next = NULL;

	if (inceput)
	{		
		nodNou->next = inceput;
		return nodNou;
	}

	else
	{
		return nodNou;
	}
	
}

Serial* inserareStivaVector(Serial* vector, int *dim, Serial serial)
{
	(*dim)++;
	Serial *temp = (Serial*)malloc(sizeof(Serial)*(*dim));
	for (int i = 0; i < (*dim)-1; i++)
	{
		temp[i+1] = vector[i];
	}
	temp[0] = deepCopy(serial);
	free(vector);
	return temp;
}

Serial extragereStivaVector(Serial **vector, int *dim)
{
	if ((*dim) > 0)
	{
		(*dim)--;
		Serial *temp = (Serial*)malloc(sizeof(Serial)*(*dim));
		for (int i = 0; i < (*dim); i++)
		{
			temp[i] = (*vector)[i+1];
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


Serial extragereStiva(nod** inceput)
{
	if (*inceput)
	{
		Serial rezultat = (*inceput)->info;
		nod* temp = *inceput;
		(*inceput) = (*inceput)->next;
		free(temp);
		return rezultat;
	}
	else
	{
		printf( "Stiva este vida!");
	}
}

void afisareSerial(Serial serial)
{
	printf("%s - %d\n", serial.titlu, serial.nr_episoade);
}

void afisareStiva(nod** inceput)
{
	if (*inceput)
	{
		while (*inceput)
		{
			afisareSerial(extragereStiva(inceput));
		}
	}
	else
	{
		printf("Stiva este nula!");
	}
}

void afisareStivaVector(Serial **seriale, int* dim)
{
	while (*dim > 0)
	{
		Serial serial = extragereStivaVector(seriale, dim);
		afisareSerial(serial);
	}
}



void main()
{
	/*nod* stiva = NULL;
	FILE* f = fopen("Fisier.txt", "r");
	int nr = 0;
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++)
	{
		stiva = inserareStiva(stiva, citireFisier(f));
	}
	afisareStiva(&stiva);
	printf("\n\n");*/

	Serial* stiva1 = NULL;
	int dim = 0;
	FILE* f = fopen("Fisier.txt", "r");
	int nr = 0;
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++)
	{
		stiva1 = inserareStivaVector(stiva1,&dim,citireFisier(f));
	}
	
	afisareStivaVector(&stiva1, &dim);
}
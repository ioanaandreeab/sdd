#include<iostream>

struct Imobil
{
	unsigned int idImobil;
	char* numeStrada;
	unsigned int nrStrada;
	int nrApartamente;
	int* vectorLocatari;
	float valoareTotala;
};

struct nod
{
	Imobil info;
	nod* stanga;
	nod* dreapta;
};

struct Nod
{
	Imobil infoLista;
	Nod* next;

};

Imobil deepCopy(Imobil i)
{
	Imobil temp;
	temp.idImobil = i.idImobil;
	temp.numeStrada = (char*)malloc(sizeof(char)*(strlen(i.numeStrada) + 1));
	strcpy(temp.numeStrada, i.numeStrada);
	temp.nrStrada = i.nrStrada;
	temp.nrApartamente = i.nrApartamente;
	temp.vectorLocatari = (int*)malloc(sizeof(int)*temp.nrApartamente);
	for (int j = 0; j < temp.nrApartamente; j++)
		temp.vectorLocatari[j] = i.vectorLocatari[j];
	temp.valoareTotala = i.valoareTotala;
	return temp;
}

Imobil citireFisier(FILE* f)
{
	Imobil imobil;
	fscanf(f, "%d", &imobil.idImobil);
	char buffer[30];
	fscanf(f, "%s", &buffer);
	imobil.numeStrada = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(imobil.numeStrada, buffer);
	fscanf(f, "%d", &imobil.nrStrada);
	fscanf(f, "%d", &imobil.nrApartamente);
	imobil.vectorLocatari = (int*)malloc(sizeof(int)*imobil.nrApartamente);
	for (int i = 0; i < imobil.nrApartamente; i++)
		fscanf(f, "%d", &imobil.vectorLocatari[i]);
	fscanf(f, "%f", &imobil.valoareTotala);
	return imobil;
}

int inaltimeArbore(nod* radacina)
{
	if (radacina)
	{
		int hs = inaltimeArbore(radacina->stanga);
		int hd = inaltimeArbore(radacina->dreapta);
		if (hs > hd)
		{
			return hs + 1;
		}
		else
		{
			return hd + 1;
		}
	}
	else
	{
		return NULL;
	}
}

int FE(nod* radacina)
{
	if (radacina)
	{
		return inaltimeArbore(radacina->stanga) - inaltimeArbore(radacina->dreapta);
	}
	else
	{
		return 0;
	}
}

nod* rotireStanga(nod* radacina)
{
	if (radacina)
	{
		nod* pivot = radacina->dreapta;
		radacina->dreapta = pivot->stanga;
		pivot->stanga = radacina;
		return pivot;
	}
	else
	{
		return NULL;
	}
}

nod* rotireDreapta(nod* radacina)
{
	if (radacina)
	{
		nod* pivot = radacina->stanga;
		radacina->stanga = pivot->dreapta;
		pivot->dreapta = radacina;
		return pivot;
	}
	else
	{
		return NULL;
	}
}

nod* inserareAVL(nod* radacina, Imobil i)
{
	if (radacina)
	{
		if (i.idImobil < radacina->info.idImobil)
			radacina->stanga = inserareAVL(radacina->stanga, i);
		else
			radacina->dreapta = inserareAVL(radacina->dreapta, i);

		if (FE(radacina) == -2)
		{
			if (FE(radacina->dreapta) == 1)
			{
				radacina->dreapta = rotireDreapta(radacina->dreapta);
			}		
			radacina = rotireStanga(radacina);
			
		}
		if (FE(radacina) == 2)
		{
			if (FE(radacina->stanga) == -1)
			{
				radacina->stanga = rotireStanga(radacina->stanga);
			}
			radacina = rotireDreapta(radacina);
		}

		return radacina;
	}

	else
	{
		nod* nodNou = (nod*)malloc(sizeof(nod));
		nodNou->info = deepCopy(i);
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;
		return nodNou;
	}
}

void afisareAVL(nod* radacina)
{
	if (radacina)
	{
		afisareAVL(radacina->stanga);
		printf("%d - %s - %d - %d - ", radacina->info.idImobil, radacina->info.numeStrada, radacina->info.nrStrada, radacina->info.nrApartamente);
		for (int i = 0; i < radacina->info.nrApartamente; i++)
			printf("%d/", radacina->info.vectorLocatari[i]);
		printf(" - %5.2f\n", radacina->info.valoareTotala);
		afisareAVL(radacina->dreapta);
	}
}

int nrImobile(nod* radacina, char* nume)
{
	if (radacina)
	{
		if (strcmp(radacina->info.numeStrada, nume) == 0)
			return 1 + nrImobile(radacina->stanga, nume) + nrImobile(radacina->dreapta, nume);
		else
			return nrImobile(radacina->stanga, nume) + nrImobile(radacina->dreapta, nume);
	}
	else
	{
		return 0;
	}
}


float valoarePersoana(nod* radacina)
{
	if (radacina)
	{
		valoarePersoana(radacina->stanga);

		int nr = 0;
		float val = 0;
		for (int j = 0; j < radacina->info.nrApartamente; j++)
			nr = nr + radacina->info.vectorLocatari[j];
		printf("Valoare de plata pe persoana din imobilul cu codul %d este %5.2f\n", radacina->info.idImobil, radacina->info.valoareTotala / (float)nr);

		valoarePersoana(radacina->dreapta);
	
	}
	else
	{
		return 0;
	}
}

void valoareApartament(nod* radacina, unsigned int cod)
{
	if (radacina)
	{
		if (cod < radacina->info.idImobil)
		{
			valoareApartament(radacina->stanga, cod);
		}
		else if (cod > radacina->info.idImobil)
		{
			valoareApartament(radacina->dreapta, cod);
		}
		else
		{
			float val;
			int nr = 0;
			for (int i = 0; i < radacina->info.nrApartamente; i++)
			{
				nr = nr + radacina->info.vectorLocatari[i];
			}
			for (int j = 0; j < radacina->info.nrApartamente; j++)
			{
				val = (radacina->info.valoareTotala / nr)*radacina->info.vectorLocatari[j];
				printf("Valoarea de plata pentru apartamentul %d din imobilul cu codul %d este %5.2f.\n", j + 1, radacina->info.idImobil, val);
			}		
		}
	}

}

Nod* inserareLista(Nod* lista, Imobil i)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->infoLista = deepCopy(i);
	nou->next = lista;
	return nou;
}

Nod* afisare(nod* radacina, int nr, Nod* lista)
{
	if (radacina)
	{
		nod* temp = radacina;
		if (temp->info.nrApartamente > nr)
			lista = inserareLista(lista, temp->info);

		lista = afisare(temp->stanga, nr, lista);
		lista = afisare(temp->dreapta, nr, lista);
	}
	return lista;
}

nod* stergereFrunze(nod* radacina)
{

	if (radacina)
	{
		if (radacina->stanga == NULL && radacina->dreapta == NULL)
		{
			free(radacina->info.numeStrada);
			free(radacina->info.vectorLocatari);
			free(radacina);
			return NULL;
			
		}
		else
		{
			radacina->stanga = stergereFrunze(radacina->stanga);
			radacina->dreapta = stergereFrunze(radacina->dreapta);
		}

		if (FE(radacina) == -2)
		{
			if (FE(radacina->dreapta) == 1)
			{
				radacina->dreapta = rotireDreapta(radacina->dreapta);
			}
			radacina = rotireStanga(radacina);

		}
		if (FE(radacina) == 2)
		{
			if (FE(radacina->stanga) == -1)
			{
				radacina->stanga = rotireStanga(radacina->stanga);
			}
			radacina = rotireDreapta(radacina);
		}
		return radacina;
	}
	else
	{
		return NULL;
	}
}

void main()
{
	nod* radacina = NULL;
	FILE* f = fopen("Fisier.txt", "r");
	int nr = 0;
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++)
		radacina = inserareAVL(radacina, citireFisier(f));
	afisareAVL(radacina);
	printf("\n\n");
	char* nume = (char*)malloc(sizeof(char)*(strlen("Florilor") + 1));
	strcpy(nume, "Florilor");
	int n = nrImobile(radacina, nume);
	printf("Nr de imobile de pe strada %s este %d.\n\n", nume, n);
	float val = valoarePersoana(radacina);
	printf("\n\n");
	valoareApartament(radacina,100);
	printf("\n\n---------------------------------\n\n");
	Nod* lista = NULL;
	lista = afisare(radacina, 2, lista);
	while (lista)
	{
		printf("%d - %s - %d - %d - ", lista->infoLista.idImobil, lista->infoLista.numeStrada, lista->infoLista.nrStrada, lista->infoLista.nrApartamente);
		for (int i = 0; i < lista->infoLista.nrApartamente; i++)
			printf("%d/", lista->infoLista.vectorLocatari[i]);
		printf(" - %5.2f\n", lista->infoLista.valoareTotala);
		lista = lista->next;
	}

	printf("\n\n---------------------------------\n\n");
	radacina = stergereFrunze(radacina);
	afisareAVL(radacina);
}
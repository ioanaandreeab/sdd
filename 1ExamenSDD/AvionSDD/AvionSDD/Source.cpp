#include<iostream>

struct Avion
{
	unsigned int cod_avion;
	char* model_avion;
	int nr_calatori;
	int* pret;
};

struct nod
{
	Avion info;
	nod* next;
	nod* stanga;
	nod* dreapta;
};

struct HashTable
{
	nod** vector;
	int dim;
};

HashTable initializare(int dimensiune)
{
	HashTable tabela;
	tabela.dim = dimensiune;
	tabela.vector = (nod**)malloc(sizeof(nod*)*dimensiune);
	for (int i = 0; i < dimensiune; i++)
		tabela.vector[i] = NULL;
	return tabela;
}

int functieHash(unsigned int cod_avion, HashTable hashtable)
{
	return cod_avion%hashtable.dim;
}

Avion deepCopy(Avion avion)
{
	Avion temp;
	temp.cod_avion = avion.cod_avion;
	temp.model_avion = (char*)malloc(sizeof(char)*(strlen(avion.model_avion) + 1));
	strcpy(temp.model_avion, avion.model_avion);
	temp.nr_calatori = avion.nr_calatori;
	temp.pret = (int*)malloc(sizeof(int)*temp.nr_calatori);
	for (int i = 0; i < temp.nr_calatori; i++)
		temp.pret[i] = avion.pret[i];
	return temp;
}

nod* inserareLista(nod* lista, Avion avion)
{
	nod*nou = (nod*)malloc(sizeof(nod));
	nou->next = NULL;
	nou->info = deepCopy(avion);
	if (lista)
	{
		nod* temp = lista;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
	else
	{
		return nou;
	}
	//return lista;
}

HashTable inserareAvion(HashTable hashtable, Avion avion)
{
	if (hashtable.dim > 0)
	{
		int codHash = functieHash(avion.cod_avion, hashtable);
		hashtable.vector[codHash] = inserareLista(hashtable.vector[codHash],avion);

	}
	else
	{
		hashtable = initializare(5);
		hashtable = inserareAvion(hashtable, avion);
	}
	return hashtable;
}

Avion citireAvion(FILE* f)
{
	Avion avion;
	fscanf(f, "%d", &avion.cod_avion);
	char buffer[100];
	fscanf(f, "%s", &buffer);
	avion.model_avion = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(avion.model_avion, buffer);
	fscanf(f, "%d", &avion.nr_calatori);
	avion.pret = (int*)malloc(sizeof(int)*avion.nr_calatori);
	for (int i = 0; i < avion.nr_calatori; i++)
		fscanf(f, "%d", &avion.pret[i]);
	return avion;
}

void afisareAvion(Avion avion)
{
	printf("%d - %s - %d\t", avion.cod_avion, avion.model_avion, avion.nr_calatori);
	for (int i = 0; i < avion.nr_calatori; i++)
		printf("%d\t", avion.pret[i]);
}

void afisareLista(nod* lista)
{
	while (lista)
	{
		afisareAvion(lista->info);
		lista = lista->next;
	}
	printf("\n");

}

void afisareTabela(HashTable hashtable)
{
	for (int i = 0; i < hashtable.dim; i++)
		afisareLista(hashtable.vector[i]);
}

//arbore binar de cautare
nod* inserareArbore(nod* radacina, Avion avion)
{
	if (radacina)
	{
		if (avion.cod_avion > radacina->info.cod_avion)
		{
			radacina->stanga = inserareArbore(radacina->stanga, avion);
		}
		else
		{
			radacina->dreapta = inserareArbore(radacina->dreapta, avion);
		}
		return radacina;
	}
	else
	{
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->info = deepCopy(avion);
		nou->dreapta=NULL;
		nou->stanga = NULL;
		return nou;
	}
}

void afisareArbore(nod* radacina)
{
	if (radacina){
		afisareArbore(radacina->stanga);
		printf("%d - %s - %d\t", radacina->info.cod_avion, radacina->info.model_avion, radacina->info.nr_calatori);
		for (int i = 0; i < radacina->info.nr_calatori; i++)
			printf("%d\t", radacina->info.pret[i]);
		printf("\n");
		afisareArbore(radacina->dreapta);
	}
}

nod* stergereArbore(nod* radacina)
{
	if (radacina)
	{
		radacina->stanga=stergereArbore(radacina->stanga);
		radacina->dreapta=stergereArbore(radacina->dreapta);
		free(radacina->info.model_avion);
		free(radacina);
		return NULL;
	}
}

nod* stergereLista(nod* radacina)
{
	if (radacina)
	{
		free(radacina->info.model_avion);
		free(radacina);
		nod* temp=radacina;
		radacina = radacina->next;
		free(temp);
	}
	return NULL;
}

HashTable  stergeretabela(HashTable hashtable)
{
	for (int i = 0; i < hashtable.dim; i++)
		hashtable.vector[i] = stergereLista(hashtable.vector[i]);
	free(hashtable.vector);
	hashtable.dim = 0;
	hashtable.vector = NULL;
	return hashtable;
}

Avion returnareAvionCod(nod* radacina, int cod)
{
	if (radacina)
	{
		if (radacina->info.cod_avion == cod)
			return radacina -> info;
		else if (radacina->info.cod_avion < cod)
			return returnareAvionCod(radacina->dreapta, cod);
		else if (radacina->info.cod_avion>cod)
			return returnareAvionCod(radacina->stanga, cod);
	}
}

nod* stergerenod(nod* radacina, int cod)
{
	if (radacina)
	{
		if (radacina->info.cod_avion == cod)
		{
			radacina=stergerenod(radacina,cod);			
		}
		else if (radacina->info.cod_avion < cod)
		{
			radacina->dreapta = stergerenod(radacina->dreapta, cod);
		}
		else if (radacina->info.cod_avion>cod)
		{
			radacina = stergerenod(radacina->stanga, cod);		
		}
		free(radacina->info.model_avion);
		
	}
	return radacina;
}
Avion returnare(HashTable hashtable, int cod_avion, int nr)
{
	Avion negasit;
	negasit.cod_avion = -1;
	if (cod_avion < 0) return negasit;
	if (hashtable.vector)
	{
		int codHash = functieHash(cod_avion, hashtable);
		if (hashtable.vector[codHash] = NULL)
		{
			return negasit;
		}
		else
		{
			if (hashtable.vector[codHash]->info.cod_avion == cod_avion)
			{
				if (hashtable.vector[codHash]->info.nr_calatori > nr)
				{
					return hashtable.vector[codHash]->info;
				}
			}
			else
			{
				nod* nou = hashtable.vector[codHash];
				while (nou != NULL && nou->info.cod_avion != cod_avion)
					nou = nou->next;
				if (nou == NULL)
					return negasit;
				else
					return nou -> info;
			}
		}
	}
	return negasit;
}

void main()
{
	nod* radacina = NULL;
	HashTable hashtable = initializare(5);
	FILE* f = fopen("avioane.txt", "r");
	int nrAvioane;
	fscanf(f, "%d", &nrAvioane);
	for (int i = 0; i < nrAvioane; i++)
	{
		hashtable = inserareAvion(hashtable, citireAvion(f));
	}
	afisareTabela(hashtable);
	//for (int i = 0; i < nrAvioane; i++)
	//	radacina = inserareArbore(radacina, citireAvion(f));
	///*afisareArbore(radacina);
	//fclose(f);*/
	///*Avion a = returnareAvionCod(radacina, 2);
	//afisareAvion(a);
	//Avion a1 = returnare(hashtable, 2, 3);
	//afisareAvion(a1);*/
	//
	//stergerenod(radacina, 2);
	//afisareArbore(radacina);
}
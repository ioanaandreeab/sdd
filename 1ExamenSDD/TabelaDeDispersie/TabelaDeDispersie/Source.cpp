#include<iostream>

struct Reteta
{
	unsigned int nrRetata;
	int nrMedicamentePrescrise;
	char** listaMedicamente;
	char* numeMedic;
	float valoareBruta;
	float procentCompensare;
};

struct nod
{
	Reteta info;
	nod* next;
};

struct hashTable
{
	nod** vector;
	int dim;
};

hashTable initializare(int dimensiune)
{
	hashTable tabela;
	tabela.dim = dimensiune;
	tabela.vector = (nod**)malloc(sizeof(nod*)*dimensiune);
	for (int i = 0; i < dimensiune; i++)
		tabela.vector[i] = NULL;
	return tabela;
}

Reteta deepCopy(Reteta r)
{
	Reteta temp;
	temp.nrRetata = r.nrRetata;
	temp.nrMedicamentePrescrise = r.nrMedicamentePrescrise;
	temp.listaMedicamente = (char**)malloc(sizeof(char*)*r.nrMedicamentePrescrise);
	for (int i = 0; i < r.nrMedicamentePrescrise; i++)
	{
		temp.listaMedicamente[i] = (char*)malloc(sizeof(char)*(strlen(r.listaMedicamente[i]) + 1));
		strcpy(temp.listaMedicamente[i], r.listaMedicamente[i]);
	}
	temp.numeMedic = (char*)malloc(sizeof(char)*(strlen(r.numeMedic) + 1));
	strcpy(temp.numeMedic, r.numeMedic);
	temp.valoareBruta = r.valoareBruta;
	temp.procentCompensare = r.procentCompensare;
	return temp;
}

Reteta citireFisier(FILE*f)
{
	Reteta r;
	fscanf(f, "%d", &r.nrRetata);
	fscanf(f, "%d", &r.nrMedicamentePrescrise);
	char buffer[30];
	r.listaMedicamente = (char**)malloc(sizeof(char*)*r.nrMedicamentePrescrise);
	for (int i = 0; i < r.nrMedicamentePrescrise; i++)
	{
		fscanf(f, "%s", &buffer);
		r.listaMedicamente[i] = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
		strcpy(r.listaMedicamente[i], buffer);
	}
	fscanf(f, "%s", &buffer);
	r.numeMedic = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(r.numeMedic, buffer);
	fscanf(f, "%f", &r.valoareBruta);
	fscanf(f, "%f", &r.procentCompensare);
	return r;
}

int functieHash(unsigned int nrReteta, hashTable tabela)
{
	return nrReteta%tabela.dim;
}

nod* inserareLista(nod* lista, Reteta r)
{
	nod* nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = deepCopy(r);
	nodNou->next = NULL;
	if (lista)
	{
		nod* temp = lista;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = nodNou;
	}
	else
	{
		lista = nodNou;
	}
	return lista;
}

hashTable inserareReteta(hashTable tabela, Reteta r)
{
	if (tabela.dim > 0)
	{
		int codHash = functieHash(r.nrRetata, tabela);
		tabela.vector[codHash] = inserareLista(tabela.vector[codHash], r);
	}
	else
	{
		tabela = initializare(10);
		tabela = inserareReteta(tabela, r);
	}
	return tabela;
}

void afisareLista(nod* lista)
{
	while (lista)
	{
		printf("%d - %d - ", lista->info.nrRetata, lista->info.nrMedicamentePrescrise);
		for (int i = 0; i < lista->info.nrMedicamentePrescrise;i++)
			printf("%s/", lista->info.listaMedicamente[i]);
		printf(" - %s -%5.2f - %5.2f\t", lista->info.numeMedic, lista->info.valoareBruta, lista->info.procentCompensare);
		lista = lista->next;
	}
	printf("\n");
}

void afisareTabela(hashTable tabela)
{
	for (int i = 0; i < tabela.dim; i++)
		afisareLista(tabela.vector[i]);
}

void validareMedicament(hashTable tabela, int cod, char* medicament)
{
	if (tabela.dim>0)
	{
		for (int i = 0; i < tabela.dim; i++)
		{
			nod* temp = tabela.vector[i];
			while (temp)
			{
				if (temp->info.nrRetata == cod)
				{
					int ok = 0;
					for (int j = 0; j < temp->info.nrMedicamentePrescrise; j++)
					{
						
						if (strcmp(temp->info.listaMedicamente[j], medicament) == 0)
						{
							ok = 1;
							printf("Medicamentul a fost prescris!");
						}
					}
						if (ok == 0)
						{
							printf("Medicamentul nu a fost prescris!");
						}										
				}
				temp = temp->next;
			}
		}
	}
}

void calculareNeta(hashTable tabela)
{
	if (tabela.dim > 0)
	{
		for (int i = 0; i < tabela.dim; i++)
		{
			nod* temp = tabela.vector[i];
			while (temp)
			{
				float valoare = 0;
				valoare = temp->info.valoareBruta - (temp->info.valoareBruta*temp->info.procentCompensare);
				printf("Valoarea neta pentru reteta %d este %5.2f\n", temp->info.nrRetata, valoare);
				temp = temp->next;
			}
		}
	}
}

float suma(hashTable tabela)
{
	if (tabela.dim > 0)
	{
		float suma = 0;
		for (int i = 0; i < tabela.dim; i++)
		{
			nod* temp = tabela.vector[i];
			while (temp)
			{
				float valoare = 0;
				valoare = temp->info.valoareBruta - (temp->info.valoareBruta*temp->info.procentCompensare);
				suma = suma + valoare;
				temp = temp->next;
			}
		}
		return suma;
	}
}

hashTable modifica(hashTable tabela, char* nume, char* numeNou)
{
	if (tabela.dim > 0)
	{
		for (int i = 0; i < tabela.dim; i++)
		{
			nod* temp = tabela.vector[i];
			while (temp)
			{
				if (strcmp(temp->info.numeMedic, nume) == 0)
				{
					free(temp->info.numeMedic);
					temp->info.numeMedic = (char*)malloc(sizeof(char)*(strlen(numeNou) + 1));
					strcpy(temp->info.numeMedic, numeNou);
				}
				temp = temp->next;
			}
		}
		return tabela;
	}
}

hashTable modificaCod(hashTable tabela,int cod)
{
	if (tabela.dim > 0)
	{
		for (int i = 0; i < tabela.dim; i++)
		{
			nod* temp = tabela.vector[i];
			while (temp)
			{
				if (temp->info.nrRetata==cod)
				{
					free(temp->info.numeMedic);
					temp->info.numeMedic = (char*)malloc(sizeof(char)*(strlen("Andrei") + 1));
					strcpy(temp->info.numeMedic, "Andrei");
				}
				temp = temp->next;
			}
		}
		return tabela;
	}
}

void nrRetete(hashTable tabela,char* nume)
{
	if (tabela.dim > 0)
	{
		int contor = 0;
		for (int i = 0; i < tabela.dim; i++)
		{
			nod*temp = tabela.vector[i];
			
			while (temp)
			{
				if (strcmp(temp->info.numeMedic, nume) == 0)
				{
					contor++;
				}
				temp = temp->next;				
			}
			
		}
		printf("Nr de retete prescrise de medicul cu numele %s este %d.", nume, contor);
	}
}


void reteteFiecare(hashTable tabela)
{
	char** medici = (char**)malloc(sizeof(char*)*tabela.dim);
	int nrMedici = 0;

	if (tabela.dim > 0)
	{
		
		for (int i = 0; i < tabela.dim; i++)
		{	
				nod*temp = tabela.vector[i];				

				while (temp)
				{
					int ok = 0;
					for (int j = 0; j < nrMedici;j++)
					if (strcmp(medici[j], temp->info.numeMedic) == 0)
					{
						ok = 1;
					}
					if (ok == 0)
					{
						medici[nrMedici] = (char*)malloc(sizeof(char)*(strlen(temp->info.numeMedic) + 1));
						strcpy(medici[nrMedici], temp->info.numeMedic);
						nrMedici++;
					}
					temp = temp->next;
				}			
		}
			
		for (int i = 0; i < nrMedici; i++)
		{
			int contor = 0;
			for (int j = 0; j < tabela.dim; j++)
			{
				
				nod*temp = tabela.vector[j];
				while (temp)
				{
					
					if (strcmp(medici[i], temp->info.numeMedic) == 0)
					{
						contor++;
					}					
					temp = temp->next;
				
				}
			
			}
			printf("Medicul cu numele %s are %d retete.\n", medici[i], contor);
		}
	}
}

void afisareFisier(hashTable tabela)
{
	FILE*f = fopen("scriere.txt", "w");
	if (tabela.dim > 0)
	{
		for (int i = 0; i < tabela.dim; i++)
		{
			nod* lista = tabela.vector[i];
			while (lista)
			{
				fprintf(f,"%d - %d - ", lista->info.nrRetata, lista->info.nrMedicamentePrescrise);
				for (int i = 0; i < lista->info.nrMedicamentePrescrise; i++)
					fprintf(f,"%s/", lista->info.listaMedicamente[i]);
				fprintf(f," - %s -%5.2f - %5.2f\n", lista->info.numeMedic, lista->info.valoareBruta, lista->info.procentCompensare);

				lista = lista->next;
			}
		}
		fclose(f);
	}
}

void vectorFunctie(hashTable tabela, char* nume, int nr)
{
	if (tabela.dim > 0)
	{
		for (int i = 0; i < tabela.dim; i++)
		{
			nod* temp = tabela.vector[i];
			
			while (temp)
			{
				if (strcmp(temp->info.numeMedic, nume) == 0 && temp->info.nrMedicamentePrescrise == nr)
				{
					printf("%d - %d - ", temp->info.nrRetata, temp->info.nrMedicamentePrescrise);
					for (int j = 0; j < temp->info.nrMedicamentePrescrise; j++)
						printf("%s/", temp->info.listaMedicamente[j]);
					printf(" - %s - %5.2f - %5.2f\n", temp->info.numeMedic, temp->info.valoareBruta, temp->info.procentCompensare);
				
				}
				temp = temp->next;
			}
			
		}
	
	}
}

nod* stergereLista(nod* inceput)
{
	while (inceput)
		{
			free(inceput->info.numeMedic);
			for (int i = 0; i < inceput->info.nrMedicamentePrescrise; i++)
				free(inceput->info.listaMedicamente[i]);
			free(inceput->info.listaMedicamente);
			nod* temp = inceput;
			inceput = inceput->next;
			free(temp);
		}

	return NULL;	
}

hashTable stergereTabela(hashTable tabela)
{
		for (int i = 0; i < tabela.dim; i++)
			tabela.vector[i] = stergereLista(tabela.vector[i]);
		free(tabela.vector);
		tabela.dim = 0;
		tabela.vector = NULL;
		return tabela;
	
}

void main()
{
	hashTable tabela = initializare(10);
	FILE * f = fopen("Fisier.txt", "r");
	int nr = 0;
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++)
	{
		tabela = inserareReteta(tabela, citireFisier(f));
	}
	fclose(f);
	afisareTabela(tabela);
	printf("\n");
	validareMedicament(tabela, 100, "Nurofen");
	printf("\n\n");
	calculareNeta(tabela);
	printf("\n\n");
	nrRetete(tabela, "Ionescu");
	afisareFisier(tabela);
	printf("\n\n");
	float a = suma(tabela);
	printf("Valoarea neta a tuturor retetelor este %5.2f.\n\n", a);
	tabela = modifica(tabela, "Popescu", "Ionescu");
	afisareTabela(tabela);
	printf("\n\n");
	tabela = modificaCod(tabela, 400);
	afisareTabela(tabela);
	printf("\n\n");	
	vectorFunctie(tabela, "Ionescu", 2);
	printf("\n\n");
	reteteFiecare(tabela);
	printf("\n\n");
	tabela = stergereTabela(tabela);
	afisareTabela(tabela);
	
}
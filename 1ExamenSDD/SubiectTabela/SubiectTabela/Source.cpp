#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define DIM 100
#define LINESIZE 256

struct Depozit {
	char* idDepozit;
	int capacitate, nrTipuriCereale;
	char ** listaCereale;
	float procentIncarcare;
};

struct Nod {
	Depozit depo;
	Nod *next;
};

int calculPozitie(Depozit d, int size) {
	int s = 0;
	for (unsigned int i = 0; i<strlen(d.idDepozit); i++)
		s += d.idDepozit[i];

	return s % size;
}

Nod* inserareLista(Nod *p, Depozit d) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->depo.idDepozit = d.idDepozit;
	nou->depo.capacitate = d.capacitate;
	nou->depo.nrTipuriCereale = d.nrTipuriCereale;
	nou->depo.listaCereale = d.listaCereale;
	nou->depo.procentIncarcare = d.procentIncarcare;

	nou->next = p;

	return nou;
}

Nod* extragere(Nod** tabela, int size, char* cereala)
{
	Nod* lista = NULL;
	for (int i = 0; i<size; i++)
	if (tabela[i]) {
		Nod* temp = tabela[i];
		while (temp) {
			for (int i = 0; i < temp->depo.nrTipuriCereale; i++)
			{
				if (strcmp(temp->depo.listaCereale[i], cereala) == 0)
				{
				   lista = inserareLista(lista, temp->depo);
				}
			}
			temp = temp->next;
		}
	}
	return lista;
}


void inserareHTable(Nod** hTab, Depozit d, int sizeHT) {
	int k;
	k = calculPozitie(d, sizeHT);

	hTab[k] = inserareLista(hTab[k], d);
}

int nrDepozite(Nod** hTab, int size, int c) {
	int s = 0;
	for (int i = 0; i<size; i++)
	if (hTab[i]) {
		Nod* tmp = hTab[i];
		while (tmp) {
			if (tmp->depo.capacitate > c)
			{
				s += 1;
			}
			tmp = tmp->next;
		}
	}

	return s;
}

float capacitate(Nod** hTab, int size)
{
	float s = 0;
	for (int i = 0; i<size; i++)
	if (hTab[i]) {
		Nod* tmp = hTab[i];
		while (tmp)		
		{
				s += (float)tmp->depo.capacitate*tmp->depo.procentIncarcare;
				tmp = tmp->next;
		}
			
		
	}
	return s;
}

Nod** marire(Nod** tabela, int size, char* id)
{
	for (int i = 0; i < size; i++)
	if (tabela[i])
	{
		Nod* temp = tabela[i];
		while (temp)
		{
			if (strcmp(temp->depo.idDepozit, id) == 0)
			{
				temp->depo.procentIncarcare = temp->depo.procentIncarcare + 0.1;
			}
			temp = temp->next;
		}
	}
	return tabela;

}

void afisareDepozit(Depozit d)
{
	printf("%s - %d - %d - ", d.idDepozit, d.capacitate,d.nrTipuriCereale);
	for (int i = 0; i < d.nrTipuriCereale; i++)
		printf("%s/", d.listaCereale[i]);
	printf(" - %5.2f\n", d.procentIncarcare);
}

void afisareLista(Nod* inceput)
{
	Nod* temp = inceput;
	while (temp)
	{
		afisareDepozit(temp->depo);
		temp = temp->next;
	}
}

void afisareHash(Nod** hash, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		if (hash[i])
			afisareLista(hash[i]);
	}
}

void main() {

	Nod **HTable;
	Depozit tmpDepozit;

	HTable = (Nod**)malloc(sizeof(Nod*)* DIM);

	for (int i = 0; i<DIM; i++)
		HTable[i] = 0;

	FILE * f;
	f = fopen("Depozite.txt", "r");

	char *token, file_buf[LINESIZE], sep_list[] = ",";

	while (fgets(file_buf, sizeof(file_buf), f)) {
		token = strtok(file_buf, sep_list);
		tmpDepozit.idDepozit = (char*)malloc((strlen(token) + 1)*sizeof(char));
		strcpy(tmpDepozit.idDepozit, token);

		token = strtok(NULL, sep_list);
		tmpDepozit.capacitate = atoi(token);

		token = strtok(NULL, sep_list);
		tmpDepozit.nrTipuriCereale = atoi(token);

		tmpDepozit.listaCereale = (char**)malloc(tmpDepozit.nrTipuriCereale * sizeof(char*));

		for (int i = 0; i<tmpDepozit.nrTipuriCereale; i++) {
			token = strtok(NULL, sep_list);
			tmpDepozit.listaCereale[i] = (char*)malloc((strlen(token) + 1) * sizeof(char));
			strcpy(tmpDepozit.listaCereale[i], token);
		}

		token = strtok(NULL, sep_list);
		tmpDepozit.procentIncarcare = atof(token);

		inserareHTable(HTable, tmpDepozit, DIM);
	}

	afisareHash(HTable, DIM);
	fclose(f);
	int a = nrDepozite(HTable, DIM, 18);
	printf("\n\nNr de depozite cu capaitatea mai mare decat cea specificata este %d.\n\n", a);
	float b = capacitate(HTable, DIM);
	printf("Cantitatea totala stocata in depozite este %5.2f.\n\n", b);
	HTable = marire(HTable, DIM, "100");
	afisareHash(HTable,DIM);
	printf("\n\n");
	Nod* lista = extragere(HTable, DIM, "grau");
	afisareLista(lista);
}

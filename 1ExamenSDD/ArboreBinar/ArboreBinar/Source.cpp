#include<iostream>

struct Vacanta
{
	int cod;
	char* destinatie;
	int nr_zile;
};

struct nod
{
	Vacanta info;
	nod* dreapta;
	nod* stanga;
};

Vacanta deepCopy(Vacanta v)
{
	Vacanta temp;
	temp.cod = v.cod;
	temp.nr_zile = v.nr_zile;
	temp.destinatie = (char*)malloc(sizeof(char)*(strlen(v.destinatie) + 1));
	strcpy(temp.destinatie, v.destinatie);
	return temp;
}

Vacanta citire(FILE* f)
{
	Vacanta v;
	fscanf(f, "%d", &v.cod);
	char buffer[30];
	fscanf(f, "%s", &buffer);
	v.destinatie = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(v.destinatie, buffer);
	fscanf(f, "%d", &v.nr_zile);
	return v;
}

nod* inserareArbore(nod* radacina, Vacanta v)
{
	if (radacina)
	{
		if (v.cod < radacina->info.cod)
			radacina->stanga = inserareArbore(radacina->stanga, v);
		else
			radacina->dreapta = inserareArbore(radacina->dreapta, v);
		return radacina;
	}
	else
	{
		nod* nodNou = (nod*)malloc(sizeof(nod));
		nodNou->info = deepCopy(v);
		nodNou->dreapta = NULL;
		nodNou->stanga = NULL;
		return nodNou;
	}
}

void afisareInOrdine(nod* radacina)
{
	if (radacina)
	{
		afisareInOrdine(radacina->stanga);
		printf("%d - %s - %d\n", radacina->info.cod, radacina->info.destinatie, radacina->info.nr_zile);
		afisareInOrdine(radacina->dreapta);
	}
}

void afisarePreOrdine(nod* radacina)
{
	if (radacina)
	{
		printf("%d - %s - %d\n", radacina->info.cod, radacina->info.destinatie, radacina->info.nr_zile);
		afisareInOrdine(radacina->stanga);
		afisareInOrdine(radacina->dreapta);
	}
}

void afisarePostOrdine(nod* radacina)
{

	if (radacina)
	{		
		afisareInOrdine(radacina->stanga);
		afisareInOrdine(radacina->dreapta);
		printf("%d - %s - %d\n", radacina->info.cod, radacina->info.destinatie, radacina->info.nr_zile);
	}
}

int inaltimeArbore(nod* radacina)
{
	if (radacina)
	{
		int hs = inaltimeArbore(radacina->stanga);
		int hd = inaltimeArbore(radacina->dreapta);
		int max = (hs > hd) ? hs : hd;
		return max + 1;
	}
	else
	{
		return 0;
	}
}

nod* stergereArbore(nod* radacina)
{
	if (radacina)
	{
		radacina->stanga = stergereArbore(radacina->stanga);
		radacina->dreapta = stergereArbore(radacina->dreapta);
		free(radacina->info.destinatie);
		free(radacina);
		return NULL;
	}
	else
	{
		return NULL;
	}
}

nod* minimDreapta(nod* rad)
{
	if (rad)
	{
		while (rad->stanga)
		{
			rad = rad->stanga; 
		}
		return rad;
	}
}

nod* stergereElement(nod* radacina, int cod)
{
	if (radacina)
	{
		if (radacina->info.cod > cod)
		{
			radacina->stanga = stergereElement(radacina->stanga, cod);
			return radacina;
		}
		else if (radacina->info.cod < cod)
		{
			radacina->dreapta = stergereElement(radacina->dreapta, cod);
			return radacina;
		}
		else 
		{
			if (radacina->stanga == NULL)//nu avem subarbore stang
			{
				nod* temp = radacina->dreapta;
				free(radacina->info.destinatie);
				free(radacina);
				return temp;
			}
			else if (radacina->dreapta = NULL)//nu avem subarbore drept
			{
				nod* temp = radacina->stanga;
				free(radacina->info.destinatie);
				free(radacina);
				return temp;
			}
			else 
			{
				nod* temp = minimDreapta(radacina->dreapta);
				free(radacina->info.destinatie);
				free(radacina);
				return temp;
			
			}
		}
	}
	else
	{
		return NULL;
	}
}

int numarVacante(nod* radacina, int nr )
{
	if (radacina)
	{			
		int ok = 0;
		if (radacina->info.nr_zile < nr)
		{
			ok = 1;
		}
		int contor = 0;
		if (ok == 1)
		{
			contor = 1 + numarVacante(radacina->stanga, nr) + numarVacante(radacina->dreapta,nr);
		}
		else
		{
			contor = numarVacante(radacina->stanga, nr) + numarVacante(radacina->dreapta, nr);
		}
		
		return contor;
	}
	else
	{
		return 0;
	}
}

int vacanteDestinatie(nod* radacina, char* nume)
{
	if (radacina)
	{
		int ok = 0;
		if (strcmp(radacina->info.destinatie, nume) == 0)
		{
			ok = 1;
		}
		int contor = 0;
		if (ok == 1)
		{
			contor = 1 + vacanteDestinatie(radacina->stanga, nume) + vacanteDestinatie(radacina->dreapta, nume);
		}
		else
		{
			contor = vacanteDestinatie(radacina->stanga, nume) + vacanteDestinatie(radacina->dreapta, nume);
		}
		return contor;
	}
	else
	{
		return 0;
	}
}

nod* schimbareDestinatii(nod* radacina, char* nume, char*numeNou)
{
	if (radacina)
	{			
		if (strcmp(radacina->info.destinatie, nume) == 0)
		{
			free(radacina->info.destinatie);
			radacina->info.destinatie = (char*)malloc(sizeof(char)*(strlen(numeNou) + 1));
			strcpy(radacina->info.destinatie, numeNou);
				
		}
			
		radacina->stanga = schimbareDestinatii(radacina->stanga, nume, numeNou);
		radacina->dreapta = schimbareDestinatii(radacina->dreapta, nume, numeNou);

		return radacina;
	
	}
	else
	{
		return NULL;
	}
	
}

nod* stergereDestinatii(nod* radacina, char* nume)
{
	if (radacina)
	{

		if (strcmp(radacina->info.destinatie, nume) == 0)
		{
			if (radacina->stanga == NULL)//nu avem subarbore stang
			{
				nod* temp = radacina->dreapta;
				free(radacina->info.destinatie);
				free(radacina);
				return temp;
			}
			else if (radacina->dreapta = NULL)//nu avem subarbore drept
			{
				nod* temp = radacina->stanga;
				free(radacina->info.destinatie);
				free(radacina);
				return temp;
			}
			else
			{
				nod* temp = minimDreapta(radacina->dreapta);
				free(radacina->info.destinatie);
				free(radacina);
				return temp;
			}
			
		}				
		radacina->stanga = stergereDestinatii(radacina->stanga, nume);
		radacina->dreapta = stergereDestinatii(radacina->dreapta, nume);
		return radacina;

	}
	else
	{
		return NULL;
	}

}

int suma(nod* radacina)
{
	if (radacina)
	{
		int ok = 0;
		int s = 0;
		if (radacina->info.nr_zile > 0)
		{
			ok = 1;
		}
		if (ok == 1)
		{
			s = radacina->info.nr_zile + suma(radacina->stanga) + suma(radacina->dreapta);
		}
		else
		{
			s = suma(radacina->stanga) + suma(radacina->dreapta);
		}
		return s;
	}
	else
	{
		return 0;
	}
}

int TarifTotal(nod* radacina, char* nume)
{
	if (radacina)
	{	
		int tarif = 0;
		int ok = 0;
		if (strcmp(radacina->info.destinatie, nume) == 0)
		{
			ok = 1;
		}		
		if (ok == 1)
		{
			tarif= (radacina->info.nr_zile)*10 + TarifTotal(radacina->stanga, nume) + TarifTotal(radacina->dreapta, nume);
					
		}
		else
		{
			tarif = TarifTotal(radacina->stanga, nume) + TarifTotal(radacina->dreapta, nume);
		
		}
				
		return tarif;

	}
	else
	{
		return 0;
	}
}

void afisareNivel(nod* radacina, int nivel, int nivelCurent)
{
	if (radacina)
	{
		if (nivel == nivelCurent)
		{
			printf("%d - %s - %d\n", radacina->info.cod, radacina->info.destinatie, radacina->info.nr_zile);
		}
		else
		{
			afisareNivel(radacina->stanga, nivel, nivelCurent + 1);
			afisareNivel(radacina->dreapta, nivel, nivelCurent + 1);
		}
	}
}

void calcul(nod* radacina)
{
	if (radacina)
	{
		int a = 0;
		if (radacina->info.cod > 0 && radacina->info.nr_zile > 0)
		{
			a = radacina->info.cod*radacina->info.nr_zile;
			printf("Vacanta cu destinatia %s are valoarea %d.\n", radacina->info.destinatie, a);
		}
		else
		{
			printf("Nu exista valori!");
		}
		calcul(radacina->stanga);	
		calcul(radacina->dreapta);
	}
	
	
}

void main()
{
	nod* radacina = NULL;
	FILE* f = fopen("Fisier.txt", "r");
	int nr = 0;
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++)
	{
		radacina = inserareArbore(radacina, citire(f));
	}
	fclose(f);

	afisareInOrdine(radacina);
	printf("\n");
	afisarePreOrdine(radacina);
	printf("\n");
	afisarePostOrdine(radacina);
	printf("\n");
	int inaltime = inaltimeArbore(radacina);
	printf("Inaltimea arborelui este: %d\n\n", inaltime);
	afisarePreOrdine(radacina);
	printf("\n");
	int c = numarVacante(radacina,7);
	printf("Nr de vacante cu nr de zile mai mic decat cel specificat este: %d.\n\n", c);
	int d = vacanteDestinatie(radacina, "Grecia");
	printf("Nr de vacante oferite de agentie pentru destinatia specficata este: %d.\n\n", d);
	afisareNivel(radacina, 2, 1);
	printf("\n\n");
	calcul(radacina);
	/*int t = TarifTotal(radacina, "Grecia");
	printf("Tariful total este: %d\n\n", t);
	schimbareDestinatii(radacina, "Grecia", "Tunisia");
	afisarePreOrdine(radacina);
	printf("\n\n");
	int s = suma(radacina);
	printf("Numarul total de zile este: %d\n\n", s);
	radacina = stergereElement(radacina, 300);
	afisarePreOrdine(radacina);
	printf("\n\n");
	radacina = stergereDestinatii(radacina, "Danemarca");
    afisarePreOrdine(radacina);
	printf("\n\n");
	radacina = stergereArbore(radacina);
	afisarePreOrdine(radacina);*/

}
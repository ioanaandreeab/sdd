#include<iostream>

//Stiva este o structura omogena s liniara in care (Lifo) adaugarea si extragerea se fac intr-un singur capat
//Coada - FIFO - inserarea se face la un capat si extragerea la celalalt capat

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

nod*inserareCoadaLista(nod* cap, Serial serial)
{
	nod*nou = (nod*)malloc(sizeof(nod));
	nou->info = serial;
	nou->next = NULL;
	if (cap)
	{
		nod*temp = cap;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = nou;
		return cap;
	}
	else
		return nou;
}
Serial extragereCoadaLista(nod**cap)
{
	if (*cap)
	{
		Serial rezultat = (*cap)->info;
		nod*aux = *cap;
		(*cap) = (*cap)->next;
		free(aux);
		return rezultat;
	}
	else
	{
		throw "Coada este goala!";
	}

}

//stiva pentru un vector

//metoda de inserare
Serial *inserareStivaVector(Serial* vector, int *dim, Serial serial)
{
	(*dim)++;
	Serial *temp = (Serial*)malloc(sizeof(Serial)*(*dim)); //malloc aloca spatiu in heap, de aceea putem sa dam direct return temp fara sa alocam alt spatiu
	for (int i = 0; i < (*dim) - 1; i++)
	{
		temp[i + 1] = vector[i];
	}
	temp[0] = serial; //se face shallow copy
	free(vector);
	return temp;
}

//metoda de extragere
Serial extrageStivaVector(Serial **vector, int *dim) //*dim deoarece se modifica dimensiunea, de aceea si vector are 2 *
{
	if ((*dim) > 0)
	{
		(*dim)--;
		Serial *temp = (Serial*)malloc(sizeof(Serial)*(*dim));
		for (int i = 0; i < (*dim) + 1; i++)
		{
			temp[i - 1] = (*vector)[i]; // i se pune in afara ca sa indexeze pozitia corecta
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
Serial creareSerial(int nr_episoade, char* titlu)
{
	Serial serial;
	serial.nr_episoade = nr_episoade;
	strcpy(serial.titlu, titlu);
	return serial;
}

void afisareSerial(Serial serial)
{
	printf("%s - %d\n", serial.titlu, serial.nr_episoade);
}

void afisareStivaVector(Serial **seriale, int* dim)
{
	while (*dim>0)
	{
		Serial serial = extrageStivaVector(seriale, dim);
		afisareSerial(serial);
	}
}

void afisareCoadaLista(nod** cap)
{
	if (*cap)
	{
		while (*cap)
		{
			afisareSerial(extragereCoadaLista(cap));
		}
	}
}

//Vacanta cautareVacantaDupaCod(nod* radacina, int cod)
//{
//	if (radacina)
//	{
//		if (radacina->info.cod == cod)
//			return radacina->info;
//		else if (radacina->info.cod < cod)
//			return cautareVacantaDupaCod(radacina->dr, cod);
//		else if (radacina->info.cod > cod)
//			return cautareVacantaDupaCod(radacina->st, cod);
//	}
//}



void main()
{
	Serial* stiva = NULL;
	int dim = 0;
	stiva = inserareStivaVector(stiva, &dim, creareSerial(4, "PB"));
	stiva = inserareStivaVector(stiva, &dim, creareSerial(6, "GT"));
	stiva = inserareStivaVector(stiva, &dim, creareSerial(8, "PL"));
	afisareStivaVector(&stiva, &dim);
	
	/*nod* coada = NULL;
	coada = inserareCoadaLista(coada, creareSerial(6, "Friends"));
	coada = inserareCoadaLista(coada, creareSerial(4, "PL"));
	coada = inserareCoadaLista(coada, creareSerial(8, "VD"));
	coada = inserareCoadaLista(coada, creareSerial(7, "PB"));
	afisareCoadaLista(&coada)*/;
}
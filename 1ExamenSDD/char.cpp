#include<iostream>
struct Rezervare
{
	unsigned int idRezervare;
	char* denumireHotel;
	unsigned char numarCamereRezervate;
	char* numeClient;
	char* perioadaRezervarii;
	float sumaPlata;
};

struct nod
{
	Rezervare info;
	nod* next;
};

struct Hash
{
	nod** vector;
	int dim;
};

int functieHash(Hash h,Rezervare r)
{
	int hash = 0;
	for (int i = 0; i < strlen(r.denumireHotel); i++)
	{
		char ajutor[20];
		hash = hash + r.denumireHotel[i];
	}
	return hash%h.dim;
}

Hash initializare(int dim)
{
	Hash h;
	h.dim = dim;
	h.vector = (nod**)malloc(sizeof(nod*)*dim);
	for (int i = 0; i < dim; i++)
		h.vector[i] = NULL;
	return h;
}

nod* inserareLista(nod* inceput, Rezervare r)
{
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = r;
	nou->next = NULL;
	if (inceput)
	{
		nod* temp = inceput;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
		return inceput;
	}
	else return nou;
}

Hash inserareHash(Hash h, Rezervare r)
{
	if (h.dim > 0)
	{
		int hash = functieHash(h, r);
		h.vector[hash] = inserareLista(h.vector[hash], r);
	}
	else
	{
		h = initializare(20);
		h = inserareHash(h, r);
	}
	return h;
}

void afisareRezervare(Rezervare r)
{
	printf("%d %s\t", r.idRezervare, r.denumireHotel);
}

void afisareLista(nod* inceput)
{
	nod* temp = inceput;
	while (temp)
	{
		afisareRezervare(temp->info);
		temp = temp->next;
	}
	printf("\n");
}

void afisareHash(Hash h)
{
	if (h.dim > 0)
	{
		for (int i = 0; i < h.dim; i++)
		if (h.vector[i])
			afisareLista(h.vector[i]);
	}
}
void main()
{
	Hash h;
	h = initializare(20);
	Rezervare r;
	r.idRezervare = 1;
	r.denumireHotel = "Ana";
	r.numarCamereRezervate = '3';
	r.numeClient="Popescu";
	r.perioadaRezervarii = "perioada";
	r.sumaPlata = 200;
	h = inserareHash(h,r);
	afisareHash(h);
}
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct Florarie {
	char* denumire;
	int nrFlori;
};

struct nod {
	Florarie info;
	nod* next;
};

//creare obiect de tip florarie
//I: denumirea florariei & nrFlori
//E: floraria
Florarie creareFlorarie(const char* denumire, int nrFlori) {
	Florarie florarie;
	florarie.denumire = (char*)malloc(sizeof(char)*(strlen(denumire) + 1)); //aloc spatiu pentru char
	strcpy(florarie.denumire, denumire); //copiez ce primesc in spatiul pe care l-am alocat
	florarie.nrFlori = nrFlori;
	return florarie;
}

//inserare la inceput
//I:obiectul ce va fi inserat & lista(nodul cap)
//E:noua lista cu noul nod la inceput
nod* inserareInceput(Florarie f, nod* cap) {
	nod* nodNou = (nod*)malloc(sizeof(nod)); //aloc spatiu pentru noul nod
	nodNou->info = creareFlorarie(f.denumire, f.nrFlori); //pun informatia utlia
	nodNou->next = cap; //setez next-ul nodul ca fiind nodul initial
	return nodNou; //returnez noua lista
}

//inserare la final
//I: adresa primului nod & info de inserat
//E: adresa primului nod - pentru cazul in care am o lista goala
nod* inserareSfarsit(Florarie f, nod* cap) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareFlorarie(f.denumire, f.nrFlori);
	nou->next = NULL;
	if (cap) {
		nod* temp = cap; //pentru deplasare
		while (temp->next) { //ma opresc inainte de ultimul nod
			temp = temp->next;
		}
		temp->next = nou; //acum sunt fix pe ultimul nod si il inserez pe asta nou
	}
	else { //nu avem lista
		cap = nou;
	}
	return cap; 
}

//inserare sortata
nod* inserareSortata(Florarie f, nod* cap) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareFlorarie(f.denumire, f.nrFlori);
	nou->next = NULL;
	if (cap) { //daca avem lista
		if (cap->info.nrFlori > f.nrFlori) { //compar informatia din primul nod cu ce vreau eu sa introduc si daca e mai mare cea curenta
			nou->next = cap; //fac legatura cu primul nod, adica asta devine noul cap
			cap = nou; //si introduc nodul 
		}
		else { //daca nodul curent e mai mic decat ce vreau eu sa introduc
			nod*p = cap; //temporar
			while (p->next && p->next->info.nrFlori < f.nrFlori) { //daca mai exista nod dupa si daca informatia din nodul urmator e mai mica decat ce vreau eu sa introduc
				p = p->next; //ma deplasez
			}
			nou->next = p->next; //altfel fac legatura
			p->next = nou; //si introduc nodul
		}
	}
	else {
		cap = nou;
	}
	return cap;
}

//medie nrFlori
float medieNrFlori(nod* cap) {
	if (cap) {
		int nr = 0, s = 0;
		while (cap)
		{
			nr++;
			s += cap->info.nrFlori;
			cap = cap->next;
		}
		return s / nr;
	}
	else return 0;
}

nod* creareNod(Florarie f, nod* next) {
	nod* nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = f;
	nodNou->next = next;
	return nodNou;
} 

nod* inserareDupaUnNod(nod* cap, Florarie f, const char* denumire) {
	nod* nou = creareNod(f, NULL);
	if (cap) {
		nod* temp = cap;
		while (temp &&strcmp(temp->info.denumire, denumire) != 0) {
			nod* nou = creareNod(f, temp->next);
			temp->next = nou;
		}
		temp = temp->next;

		if (temp == NULL) {
			cap = inserareSfarsit(f, cap);
		}
		else {
			nou->next = temp->next;
			temp->next = nou;
		}
	}
	else {
		cap = nou;
	}
	return cap;

}

nod* inserareInainteDeUnNod(nod* cap, const char* nume, Florarie f) {
	nod* nou = creareNod(f, NULL);
	if (cap) {
		nod* p = cap;
		nod* adresaMax = NULL;
		while (p) {
			if (p->next && strcmp(p->next->info.denumire, nume) == 0) {
				if (!adresaMax) {
					adresaMax = p;
				}
				if (p->next->info.nrFlori > adresaMax->next->info.nrFlori) {
					adresaMax = p;
				}
			}
			p = p->next;
		}
		if (adresaMax == NULL) {
			if (strcmp(cap->info.denumire, nume) == 0) {
				cap = inserareInceput(f, cap);
			}
			else {
				cap = inserareSfarsit(f, cap);
			}
		}
		else {
			if (strcmp(cap->info.denumire, nume) == 0) {
				if (cap->info.nrFlori > adresaMax->info.nrFlori) {
					cap = inserareInceput(f, cap);
					return cap;
				}
			}
			nou->next = adresaMax->next;
			adresaMax->next = nou;
		}
	}
	else {
		cap = nou;
	}
	return cap;
}

void afisareLista(nod* lista) {
	nod* temp = lista;
	while (temp)
	{
		printf("\nFloraria %s are %d tipuri de flori", temp->info.denumire, temp->info.nrFlori);
		temp = temp->next;
	}
}

nod* stergereLista(nod* cap) {
	while (cap) {
		nod* temp = cap; //au aceeasi adresa, catre primul nod; altfel n-as mai avea acces la info - nodul care se dezaloca
		cap = cap->next; //acum temp pointeaza catre elem 1 si cap catre elem 2 -noul incaput de lista
		free(cap->info.denumire);
		free(temp);
	}
	return cap; //echivalent cu return NULL
}



void main() {
	nod* florarie = NULL;
	florarie = inserareSortata(creareFlorarie("Iris", 120), florarie);
	florarie = inserareSortata(creareFlorarie("Magna", 40), florarie);
	florarie = inserareSortata(creareFlorarie("Floris", 500), florarie);
	florarie = inserareSortata(creareFlorarie("Magda", 150), florarie);
	
	afisareLista(florarie);

	system("pause");
}
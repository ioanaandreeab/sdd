#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

struct Carte {
	int id;
	char nrAutori;
	char ** Autori;
	char * titlu;

};


struct Nod {
	Carte book; //informatia utila
	Nod* next; //ADRESA urmatorului nod
};

//sortare dupa id
Nod* sortareBubble(Nod* l)
{
	if (!l || !l->next)
		return l;

	char vb = 1;
	Nod *p, *q, *r;

	while (vb)
	{
		vb = 0; //nu are loc nici o interschimbare

		if (l->book.id > l->next->book.id)
		{
			//criteriul de sortare neindeplinit intre primul si al doilea nod
			p = l->next;
			q = p->next;
			
			//interschimbare
			l->next = q;
			p->next = l;
			l = p;
			vb = 1;
		}

		Nod* t = l;
		while (t->next->next) //t->next->next corespunde nodului j
		{
			p = t->next; //p este nodul i
			q = p->next; //q este nodul j
			if (p->book.id > q->book.id)
			{
				r = q->next; //corespunde lui j+1;

				t->next = q;
				p->next = r;
				q->next = p;
				vb = 1;
			}
			t = t->next;
		}
	}
	return l;
}

//inserare nod la inceput
Nod* inserareNodLista(Nod* l, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod)); //alocare nod in heap seg
	nou->book = c;
	nou->next = l;

	return nou; //noul inceput de lista salvat in apelator in pointerul de gestionare a listei simple
}

//parsare lista smipla
void parseList(Nod* l)
{
	while (l) {
		printf("Id carte: %d\n", l->book.id);
		for (char i = 0; i < l->book.nrAutori; i++)
			printf("\t%s\n", l->book.Autori[i]);
		l = l->next;
	}
}

//dezalocare lista
Nod* dezalocareLista(Nod* l){
	while (l) {
		Nod* t = l; //nodul care se dezaloca

		l = l->next; //noul inceput de lista
		
		for (char i = 0; i < t->book.nrAutori; i++)
		{
			free(t->book.Autori[i]); //dezalocare linie i din matr de char (nume autori)
		}
		free(t->book.Autori);

		free(t->book.titlu); //dezalocare heap seg pt titlu carte
		//id-ul e int, e in stack nu in heap pentru ca e int si deci nu se dezaloca
		free(t); //dezalocare nod
	}
	return l;
}

void main()
{
	//lista - structura dinamica (poate primi sau pierde cu usurinta elemente la runtime)
	Nod* list=NULL; //list=0;
	Carte temp;
	FILE* f;
	char buffer[150];

	f = fopen("Text.txt", "r");

	fgets(buffer, sizeof(buffer), f); //citire id carte 1
	while (!feof(f)) {
		temp.id = atoi(buffer); //conversie la intreg pe 4 bytes(int)

		fgets(buffer, sizeof(buffer), f); //citire nr autori din fisier
		temp.nrAutori = atoi(buffer); //conversie string la intreg pe 1 byte(char)

		temp.Autori = (char**)malloc(temp.nrAutori * sizeof(char*));

		for (char i = 0; i < temp.nrAutori; i++)
		{
			fgets(buffer, sizeof(buffer), f); //citire nr autori i din fisier
			char* token = strtok(buffer, "\n"); //asta
			strcpy(buffer, token); //si asta pentru disparitia spatiilor la afisare
			temp.Autori[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); //alocare heap segment pt nume autor i

			strcpy(temp.Autori[i], buffer); //copiere autori din buffer in heap seg alocat anterior

		}
		fgets(buffer, sizeof(buffer), f); //citire titlu carte
		char* token = strtok(buffer, "\n");
		strcpy(buffer, token);
		temp.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(temp.titlu, buffer); //copiere titlu carte in heap alocat anterior

		//de implementat:
		//inserare carte ca nod in lista simpla list (functie)

		list=inserareNodLista(list, temp);

		fgets(buffer, sizeof(buffer), f);//citire id carte urm
	}

	parseList(list);

	list=sortareBubble(list);


	printf("Lista dupa sortare\n\n");

	//functie pt parsare lista simpla
	parseList(list);

	//functie pt stergerea unui nod in lista simpla

	//functie pentru sortarea nodurilor din lista simpla( id este criteriul de sortare)

	//functie pt dezalocarea structurii de date de tip lista

	list = dezalocareLista(list);

	printf("Lista dupa dezalocare:\n");

	parseList(list);


//inserare nod in lista dubla
//dezalocare structura de tip lista dubla
//sortare noduri lista dubla cu modificarea adreselor de legatura
//determinare carti cu un nr minim de autori -> adica si afisare

	fclose(f);
}


	//functie pt parsare lista simpla

	//functie pt stergerea unui nod in lista simpla

	//functie pentru sortarea nodurilor din lista simpla( id este criteriul de sortare)

	//functie pt dezalocarea structurii de date de tip lista

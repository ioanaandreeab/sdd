#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 256

struct Task {
	int idTask;
	char *dataAsignare;
	char *numeInginerAsignat;
	int nivelComplexitate;
	char *stareTask; // "nou", "deschis", "in lucru", "rezolvat", "inchis"
};

struct NodArb {
	Task tsk;
	NodArb *st, *dr;

};

struct nod
{
	Task info;
	nod* next;
};

NodArb *inserare(NodArb *r, Task t, int &er)
{
	if (r == NULL){
		NodArb *nou = new NodArb;
		nou->dr = NULL;
		nou->st = NULL;
		nou->tsk = t;
		r = nou;
	}
	else if (r->tsk.idTask == t.idTask) er = 1;
	else if (r->tsk.idTask > t.idTask)
		r->st = inserare(r->st, t, er);
	else
		r->dr = inserare(r->dr, t, er);
	return r;
}

int nrTaskuri(NodArb *r, char * inginer)
{
	if (r){
		
		if (!strcmp(r->tsk.numeInginerAsignat, inginer))
			return 1 + nrTaskuri(r->st, inginer) + nrTaskuri(r->dr, inginer);
		else
		return  nrTaskuri(r->st, inginer) + nrTaskuri(r->dr, inginer);
	}

	return 0;
}

void afisarePreordine(NodArb * radacina)
{
	if (radacina)
	{
		printf("%d - %s - %s - %d - %s\n", radacina->tsk.idTask, radacina->tsk.dataAsignare, radacina->tsk.numeInginerAsignat, radacina->tsk.nivelComplexitate, radacina->tsk.stareTask);
		afisarePreordine(radacina->st);
		afisarePreordine(radacina->dr);
	}
}

NodArb* schimbareTaskuri(NodArb* radacina,char* stare, char* stareNoua)
{
	if (radacina)
	{
		if (strcmp(radacina->tsk.stareTask,stare)==0)
		{
			free(radacina->tsk.stareTask);
			radacina->tsk.stareTask = (char*)malloc(sizeof(char)*(strlen(stareNoua) + 1));
			strcpy(radacina->tsk.stareTask, stareNoua);
			
		}
		radacina->st = schimbareTaskuri(radacina->st,stare,stareNoua);
		radacina->dr = schimbareTaskuri(radacina->dr,stare,stareNoua);

		return radacina;
	}
	else
	{
		return NULL;
	}
}

nod* inserareInceput(nod* inceput, Task t)
{
	nod*nodNou = (nod*)malloc(sizeof(nod));
	nodNou->info = t;
	nodNou->next = inceput;
	return nodNou;
	
}

nod* afisareTaskuri(NodArb* radacina, int nivel, nod* lista)
{
	
	if (radacina)
	{	
		NodArb* temp = radacina;
		if (temp->tsk.nivelComplexitate > nivel)
		{				
			lista = inserareInceput(lista, temp->tsk);	
			
		}
		lista=afisareTaskuri(temp->st, nivel,lista);
		lista=afisareTaskuri(temp->dr, nivel,lista);
		
	}
	return lista;	
}

NodArb* deleteLeaves(NodArb* radacina)
{
	if (radacina)
	{
		if (radacina->st == NULL && radacina->dr == NULL)
		{
			free(radacina->tsk.dataAsignare);
			free(radacina->tsk.numeInginerAsignat);
			free(radacina->tsk.stareTask);
			free(radacina);
			return NULL;
		}
		else
		{
			radacina->st = deleteLeaves(radacina->st);
			radacina->dr = deleteLeaves(radacina->dr);
		}
	}
	return radacina;
}

void main() {

	Task t;
	NodArb *root = NULL;

	FILE * f;
	f = fopen("Tasks.txt", "r");

	char *token, file_buf[LINESIZE], sep_list[] = ",;";

	while (fgets(file_buf, sizeof(file_buf), f)) {
		token = strtok(file_buf, sep_list);
		t.idTask = atoi(token);

		token = strtok(NULL, sep_list);
		t.dataAsignare = (char*)malloc((strlen(token) + 1) * sizeof(char));
		strcpy(t.dataAsignare, token);

		token = strtok(NULL, sep_list);
		t.numeInginerAsignat = (char*)malloc((strlen(token) + 1) * sizeof(char));
		strcpy(t.numeInginerAsignat, token);

		token = strtok(NULL, sep_list);
		t.nivelComplexitate = atoi(token);

		token = strtok(NULL, sep_list);
		t.stareTask = (char*)malloc((strlen(token) + 1) * sizeof(char));
		strcpy(t.stareTask, token);		

		int err = 0;
		root = inserare(root, t, err);
		if (err) {
			printf("\nTask-ul cu id %d exista in arbore.\n", t.idTask);
			free(t.dataAsignare);
			free(t.numeInginerAsignat);
			free(t.stareTask);
		}
		else
			printf("\nTask-ul cu id %d a fost inserat\n", t.idTask);
	}

	fclose(f);
	printf("\n\n");
	afisarePreordine(root);
	printf("\n\n");
	char* nume = (char*)malloc(sizeof(char)*(strlen("Ionescu") + 1));
	strcpy(nume, "Ionescu");
	int c = nrTaskuri(root, nume);
	printf("Nr de takuri asignate inginerului %s este %d.\n\n", nume, c);
	root = schimbareTaskuri(root,"nou","deschis");
	afisarePreordine(root);
	printf("\n\n");
	printf("\n\n---------------------------------\n\n");
	nod* lista = NULL;
	lista = afisareTaskuri(root, 3,lista);	
	while (lista)
	{
		printf("%d - %s - %s - %d - %s\n", lista->info.idTask, lista->info.dataAsignare, lista->info.numeInginerAsignat, lista->info.nivelComplexitate, lista->info.stareTask);
		lista = lista->next;
	}
	printf("\n\n---------------------------------\n\n");
	//printf("\n\n");
	//root = deleteLeaves(root);
	//afisarePreordine(root);
	
	//int a = nr(root, 2);
	//printf("%d\n\n", a);

	/*Task* vector = (Task*)malloc(sizeof(Task)*a);
	vector = afisareTaskuri(root, 2, &vector);
	for (int i = 0; i < a; i++)
	{
		printf("%d - %s - %s - %d - %s\n", vector[i].idTask, vector[i].dataAsignare, vector[i].numeInginerAsignat, vector[i].nivelComplexitate, vector[i].stareTask);
	}*/

	/*root = afisareTaskuri(root, 3);
	afisarePreordine(root);*/
}
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Film {
	char* titlu;
	int durata;
};

struct nodLista {
	Film info;
	nodLista* next;
};

Film initFilm(const char* titlu, int durata) {
	Film f;
	f.titlu = (char*)malloc(sizeof(char)*(strlen(titlu) + 1));
	strcpy(f.titlu, titlu);
	f.durata = durata;

	return f;
}

void afisareFilm(Film f) {
	printf("\nFilmul %s are %d minute", f.titlu, f.durata);
}

nodLista* inserareLaInceputListaCirculara(nodLista* cap, Film f) {
	nodLista* nou = (nodLista*)malloc(sizeof(nodLista));
	nou->info = f;
	if (cap) {
		nou->next = cap;
		nodLista* temp = cap;
		while (temp->next != cap) {
			temp = temp->next;
		}
		temp->next = nou;
		return nou;
	}
	else {
		nou->next = nou;
		return nou;
	}
}

void afisareLista(nodLista* cap) {
	if (cap) {
		nodLista* temp = cap->next;
		afisareFilm(cap->info);
		while (temp != cap) {
			afisareFilm(temp->info);
			temp = temp->next;
		}
	}
}

Film* copiazaFilmeCuDurataMare(nodLista* cap, int prag, int &lungimeVector) {
	if (cap) {
		lungimeVector = 0;
		nodLista* temp = cap;
		//determin lungimea vectorului
		do {
			if (temp->info.durata > prag) {
				lungimeVector++;
			}
			temp = temp->next;
		} while (temp != cap);

		Film* filme = (Film*)malloc(sizeof(Film)*lungimeVector);
		temp = cap;
		lungimeVector = 0;
		do {
			if (temp->info.durata > prag) {
				filme[lungimeVector++] = initFilm(temp->info.titlu, temp->info.durata);
			}
			temp = temp->next;
		} while (temp != cap);
		return filme;
	}
	else {
		return NULL;
	}
}

void stergereListaCirculara(nodLista* &cap) {
	if (cap) {
		nodLista* temp = cap->next;
		while (temp != cap) {
			nodLista* aux = temp->next;
			free(temp->info.titlu);
			free(temp);
			temp = aux;
		}
		free(cap->info.titlu);
		free(cap);
		cap = NULL;
	}
}

struct nodArbore {
	nodArbore* st;
	nodArbore* dr;
	Film info;
};

nodArbore* inserareInArbore(nodArbore* rad, Film f) {
	if (rad) {
		if (rad->info.durata > f.durata) {
			rad->st = inserareInArbore(rad->st, f);
		}
		else {
			rad->dr = inserareInArbore(rad->dr, f);
		}
		return rad;
	}
	else {
		nodArbore* nou = (nodArbore*)malloc(sizeof(nodArbore));
		nou->info = initFilm(f.titlu, f.durata);
		nou->dr = NULL;
		nou->st = NULL;
		return nou;
	}
}

nodArbore* copiereDinListaInArbore(nodLista* cap) {
	if (cap) {
		nodArbore* rad = NULL;
		nodLista* temp = cap;
		do {
			rad = inserareInArbore(rad,
				initFilm(temp->info.titlu, temp->info.durata));
			temp = temp->next;
		} while (temp != cap);
		return rad;
	}
	else {
		return NULL;
	}
}

void afisareArboreInordine(nodArbore* rad) {
	if (rad) {
		afisareArboreInordine(rad->st);
		afisareFilm(rad->info);
		afisareArboreInordine(rad->dr);
	}
}

void main() {
	nodLista* cap = NULL;
	cap = inserareLaInceputListaCirculara(cap, initFilm("101 Dalmatieni", 120));
	cap = inserareLaInceputListaCirculara(cap, initFilm("100 Days without Summer", 140));
	cap = inserareLaInceputListaCirculara(cap, initFilm("Pulp Fiction", 130));
	cap = inserareLaInceputListaCirculara(cap, initFilm("A clockwork orange", 100));
	cap = inserareLaInceputListaCirculara(cap, initFilm("Star Wars IV", 200));

	afisareLista(cap);

	int dim = 0;
	Film* filme = copiazaFilmeCuDurataMare(cap, 135, dim);
	printf("\n\nVector: \n");
	for (int i = 0; i < dim; i++) {
		afisareFilm(filme[i]);
	}
	for (int i = 0; i < dim; i++) {
		free(filme[i].titlu);
	}
	free(filme);
	
	printf("\n\nArbore: \n");
	nodArbore* rad = copiereDinListaInArbore(cap);
	afisareArboreInordine(rad);


}
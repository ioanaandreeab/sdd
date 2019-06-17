#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Team {
	int code;
	char* captain;
	int topic;
	int noStudents;
	float points;
};

struct HT {
	Team* *vector;
	int dim;
};

Team initTeam(int code, const char* captain, int topic, int noStudents, float points) {
	Team t;
	t.code = code;
	t.captain = (char*)malloc(sizeof(char)*(strlen(captain) + 1));
	strcpy(t.captain, captain);
	t.topic = topic;
	t.noStudents = noStudents;
	t.points = points;

	return t;
}

void printTeam(Team t) {
	printf("\nTeam %d, captain - %s, topic - %d, has %d students and %5.2f points", t.code, t.captain, t.topic, t.noStudents, t.points);
}

HT initHT(int dim) {
	HT h;
	h.dim = dim;
	h.vector = (Team**)malloc(sizeof(Team*)*dim);
	for (int i = 0; i < dim; i++) {
		h.vector[i] = NULL;
	}

	return h;
}

int hashFunction(Team t, int dim) {
	return t.code%dim;
}

int insertIntoHT(HT h, Team t) {
	if (h.dim > 0) {
		int hashCode = hashFunction(t, h.dim);
		if (h.vector[hashCode]) { //am coliziune
			int index = (hashCode + 1) % h.dim;
			while (index != hashCode && h.vector[index]) {//cat timp imi da diferit,dar totusi am ceva in vector
				index = (index + 1) % h.dim; //continui sa cresc
			}
			if (index == hashCode) { //daca imi da acelasi lucru
				return -1; //tabela plina
			}
			else { //am gasit un loc liber
				h.vector[index] = (Team*)malloc(sizeof(Team));
				*h.vector[index] = initTeam(t.code, t.captain, t.topic, t.noStudents, t.points);
				return index;
			}
		}
		else { //nu am coliziune si inserez direct
			h.vector[hashCode] = (Team*)malloc(sizeof(Team));
			*h.vector[hashCode] = initTeam(t.code, t.captain, t.topic, t.noStudents, t.points);
			return hashCode;
		}
	}
	else {
		return -2; //nu exista tabela
	}
}

void printHT(HT h) {
	if (h.dim > 0) {
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i]) {
				printTeam(*h.vector[i]);
			}
		}
	}
}

Team** averagePointsOverValue(HT h, float prag, int &no) {
	if (h.dim > 0) {
		int noTeams = 0;
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i]) {
				if (h.vector[i]->points / h.vector[i]->noStudents > prag) {
					noTeams++;
				}
			}
		}
		no = noTeams;
		Team** teams = (Team**)malloc(sizeof(Team*)*noTeams);
		noTeams = 0;
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i]) {
				if (h.vector[i]->points / h.vector[i]->noStudents > prag) {
					teams[noTeams++] = h.vector[i];
				}
			}
		}
		return teams;
	}
}

struct nodDublu {
	nodDublu* next;
	nodDublu* prev;
	int cod;
};

struct LDI {
	nodDublu* prim;
	nodDublu* ultim;
};

LDI insertIntoDoubleList(LDI lista, int code) {
	nodDublu* nou = (nodDublu*)malloc(sizeof(nodDublu));
	nou->cod = code;
	nou->prev = NULL;
	nou->next = lista.prim;
	if (lista.prim) {
		lista.prim->prev = nou;
		lista.prim = nou;
	}
	else {
		lista.prim = lista.ultim = nou;
	}
	return lista;
}

float averageNoStudentsPerTeam(HT h) {
	if (h.dim > 0) {
		int teams = 0;
		int students = 0;
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i] != NULL) {
				teams++;
				students += h.vector[i]->noStudents;
			}
		}
		return students / teams;
	}
}

void printList(LDI list) {
	nodDublu* temp = list.prim;
	while (temp) {
		printf(" %d", temp->cod);
		temp = temp->next;
	}
}

LDI HTtoDoubleListOfCodes(HT h, LDI list) {
	if (h.dim > 0) {
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i] != NULL) {
				list = insertIntoDoubleList(list, h.vector[i]->code);
			}
		}
		return list;
	}
}

void main() {
	HT h = initHT(6);
	insertIntoHT(h, initTeam(24, "Ioana", 2, 5, 120));
	insertIntoHT(h, initTeam(54, "Denisa", 3, 3, 50));
	insertIntoHT(h, initTeam(17, "Andrei", 1, 7, 30));
	insertIntoHT(h, initTeam(28, "Teo", 2, 6, 70));
	insertIntoHT(h, initTeam(69, "Liviu", 1, 5, 100));

	printHT(h);

	printf("\n\nTeams with high points/player:");

	int no = 0;
	Team** teamsOverValue = averagePointsOverValue(h, 10, no);
	for (int i = 0; i < no; i++) {
		printTeam(*(teamsOverValue[i]));
	}

	float average = averageNoStudentsPerTeam(h);
	printf("\n\nThe average number of students per team is %5.2f", average);

	printf("\n\nThe list w/ teams' codes is:");
	LDI lista;
	lista.prim = lista.ultim = NULL;
	lista = HTtoDoubleListOfCodes(h, lista);
	printList(lista);
}
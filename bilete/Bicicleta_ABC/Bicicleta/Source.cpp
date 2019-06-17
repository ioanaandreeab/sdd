#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Bicicleta {
	int id;
	int minuteUtilizare;
	int idStatieParcare;
	int nrUtilizari;
	char* numeUtilizator;
};

Bicicleta initBicicleta(int id, int minuteUtilizare, int idStatieParcare, int nrUtilizari, const char* numeUtilizator) {
	Bicicleta b;
	b.id = id;
	b.minuteUtilizare = minuteUtilizare;
	b.idStatieParcare = idStatieParcare;
	b.nrUtilizari = nrUtilizari;
	b.numeUtilizator = (char*)malloc(sizeof(char)*(strlen(numeUtilizator) + 1));
	strcpy(b.numeUtilizator, numeUtilizator);

	return b;
}

void afisareBicicleta(Bicicleta b) {
	printf("\nBicicleta cu id-ul %d, durata de utilizare intr-o zi %d minute, parcata la statia nr. %d, are %d utilizari pe zi de catre %s", b.id, b.minuteUtilizare, b.idStatieParcare, b.nrUtilizari, b.numeUtilizator);
}

struct nodArbore {
	nodArbore* st;
	nodArbore* dr;
	Bicicleta info;
};

nodArbore* inserareInArbore(nodArbore* rad, Bicicleta b) {
	if (rad) {
		if (rad->info.minuteUtilizare > b.minuteUtilizare) {
			rad->st = inserareInArbore(rad->st, b);
		}
		else {
			rad->dr = inserareInArbore(rad->dr, b);
		}
		return rad;
	}
	else {
		nodArbore* nou = (nodArbore*)malloc(sizeof(nodArbore));
		nou->st = nou->dr = NULL;
		nou->info = initBicicleta(b.id, b.minuteUtilizare, b.idStatieParcare, b.nrUtilizari, b.numeUtilizator);
		return nou;
	}
}

void afisareArboreInordine(nodArbore* rad) {
	if (rad) {
		afisareArboreInordine(rad->st);
		afisareBicicleta(rad->info);
		afisareArboreInordine(rad->dr);
	}
}

void determinareMinuteTotal(nodArbore* rad, int &total) {
	if (rad) {
		determinareMinuteTotal(rad->st, total);
		total += rad->info.minuteUtilizare;
		determinareMinuteTotal(rad->dr, total);
	}
}

void calculareProfitTotal(nodArbore* rad, int &profit) {
	if (rad) {
		calculareProfitTotal(rad->st,profit);
		profit += rad->info.minuteUtilizare > 10 ? (rad->info.minuteUtilizare - 10) * 2 : rad->info.minuteUtilizare * 2;
		calculareProfitTotal(rad->dr, profit);
	}
}

void main() {
	nodArbore* rad = NULL;
	rad = inserareInArbore(rad, initBicicleta(1, 60, 3, 3, "Ioana"));
	rad = inserareInArbore(rad, initBicicleta(2, 40, 2, 2, "Denisa"));
	rad = inserareInArbore(rad, initBicicleta(3, 90, 1, 1, "Andrei"));
	rad = inserareInArbore(rad, initBicicleta(4, 30, 1, 2, "Teo"));
	rad = inserareInArbore(rad, initBicicleta(5, 25, 3, 5, "Stefan"));

	afisareArboreInordine(rad);
	int minute = 0;
	determinareMinuteTotal(rad, minute);
	printf("\nNr total de minute in care bicicletele au fost utilizate intr-o zi: %d", minute);
	int profit = 0;
	calculareProfitTotal(rad, profit);
	printf("\nProfitul total obtinut este de: %d", profit);
}
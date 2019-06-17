#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Factura {
	int nrFactura;
	char* dataEmiterii;
	char* numeClient;
	float valFactura;
	int nrProduse;
};

Factura initFactura(int nrFactura, const char* dataEmiterii, const char* numeClient, float valFactura, int nrProduse) {
	Factura f;
	f.nrFactura = nrFactura;
	f.dataEmiterii = (char*)malloc(sizeof(char)*(strlen(dataEmiterii) + 1));
	strcpy(f.dataEmiterii, dataEmiterii);
	f.numeClient = (char*)malloc(sizeof(char)*(strlen(numeClient) + 1));
	strcpy(f.numeClient, numeClient);
	f.valFactura = valFactura;
	f.nrProduse = nrProduse;

	return f;
}

void afisareFactura(Factura f) {
	printf("\nFactura cu nr %d, emisa pe %s, client %s, in valoare de %5.2f lei are %d produse", f.nrFactura, f.dataEmiterii, f.numeClient, f.valFactura, f.nrProduse);
}

struct nodAVL {
	nodAVL* st;
	nodAVL* dr;
	Factura info;
};

int calculeazaInaltime(nodAVL* rad) {
	if (rad) {
		int inaltimeS = calculeazaInaltime(rad->st);
		int inaltimeD = calculeazaInaltime(rad->dr);
		return 1 + (inaltimeS > inaltimeD ? inaltimeS : inaltimeD);
	}
	else {
		return 0;
	}
}

int gradDeEchilibru(nodAVL* rad) {
	if (rad) {
		int inaltimeS = calculeazaInaltime(rad->st);
		int inaltimeD = calculeazaInaltime(rad->dr);
		return inaltimeS - inaltimeD;
	}
	else {
		return 0;
	}
}

nodAVL* rotireLaDreapta(nodAVL* rad) {
	if (rad) {
		nodAVL* aux = rad->st; //nodul de care trag
		rad->st = aux->dr;
		aux->dr = rad;
		return aux;
	}
	else {
		return rad;
	}
}

nodAVL* rotireLaStanga(nodAVL* rad) {
	if (rad) {
		nodAVL* aux = rad->dr;
		rad->dr = aux->st;
		aux->st = rad;
		return aux;
	}
	else {
		return rad;
	}
}

nodAVL* inserareAVL(nodAVL* rad, Factura f){
	if (rad) {
		if (rad->info.nrFactura > f.nrFactura) {
			rad->st = inserareAVL(rad->st, f);
		}
		else {
			rad->dr = inserareAVL(rad->dr, f);
		}
		if (gradDeEchilibru(rad) == 2) { //dezechilibru pe stanga
			if (gradDeEchilibru(rad->st) != 1) { //nu e dezechilibru total stanga
				rad->st = rotireLaStanga(rad->st);
			}
			rad = rotireLaDreapta(rad);
		}
		if (gradDeEchilibru(rad) == -2) {
			if (gradDeEchilibru(rad->dr) != -1) {
				rad->dr = rotireLaDreapta(rad->dr);
			}
			rad = rotireLaStanga(rad);
		}
		//inainte sa returnam facem verificari
		return rad;
	}
	else {
		nodAVL* nou = (nodAVL*)malloc(sizeof(nodAVL));
		nou->info = initFactura(f.nrFactura, f.dataEmiterii, f.numeClient, f.valFactura, f.nrProduse);
		nou->dr = nou->st = NULL;
		return nou;
	}
}

void afisareNivel(nodAVL* rad, int nivel) {
	if (rad) {
		//daca nivelul dat e 0
		if (nivel == 0) { //afisez ce am in radacina
			afisareFactura(rad->info);
		}
		else {//decrementez nivelul si apelez
			nivel--; //decrementez pentru ca, sa zicem ca ii dau 2, el trb sa decrementeze pana ajunge la 0 care o sa fie nivelul curent pe care l-am cerut eu
			afisareNivel(rad->st, nivel);
			afisareNivel(rad->dr, nivel);
		}
	}
}

void afisarePeNivele(nodAVL* rad) {
	if (rad) {
		int nrNivele = calculeazaInaltime(rad);
		for (int i = 0; i < nrNivele; i++) {
			printf("\nNivel %d: \n", i + 1);
			afisareNivel(rad, i);
		}
	}
}

void traversareInordine(nodAVL* rad) {
	if (rad) {
		traversareInordine(rad->st);
		afisareFactura(rad->info);
		traversareInordine(rad->dr);
	}
}

void aflaFacturiDupaNume(nodAVL* rad, const char* numeClient, int &nrFacturi) {
	if (rad) {
		aflaFacturiDupaNume(rad->st, numeClient, nrFacturi);
		if (strcmp(numeClient, rad->info.numeClient) == 0) {
			nrFacturi++;
		}
		aflaFacturiDupaNume(rad->dr, numeClient, nrFacturi);
	}
}

void corectareFacturi(nodAVL* &rad) {
	if (rad) {
		corectareFacturi(rad->st);
		rad->info.valFactura = rad->info.valFactura - 2.50;
		corectareFacturi(rad->dr);
	}
}

nodAVL* stergereNodAVL(nodAVL* rad, int nrFactura) {
	if (rad) {
		if (rad->info.nrFactura < nrFactura) {
			rad->dr = stergereNodAVL(rad->dr, nrFactura);
		}
		else {
			if (rad->info.nrFactura > nrFactura) {
				rad->st = stergereNodAVL(rad->st, nrFactura);
			}
			else { //am gasit nodul
				if (rad->st == NULL) { //nu are descendent in stanga
					nodAVL* tmp = rad->dr; //poate fi null sau nu
					//dezaloc nodul curent
					free(rad->info.dataEmiterii);
					free(rad->info.numeClient);
					free(rad);

					rad = tmp; //refac legatura;
				}
				else if (rad->dr == NULL) {
						nodAVL* tmp = rad->st;
						//dezaloc
						free(rad->info.dataEmiterii);
						free(rad->info.numeClient);
						free(rad);
						
						rad = tmp;
				}
				else { //cazul cu 2 descendenti
					nodAVL* tmp = rad->st;
					while (tmp->dr)
						tmp = tmp->dr; //cel mai din dreapta nod din arborele din stanga
					free(rad->info.dataEmiterii);
					free(rad->info.numeClient);
					//mut datele din tmp in rad
					rad->info.dataEmiterii = (char*)malloc(sizeof(char)*(strlen(tmp->info.dataEmiterii) + 1));
					strcpy(rad->info.dataEmiterii, tmp->info.dataEmiterii);
					rad->info.numeClient = (char*)malloc(sizeof(char)*(strlen(tmp->info.numeClient) + 1));
					strcpy(rad->info.numeClient, tmp->info.numeClient);
					rad->info.nrFactura = tmp->info.nrFactura;
					rad->info.nrProduse = tmp->info.nrProduse;
					rad->info.valFactura = tmp->info.valFactura;
					
					//sterg nodul, care acum se afla pe pozitia lui tmp adica cel mai din dreapta nod din arborele din stanga
					rad->st = stergereNodAVL(rad->st, tmp->info.nrFactura);
				}
			}
		}

	}
	//dupa ce stergem facem verificarile
	if (rad) {
		if (gradDeEchilibru(rad) == 2) { //dezechilibru pe stanga
			if (gradDeEchilibru(rad->st) != 1) { //nu e dezechilibru total stanga
				rad->st = rotireLaStanga(rad->st);
			}
			rad = rotireLaDreapta(rad);
		}
		if (gradDeEchilibru(rad) == -2) {
			if (gradDeEchilibru(rad->dr) != -1) {
				rad->dr = rotireLaDreapta(rad->dr);
			}
			rad = rotireLaStanga(rad);
		}
	}
	return rad;
}


void main() {
	nodAVL* rad = NULL;
	rad = inserareAVL(rad, initFactura(24, "25.05.2019", "Ioana", 250.20, 3));
	rad = inserareAVL(rad, initFactura(15, "31.05.2019", "Denisa", 1000.30, 7));
	rad = inserareAVL(rad, initFactura(10, "25.05.2019", "Ioana", 120.10, 8));
	rad = inserareAVL(rad, initFactura(36, "02.01.2019", "Teodor", 780.50, 4));
	rad = inserareAVL(rad, initFactura(85, "10.03.2019", "Ilinca", 200.00, 3));

	printf("Traversare inordine: ");
	traversareInordine(rad);

	printf("\n\nAfisare pe nivele: ");
	afisarePeNivele(rad);

	int nrFacturi = 0;
	aflaFacturiDupaNume(rad, "Ioana", nrFacturi);
	printf("\n\nClientul Ioana are %d facturi", nrFacturi);

	printf("\n\nDupa corectarea facturilor, acestea sunt acum: ");
	corectareFacturi(rad);
	traversareInordine(rad);

	rad = stergereNodAVL(rad, 15);
	printf("\n\nDupa stergerea facturii, arborele este: ");
	traversareInordine(rad);
}
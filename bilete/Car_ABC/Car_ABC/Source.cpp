#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Car {
	int idVehicle;
	char* carMaker;
	int year;
	float price;
	int noSeats;
};

Car* initCar(int idVehicle, const char* carMaker, int year, float price, int noSeats) {
	Car* c =(Car*)malloc(sizeof(Car));
	c->idVehicle = idVehicle;
	c->carMaker = (char*)malloc(sizeof(char)*(strlen(carMaker) + 1));
	strcpy(c->carMaker, carMaker);
	c->year = year;
	c->price = price;
	c->noSeats = noSeats;
	
	return c;
}

void displayCar(Car c) {
	printf("\nCar %d, made by %s in %d, has %d seats and costs %5.2f euros", c.idVehicle, c.carMaker, c.year, c.noSeats, c.price);
}

struct nodArbore {
	nodArbore* st;
	nodArbore* dr;
	Car* info;
};

nodArbore* insertInTree(nodArbore* rad, Car* c) {
	if (rad) {
		if (rad->info->idVehicle < c->idVehicle) {
			rad->dr = insertInTree(rad->dr, c);
		}
		else {
			rad->st = insertInTree(rad->st, c);
		}
		return rad;
	}
	else {
		nodArbore* nou = (nodArbore*)malloc(sizeof(nodArbore));
		nou->dr = nou->st = NULL;
		nou->info = c;
		return nou;
	}
}

void displayDescending(nodArbore* rad) {
	if (rad) {
		displayDescending(rad->dr);
		displayCar(*(rad->info));
		displayDescending(rad->st);
	}
}

Car extractById(nodArbore* rad, int id) {
	Car rez;
	if (rad) {
		if (rad->info->idVehicle==id) {
			rez = *(rad->info);
			return rez;
		}
		else if (rad->info->idVehicle > id) {
			return extractById(rad->st, id);
		}
		else return extractById(rad->dr, id);
	}
}

float maxPrice(nodArbore* rad, float &max, Car &car) {
	if (rad) {
		if (rad->info->price > max) {
			max = rad->info->price;
			car = *(rad->info);
		}
		return maxPrice(rad->st, max,car) > maxPrice(rad->dr, max,car) ? maxPrice(rad->st, max,car) : maxPrice(rad->dr, max,car);
	}
	else {
		return max;
	}
}

void main() {
	nodArbore* rad = NULL;
	rad = insertInTree(rad, initCar(42, "Volkswagen", 2015, 12000, 5));
	rad = insertInTree(rad, initCar(56, "Audi", 2018, 72000, 5));
	rad = insertInTree(rad, initCar(20, "Renault", 2015, 9000, 2));
	rad = insertInTree(rad, initCar(35, "Dacia", 2015, 7500, 5));
	rad = insertInTree(rad, initCar(17, "Peugeot", 2015, 20000, 2));
	displayDescending(rad);

	printf("\n\nMasina cu id-ul cautat este:");
	Car rez = extractById(rad, 56);
	displayCar(rez);

	printf("\n\nMasina cu cel mai mare pret este:");
	float max = 0;
	Car maxCar;
	maxPrice(rad, max, maxCar);
	displayCar(maxCar);
}
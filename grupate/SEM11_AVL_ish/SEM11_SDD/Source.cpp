#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

//Dreapta-Stanga -> calcul pentru grad de echilibru
//

struct movie {
	int id;
	char* director;
};

struct node {
	movie info;
	node* right;
	node* left;
};

movie createMovie(int id, const char* director) {
	movie m;
	m.id = id;
	m.director = (char*)malloc(sizeof(char)*strlen(director) + 1);
	strcpy(m.director, director);
	
	return m;
}

void printMovie(movie m) {
	printf("\nId: %d, Director: %s", m.id, m.director);
}

node* initNode(movie info, node* left, node* right) {
	node* newNode = (node*)malloc(sizeof(node));
	newNode->info = info;
	newNode->left = left;
	newNode->right = right;

	return newNode;
}

node* rotateToLeft(node* root) {
	if (root) {
		node* pivot = root->right;
		root->right = pivot->left;
		pivot->left = root;
		return pivot;
	}
	else {
		return root;//echivalent cu null pentru ca e null oricum
	}
}

node* rotateToRight(node* root) {
	if (root) {
		node* pivot = root->left;
		root->left = pivot->right;
		pivot->right = root;
		return pivot;
	}
	else {
		return root;
	}
}

//calculeaza inaltimea arborelui
int calculateHeight(node* root) {
	if (root) {
		int rH = calculateHeight(root->right);
		int lH = calculateHeight(root->left);
		return 1 + (lH < rH ? lH : rH);
	}
	else {
		return 0;
	}
}

//functie pentru calcul grad de echilibru
int calculateED(node* root) {
	if (root) {
		int hR = calculateHeight(root->right);
		int hL = calculateHeight(root->left);
		return hR - hL;
	}
	else return 0;
}

node* insertAVL(movie info, node* root) {
	if (root) {
		if (info.id < root->info.id) {
			root->left = insertAVL(info, root->left);
		}
		else {
			root->right = insertAVL(info, root->right);
		}
		int ED = calculateED(root);
		if (ED == 2) { //dezechilibru pe partea dreapta
			int EDR = calculateED(root->right);
			if (EDR == 1) { //si-a pastrat semnul; e ala in linie, adica dezechilibru total
				root = rotateToLeft(root);
			}
			else { 
				root->right = rotateToRight(root->right);
				root = rotateToLeft(root);
			}
		}
		if (ED == -2) { //dezechilibru la stanga
			int EDL = calculateED(root->left);
			if (EDL == -1) { //si-a pastrat semnul => dezechilibru total
				root = rotateToRight(root);
			}
			else {
				root->left = rotateToLeft(root->left);
				root = rotateToRight(root);
			}
		}

		return root;
	}
	else {
		return initNode(info, NULL, NULL);
	}
}

void printTree(node* root) {
	if (root) {
		printTree(root->left);
		printMovie(root->info);
		printTree(root->right);
	}
}

void main() {
	node* root = NULL;
	root = insertAVL(createMovie(1, "Wes Anderson"), root);
	root = insertAVL(createMovie(2, "Tarantino"), root);
	root = insertAVL(createMovie(3, "Stanley Kubrik"), root);
	root = insertAVL(createMovie(4, "Stanley Kubrik"), root);
	root = insertAVL(createMovie(5, "Stanley Kubrik"), root);
	root = insertAVL(createMovie(6, "Stanley Kubrik"), root);
	printTree(root);

	system("pause");
}
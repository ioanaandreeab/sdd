#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
//ARBORE BINAR DE CAUTARE
struct Author {
	char* name;
	int age;
};

struct Book {
	int id;
	Author author;
	int noPages;
};

//structura pentru un nod din arbore
struct node {
	Book info;
	node* right;
	node* left;
};

node* initNode(Book info, node* left, node* right) {
	node* newNode = (node*)malloc(sizeof(node));
	newNode->info = info; //shallow copy
	newNode->left = left;
	newNode->right = right;

	return newNode;
}

Book readBook(FILE* f) {
	Book b;
	fscanf(f, "%d", &b.id);
	char buffer[20]; //citesc in buffer
	fscanf(f, "%s", buffer);
	b.author.name = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(b.author.name, buffer); //nu e necesar pentru ca e pointer
	fscanf(f, "%d", &b.author.age);
	fscanf(f, "%d", &b.noPages);

	return b;
}

//pentru inserare am nev de cartea pe care o inserez si de radacina arborelui, adica de arbore
node* insertTree(Book book, node* root) {
	//compar unde trebuie sa pun cartea
	if(root){
		if (book.id < root->info.id)
		{
			root->left = insertTree(book, root->left);
			return root;
		}
		else {
			root->right = insertTree(book, root->right);
			return root;
		}
	}
	else {
		return initNode(book, NULL, NULL); //nodul devine radacina pentru ca nu exista radacina
	}
}

void printBook(Book b) {
	printf("%d. Book written by %s has %d pages. %d\n", b.id, b.author.name, b.noPages, b.author.age);
}

void printTree(node* root) {
	if (root) {
		//in ordine
		printTree(root->left); //arborele din stanga 
		printBook(root->info); //radacina
		printTree(root->right); //arborele din dreapta
	}
}

//cautare dupa id
Book findBookById(node* root, int id) {
	if (root) {
		//verific daca radacina e elementul pe care il caut
		if (root->info.id == id) {
			return root->info;
		}
		else {
			//verific daca id-ul e mai mic decat id-ul radacinii => atunci m-as duce in stanga
			if (root->info.id > id) {
				return findBookById(root->left, id);
			}
			else {
				return findBookById(root->right, id);
			}
		}
	}
	else {
		Book b;
		b.id = -1;
		b.author.age = -1;
		b.author.name = NULL;
		b.noPages = -1;

		return b;
	}
}

int height(node* root) {
	if (root) {
		int hleft = height(root->left);
		int hright = height(root->right);
		return 1 + (hleft > hright ? hleft : hright);
	}
	else {
		return 0;
	}
}

//afisare a nodurilor de pe un anumit nivel pe care il dau eu ca param
void afisareDePeNivel(node* root, int nivelDorit, int nivelCurent) {
	if (root) {
		if (nivelDorit == nivelCurent) {
			printBook(root->info);
		}
		else {
			afisareDePeNivel(root->left, nivelDorit, nivelCurent + 1);
			afisareDePeNivel(root->right, nivelDorit, nivelCurent + 1);
		}
	}
}

//stergere dintr-un arbore de facut -> atunci cand sterg radacina trebuie sa schimb locul pentru radacina

void main() {
	FILE* f = fopen("books.txt", "r");
	//am nevoie de radacina pentru arbore
	node* root = NULL;

	int noBooks=0;
	fscanf(f, "%d", &noBooks);
	for (int i = 0; i < noBooks; i++) {
		root = insertTree(readBook(f), root);
	}

	printTree(root);

	printf("\n\n");
	printBook(findBookById(root, 8));

	int heightTree = height(root);
	printf("\nInaltimea arborelui este: %d\n",heightTree);

	afisareDePeNivel(root, 3, 1);
	system("pause");
}
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

//heap
//bst(binary search tree) -- arbore binar de cautare 
//hashTable

struct HEAP {
	int* vector;
	int length;
};

HEAP initHeap(int length) {
	HEAP h;
	h.vector = (int*)malloc(sizeof(int)*length);
	h.length = length;
	
	return h;
}

//filtreaza vectorul a.i. sa fie in forma de maxim, adica maximul va fi pe prima pozitie, de restul nu stim exact dar nu ne intereseaza
void filter(HEAP heap, int index)
{
	if (index < heap.length)
	{
		int LeftPoz = 2 * index + 1;
		int RightPoz = 2 * index + 2;
		int maxPos = index;

		if (LeftPoz < heap.length && heap.vector[maxPos] < heap.vector[LeftPoz])
		{
			maxPos = LeftPoz;
		}

		if (RightPoz < heap.length && heap.vector[maxPos] < heap.vector[RightPoz])
		{
			maxPos = RightPoz;
		}

		if (maxPos != index)
		{
			int aux = heap.vector[index];
			heap.vector[index] = heap.vector[maxPos];
			heap.vector[maxPos] = aux;
			if (2 * maxPos + 1 < heap.length)
			{
				filter(heap, maxPos);
			}
		}
	}
}

//trebuie sa mutam elementele din heap intr-un arbore binar de cautare
struct treeNode {
	treeNode* left;
	treeNode* right;
	int info;
};

treeNode* insertInTree(treeNode* root, int info) {
	if (root) {
		if (info < root->info) {
			root->left = insertInTree(root->left, info);
		}
		else {
			root->right = insertInTree(root->right, info);
		}
		return root;
	}
	else {
		treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
		newNode->left = newNode->right = NULL;
		newNode->info = info;
		return newNode;
	}
}

//o sa fie afisat sortat -- in ordine
void printTreeInOrder(treeNode* root) {
	if (root) {
		printTreeInOrder(root->left);
		printf("%d ", root->info);
		printTreeInOrder(root->right);
	}
}

//elementele din arbore le vom pune acum intr-un hashTable; folosim chaining cu lista simpla
struct nod {
	int info;
	nod* next;
};

struct HashTable {
	nod* *heads;
	int length;
};

HashTable initHashTable(int length) {
	HashTable h;
	h.length = length;
	h.heads = (nod**)malloc(sizeof(nod*)*length);
	for (int i = 0; i < length; i++) {
		h.heads[i] = NULL;
	}

	return h;
}

int hashFunction(int length, int info) {
	return info % length;
}

//inserare la inceput in lista
void insertInList(nod* &head, int info) {
	nod* newNode = (nod*)malloc(sizeof(nod)); 
	newNode->info = info;
	newNode->next = head;

	head = newNode; //devine noul cap de lista
}

void insertInHashTable(HashTable h, int info) {
	if (h.length > 0) {
		int code = hashFunction(h.length, info);
		insertInList(h.heads[code], info);
	}
}

//functie care muta elementele din arbore in hashtable; seamana ft tare cu functia de 
//daca voiam sa inserez doar elementele care trec de un anumit prag, de ex. vreau sa fie val mai mici de 4
//pun aici conditie la root & primesc parametru
void transformTreeToHT(treeNode* root, HashTable h) {
	if (root) {
		transformTreeToHT(root->left, h);
		insertInHashTable(h, root->info);
		transformTreeToHT(root->right,h);
	}
}

//functie de afisare a listei
void printList(nod* head) {
	while (head) {
		printf("%d, ", head->info);
		head = head->next;
	}
}

//functie de afisare ht
void printHT(HashTable h) {
	for (int i = 0; i < h.length; i++) {
		printf("%d. ", (i + 1));//linia pe care suntem
		if (h.heads[i]) {
			printList(h.heads[i]);
		}
		printf("\n");
	}
}

void main() {
	HEAP heap = initHeap(6);
	for (int i = 0; i < heap.length; i++) {
		heap.vector[i] = i + 1;
	}

	//filtrare de jos in sus; de la ultimul parinte
	for (int i = (heap.length - 2) / 2; i >= 0; i--)
	{
		filter(heap, i);
	}

	//afisez heap-ul
	for (int i = 0; i < heap.length; i++) {
		printf("%d ", heap.vector[i]);
	}

	treeNode* root = NULL;
	//inserez elementele din heap in arbore
	for (int i = 0; i < heap.length; i++) {
		root = insertInTree(root, heap.vector[i]);
	}

	//afisez arborele
	printf("\nTree\n");
	printTreeInOrder(root);

	//pun elementele din arbore in hashtable
	HashTable h = initHashTable(7);
	transformTreeToHT(root, h);
	//afisare hash
	printf("\nHT:\n");
	printHT(h);

	system("pause");
}
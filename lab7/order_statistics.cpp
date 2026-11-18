/*
Ideea de baza

	Un arbore de statistica de ordine este un arbore binar de cautare in care fiecare nod retine si campul size,
adica numarul de noduri din subarborele care are ca radacina nodul respectiv (inclusiv nodul). Cu ajutorul lui size
putem afla rangul unui nod fara sa parcurgem tot arborele: rangul nodului x in subarborele lui este size(x->left) + 1.

1. Arborele binar de cautare perfect echilibrat

	BUILD_TREE - divide et impera pe cheile 1..n: radacina este elementul din mijloc, iar subarborii se construiesc
recursiv din cele doua jumatati. Size-ul se calculeaza la intoarcere: size = size(stanga) + size(dreapta) + 1.
Fiecare apel face un numar constant de operatii pentru un singur nod => T(n) = 2T(n/2) + O(1) => O(n).
(Nu este O(n log n) ca la merge sort, unde pe fiecare nivel se face si interclasarea in O(n).)

	OS_SELECT - comparam indicele cautat cu rangul nodului curent: daca sunt egale am gasit nodul, daca e mai mic
mergem in stanga, altfel in dreapta (si scadem din indice nodurile lasate in urma) => O(h) = O(log n).

	OS_DELETE - stergere din arbore binar de cautare, nodul fiind cautat dupa rang. Stim dinainte ca nodul exista,
deci scadem size-ul pe drum in timp ce coboram => O(h) = O(log n).
	Arborele nu trebuie re-echilibrat: stergerea nu creste inaltimea, deci ramane cel mult log n (n initial).

	OS_SELECT seamana cu Randomized-Select / QuickSelect: stim cate elemente sunt mai mici decat "pivotul" (nodul
curent) si continuam doar intr-o parte, doar ca partitia este deja facuta de structura arborelui.

2. Bonus - arbore rosu-negru

	Algoritmul din Cormen (editia 4: cap. 13 si cap. 17.1), dar cu NULL in loc de T.nil. Un nod NULL este
considerat negru si are size 0 (functiile culoare si marime), iar la stergere parintele lui x se retine separat
(xParent), pentru ca x poate fi NULL. Size-ul se creste pe drum la insertie, se scade pentru stramosii nodului care
dispare la stergere si se recalculeaza pentru cele 2 noduri la fiecare rotatie.
	BUILD = n inserari, fiecare O(log n) => O(n log n). SELECT si DELETE sunt O(log n) (inaltimea <= 2 log(n + 1)).

3. Complexitati

	                     un apel SELECT    un apel DELETE    BUILD
	arbore echilibrat    O(log n)          O(log n)          O(n)
	rosu-negru           O(log n)          O(log n)          O(n log n)   (n inserari a cate O(log n))

	Testul complet (BUILD + de n ori SELECT si DELETE):
	- arbore echilibrat: O(n) + n * O(log n) + n * O(log n) = O(n log n)
	- rosu-negru:        O(n log n) + n * O(log n) + n * O(log n) = O(n log n)
	Ambii au aceeasi complexitate pe tot testul, diferenta este doar la constante (fixup-uri, rotatii, recolorari).

Interpretarea graficelor (operatii = atribuiri + comparatii, media pe 5 teste)

	Html-ul "Valori operatii" - totalul pentru n apeluri
	Observatie: BUILD la arborele echilibrat este o dreapta, iar SELECT, DELETE si BUILD la rosu-negru cresc
putin mai repede decat o dreapta.
	Explicatie: BUILD echilibrat este O(n), iar celelalte sunt n apeluri a cate O(log n) => O(n log n).


	Html-ul "Operatii medii pe apel" - totalul impartit la n
	Observatie: BUILD la arborele echilibrat este o linie orizontala (6 operatii pe nod), iar SELECT, DELETE si o
inserare la rosu-negru au forma logaritmica (de ex. SELECT: 13, 22, 29, 32 pentru n = 100, 1.000, 5.000, 10.000).
	Explicatie: un apel SELECT / DELETE coboara un singur drum de lungime O(log n), iar la BUILD echilibrat fiecare
nod costa O(1). Se imparte la n (numarul de apeluri), nu la root->size, pentru ca root->size scade pana la 0.
	SELECT este identic la ambii arbori (aceleasi operatii pe nod, inaltimi apropiate), iar DELETE este mai scump la
rosu-negru din cauza fixup-ului.
*/

#include "order_statistics.h"

#include "catch2.hpp"
#include <iostream>

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define TEST 5

using namespace std;

// nr de operati se face doar pe arbore binar de cautare echilibrat
// daca se vrea bonus -> direct cu red-black tree sau avl
// daca nu bonus -> arbore binar de cautare echilibrat
// la prety print -  cel de data trecuta + sa se afiseze si size ul
// graficele impartite la size => se vad logaritmic

namespace lab07
{
	// === RED BLACK TREE

	string culoare(RB_Node* nod) {
		if (nod == NULL)
			return "BLACK";
		return nod->color;
	}

	int marime(RB_Node* nod) {
		if (nod == NULL)
			return 0;
		return nod->size;
	}

	// prin referinta pentru modificare
	void left_rotate(RB_Node*& root, RB_Node* x, Operation* op) {
		RB_Node* y = x->right;
		x->right = y->left;
		if (y->left)
			y->left->parent = x;

		y->parent = x->parent;

		if (x->parent == NULL)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else x->parent->right = y;

		y->left = x;
		x->parent = y;

		// y inlocuieste x => size vechi al lui x
		y->size = x->size;
		x->size = marime(x->left) + marime(x->right) + 1;

		// 9 atr + 3 cmp
		if (op)
			op->count(12);
	}

	// prin referinta pentru modificare
	void right_rotate(RB_Node*& root, RB_Node* x, Operation* op) {
		RB_Node* y = x->left;
		x->left = y->right;
		if (y->right)
			y->right->parent = x;

		y->parent = x->parent;

		if (x->parent == NULL)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else x->parent->right = y;

		y->right = x;
		x->parent = y;

		// y inlocuieste x => size vechi al lui x
		y->size = x->size;
		x->size = marime(x->left) + marime(x->right) + 1;

		// 9 atr + 3 cmp
		if (op)
			op->count(12);
	}

	// ==== insertie

	// prin referinta pentru modificare
	void RB_Insert_FixUp(RB_Node*& root, RB_Node* z, Operation* op) {
		while (culoare(z->parent) == "RED") {
			// while + if
			if (op)
				op->count(2);

			if (z->parent ==  z->parent->parent->left) {
				RB_Node* y = z->parent->parent->right;// unchi

				if (op)
					op->count(2);

				if (culoare(y) == "RED") {
					z->parent->color = "BLACK";
					y->color = "BLACK";
					z->parent->parent->color = "RED";
					z = z->parent->parent;
					// caz 1

					if (op)
						op->count(4);
				}
				else {
					if (op)
						op->count(1);

					if (z == z->parent->right) {
						z = z->parent;
						left_rotate(root, z, op);
						// caz 2

						if (op)
							op->count();
					}

					z->parent->color = "BLACK";
					z->parent->parent->color = "RED";
					right_rotate(root, z->parent->parent, op);
					// caz 3

					if (op)
						op->count(2);
				}
			}
			else {
				RB_Node* y = z->parent->parent->left;

				if (op)
					op->count(2);

				if (culoare(y) == "RED") {
					z->parent->color = "BLACK";
					y->color = "BLACK";
					z->parent->parent->color = "RED";
					z = z->parent->parent;
					// caz 1

					if (op)
						op->count(4);
				}
				else {
					if (op)
						op->count();

					if (z == z->parent->left) {
						z = z->parent;
						right_rotate(root, z, op);
						// caz 2

						if (op)
							op->count();
					}

					z->parent->color = "BLACK";
					z->parent->parent->color = "RED";
					left_rotate(root, z->parent->parent, op);
					// caz 3

					if (op)
						op->count(2);
				}
			}
		}

		// ultima cond while + culoare root
		if (op)
			op->count(2);

		root->color = "BLACK";
	}

	// prin referinta pentru modificare
	void RB_Insert(RB_Node*& root, RB_Node* z, Operation* op) {
		RB_Node* x = root;
		RB_Node* y = NULL;

		while (x != NULL) {
			y = x;
			x->size++;

			if (z->key < x->key)
				x = x->left;
			else x = x->right;

			// cond while + y + size + cmp + x
			if (op)
				op->count(5);
		}

		z->parent = y;

		if (y == NULL)
			root = z;
		else if (z->key < y->key)
			y->left = z;
		else y->right = z;

		z->left = NULL;
		z->right = NULL;
		z->color = "RED";
		z->size = 1;

		if (op)
			op->count(9);

		RB_Insert_FixUp(root, z, op);
	}

	// ==== insertie

	// ==== stergere

	RB_Node* treeMin(RB_Node* x, Operation* op) {
		while (x->left != NULL) {
			x = x->left;

			// cond while + atr
			if (op)
				op->count(2);
		}

		// cond while
		if (op)
			op->count();

		return x;
	}

	RB_Node* treeMax(RB_Node* x) {
		while (x->right != NULL)
			x = x->right;
		return x;
	}

	// prin referinta pentru modificare
	void RB_Transplant(RB_Node*& root, RB_Node* u, RB_Node* v, Operation* op) {
		if (u->parent == NULL)
			root = v;
		else if (u == u->parent->left)
			u->parent->left = v;
		else u->parent->right = v;

		if (v != NULL)
			v->parent = u->parent;

		if (op)
			op->count(4);
	}

	// prin referinta pentru modificare
	// x poate fi null => se trimite parintele
	void RB_Delete_FixUp(RB_Node*& root, RB_Node* x, RB_Node* xParent, Operation* op) {
		while (x != root && culoare(x) == "BLACK") {
			// 2 cond while + if urmator
			if (op)
				op->count(3);

			if (x == xParent->left) { // x poate fi NULL => xParent, nu x->parent
				RB_Node* w = xParent->right;

				if (op)
					op->count(2);

				if (culoare(w) == "RED") {
					w->color = "BLACK";
					xParent->color = "RED";
					left_rotate(root, xParent, op);
					w = xParent->right;
					// caz 1

					if (op)
						op->count(3);
				}

				if (op)
					op->count(2);

				if (culoare(w->left) == "BLACK" && culoare(w->right) == "BLACK") {
					w->color = "RED";
					x = xParent;
					xParent = x->parent;
					// caz 2

					if (op)
						op->count(3);
				}
				else {
					if (op)
						op->count();

					if (culoare(w->right) == "BLACK") {
						w->left->color = "BLACK";
						w->color = "RED";
						right_rotate(root, w, op);
						w = xParent->right;
						// caz 3

						if (op)
							op->count(3);
					}

					w->color = xParent->color;
					xParent->color = "BLACK";
					w->right->color = "BLACK";
					left_rotate(root, xParent, op);
					// caz 4

					x = root;

					if (op)
						op->count(4);
				}
			}
			else {
				RB_Node* w = xParent->left;

				if (culoare(w) == "RED") {
					w->color = "BLACK";
					xParent->color = "RED";
					right_rotate(root, xParent, op);
					w = xParent->left;
					// caz 1

					if (op)
						op->count(3);
				}

				if (op)
					op->count(2);

				if (culoare(w->right) == "BLACK" && culoare(w->left) == "BLACK") {
					w->color = "RED";
					x = xParent;
					xParent = x->parent;
					// caz 2

					if (op)
						op->count(3);
				}
				else {
					if (op)
						op->count();

					if (culoare(w->left) == "BLACK") {
						w->right->color = "BLACK";
						w->color = "RED";
						left_rotate(root, w, op);
						w = xParent->left;
						// caz 3

						if (op)
							op->count(3);
					}

					w->color = xParent->color;
					xParent->color = "BLACK";
					w->left->color = "BLACK";
					right_rotate(root, xParent, op);
					// caz 4

					x = root;

					if (op)
						op->count(4);
				}
			}
		}

		// ultima cond while + culoare
		if (op)
			op->count(3);
		if (x != NULL)
			x->color = "BLACK";
	}

	// prin referinta pentru modificare
	void RB_Delete(RB_Node*& root, RB_Node* z, Operation* op) {
		// actualizare size stramosi z
		RB_Node* p;
		if (z->left == NULL || z->right == NULL)
			p = z->parent;
		else p = treeMin(z->right, op)->parent;

		if (op)
			op->count(3);

		while (p != NULL) {
			p->size--;
			p = p->parent;

			if (op)
				op->count(3);
		}

		if (op)
			op->count();

		RB_Node* y = z;
		RB_Node* x = NULL;
		RB_Node* xParent = NULL;
		string yOgColor = y->color;

		if (op)
			op->count(4);

		if (z->left == NULL) {
			x = z->right;
			xParent = z->parent;
			RB_Transplant(root, z, z->right, op);

			if (op)
				op->count(3);
		}
		else if (z->right == NULL) {
			x = z->left;
			xParent = z->parent;
			RB_Transplant(root, z, z->left, op);

			if (op)
				op->count(4);
		}
		else {
			y = treeMin(z->right, op);
			yOgColor = y->color;
			x = y->right;

			if (y != z->right) {
				xParent = y->parent;
				RB_Transplant(root, y, y->right, op);
				y->right = z->right;
				y->right->parent = y;
			}
			else xParent = y;

			RB_Transplant(root, z, y, op);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
			y->size = z->size;

			if (op)
				op->count(13);
		}

		delete z;

		if (op)
			op->count();

		if (yOgColor == "BLACK")
			RB_Delete_FixUp(root, x, xParent, op);
	}

	// ==== stergere

	// === build RB

	RB_Node* buildRDTree(int n, Operation* op) {
		RB_Node* root = NULL; // arborele porneste gol

		for (int i = 1; i <= n; i++) {
			RB_Node* nod = new RB_Node;
			nod->key = i;

			if (op)
				op->count();

			RB_Insert(root, nod, op);
		}

		return root;
	}

	// === build RB

	// la fel ca osSelect
	RB_Node* RB_Select(RB_Node* root, int i, Operation* op) {
		if (root == NULL)
			return NULL;

		int r = marime(root->left);

		if (op)
			op->count(2);

		if (i == r)
			return root;

		if (op)
			op->count();

		if (i < r)
			return RB_Select(root->left, i, op);
		return RB_Select(root->right, i - r - 1, op);
	}

	void RB_OS_Delete(RB_Node*& root, int i, Operation* op) {
		// gasirea nodului face parte din stergere => se numara la delete
		RB_Node* z = RB_Select(root, i, op);
		if (z == NULL)
			return;

		RB_Delete(root, z, op);
	}

	void afisRB(int nivel, RB_Node* nod) {
		for (int i = 0; i < nivel; i++)
			cout << '\t';

		if (nod == NULL) {
			cout << "-\n";
			return;
		}

		cout << nod->key << " " << nod->color << " (size: " << nod->size << ")\n";

		if (nod->left != NULL || nod->right != NULL) {
			afisRB(nivel + 1, nod->left);
			afisRB(nivel + 1, nod->right);
		}
	}

	void prettyPrintRB(RB_Node* root) {
		if (root == NULL) {
			cout << "Arborele este gol\n";
			return;
		}

		afisRB(0, root);
	}

	void freeRB(RB_Node* root) {
		if (root == NULL)
			return;

		freeRB(root->left);
		freeRB(root->right);
		delete root;
	}

	// === RED BLACK TREE

	// ======= arbore echilibrat

	OS_Node* buildTree(int n, int left, int right, Operation* op)
	{
		// TODO: Implement the buildTree function

		if (op)
			op->count();

		if (left > right)
			return NULL;

		int mij = (left + right) / 2;

		OS_Node* nodNou = (OS_Node*)malloc(sizeof(OS_Node));
		nodNou->key = mij;
		nodNou->left = buildTree(n, left, mij - 1, op);
		nodNou->right = buildTree(n, mij + 1, right, op);

		nodNou->size = 1;
		if (nodNou->left)
			nodNou->size += nodNou->left->size;
		if (nodNou->right)
			nodNou->size += nodNou->right->size;

		// key, left, right, size
		if (op)
			op->count(4);

		return nodNou;
	}

	OS_Node* osSelect(OS_Node* root, int i, Operation* op)
	{
		// TODO: Implement the osSelect function

		if (root == NULL)
			return NULL;

		int r = 0;
		if (root->left)
			r = root->left->size;

		// comp + atr
		if (op)
			op->count(2);

		if (i == r)
			return root;

		// de la apel rec
		if (op)
			op->count();

		if (i < r)
			return osSelect(root->left, i, op);
		return osSelect(root->right, i - r - 1, op);
	}

	OS_Node* osDelete(OS_Node* root, int i, Operation* op)
	{
		// TODO: Implement the osDelete function

		if (root == NULL || i < 0 || i >= root->size)
			return root;

		int r = 0;
		if (root->left)
			r = root->left->size;

		root->size--; // arborele sters in subarbore root

		// atr r, size, cmp
		if (op)
			op->count(3);

		if (i < r) {
			if (op)
				op->count();

			root->left = osDelete(root->left, i, op);
			return root;
		}

		// de ce este pus? - nr operatiilor se face doar pe arbore
		if (op)
			op->count();

		if (i > r) {
			if (op)
				op->count();

			root->right = osDelete(root->right, i - r - 1, op);
			return root;
		}

		// root trebuie sters

		// cmp din if
		if (op)
			op->count();

		if (root->left == NULL) { // 1 copil
			if (op)
				op->count();

			OS_Node* copil = root->right;
			free(root);
			return copil;
		}

		// cmp in if
		if (op)
			op->count();

		if (root->right == NULL) { // 1 copil
			if (op)
				op->count();

			OS_Node* copil = root->left;
			free(root);
			return copil;
		}

		// 2 copii => succesor (cel mai mic dreapta)
		OS_Node* succ = root->right;
		if (op)
			op->count();

		while (succ->left != NULL) {
			// atr + cmp
			if (op)
				op->count(2);

			succ = succ->left;
		}

		// ultima cmp din while + key + right
		if (op)
			op->count(3);

		root->key = succ->key;
		root->right = osDelete(root->right, 0, op);

		return root;
	}

	void afisOS(int nivel, OS_Node* nod) {
		for (int i = 0; i < nivel; i++)
			cout <<'\t';

		if (nod == NULL) {
			cout << "-\n";
			return;
		}

		cout << nod->key << " (size: " << nod->size << ")\n";

		if (nod->left != NULL || nod->right != NULL) {
			afisOS(nivel + 1, nod->left);
			afisOS(nivel + 1, nod->right);
		}
	}

	void prettyPrint(OS_Node* root)
	{
		// TODO: Implement the prettyPrint function

		if (root == NULL) {
			cout << "Arborele este gol\n";
			return;
		}

		afisOS(0, root);
	}

	void freeOS(OS_Node* root) {
		if (root == NULL)
			return;

		freeOS(root->left);
		freeOS(root->right);
		free(root);
	}

	// ======= arbore echilibrat

	void demo(int n)
	{
		// TODO: Implement the demo function as described in the readme.
		// This might include:
		// 1. Calling buildTree(n) and pretty-printing the result.
		// 2. Calling osSelect for a few random ranks.
		// 3. Calling osDelete for a few random ranks and pretty-printing after each deletion.

		cout << "Arbore binar de cautare perfect echilibrat\n\n";
		OS_Node* root = buildTree(n, 1, n);
		cout << "Arborele: \n";
		prettyPrint(root);

		cout << "\nOS_Select pt 3 indici random:\n";
		for (int i = 0; i < 3; i++) {
			int index = rand() % root->size;
			OS_Node* nod = osSelect(root, index);
			cout << "index: " << index << ", valoare key: " << nod->key << "\n";
		}

		cout << "\nOS_Select + OS_Delete pt 3 indici random:\n";
		for (int i = 0; i < 3; i++) {
			int index = rand() % root->size;
			OS_Node* nod = osSelect(root, index);
			cout << "index: " << index << ", valoare key: " << nod->key << "\n";

			root = osDelete(root, index);
			cout << "Dupa OS_Delete\n";
			prettyPrint(root);
			cout << '\n';
		}

		freeOS(root);

		cout << "\nArbore red-black\n\n";
		RB_Node* rbRoot = buildRDTree(n, NULL);
		cout << "Arborele: \n";
		prettyPrintRB(rbRoot);

		cout << "\nRB_Select pt 3 indici random:\n";
		for (int i = 0; i < 3; i++) {
			int index = rand() % rbRoot->size;
			RB_Node* nod = RB_Select(rbRoot, index, NULL);
			cout << "index: " << index << ", valoare key: " << nod->key << "\n";
		}

		cout << "\nRB_Select + RB_Delete pt 3 indici random:\n";
		for (int i = 0; i < 3; i++) {
			int index = rand() % rbRoot->size;
			RB_Node* nod = RB_Select(rbRoot, index, NULL);
			cout << "index: " << index << ", valoare key: " << nod->key << "\n";

			RB_Delete(rbRoot, nod, NULL);
			cout << "Dupa RB_Delete\n";
			prettyPrintRB(rbRoot);
			cout << '\n';
		}

		freeRB(rbRoot);
	}

	// lipsea profiler din template asa ca am adaugat eu
	void performance(Profiler& profiler)
	{
		// TODO: Implement the performance evaluation as described in section 3.4 of the readme.

		profiler.reset("Valori operatii");

		for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
			cout << "size actual: " << size << '\n';

			for (int test = 0; test < TEST; test++) {
				// arbore perfect echilibrat
				auto opBuild = profiler.createOperation("Build_Echilibrat", size);
				auto opSelect = profiler.createOperation("Select_Echilibrat", size);
				auto opDelete = profiler.createOperation("Delete_Echilibrat", size);

				OS_Node* root = buildTree(size, 1, size, &opBuild);

				// de n ori OS_SELECT + OS_DELETE pe un indice random dintre elementele ramase
				for (int ramase = size; ramase > 0; ramase--) {
					int i = rand() % ramase;
					osSelect(root, i, &opSelect);
					root = osDelete(root, i, &opDelete);
				}

				freeOS(root); // ar trebui sa fie deja gol

				// arbore rosu-negru
				auto opBuildRB = profiler.createOperation("Build_RB", size);
				auto opSelectRB = profiler.createOperation("Select_RB", size);
				auto opDeleteRB = profiler.createOperation("Delete_RB", size);

				RB_Node* rbRoot = buildRDTree(size, &opBuildRB);

				for (int ramase = size; ramase > 0; ramase--) {
					int i = rand() % ramase;
					RB_Select(rbRoot, i, &opSelectRB);
					RB_OS_Delete(rbRoot, i, &opDeleteRB);
				}

				freeRB(rbRoot);
			}
		}

		profiler.divideValues("Build_Echilibrat", TEST);
		profiler.divideValues("Select_Echilibrat", TEST);
		profiler.divideValues("Delete_Echilibrat", TEST);

		profiler.divideValues("Build_RB", TEST);
		profiler.divideValues("Select_RB", TEST);
		profiler.divideValues("Delete_RB", TEST);

		// cerinta - un grafic cu 3 serii
		profiler.createGroup("Arbore_Echilibrat", "Build_Echilibrat", "Select_Echilibrat", "Delete_Echilibrat");
		profiler.createGroup("Arbore_RB", "Build_RB", "Select_RB", "Delete_RB");

		profiler.showReport();

		// operatii medii (total / n) => curbe logaritmice

		profiler.reset("Operatii medii pe apel");

		for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
			cout << "size actual: " << size << '\n';

			auto opBuild = profiler.createOperation("Build_Echilibrat_mediu", size);
			auto opSelect = profiler.createOperation("Select_Echilibrat_mediu", size);
			auto opDelete = profiler.createOperation("Delete_Echilibrat_mediu", size);

			auto opBuildRB = profiler.createOperation("Build_RB_mediu", size);
			auto opSelectRB = profiler.createOperation("Select_RB_mediu", size);
			auto opDeleteRB = profiler.createOperation("Delete_RB_mediu", size);

			for (int test = 0; test < TEST; test++) {
				// arbore perfect echilibrat
				OS_Node* root = buildTree(size, 1, size, &opBuild);

				for (int ramase = size; ramase > 0; ramase--) {
					int i = rand() % ramase;
					osSelect(root, i, &opSelect);
					root = osDelete(root, i, &opDelete);
				}

				freeOS(root);

				// arbore rosu-negru
				RB_Node* rbRoot = buildRDTree(size, &opBuildRB);

				for (int ramase = size; ramase > 0; ramase--) {
					int i = rand() % ramase;
					RB_Select(rbRoot, i, &opSelectRB);
					RB_OS_Delete(rbRoot, i, &opDeleteRB);
				}

				freeRB(rbRoot);
			}

			// impartirea la size (numarul de apeluri) se face aici, pentru ca e diferita la fiecare punct,
			// iar divideValues imparte toate punctele seriei la acelasi numar
			// count(total / size - total) => in punctul curent ramane doar total / size
			// (se imparte la size, nu la root->size, pentru ca root->size scade pana la 0)
			opBuild.count(opBuild.get() / size - opBuild.get());
			opSelect.count(opSelect.get() / size - opSelect.get());
			opDelete.count(opDelete.get() / size - opDelete.get());

			opBuildRB.count(opBuildRB.get() / size - opBuildRB.get());
			opSelectRB.count(opSelectRB.get() / size - opSelectRB.get());
			opDeleteRB.count(opDeleteRB.get() / size - opDeleteRB.get());
		}

		profiler.divideValues("Build_Echilibrat_mediu", TEST);
		profiler.divideValues("Select_Echilibrat_mediu", TEST);
		profiler.divideValues("Delete_Echilibrat_mediu", TEST);

		profiler.divideValues("Build_RB_mediu", TEST);
		profiler.divideValues("Select_RB_mediu", TEST);
		profiler.divideValues("Delete_RB_mediu", TEST);

		profiler.createGroup("Arbore_Echilibrat_mediu", "Build_Echilibrat_mediu", "Select_Echilibrat_mediu", "Delete_Echilibrat_mediu");
		profiler.createGroup("Arbore_RB_mediu", "Build_RB_mediu", "Select_RB_mediu", "Delete_RB_mediu");

		profiler.showReport();
	}

	// verifica daca size-ul fiecarui nod este corect
	bool sizeCorectOS(OS_Node* nod) {
		if (nod == NULL)
			return true;

		int s = 1;
		if (nod->left != NULL)
			s += nod->left->size;
		if (nod->right != NULL)
			s += nod->right->size;

		return nod->size == s && sizeCorectOS(nod->left) && sizeCorectOS(nod->right);
	}

	// intoarce inaltimea neagra sau -1 daca arborele nu respecta proprietatile rosu-negru / size
	int verificaRB(RB_Node* nod) {
		if (nod == NULL)
			return 1;

		if (nod->size != marime(nod->left) + marime(nod->right) + 1)
			return -1;

		if (nod->left != NULL && nod->left->parent != nod)
			return -1;
		if (nod->right != NULL && nod->right->parent != nod)
			return -1;

		if (nod->color == "RED" && (culoare(nod->left) == "RED" || culoare(nod->right) == "RED"))
			return -1;

		int hl = verificaRB(nod->left);
		int hr = verificaRB(nod->right);
		if (hl == -1 || hr == -1 || hl != hr)
			return -1;

		if (nod->color == "BLACK")
			return hl + 1;
		return hl;
	}

	TEST_CASE("Order Statistics Tree")
	{
		// TODO: Implement unit tests for the Order Statistics Tree
		// Use REQUIRE( [condition] ); to perform checks.

		SECTION("BUILD TREE") {
			OS_Node* root = buildTree(11, 1, 11);

			REQUIRE(root != NULL);
			REQUIRE(root->key == 6);
			REQUIRE(root->size == 11);

			REQUIRE(root->left->key == 3);
			REQUIRE(root->left->size == 5);
			REQUIRE(root->right->key == 9);
			REQUIRE(root->right->size == 5);

			REQUIRE(sizeCorectOS(root));

			freeOS(root);

			REQUIRE(buildTree(0, 1, 0) == NULL);
        }
		SECTION("OS SELECT") {
			OS_Node* root = buildTree(11, 1, 11);

			for (int i = 0; i < 11; i++) {
				OS_Node* nod = osSelect(root, i);
				REQUIRE(nod != NULL);
				REQUIRE(nod->key == i + 1);
			}

			REQUIRE(osSelect(root, 11) == NULL);
			REQUIRE(osSelect(root, -1) == NULL);

			freeOS(root);
        }
		SECTION("OS DELETE") {
			OS_Node* root = buildTree(11, 1, 11);

			// se sterge radacina (6), are 2 copii => ii ia locul succesorul 7
			root = osDelete(root, 5);
			REQUIRE(root->key == 7);
			REQUIRE(root->size == 10);
			REQUIRE(sizeCorectOS(root));

			// cheile ramase: 1 2 3 4 5 7 8 9 10 11
			int ramase[] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11};
			for (int i = 0; i < 10; i++)
				REQUIRE(osSelect(root, i)->key == ramase[i]);

			// se sterge un nod din dreapta (cheia 10, indice 8)
			root = osDelete(root, 8);
			REQUIRE(root->size == 9);
			REQUIRE(sizeCorectOS(root));
			REQUIRE(osSelect(root, 8)->key == 11);

			// se sterg toate nodurile ramase, mereu ultimul
			for (int k = 9; k > 0; k--) {
				root = osDelete(root, k - 1);
				REQUIRE(sizeCorectOS(root));
			}
			REQUIRE(root == NULL);
        }

		SECTION("BONUS - ROSU-NEGRU") {
			RB_Node* rbRoot = buildRDTree(11, NULL);

			REQUIRE(rbRoot->color == "BLACK");
			REQUIRE(rbRoot->size == 11);
			REQUIRE(verificaRB(rbRoot) != -1);

			for (int i = 0; i < 11; i++)
				REQUIRE(RB_Select(rbRoot, i, NULL)->key == i + 1);

			// se sterg random toate nodurile si se verifica arborele dupa fiecare stergere
			for (int ramase = 11; ramase > 0; ramase--) {
				RB_OS_Delete(rbRoot, rand() % ramase, NULL);

				REQUIRE(verificaRB(rbRoot) != -1);
				REQUIRE(marime(rbRoot) == ramase - 1);
			}
			REQUIRE(rbRoot == NULL);

			// arbore mai mare
			rbRoot = buildRDTree(1000, NULL);
			REQUIRE(verificaRB(rbRoot) != -1);
			for (int i = 0; i < 1000; i++)
				REQUIRE(RB_Select(rbRoot, i, NULL)->key == i + 1);

			for (int ramase = 1000; ramase > 0; ramase--) {
				RB_OS_Delete(rbRoot, rand() % ramase, NULL);
				REQUIRE(verificaRB(rbRoot) != -1);
			}
			REQUIRE(rbRoot == NULL);
		}
	}

} // namespace lab07
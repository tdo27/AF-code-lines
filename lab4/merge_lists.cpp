/*
Ideea de baza a algoritmului

	Pentru a interclasa eficient k liste sortate intr-o singura lista finala, algoritmul foloseste Min-Heap.
	In loc sa interclaseze listele doua cate doua, ceea ce ar necesita mai multe operatii, algoritmul pastreaza in heap pointeri
catre listele care mai contin elemente. Astfel, poate identifica rapid lista al carei prim element este cel mai mic.

Pasii principali ai algoritmului sunt urmatorii:

	- Mai intai, sunt eliminate din vectorul initial listele goale.
	- Se construieste un Min-Heap folosind functia build_heap_bottomup. In acest fel, la radacina heap-ului se afla intotdeauna
lista al carei prim element are valoarea minima dintre toate listele ramase.
	- Cat timp heap-ul mai contine liste (size > 0), se repeta urmatoarele operatii:
		- Se extrage primul element din lista aflata la radacina heap-ului (lists[0]) si se adauga la sfarsitul listei rezultat,
folosind functiile remove_first si insert_last.
		- Daca lista respectiva a devenit goala, aceasta este inlocuita cu ultima lista din heap,
iar dimensiunea heap-ului este redusa cu o unitate (size--).
		- Se apeleaza functia min_heapify asupra radacinii pentru a restabili proprietatea de Min-Heap.
In urma acestei operatii, pointerii catre liste sunt rearanjati astfel incat lista cu urmatorul element minim sa ajunga la radacina.

	- Prin repetarea acestor pasi, elementele sunt preluate in ordine crescatoare si adaugate in lista finala pana cand toate listele initiale au fost parcurse.

Analiza complexitatii

	Construirea heap-ului initial, care contine cel mult k liste, are o complexitate de O(k).
	Fie n numarul total de elemente din toate listele. Pentru fiecare element extras, operatiile de eliminare din lista initiala si de adaugare in lista rezultat
au complexitatea O(1).
	Dupa fiecare extragere, heap-ul trebuie reorganizat cu ajutorul functiei min_heapify. In cel mai rau caz, aceasta operatie parcurge inaltimea heap-ului,
avand o complexitate de O(log k).
	Deoarece fiecare dintre cele n elemente este procesat o singura data, operatiile de extragere si reorganizare a heap-ului au impreuna o complexitate de O(n*log k).

Interpretarea graficelor
	Cazul 1: k fix si n variabil
	In primul caz, numarul de liste k este mentinut constant, avand valorile 5, 10 si 100, in timp ce numarul total de elemente n variaza intre 100 si 10.000.
	Graficul arata ca numarul total de operatii, reprezentate de comparatii si atribuiri, creste aproximativ liniar odata cu cresterea lui n.
	Pentru fiecare valoare a lui k, se obtine o curba cu o panta diferita. Cu cat numarul de liste este mai mare, cu atat sunt necesare mai multe operatii pentru procesarea fiecarui element.
	Astfel, curba corespunzatoare lui k=100 se situeaza deasupra celor pentru k=10 si k=5, indicand un numar mai mare de operatii. Acest lucrur se potriveste si cu complexitatea O(n*log k).

	Cazul 2: n fix si k variabil
	In al doilea caz, numarul total de elemente este mentinut constant, la valoarea n=10.000, in timp ce numarul de liste k variaza intre 10 si 500.
	Graficul evidentiaza o crestere a numarului de operatii odata cu marirea lui k. Deoarece n ramane constant, complexitatea O(n*log k) depinde de variatia lui log k.
	La inceput, cresterea numarului de operatii este mai vizibila, iar pe masura ce k devine mai mare, ritmul de crestere se reduce treptat.
	Acest comportament este specific functiei logaritmice si poate fi explicat prin faptul ca inaltimea unui heap binar creste proportional cu log k.
*/

#include "merge_lists.h"

#include "catch2.hpp"

#include <iostream>

#define MAX_SIZE_N 10000
#define STEP_SIZE_N 100
#define MAX_SIZE_K 500
#define STEP_SIZE_K 10
#define TEST 5

using namespace std;

// l1: 7->9->11
// l2: 3->5->7
// l3: 0->9->15
// etc.

// impartire n elemente in k liste, listele fiind de dimensiuni diferite
// se genereaza un vector de dimensiune k, unde se retin dimensiunile listelor
// dim intre n / (2 * k), n / k
// sizes[k]
// FillRandomArray(sizes, k, n / (2 * k), n / k) -- n / k - in cel mai rau caz se pun k * (n / k) = n elemente din start
// for (int i = 0; i < k; i++)
//		tot += sizes[i]
// bucla -> tot == n, cat timp nu este asa, tot++; sizes[rand()%k]++;
// se fac listele pentru cele k sizes

// cerinta 1 - crearea k liste, n elemente total + interclasarea a 2 siruri

// cerinta 2 - min-heap - unde lk -> first sunt nodurile, cand vreau sa scot un element
// pentru ca a fost interclasat, merg la urmatorul din lista si dupa aplic heapify
// a[i] > a[l]
// a[i] = first->value
// a[0] - remove_first dupa insert_last

// daca lista este goala dupa remove_first, scad dimensiunea heapului, aduc o lista ramasa ca radacina
// dupa aplic heapify

// O(n * logK k) - complexitate

// cerinta 3 - doar pe AVG
// caz 1) k - fix si se variaza n (n de la 100 pana la 10000, din 100), pt k = 5, 10, 100
// pt fiecare k, cele 3 curbe intr-un singur grafic
// caz 2) n - fix, k se variaza intre 10 la 500, din 10 in 10

// 2 pagini HTML - unul pentru fiecare caz - se poate face direct total operatie, fara asignari si comparatii diferit

namespace lab04 {
	NodeT* create_node(int value)
	{
		// TODO: Function to create a node with a specified value, with next pointing to NULL.

		NodeT *p = (NodeT*)malloc(sizeof(NodeT));

		if (p == NULL)
			return NULL;

		p->value = value;
		p->next = NULL;
		return p;
	}

	ListT* create_list(int* values, int n) 
	{
		// TODO: Function to create a list with values specified from the input array. If the array is empty or NULL, create an empty list.

		ListT *l = (ListT*)malloc(sizeof(ListT));

		if (l == NULL)
			return NULL;

		l->first = l->last = NULL;

		if (values == NULL || n <= 0)
			return l;

		NodeT *p = create_node(values[0]);
		l->first = l->last = p;

		int i = 1;
		while (i < n) {
			p = create_node(values[i]);
			l->last->next = p;
			l->last = p;
			i++;
		}

		return l;
	}

	NodeT* remove_first(ListT* list) 
	{
		// TODO: Function to remove the first node from the list. Return the removed node. If the list is empty or NULL return NULL. 
		//       DO NOT forget to set the ->next of the removed node to NULL.

		if (list == NULL || list->first == NULL)
			return NULL;

		NodeT *p = list->first;
		list->first = p->next;
		p->next = NULL;
		return p;
	}

	void insert_last(ListT* list, NodeT* node) {
		// TODO: Function to insert a node as the tail of the list.

		if (node == NULL || list == NULL) {
			cout << "ceva este null (nu a fost initializat)\n";
			return ;
		}
		// lista goala
		if (list->first == NULL) {
			node->next = NULL;
			list->first = list->last = node;
		}
		else {
			list->last->next = node;
			node->next = NULL;
			list->last = node;
		}
	}

	void print_list(ListT* list)
	{
		// TODO: Function to print the contents of the lists. A suggestive message should be printed if the list is empty or NULL.
		if (list == NULL) {
			cout << "lista este NULL (nu a fost initializata)\n";
			return;
		}

		if (list->first == NULL) {
			cout << "lista este goala\n";
			return;
		}

		cout << "lista: \n";
		NodeT *p = list->first;
		while (p != NULL) {
			cout << p->value << " ";
			p = p->next;
		}
		cout << '\n';
	}

	bool is_empty(ListT* list)
	{
		// TODO: Return true if the list is empty. Return false otherwise.
		if (list->first == NULL)
			return true;
		return false;
	}

	void destroy_list(ListT** list)
	{
		// TODO: Free the memory of each node and finally the memory allocated for the list.
		if (list == NULL || *list == NULL)
			return;

		NodeT *p = (*list)->first;

		while (p != NULL) {
			(*list)->first = p->next;
			free(p);
			p = (*list)->first;
		}

		free(*list);
	}


	//GENERATION AND MERGING ALGORITHMS
	ListT** generate_k_sorted_lists(int n, int k)
	{
		// TODO: Generate k sorted lists of equal size that have the sum of elements equal to n. The lists should be stored in an array.

		ListT** lists = (ListT**)malloc(k * sizeof(ListT*));
		int* sizes = (int*)malloc(sizeof(int) * k);
		int tot = 0;

		FillRandomArray(sizes, k, n / (2 * k), n / k);
		// n / k - in cel mai rau caz se pun k * (n / k) = n elemente din start
		for (int i = 0; i < k; i++)
			tot += sizes[i];
		while (tot < n) {
			tot++;
			sizes[rand()%k]++;
		}

		for (int i = 0; i < k; i++) {
			int *values = (int*)malloc(sizeof(int) * sizes[i]);
			if (sizes[i] > 0)
				FillRandomArray(values, sizes[i], 10, 500, false, ASCENDING);
			lists[i] = create_list(values, sizes[i]);
			free(values);
		}

		free(sizes);
		return lists;
	}

	ListT* merge_2_lists(ListT* list_A, ListT* list_B)
	{
		// TODO: Merge two sorted lists.

		ListT** lists = (ListT**)malloc(2 * sizeof(ListT*));
		lists[0] = list_A;
		lists[1] = list_B;

		ListT* res = merge_k_lists(lists, 2);
		free(lists);

		return res;
	}

	int parent(int i) {
		return (i - 1) / 2;
	}

	int left(int i) {
		return (2 * i) + 1;
	}

	int right(int i) {
		return (2 * i) + 2;
	}

	void min_heapify(ListT* lists[], int size, int i, Operation *op)
	{
		// TODO: Apply min-heapify considering the heads of the lists (you should swap the lists pointers based on the heads' values)
		int lMin = i;
		int l = left(i);
		int r = right(i);

		if (l < size && l >= 0) {
			if (op)
				op->count();

			if (lists[l]->first->value < lists[i]->first->value)
				lMin = l;
			else lMin = i;
		}

		if (r < size && r >= 0) {
			if (op)
				op->count();

			if (lists[r]->first->value < lists[lMin]->first->value)
				lMin = r;
		}

		if (lMin != i) {
			if (op)
				op->count(3);

			//swap(lists[i]->first->value, lists[lMin]->first->value);
			// sub forma asta se interschimba valorile, nu listele in sine

			ListT* p = lists[i];
			lists[i] = lists[lMin];
			lists[lMin] = p;

			min_heapify(lists, size, lMin, op);
		}
	}

	void build_heap_buttomup(ListT* lists[], int size, Operation* op)
	{
		// TODO: Build the heap of lists in a bottom-up manner using min-heapify.

		for (int i = size / 2 - 1; i >= 0; --i) {
			min_heapify(lists, size, i, op);
		}
	}

	ListT* merge_k_lists(ListT* lists[], int size, Operation* op)
	{
		// TODO: Merge the k sorted lists.

		ListT* res = create_list(NULL, 0);

		// daca am liste goale
		int v_size = 0;
		for (int i = 0; i < size; i++) {
			if (!is_empty(lists[i])) {
				lists[v_size] = lists[i];
				v_size++;
			}
		}
		size = v_size; // doar nr de liste non empty

		if (size > 0)
			build_heap_buttomup(lists, size, op);

		while (size > 0) {
			NodeT *minNod = remove_first(lists[0]);
			insert_last(res, minNod);

			// op din remove_first si insert_last
			// 3 remove_first (la swap ul listelor)
			// 1 cand salvez in minNod
			// 3 insert_last (atat in prima situatie cand lista este goala (node->next = NULL,
			// list->first = list->last = node) cat si cand nu este (list->last->next = node,
			// node->next = NULL, list->last = node
			if (op)
				op->count(7);

			if (is_empty(lists[0])) {
				lists[0] = lists[size - 1];
				size--;
				if (op)
					op->count();
			}

			if (size > 0)
				min_heapify(lists, size, 0, op);
		}

		return res;
	}

    void demonstrate(int n, int k)
    {
        // TODO: Write a small demo for the list operations, list generation and merging functions.

		// cerinta 1 - interclasarea a doua liste
		// am ales ca cele doua liste sa aiba 20 de elemente impreuna
		ListT** lists_2 = generate_k_sorted_lists(20, 2);

		cout << "prima lista: \n";
		print_list(lists_2[0]);
		cout << "\na doua lista: \n";
		print_list(lists_2[1]);

		ListT* res = merge_2_lists(lists_2[0], lists_2[1]);
		cout << "\ninterclasarea celor 2 liste: \n";
		print_list(res);

		// se poate folosi free ca am distrus la creare nodurile si ele sunt deja goale
		free(lists_2[0]);
		free(lists_2[1]);
		free(lists_2);

		destroy_list(&res);// am dat free la res pentru a fi folosit si la a 2-a cerinta
		// cerinta 2 - interclasarea a k liste
		ListT** lists = generate_k_sorted_lists(n, k);
		ListT** og_list = (ListT**)malloc(k * sizeof(ListT*));
		for (int i = 0; i < k; i++) {
			og_list[i] = lists[i];
		}
		// necesar pentru eliberare memorie, deoarece eu modific lists

		cout << '\n';
		for (int i = 0; i < k; i++) {
			cout << "lista " << i << ": \n";
			print_list(lists[i]);
		}

		res = merge_k_lists(lists, k);

		cout << "\ninterclasarea celor " << k << " liste: \n";
		print_list(res);

		for (int i = 0; i < k; i++) {
			free(og_list[i]);
		}
		free(og_list);
		free(lists);

		destroy_list(&res);
    }

	bool is_list_sorted(ListT* list, int expected_size) {
		if (list == NULL || list->first == NULL) {
			return expected_size == 0;
		}

		int count = 0;
		NodeT* current = list->first;

		while (current != NULL) {
			count++;
			if (current->next != NULL && current->value > current->next->value) {
				return false;
			}
			current = current->next;
		}

		return count == expected_size;
	}

    TEST_CASE("Merge lists")
    {
        // TODO: Write your unit tests here
        // Use REQUIRE( [condition] ); to perform checks
        // You can use the generate_k_sorted_lists to generate input data
        // and create a IsListSorted method to test the result of your sorting function on the lists.
        // For example:
        //   ListT* lists[100] = generate_k_sorted_lists(1000, 100)
		//	 ListT* sorted_list = merge_k_lists(lists, 100)
		//   for(int i=0; i<100; i++)
        //   	REQUIRE( IsListSorted(lists[i]) );

		// n mic, k mic
		int n1 = 20, k1 = 4;
		ListT** lists1 = generate_k_sorted_lists(n1, k1);
		ListT** og_list1 = (ListT**)malloc(k1 * sizeof(ListT*));
		for (int i = 0; i < k1; i++) og_list1[i] = lists1[i];

		ListT* sorted_list1 = merge_k_lists(lists1, k1, NULL);
		REQUIRE(is_list_sorted(sorted_list1, n1));

		for (int i = 0; i < k1; i++) free(og_list1[i]);
		free(og_list1);
		free(lists1);
		destroy_list(&sorted_list1);


		// n mare, k mare
		int n2 = 1000, k2 = 100;
		ListT** lists2 = generate_k_sorted_lists(n2, k2);
		ListT** og_list2 = (ListT**)malloc(k2 * sizeof(ListT*));
		for (int i = 0; i < k2; i++) og_list2[i] = lists2[i];

		ListT* sorted_list2 = merge_k_lists(lists2, k2, NULL);
		REQUIRE(is_list_sorted(sorted_list2, n2));

		for (int i = 0; i < k2; i++) free(og_list2[i]);
		free(og_list2);
		free(lists2);
		destroy_list(&sorted_list2);


		// k = 1 (o singură lista)
		int n3 = 50, k3 = 1;
		ListT** lists3 = generate_k_sorted_lists(n3, k3);
		ListT** og_list3 = (ListT**)malloc(k3 * sizeof(ListT*));
		for (int i = 0; i < k3; i++) og_list3[i] = lists3[i];

		ListT* sorted_list3 = merge_k_lists(lists3, k3, NULL);
		REQUIRE(is_list_sorted(sorted_list3, n3));

		for (int i = 0; i < k3; i++) free(og_list3[i]);
		free(og_list3);
		free(lists3);
		destroy_list(&sorted_list3);
    }

    void performance(Profiler& profiler, ListsCase whichCase)
    {
        // TODO: Write your performance analysis here, as explained in the readme.
        // You should use profiler.createOperation to pass the counter to each sorting function

		if (whichCase == FIXED_K) {
			// k = 5, k = 10, k = 100
			profiler.reset("k_Fixat");

			int K[] = {5, 10, 100};

			for (int i = 0; i < 3; i++) {
				cout << "k actual: " << K[i] << '\n';

				for (int size = STEP_SIZE_N; size <= MAX_SIZE_N; size += STEP_SIZE_N) {
					cout << "size actual: " << size << '\n';

					for (int test = 0; test < TEST; test++){
						ListT** lists = generate_k_sorted_lists(size, K[i]);

						ListT** og_list = (ListT**)malloc(K[i] * sizeof(ListT*));
						for (int j = 0; j < K[i]; j++) {
							og_list[j] = lists[j];
						}

						ListT* res = NULL;

						if (K[i] == 5) {
							auto op5 = profiler.createOperation("Operation_5", size);
							res = merge_k_lists(lists, K[i], &op5);
						}
						if (K[i] == 10) {
							auto op10 = profiler.createOperation("Operation_10", size);
							res = merge_k_lists(lists, K[i], &op10);
						}
						if (K[i] == 100) {
							auto op100 = profiler.createOperation("Operation_100", size);
							res = merge_k_lists(lists, K[i], &op100);
						}

						for (int j = 0; j < K[i]; j++) {
							free(og_list[j]);
						}
						free(og_list);
						free(lists);
						destroy_list(&res);
					}
				}
			}

			profiler.createGroup("Interclasare_k_fix", "Operation_5", "Operation_10", "Operation_100");
			profiler.showReport();
		}
		else {
			// n = 10000
			profiler.reset("n_Fixat");

			for (int k = 10; k < MAX_SIZE_K; k += STEP_SIZE_K) {
				cout << "k actual: " << k << '\n';

				for (int test = 0; test < TEST; test++) {
					ListT** lists = generate_k_sorted_lists(MAX_SIZE_N, k);

					ListT** og_list = (ListT**)malloc(k * sizeof(ListT*));
					for (int j = 0; j < k; j++) {
						og_list[j] = lists[j];
					}

					auto opK = profiler.createOperation("Operation_N_fix", k);
					ListT* res = merge_k_lists(lists, k, &opK);

					for (int j = 0; j < k;  j++) {
						free(og_list[j]);
					}
					free(og_list);
					free(lists);
					destroy_list(&res);
				}
			}

			profiler.showReport();
		}
    }

} // namespace lab04

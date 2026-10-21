/*

1. Analiza numarului de operatii (atribuiri, comparatii si efort total)

Observatie: Graficele pentru atribuiri, comparatii si efortul total se suprapun pentru cele doua variante de Insertion Sort, iterativa si recursiva.

Explicatie: Ambele variante folosesc aceeasi logica de sortare. Diferenta este ca varianta iterativa foloseste bucle, iar cea recursiva foloseste apeluri de functii.
Elementele sunt comparate si mutate de acelasi numar de ori, astfel ca numarul total de operatii ramane aproape identic.

2. Analiza timpului de rulare

Observatie: Varianta recursiva este semnificativ mai lenta. Pentru 10.000 de elemente, varianta iterativa a avut 2646 de unitati de timp,
in timp ce cea recursiva a ajuns la 6025, fiind de peste doua ori mai lenta.

Explicatie: Diferenta apare din cauza apelurilor recursive, care necesita timp suplimentar pentru gestionarea parametrilor si a stivei de executie.
Functiile sunt apelate de foarte multe ori, in timp ce varianta iterativa foloseste cateva variabile locale si evita acest cost suplimentar.

3. Confirmarea complexitatii teoretice

Observatie: Graficele arata o crestere accelerata, aproximativ parabolica, atat pentru numarul de operatii, cat si pentru timpul de rulare.

Explicatie: Acest lucru confirma complexitatea O(N^2) a algoritmului Insertion Sort in cazul mediu. Practic, daca numarul de elemente creste de 10 ori,
numarul de operatii poate creste de aproximativ 100 de ori.

4. Consumul de memorie

Observatie: Varianta recursiva poate deveni problematica atunci cand este folosita pentru vectori mari.

Explicatie: Fiecare apel recursiv ocupa spatiu pe stiva de executie, iar adancimea recursivitatii poate ajunge pana la N. Din acest motiv, exista riscul de Stack Overflow.
In schimb, varianta iterativa sorteaza elementele direct in vector si foloseste memorie suplimentara constanta, avand complexitatea spatiala O(1),
ceea ce o face mai eficienta din acest punct de vedere.

*/

#include "heap.h"

#include "catch2.hpp"

#include <iostream>

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define TEST 5
#define NR_TESTS 100

using namespace std;

/// bottom-up - se foloseste cand nu modific dimensiunea vectorului
///
/// worst case maxHeap - vector sortat crescator
/// p(i) = (i - 1) / 2
/// l(i) = 2 * i + 1
/// r(i) = 2 * i + 2
/// ultima frunza = n - 1 => p(n / 2 - 1) = n - 1 => for ul este pana la n / 2 - 1, nu n / 2
///
/// top-down - se foloseste cand elimin/adaug noduri/elemente din vector
///
/// prima pagina html
///
/// grafice - unul iterativ vs recursiv (asignari)
/// grafice - unul iterativ vs recursiv (comparati)
/// grafice - unul iterativ vs recursiv (total)
/// grafice - unul iterativ vs recursiv (timp)
///
/// a doua pagina html
///
/// caz avg - asg, cmp, total - cu bottom up si top down in acelasi grafic
///
/// a treia pagina html
///
/// caz best - asg, cmp, total - cu bottom up si top down in acelasi grafic
///
/// pt benchmark - un numar mare de teste - minim 100

namespace lab02
{
    /////  mai trebuie scris si pentru timp
    void iterativeSort(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        // TODO: iterative implementation of a sorting algorithm from Laboratory 1 here
        // Insertion Sort

        for (int i = 1; i < n; i++) {
            if (opAsg)
                opAsg->count();

            int val = values[i];
            int j = i - 1;

            while (j >= 0) {

                if (opCmp)
                    opCmp->count();

                if (values[j] > val) {
                    if (opAsg)
                        opAsg->count();

                    values[j + 1] = values[j];
                    j--;
                }
                else break;
            }

            if (opAsg)
                opAsg->count();

            values[j + 1] = val;
        }

    }

    void recursiveShift(int* values, int val, int j, Operation* opAsg, Operation* opCmp) {
        if (j >= 0) {
            if (opCmp) {
                opCmp->count();
            }

            if (values[j] > val) {
                if (opAsg) {
                    opAsg->count();
                }

                values[j + 1] = values[j];
                recursiveShift(values, val, j - 1, opAsg, opCmp);
                return;
            }
        }

        if (opAsg) {
            opAsg->count();
        }

        values[j + 1] = val;
    }

    void recursiveSort(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        // TODO: recursive implementation of the same sorting algorithm from Laboratory 1 here

        if (n <= 1)
            return;

        recursiveSort(values, n - 1, opAsg, opCmp);

        if (opAsg) {
            opAsg->count();
        }

        int val = values[n - 1];
        int j = n - 2;

        recursiveShift(values, val, j, opAsg, opCmp);
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

    void maxHeapify(int *values, int n, int i, Operation *opAsg, Operation *opCmp) {
        int lMax = i;
        int l = left(i);
        int r = right(i);

        if (l < n && l >= 0) {

            if (opCmp) {
                opCmp->count();
            }

            if (values[l] > values[i])
                lMax = l;
            else lMax = i;
        }

        if (r < n && r >= 0) {

            if (opCmp) {
                opCmp->count();
            }

            if (values[r] > values[lMax])
                lMax = r;
        }

        if (lMax != i) {
            if (opAsg) {
                opAsg->count(3);
            }

            swap(values[i], values[lMax]);
            maxHeapify(values, n, lMax, opAsg, opCmp);
        }
    }

    void buildHeap_BottomUp(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        // TODO: bottom-�up build heap implementation here

        for (int i = n / 2 - 1; i >= 0; --i) {
            maxHeapify(values, n, i, opAsg, opCmp);
        }
    }

    void shiftUp(int *values, int i, Operation* opAsg, Operation* opCmp) {
        int p = parent(i);
        while (i > 0) {

            if (opCmp) {
                opCmp->count();
            }

            if (values[p] < values[i]) {

                if (opAsg) {
                    opAsg->count(3);
                }

                swap(values[p], values[i]);

                i = p;
                p = parent(i);
            }
            else break;
        }
    }

    void buildHeap_TopDown(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        // TODO: top-down build heap implementation here

        for (int i = 1; i < n; i++) {
            shiftUp(values, i, opAsg, opCmp);
        }
    }

    void heapSort(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        // TODO: Heap sort implementation here

        buildHeap_BottomUp(values, n, opAsg, opCmp);

        for (int i = n - 1; i >= 1; --i) {

            if (opAsg)
                opAsg->count(3);

            swap(values[0], values[i]);
            maxHeapify(values, i, 0, opAsg, opCmp);
        }
    }

    void demonstrate(int size)
    {
        // TODO: Write a small demo for each method here

        int *v, *v1;
        v = (int *)malloc(size * sizeof(int));
        v1 = (int *)malloc(size * sizeof(int));
        FillRandomArray(v, size, 10, 500);
        memcpy(v1, v, sizeof(int) * size);

        cout << "Vectorul pentru max-heap\n";
        for (int i = 0; i < size; i++) {
            cout << v[i] << " ";
        }
        cout << '\n';

        buildHeap_BottomUp(v, size);

        cout << "\nVectorul dupa buildHeap_BottomUp\n";
        for (int i = 0; i < size; i++) {
            cout << v[i] << " ";
        }
        cout << '\n';

        buildHeap_TopDown(v1, size);

        cout << "\nVectorul dupa buildHeap_TopDown\n";
        for (int i = 0; i < size; i++) {
            cout << v1[i] << " ";
        }
        cout << '\n';

        heapSort(v, size);

        cout << "\nVectorul dupa heapSort\n";
        for (int i = 0; i < size; i++) {
            cout << v[i] << " ";
        }
        cout << '\n';



        FillRandomArray(v, size, 10, 500);
        memcpy(v1, v, sizeof(int) * size);

        cout << "\nVectorul care trebuie sortat:\n";
        for (int i = 0; i < size; i++) {
            cout << v[i] << " ";
        }
        cout << '\n';

        iterativeSort(v, size);
        cout << "\nDupa sortare iterativa:\n";
        for (int i = 0; i < size; i++) {
            cout << v[i] << " ";
        }
        cout << '\n';

        recursiveSort(v1, size);
        cout << "\nDupa sortare recursiva:\n";
        for (int i = 0; i < size; i++) {
            cout << v1[i] << " ";
        }
        cout << '\n';
    }

    TEST_CASE("Heap methods")
    {
        // TODO: Write your unit tests here
        // Use REQUIRE( [condition] ); to perform checks
        // You can use the FillRandomArray to generate input data
        // and the IsSorted method to test the result of your sorting function.
        // For example:
        //   int data[100];
        //   FillRandomArray(data, 100);
        //   bubbleSort(data, 100);
        //   REQUIRE( IsSorted(data, 100) );

        int values[100];

        FillRandomArray(values, 100);
        heapSort(values, 100);
        REQUIRE(IsSorted(values, 100));

        FillRandomArray(values, 100);
        iterativeSort(values, 100);
        REQUIRE(IsSorted(values, 100));

        FillRandomArray(values, 100);
        recursiveSort(values, 100);
        REQUIRE(IsSorted(values, 100));

        FillRandomArray(values, 100);
        buildHeap_BottomUp(values, 100);
        bool isMaxHeap = true;
        for (int i = 0; i < 100; i++) {
            int l = left(i);
            int r = right(i);
            if (l < 100 && values[l] > values[i])
                isMaxHeap = false;
            if (r < 100 && values[r] > values[i])
                isMaxHeap = false;
        }
        REQUIRE(isMaxHeap);

    }

    void performance(Profiler& profiler, AnalysisCase whichCase)
    {
        // TODO: Write your performance analysis here, as explained in the readme
        // You should use profiler.createOperation to pass the counter to each sorting function

        int v_Heap[MAX_SIZE], v_Heap_Copy[MAX_SIZE];
        int v_Sort[MAX_SIZE], v_Sort_Copy[MAX_SIZE];

        for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
            cout << "size actual: " << size << '\n';

            ///av - 5 teste, wo, bes - 1 test
            int nrTest = 1;
            if (whichCase == AVERAGE) {
                nrTest = 5;
            }

            while (nrTest--) {
                switch (whichCase) {
                    case AVERAGE:
                        FillRandomArray(v_Heap, size, 10, 50000, false, UNSORTED);
                        FillRandomArray(v_Sort, size, 10, 50000, false, UNSORTED);
                        break;

                    // Heap Sort nu are caz de Best si Worst

                    // pentru Build Heap - vector sortat descrescator
                    // pentru sortare lab 1 - vector sortat crescator
                    case BEST:
                        FillRandomArray(v_Heap, size, 10, 50000, false, DESCENDING);
                        FillRandomArray(v_Sort, size, 10, 50000, false, ASCENDING);
                        break;

                    // pentru Build Heap - vector sortat crescator
                    // pentru sortare lab 1 - vector sortat descrescator
                    case WORST:
                        FillRandomArray(v_Heap, size, 10, 50000, false, ASCENDING);
                        FillRandomArray(v_Sort, size, 10, 50000, false, DESCENDING);
                        break;
                }

                memcpy(v_Heap_Copy, v_Heap, size * sizeof(int));

                auto opBottomUpAsg = profiler.createOperation("Asg_BottomUp", size);
                auto opBottomUpCmp = profiler.createOperation("Cmp_BottomUp", size);
                buildHeap_BottomUp(v_Heap, size, &opBottomUpAsg, &opBottomUpCmp);

                memcpy(v_Heap, v_Heap_Copy, size * sizeof(int));
                auto opTopDownAsg = profiler.createOperation("Asg_TopDown", size);
                auto opTopDownCmp = profiler.createOperation("Cmp_TopDown", size);
                buildHeap_TopDown(v_Heap, size, &opTopDownAsg, &opTopDownCmp);

                memcpy(v_Heap, v_Heap_Copy, size * sizeof(int));
                auto opHeapSortAsg = profiler.createOperation("Asg_HeapSort", size);
                auto opHeapSortCmp = profiler.createOperation("Cmp_HeapSort", size);
                heapSort(v_Heap, size, &opHeapSortAsg, &opHeapSortCmp);


                memcpy(v_Sort_Copy, v_Sort, size * sizeof(int));
                auto opIterativeSortAsg = profiler.createOperation("Asg_IterativeSort", size);
                auto opIterativeSortCmp = profiler.createOperation("Cmp_IterativeSort", size);
                iterativeSort(v_Sort, size, &opIterativeSortAsg, &opIterativeSortCmp);

                memcpy(v_Sort, v_Sort_Copy, size * sizeof(int));
                auto opRecursiveSortAsg = profiler.createOperation("Asg_RecursiveSort", size);
                auto opRecursiveSortCmp = profiler.createOperation("Cmp_RecursiveSort", size);
                recursiveSort(v_Sort, size, &opRecursiveSortAsg, &opRecursiveSortCmp);

            }


        }

        if (whichCase == AVERAGE) {
            profiler.divideValues("Asg_BottomUp", TEST);
            profiler.divideValues("Cmp_BottomUp", TEST);

            profiler.divideValues("Asg_TopDown", TEST);
            profiler.divideValues("Cmp_TopDown", TEST);

            profiler.divideValues("Asg_HeapSort", TEST);
            profiler.divideValues("Cmp_HeapSort", TEST);


            profiler.divideValues("Asg_IterativeSort", TEST);
            profiler.divideValues("Cmp_IterativeSort", TEST);

            profiler.divideValues("Asg_RecursiveSort", TEST);
            profiler.divideValues("Cmp_RecursiveSort", TEST);

        }

        profiler.addSeries("BottomUp_Series", "Asg_BottomUp", "Cmp_BottomUp");
        profiler.addSeries("TopDown_Series", "Asg_TopDown", "Cmp_TopDown");
        profiler.addSeries("HeapSort_Series", "Asg_HeapSort", "Cmp_HeapSort");

        profiler.addSeries("IterativeSort_Series", "Asg_IterativeSort", "Cmp_IterativeSort");
        profiler.addSeries("RecursiveSort_Series", "Asg_RecursiveSort", "Cmp_RecursiveSort");


        /// html_1
        profiler.createGroup("IterativeVSRecursive_Asg", "Asg_IterativeSort", "Asg_RecursiveSort");
        profiler.createGroup("IterativeVSRecursive_Cmp", "Cmp_IterativeSort", "Cmp_RecursiveSort");
        profiler.createGroup("IterativeVSRecursive_Series", "IterativeSort_Series", "RecursiveSort_Series");
        benchmark(profiler, whichCase);


        /// html_2 + html_3
        profiler.createGroup("Build_Asg", "Asg_BottomUp", "Asg_TopDown");
        profiler.createGroup("Build_Cmp", "Cmp_BottomUp", "Cmp_TopDown");
        profiler.createGroup("Build_Series", "BottomUp_Series", "TopDown_Series");
    }

    void benchmark(Profiler& profiler, AnalysisCase whichCase)
    {
        // TODO: Write your benchmarks here, as explained in the readme
        // You should use profiler.startTimer/stopTimer for measuring and plotting measured time
        int v[MAX_SIZE];

        for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
            FillRandomArray(v, size);

            /// iterativeSort
            int **matrix = (int **)malloc(NR_TESTS * sizeof(int *));
            for (int test = 0; test < NR_TESTS; ++test) {
                matrix[test] = (int *)malloc(MAX_SIZE * sizeof(int));
                memcpy(matrix[test], v, size * sizeof(int));
            }

            profiler.startTimer("IterativeSort", size);
            for (int test = 0; test < NR_TESTS; ++test) {
                iterativeSort(matrix[test], size);
            }
            profiler.stopTimer("IterativeSort", size);

            for (int test = 0; test < NR_TESTS; ++test) {
                free(matrix[test]);
            }
            free(matrix);

            // recursive Sort
            matrix = (int **)malloc(NR_TESTS * sizeof(int *));
            for (int test = 0; test < NR_TESTS; ++test) {
                matrix[test] = (int *)malloc(MAX_SIZE * sizeof(int));
                memcpy(matrix[test], v, size * sizeof(int));
            }

            profiler.startTimer("RecursiveSort", size);
            for (int test = 0; test < NR_TESTS; ++test) {
                recursiveSort(matrix[test], size);
            }
            profiler.stopTimer("RecursiveSort", size);

            for (int test = 0; test < NR_TESTS; ++test) {
                free(matrix[test]);
            }
            free(matrix);
        }

        profiler.createGroup("IterativeVSRecursive_Timp", "IterativeSort", "RecursiveSort");
        //profiler.showReport();
    }

} // namespace lab02

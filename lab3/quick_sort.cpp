/*
1. QuickSort (cu partitionare Lomuto)

Ideea de functionare:

    QuickSort foloseste metoda Divide et Impera. Alege un element numit pivot, care in varianta Lomuto este ultimul element din sub-sir,
apoi rearanjeaza elementele astfel incat cele mai mici sau egale cu pivotul sa fie in stanga lui, iar cele mai mari sa fie in dreapta.
Dupa partitionare, pivotul ajunge pe pozitia pe care o va avea in vectorul sortat. Procesul se repeta recursiv pentru cele doua parti rezultate,
pana cand intregul vector este sortat.

Complexitate:

    Caz favorabil (Best Case): O(n log n), atunci cand pivotul imparte vectorul in doua parti aproximativ egale la fiecare pas.
    Caz mediu (Average Case): O(n log n), atunci cand partitionarile sunt in general echilibrate.
    Caz defavorabil (Worst Case): O(n^2), atunci cand partitionarile sunt foarte dezechilibrate. In varianta Lomuto, fara randomizarea pivotului,
acest caz poate aparea daca vectorul este deja sortat crescator sau descrescator. La fiecare pas, pivotul ajunge la un capat,
iar restul elementelor formeaza o singura subproblema, ceea ce duce la o adancime a recursivitatii de O(n).



    QuickSort (cu partitionare Hoare)

Ideea de functionare:

    Spre deosebire de metoda Lomuto, care parcurge vectorul intr-o singura directie folosind un singur indice,
metoda Hoare foloseste doi indici care pornesc de la capetele vectorului si se deplaseaza unul spre celalalt.
    Indicele din stanga avanseaza pana cand gaseste un element mai mare sau egal cu pivotul.
    Indicele din dreapta se deplaseaza spre stanga pana cand gaseste un element mai mic sau egal cu pivotul.
    Daca cei doi indici nu s-au intersectat, elementele gasite sunt interschimbate. Procesul se repeta pana cand indicii se intalnesc sau se depasesc.
    O particularitate a metodei Hoare este ca pivotul nu ajunge neaparat pe pozitia sa finala in urma partitionarii. In schimb, elementele sunt impartite in doua segmente,
astfel incat cele din stanga sa fie mai mici sau egale cu cele din dreapta, in raport cu pozitia de separare. Din acest motiv, apelurile recursive se fac pentru segmentele [l ... q] si [q + 1 ... r].

Complexitate:

    Caz favorabil (Best Case): O(n log n). Apare atunci cand vectorul este impartit in doua parti aproximativ egale la fiecare pas al recursivitatii.
    Caz mediu (Average Case): O(n log n). In general, algoritmul are o performanta buna atunci cand partitionarile nu sunt foarte dezechilibrate.
    Caz defavorabil (Worst Case): O(n^2). Apare atunci cand pivotul imparte vectorul in mod repetat in doua parti foarte inegale, de exemplu,
una cu (n-1) elemente si cealalta fara elemente.
    Un astfel de caz poate aparea daca pivotul este ales mereu ca primul element, iar vectorul este deja sortat. Alegerea pivotului din mijlocul vectorului, asa cum am procedat in cod,
reduce riscul acestui comportament pentru vectorii deja sortati si poate duce la partitionari mai echilibrate.



    QuickSort (Hoare) vs. QuickSort (Lomuto)

    Desi ambele variante au aceleasi complexitati in cazul favorabil, mediu si defavorabil, exista cateva diferente importante in practica:

    - Eficienta la executie: Partitionarea Hoare face, in general, mai putine interschimbari (swap-uri) decat partitionarea Lomuto, uneori chiar de aproximativ 3 ori mai putine.
Astfel, se reduc operatiile asupra memoriei, ceea ce poate duce la timpi de executie mai buni.
    - Elemente duplicate: Atunci cand vectorul contine multe elemente egale sau chiar numai valori identice, Lomuto poate ajunge la o complexitate de O(n^2).
Hoare gestioneaza mai bine aceasta situatie, deoarece indicii se opresc si in dreptul elementelor egale cu pivotul, iar interschimbarea acestora ajuta la mentinerea unor partitii echilibrate.
Astfel, in cazul unui vector cu toate elementele egale, Hoare poate pastra o complexitate de O(n log n), in timp ce Lomuto poate avea o performanta mult mai slaba.

2. HeapSort (am adaugat si asta pentru ca am uitat sa scriu ceva legat de complexitati la laboratorul anterior)

Ideea de functionare:

    HeapSort foloseste o structura de date numita Max-Heap, organizata ca un arbore binar aproape complet. In aceasta structura,
fiecare parinte este mai mare sau egal cu copiii sai, astfel incat cel mai mare element se afla mereu in varful heap-ului.

Algoritmul functioneaza in doua etape:

    Construieste un Max-Heap din vectorul nesortat, folosind functia buildHeap_BottomUp.
    Muta cel mai mare element la finalul vectorului, il elimina din heap-ul activ si foloseste maxHeapify pentru a reface proprietatea de Max-Heap.
    Procesul se repeta pana cand toate elementele sunt sortate.

Complexitate:

    Toate cazurile (Best, Average, Worst): O(n log n). Construirea heap-ului dureaza O(n), iar fiecare dintre cele n-1 extrageri
necesita cel mult O(log n) pentru refacerea structurii.



3. Insertion Sort

Ideea de functionare:

    Insertion Sort construieste treptat partea sortata a vectorului. La fiecare pas, ia un element din partea nesortata si il introduce pe
pozitia corecta in partea sortata din stanga. Pentru a face loc, elementele mai mari sunt mutate cu o pozitie la dreapta.

Complexitate:

    Caz favorabil: O(n), atunci cand vectorul este deja sortat.
    Caz mediu si defavorabil: O(n^2), atunci cand multe elemente trebuie mutate pentru a ajunge pe pozitiile corecte.



4. QuickSort Hibrid (Hybrid QuickSort)

Ideea de functionare:

    QuickSort Hibrid combina QuickSort cu Insertion Sort pentru a reduce timpul de executie, mai ales atunci cand vectorul este impartit in sub-secvente mici.
    QuickSort este eficient pentru vectori mari, dar, pe masura ce sub-secventele devin tot mai mici, apelurile recursive ajung sa consume timp fara sa mai aduca un avantaj semnificativ.
Insertion Sort este mai potrivit pentru aceste secvente mici, deoarece are o implementare simpla si un cost redus de executie.
    Algoritmul foloseste un prag (threshold) care stabileste cand se face trecerea de la QuickSort la Insertion Sort. Daca dimensiunea unei partitii este mai mare decat pragul,
se continua cu QuickSort. Cand dimensiunea ajunge sub prag, sortarea se face cu Insertion Sort.

Complexitate:

    Caz mediu: O(n log n).
    Caz defavorabil: O(n^2), daca QuickSort ajunge la partitionari foarte dezechilibrate.

    Complexitatea asimptotica ramane aceeasi ca la QuickSort, dar timpul efectiv de executie poate fi mai mic datorita reducerii numarului de apeluri recursive
si folosirii mai eficiente a Insertion Sort pentru secvente mici.



5. QuickSelect (Randomized Select)

Ideea de functionare:

    QuickSelect este folosit pentru a gasi al k-lea cel mai mic element dintr-un vector nesortat, fara sa fie nevoie sa sorteze intregul vector.
    Algoritmul functioneaza asemanator cu QuickSort: alege un pivot si partitioneaza vectorul in jurul lui. Diferenta este ca, dupa partitionare,
verifica in ce parte se afla elementul cautat si continua doar pe acea parte. Alegerea randomizata a pivotului reduce riscul unor partitionari foarte dezechilibrate.

Complexitate:

    Caz mediu: O(n), deoarece la fiecare pas este procesata doar o parte din vector, iar dimensiunea problemei scade, in medie, suficient de mult.
    Caz favorabil: O(n), atunci cand partitionarile reduc eficient dimensiunea problemei.
    Caz defavorabil: O(n^2), atunci cand pivotul ales este in mod repetat cel mai mic sau cel mai mare element ramas.



Analiza comparativa - QuickSort vs. HeapSort

    Ambii algoritmi au o complexitate medie de O(n log n), dar QuickSort este adesea mai rapid in practica. Unul dintre motive este modul in care acceseaza memoria.
    QuickSort parcurge vectorul in mod relativ secvential, ceea ce ajuta procesorul sa foloseasca eficient memoria cache.
    HeapSort acceseaza elementele in functie de structura heap-ului, sarind intre pozitii precum i, 2*i si 2*i+1. Acest lucru poate duce la mai multe accesari lente ale memoriei.
    Pe de alta parte, HeapSort are avantajul ca garanteaza o complexitate de O(n log n) chiar si in cazul defavorabil.
QuickSort poate ajunge la O(n^2) daca pivotii duc in mod repetat la partitionari dezechilibrate.



Analiza comparativa - QuickSort vs. Hybrid QuickSort

    Diferenta principala dintre cele doua variante apare atunci cand vectorul este impartit in secvente mici.
    In QuickSort clasic, algoritmul continua sa faca apeluri recursive chiar si atunci cand au ramas doar cateva elemente de sortat.
Pentru secvente foarte mici, costul acestor apeluri poate deveni mai mare decat munca efectiva de sortare.
    Hybrid QuickSort evita acest lucru prin folosirea unui prag (threshold). Cand dimensiunea unei partitii scade sub prag, recursivitatea se opreste pentru acea secventa,
iar sortarea este continuata cu Insertion Sort. Astfel, se reduc apelurile recursive inutile si se profita de eficienta Insertion Sort pentru vectorii mici.


Cum se determina pragul optim?

    Pragul influenteaza direct timpul de executie al variantei hibride, asa ca este important sa fie ales corespunzator.

    Praguri mici (de exemplu, 5-10): QuickSort continua sa imparta vectorul pana la secvente foarte mici. Astfel, se fac mai multe apeluri recursive,
iar timpul consumat de acestea poate reduce performanta algoritmului.
    Praguri mari (de exemplu, 45-50): QuickSort se opreste mai devreme, iar Insertion Sort trebuie sa sorteze secvente mai mari.
Deoarece Insertion Sort are o complexitate de Θ(n^2) in cazul mediu si defavorabil, timpul de executie poate creste.
    Pragul optim: Se afla intre aceste doua situatii si reprezinta valoarea pentru care timpul total de executie este cel mai mic.
La acest prag se obtine un echilibru bun intre reducerea apelurilor recursive din QuickSort si eficienta Insertion Sort pentru secvente mici.

    Pragul optim poate fi determinat, ruland functia benchmark pentru mai multe valori ale pragului si comparand timpii obtinuti.
Daca reprezentam grafic timpul de executie in functie de prag, ne putem astepta la o curba aproximativ in forma de U: la inceput, timpul scade pe masura ce pragul creste,
apoi ajunge la un minim si incepe sa creasca din nou.

    Valoarea pragului din punctul minim al graficului este cea mai buna dintre valorile testate, deoarece ofera cel mai mic timp de executie pentru setul de date
si conditiile folosite in test.
*/

#include "quick_sort.h"

#include "catch2.hpp"

#include <iostream>

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define TEST 5
#define NR_TEST 100

using namespace std;

// Quick Sort
// Hoare - mai eficienta - mai greu de implementat - in laborator
// Lomuto - mai simplu de implementat - in Cormen
// se poate alege oricare dintre ele
// 3 pagini html - 3 grafice - cmp, asg, total fiecare (o pagina avg, una best, una worst)
// best case - idee 1: pivot element la mijloc (Hoare)
//                     swap(A[m], A[r]) (Lomuto)
//           - idee 2: incep de la finalul partitiei si imi dau seama cum formez vectorul
//             ideea 2 este bonus

// o pagina html - 3 grafice cu 2 curbe (QuickSort vs Heapsort) - cmp, asg, total

// Quick sort - hibridizat pana la un threshold folosind pt 0 ... threshold - se foloseste Insertion Sort
// o pagina html - 4 grafice - pe Ox trebuie threshold (merge de la 5 la 50) - asg, cmp, total, timp

// o pagina html - analiza Quick Sort - hibrid Quick sort
//               - 4 grafice asg, cmp, total, timp

// Quick Select - randomized Select capitol 9 Cormen - FARA GRAFICE
// Quick Select - randomized Select este bonus

namespace lab03
{
    int partition_Hoare(int* values, int l, int r, Operation* opAsg, Operation* opCmp) {
        if (opAsg)
            opAsg->count();

        int pivot = values[l];
        int i = l - 1;
        int j = r + 1;

        // o valoare din stanga mai mare ca pivotul
        do {
            i++;
            if (opCmp)
                opCmp->count();
        }while (values[i] < pivot);

        // o valoare din dreapta mai mica decat pivotul
        do {
            j--;
            if (opCmp)
                opCmp->count();
        }while (values[j] > pivot);

        if (i >= j)
            return j;


        if (opAsg)
            opAsg->count(3);
        swap(values[i], values[j]);
    }

    void quick_Hoare(int* values, int l, int r, Operation* opAsg, Operation* opCmp) {
        if (l < r) {
            int q = partition_Hoare(values, l, r, opAsg, opCmp);

            // La Hoare, elementul de pe pozitia q NU este garantat pe pozitia finala
            // De aceea apelurile recursive se fac pe [l ... q] si [q + 1 ... r].
            quick_Hoare(values, l, q, opAsg, opCmp);
            quick_Hoare(values, q + 1, r, opAsg, opCmp);
        }
    }

    // am folosit partitionarea Lomuto pentru a rezolva cerintele obligatorii
    int partition_Lomuto(int* values, int l, int r, Operation* opAsg = nullptr, Operation* opCmp = nullptr) {
        if (opAsg)
            opAsg->count();

        int pivot = values[r];
        int i = l - 1;

        for (int j = l; j <= r - 1; j++) {

            if (opCmp)
                opCmp->count();

            if (values[j] <= pivot) {
                i++;

                if (opAsg)
                    opAsg->count(3);
                swap(values[i], values[j]);
            }
        }

        if (opAsg)
            opAsg->count(3);

        swap(values[i + 1], values[r]);
        return i + 1;
    }

    void quick_Lomuto(int* values, int l, int r, Operation* opAsg, Operation* opCmp) {
        if (l < r) {
            int pivot = partition_Lomuto(values, l, r, opAsg, opCmp);

            quick_Lomuto(values, l, pivot - 1, opAsg, opCmp);
            quick_Lomuto(values, pivot + 1, r, opAsg, opCmp);
        }
    }

    void quickSort(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        // TODO: Quick sort implementation here

        // prin Lomuto
        quick_Lomuto(values, 0, n - 1, opAsg, opCmp);
    }

// inceput Heap Sort - lab 2
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
        for (int i = n / 2 - 1; i >= 0; --i) {
            maxHeapify(values, n, i, opAsg, opCmp);
        }
    }

    void heapSort(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        buildHeap_BottomUp(values, n, opAsg, opCmp);

        for (int i = n - 1; i >= 1; --i) {

            if (opAsg)
                opAsg->count(3);

            swap(values[0], values[i]);
            maxHeapify(values, i, 0, opAsg, opCmp);
        }
    }
// final Heap sort - lab 2

// inceput Insertion sort - lab 1
    void insertionSort(int* values, int l, int r, Operation* opAsg, Operation* opCmp)
    {
        for (int i = l + 1; i <= r; i++) {
            if (opAsg)
                opAsg->count();

            int val = values[i];
            int j = i - 1;

            while (j >= l) {

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
// final Insertion sort - lab 1

    void quick_hybrid_Lomuto(int *values, int l, int r, int threshold, Operation *opAsg, Operation *opCmp) {
        int size = r - l + 1;

        if (size <= threshold)
            insertionSort(values, l, r, opAsg, opCmp);
        else if (l < r) {
            int pivot = partition_Lomuto(values, l, r, opAsg, opCmp);

            quick_hybrid_Lomuto(values, l, pivot - 1, threshold, opAsg, opCmp);
            quick_hybrid_Lomuto(values, pivot + 1, r, threshold, opAsg, opCmp);
        }
    }

    void hybridizedQuickSort(int* values, int n, int threshold, Operation* opAsg, Operation* opCmp)
    {
        // TODO: Hybridized quick sort implementation here

        quick_hybrid_Lomuto(values, 0, n - 1, threshold, opAsg, opCmp);
    }


    // cu srand function
    int partition_rand(int* values, int l, int r)
    {
        // genereaza un nr random intre l si r
        srand(time(NULL));
        int random = l + rand() % (r - l);

        // swap values[random] cu values[r]
        // la Lomuto - pivotul este ultimul element
        swap(values[random],  values[r]);

        return partition_Lomuto(values, l, r);
    }

    int quickSelect_call(int *values, int l, int r, int k, Operation* opAsg, Operation* opCmp) {
        if (l == r)
            return values[l];

        // genereaza un nr random intre l si r
        srand(time(NULL));
        int random = l + rand() % (r - l);

        if (opAsg) {
            opAsg->count(3);
        }

        // swap values[random] cu values[r]
        // la Lomuto - pivotul este ultimul element
        swap(values[random],  values[r]);


        int q = partition_Lomuto(values, l, r, opAsg, opCmp);
        // cate el sunt in partea stanga, inclusiv pivotul
        int poz_pivot = q - l + 1;

        if (k == poz_pivot)
            return values[q];

        // el se afla in stanga pivotului
        if (k < poz_pivot)
            return quickSelect_call(values, l, q - 1, k, opAsg, opCmp);

        // el se afla in dreapta pivotului
        return quickSelect_call(values, q + 1, r, k - poz_pivot, opAsg, opCmp);
    }

    int quickSelect(int* values, int n, int k, Operation* opAsg, Operation* opCmp)
    {
        // TODO: Quick select implementation here

        return quickSelect_call(values, 0, n - 1, k, opAsg, opCmp);
    }

    void demonstrate(int size)
    {
        // TODO: Write a small demo for each sorting method here

        int *v;
        v = (int *)malloc(size * sizeof(int));
        FillRandomArray(v, size, 10, 500);

        cout << "vectorul inainte de QuickSort: \n";
        for (int i = 0; i < size; i++)
            cout << v[i] << " ";
        cout << '\n';

        quickSort(v, size);

        cout << "\nvectorul dupa QuickSort: \n";
        for (int i = 0; i < size; i++)
            cout << v[i] << " ";
        cout << '\n';



        FillRandomArray(v, size, 10, 500);

        cout << "\nvectorul inainte de QuickSort_Hibritizat: \n";
        for (int i = 0; i < size; i++)
            cout << v[i] << " ";
        cout << '\n';

        hybridizedQuickSort(v, size);

        cout << "\nvectorul dupa QuickSort_Hibritizat: \n";
        for (int i = 0; i < size; i++)
            cout << v[i] << " ";
        cout << '\n';



        FillRandomArray(v, size, 10, 500);
        int k = 3;
        cout << "\nvectorul: \n";
        for (int i = 0; i < size; i++)
            cout << v[i] << ' ';
        cout << '\n';

        cout << "\nal " << k << "-lea cel mai mic element care ar fi in sir dupa sortarea lui\n";

        int valoare_k = quickSelect(v, size, k);
        cout << "\nvaloarea de pe pozitia " << k << " dupa apelarea functiei quickSelect: " << valoare_k << "\n";

        quickSort(v, size);
        cout << "\nvectorul sortat (pentru verificarea corectitudinii valori gasite)\n";
        for (int i = 0; i < size; i++)
            cout << v[i] << " ";
        cout << '\n';

        free(v);
    }

    TEST_CASE("Quick sort")
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
    }


    // exemplu n = 7
    // values: 1 3 2 5 7 6 4
    void genereazaBestCase_Lomuto_idee2(int *values, int l, int r, int min_range, int max_range) {
        if (l > r)
            return ;

        if (l == r) {
            values[l] = min_range;
            return ;
        }

        // mijlocul valorilor disponibile
        int mij = min_range + (max_range - min_range) / 2;

        // pivot = values[r] => mij se pune pe ultima pozitie
        values[r] = mij;

        int l_size = mij - min_range;
        int mij_poz = l + l_size;

        // jumatate stanga - valori strict mai mici
        genereazaBestCase_Lomuto_idee2(values, l, mij_poz - 1, min_range, mij - 1);

        // jumatatea dreapta - valori strict mai mari - oprire inaintea pivotului
        genereazaBestCase_Lomuto_idee2(values, mij_poz, r - 1, mij + 1, max_range);
    }

    void performance(Profiler& profiler, AnalysisCase whichCase)
    {
        // TODO: Write your performance analysis here, as explained in the readme
        // You should use profiler.createOperation to pass the counter to each sorting function

        int v_Quick[MAX_SIZE], v_Quick_Copy[MAX_SIZE];
// 1
        profiler.reset("Analiza_QuickSort");

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
                        FillRandomArray(v_Quick, size, 10, 50000, false, UNSORTED);
                        break;

                    case BEST:
                        genereazaBestCase_Lomuto_idee2(v_Quick, 0, size - 1, 1, size);
                        break;

                    case WORST:
                        FillRandomArray(v_Quick, size, 10, 50000, false, ASCENDING);
                        break;
                }

                memcpy(v_Quick_Copy, v_Quick, size * sizeof(int));

                auto opQuickSortAsg = profiler.createOperation("Asg_QuickSort", size);
                auto opQuickSortCmp = profiler.createOperation("Cmp_QuickSort", size);
                quickSort(v_Quick_Copy, size, &opQuickSortAsg, &opQuickSortCmp);
            }
        }

        if (whichCase == AVERAGE) {
            profiler.divideValues("Asg_QuickSort", TEST);
            profiler.divideValues("Cmp_QuickSort", TEST);
        }

        profiler.addSeries("QuickSort_Series", "Asg_QuickSort", "Cmp_QuickSort");

        profiler.showReport();


// 2
        profiler.reset("Comparatie_QuickSort_VS_HeapSort");

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
                        FillRandomArray(v_Quick, size, 10, 50000, false, UNSORTED);
                        break;

                    case BEST:
                        genereazaBestCase_Lomuto_idee2(v_Quick, 0, size - 1, 1, size);
                        break;

                    case WORST:
                        FillRandomArray(v_Quick, size, 10, 50000, false, ASCENDING);
                        break;
                }

                memcpy(v_Quick_Copy, v_Quick, size * sizeof(int));

                auto opQuickSortAsg = profiler.createOperation("Asg_QuickSort", size);
                auto opQuickSortCmp = profiler.createOperation("Cmp_QuickSort", size);
                quickSort(v_Quick, size, &opQuickSortAsg, &opQuickSortCmp);

                memcpy(v_Quick, v_Quick_Copy, size * sizeof(int));
                auto opHeapSortAsg = profiler.createOperation("Asg_HeapSort", size);
                auto opHeapSortCmp = profiler.createOperation("Cmp_HeapSort", size);
                heapSort(v_Quick, size, &opHeapSortAsg, &opHeapSortCmp);
            }
        }

        if (whichCase == AVERAGE) {
            profiler.divideValues("Asg_QuickSort", TEST);
            profiler.divideValues("Cmp_QuickSort", TEST);

            profiler.divideValues("Asg_HeapSort", TEST);
            profiler.divideValues("Cmp_HeapSort", TEST);
        }

        profiler.addSeries("QuickSort_Series", "Asg_QuickSort", "Cmp_QuickSort");
        profiler.addSeries("HeapSort_Series", "Asg_HeapSort", "Cmp_HeapSort");

        profiler.createGroup("QuickVSHeap_Asg", "Asg_QuickSort", "Asg_HeapSort");
        profiler.createGroup("QuickVSHeap_Cmp", "Cmp_QuickSort", "Cmp_HeapSort");
        profiler.createGroup("QuickVSHeap_Series", "QuickSort_Series", "HeapSort_Series");

        profiler.showReport();


// 3
        profiler.reset("Comparatie_Quick_VS_QuickHibridizat");

        int prag_optim = 30; // pragul optim ales pentru comparatie

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
                        FillRandomArray(v_Quick, size, 10, 50000, false, UNSORTED);
                        break;

                    case BEST:
                        genereazaBestCase_Lomuto_idee2(v_Quick, 0, size - 1, 1, size);
                        break;

                    case WORST:
                        FillRandomArray(v_Quick, size, 10, 50000, false, ASCENDING);
                        break;
                }

                memcpy(v_Quick_Copy, v_Quick, size * sizeof(int));

                auto opQuickSortAsg = profiler.createOperation("Asg_QuickSort", size);
                auto opQuickSortCmp = profiler.createOperation("Cmp_QuickSort", size);
                quickSort(v_Quick, size, &opQuickSortAsg, &opQuickSortCmp);

                memcpy(v_Quick, v_Quick_Copy, size * sizeof(int));
                auto opQuickHybridSortAsg = profiler.createOperation("Asg_QuickHibridSort", size);
                auto opQuickHybridSortCmp = profiler.createOperation("Cmp_QuickHibridSort", size);
                hybridizedQuickSort(v_Quick, size, prag_optim, &opQuickHybridSortAsg, &opQuickHybridSortCmp);
            }
        }

        if (whichCase == AVERAGE) {
            profiler.divideValues("Asg_QuickSort", TEST);
            profiler.divideValues("Cmp_QuickSort", TEST);

            profiler.divideValues("Asg_QuickHibridSort", TEST);
            profiler.divideValues("Cmp_QuickHibridSort", TEST);
        }

        // am facut aici masurarea timpului pentru Quick Sort VS Quick Sort Hibridizat
        // in benchmark am facut masurarea doar pentru Quick Sort Threshold
        // (in special pentru ca trebuie sa schimb Ox cu threshold)
        if (whichCase == AVERAGE) {
            for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
                FillRandomArray(v_Quick, size, 10, 50000, false, UNSORTED);

                int **matrixQS = (int **)malloc(NR_TEST * sizeof(int *));
                int **matrixHyb = (int **)malloc(NR_TEST * sizeof(int *));
                for (int test = 0; test < NR_TEST; ++test) {
                    matrixQS[test] = (int *)malloc(size * sizeof(int));
                    matrixHyb[test] = (int *)malloc(size * sizeof(int));
                    memcpy(matrixQS[test], v_Quick, size * sizeof(int));
                    memcpy(matrixHyb[test], v_Quick, size * sizeof(int));
                }

                profiler.startTimer("Timp_QuickSort", size);
                for (int test = 0; test < NR_TEST; test++)
                    quickSort(matrixQS[test], size);
                profiler.stopTimer("Timp_QuickSort", size);

                profiler.startTimer("Timp_QuickHibridSort", size);
                for (int test = 0; test < NR_TEST; test++)
                    hybridizedQuickSort(matrixHyb[test], size, prag_optim);
                profiler.stopTimer("Timp_QuickHibridSort", size);

                for (int test = 0; test < NR_TEST; test++) {
                    free(matrixQS[test]);
                    free(matrixHyb[test]);
                }
                free(matrixQS);
                free(matrixHyb);
            }
        }

        profiler.addSeries("QuickSort_Series", "Asg_QuickSort", "Cmp_QuickSort");
        profiler.addSeries("QuickHybridSort_Series", "Asg_QuickHibridSort", "Cmp_QuickHibridSort");

        profiler.createGroup("QuickVSQuickHybrid_Asg", "Asg_QuickSort", "Asg_QuickHibridSort");
        profiler.createGroup("QuickVSQuickHybrid_Cmp", "Cmp_QuickSort", "Cmp_QuickHibridSort");
        profiler.createGroup("QuickVSQuickHybrid_Series", "QuickSort_Series", "QuickHybridSort_Series");

        if (whichCase == AVERAGE) {
            profiler.createGroup("QuickVSQuickHybrid_Timp", "Timp_QuickSort", "Timp_QuickHibridSort");
        }

        profiler.showReport();


// 4
        // am mutat numararea operatiilor pentru Quick Sort Hibridizat in benchmark
        benchmark(profiler, whichCase);

    }

    void benchmark(Profiler& profiler, AnalysisCase whichCase)
    {
        // TODO: Write your benchmarks here, as explained in the readme
        // You should use profiler.startTimer/stopTimer for measuring and plotting measured time

        if (whichCase != AVERAGE)
            return;

        // l-am adaugat pt ca am observat ca profiler.reset() nu reseteaza si graficele de timp
        // daca as fi folosit profiler trimis ca parametru, ar fi ramas graficele de timp de la Quick Sort VS Quick Sort Hibridizat
        Profiler localProfiler("Analiza_Prag_QuickSort_Hibridizat");

        int v[MAX_SIZE], v_Copy[MAX_SIZE];
        FillRandomArray(v, MAX_SIZE, 10, 50000);

        for (int prag = 5; prag <= 50; prag++) {
            cout << "pragul actual: " << prag << '\n';

            memcpy(v_Copy, v, MAX_SIZE * sizeof(int));
            auto opQuickSortHybridAsg = localProfiler.createOperation("Asg_Prag", prag);
            auto opQuickSortHybridCmp = localProfiler.createOperation("Cmp_Prag", prag);
            hybridizedQuickSort(v_Copy, MAX_SIZE, prag, &opQuickSortHybridAsg, &opQuickSortHybridCmp);


            int **matrixPrag = (int **)malloc(NR_TEST * sizeof(int *));
            for (int test = 0; test < NR_TEST; ++test) {
                matrixPrag[test] = (int *)malloc(MAX_SIZE * sizeof(int));
                memcpy(matrixPrag[test], v, MAX_SIZE * sizeof(int));
            }

            localProfiler.startTimer("Timp_Prag", prag);
            for (int test = 0; test < NR_TEST; test++) {
                hybridizedQuickSort(matrixPrag[test], MAX_SIZE, prag);
            }
            localProfiler.stopTimer("Timp_Prag", prag);

            for (int test = 0; test < NR_TEST; test++) {
                free(matrixPrag[test]);
            }
            free(matrixPrag);
        }

        localProfiler.addSeries("Total_Operatii_Prag", "Asg_Prag", "Cmp_Prag");

        localProfiler.showReport();
    }

} // namespace lab03

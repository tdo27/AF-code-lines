/*
Observatii si analiza complexitatii pentru metodele de sortare directa
(pe baza graficelor generate de profiler)

1. Bubble Sort (Metoda Bulelor)

* Complexitate: In Best Case avem O(n). Aici ne ajuta flag-ul „ok”, care verifica daca vectorul este deja sortat.
Daca la prima parcurgere nu se face nicio modificare, algoritmul isi da seama ca vectorul este deja sortat si se opreste.

In Average si Worst Case avem O(n^2), deoarece sunt necesare mai multe parcurgeri ale vectorului si, in special in cazul unui vector sortat descrescator, foarte multe swap-uri.

* Memorie: Bubble Sort are complexitate spatiala O(1), deoarece nu foloseste structuri auxiliare care sa creasca odata cu dimensiunea vectorului.
Sunt folosite doar cateva variabile, cum ar fi „ok”, indicii de parcurgere si o variabila temporara pentru swap.

* Stabilitate: Bubble Sort este STABIL. Swap-ul se face doar atunci cand primul element este strict mai mare decat al doilea.
Daca valorile sunt egale, acestea nu sunt schimbate intre ele, deci isi pastreaza ordinea initiala.

2. Selection Sort (Sortarea prin Selectie)

* Complexitate: Selection Sort are O(n^2) in toate cazurile: Best, Average si Worst Case.
Chiar daca vectorul este deja sortat, algoritmul continua sa parcurga elementele pentru fiecare pozitie, pentru a gasi minimul.

* Numar de comparatii: Pentru prima pozitie trebuie verificate n-1 elemente, pentru a doua n-2, apoi n-3 si tot asa. Astfel, avem:

  (n-1) + (n-2) + ... + 2 + 1
  = n(n-1)/2
  = O(n^2)

Din acest motiv, numarul de comparatii este aproape acelasi in toate cazurile.
Algoritmul nu poate sa se opreasca mai devreme doar pentru ca vectorul este deja sortat.

* Memorie: Selection Sort are complexitate spatiala O(1). Sortarea se face direct in vector, fara sa fie nevoie de un vector auxiliar.
Sunt folosite doar cateva variabile, cum ar fi indicele minimului si o variabila temporara pentru swap.

Chiar daca numarul de comparatii este O(n^2), memoria suplimentara ramane O(1).
Cu alte cuvinte, Selection Sort este destul de costisitor ca timp, dar foloseste foarte putina memorie.

* Stabilitate: Selection Sort este INSTABIL. Atunci cand facem swap intre elementul curent si minimul gasit mai incolo in vector, putem schimba ordinea relativa a unor elemente egale.

3. Insertion Sort (Sortarea prin Insertie)

* Complexitate: In Best Case avem O(n). Daca vectorul este deja sortat, bucla „while” se opreste imediat, deoarece nu este nevoie sa mutam elementele.

In Average si Worst Case avem O(n^2). In cazul cel mai nefavorabil, cand vectorul este sortat invers, pentru fiecare element trebuie mutate spre dreapta toate elementele mai mari din stanga lui.

* Memorie: Insertion Sort are complexitate spatiala O(1). Elementele sunt mutate direct in vectorul initial, fara sa fie nevoie de un vector auxiliar.
Sunt folosite doar cateva variabile pentru elementul care trebuie inserat si pentru parcurgere.

* Stabilitate: Insertion Sort este STABIL. Elementele sunt mutate doar cat timp sunt strict mai mari decat valoarea care urmeaza sa fie inserata.
In cazul valorilor egale, ordinea lor nu se schimba.

4. Binary Insertion Sort

* Complexitate: Fata de Insertion Sort clasic, aici avem mai putine comparatii deoarece folosim cautarea binara pentru a gasi pozitia corecta.
Cautarea pozitiei se face in O(log n), insa timpul total poate ajunge la O(n^2) in Average si Worst Case, deoarece elementele trebuie in continuare mutate spre dreapta pentru a face loc celui nou.

In Best Case, pentru un vector deja sortat, avem aproximativ O(n log n) comparatii si foarte putine mutari.

* Memorie: Daca binary search este implementat iterativ, Binary Insertion Sort foloseste O(1) memorie suplimentara.
Sunt necesare doar cateva variabile pentru pozitii, elementul inserat si mutarea elementelor.

Daca „binarySearchRecursiv” este implementata recursiv, apelurile functiei folosesc O(log n) memorie pe stiva.
In acest caz, complexitatea spatiala poate ajunge la O(log n).

* Stabilitate: Binary Insertion Sort este STABIL. In implementarea folosita, atunci cand sunt gasite valori egale, pozitia returnata este dupa acestea.
Astfel, noul element este inserat dupa elementele egale si ordinea lor initiala este pastrata.

Concluzii din Profiler:

Daca ne uitam la grafice, Insertion Sort este cel mai bun in Best Case, mai ales atunci cand vectorul este deja sortat.
Selection Sort are un comportament foarte constant, deoarece face aproximativ acelasi numar de comparatii indiferent de ordinea initiala a datelor.

Selection Sort arata foarte bine diferenta dintre complexitatea in timp si cea in spatiu. Chiar daca ajunge la aproximativ n(n-1)/2 comparatii, memoria suplimentara folosita ramane O(1).
Deci este mai costisitor ca timp, dar eficient din punct de vedere al memoriei.

Bubble Sort ajunge de obicei sa fie cel mai lent in cazurile nefavorabile, in special din cauza numarului mare de atribuiri.
La fiecare swap sunt necesare 3 atribuiri, iar atunci cand avem multe swap-uri, acest lucru se vede destul de clar in grafice. Ca si celelalte metode, foloseste O(1) memorie suplimentara.

La Binary Insertion Sort se observa ca, desi numarul de comparatii este mai mic decat la Insertion Sort clasic, numarul de atribuiri pentru shiftari ramane destul de apropiat.
Motivul este ca, dupa ce gasim pozitia folosind cautarea binara, elementele trebuie in continuare mutate pentru a face loc noului element.

In concluzie, toate metodele analizate folosesc O(1) memorie suplimentara atunci cand cautarea binara este implementata iterativ.
In cazul unei cautari binare recursive, memoria poate ajunge la O(log n) din cauza apelurilor recursive.
Prin urmare, alegerea algoritmului nu depinde doar de timpul de executie si de numarul de comparatii sau atribuiri, ci si de modul in care este folosita memoria.
*/




#include "direct_sort.h"

#include "catch2.hpp"

#include <iostream>
#include <algorithm> /// clasa inclusa pentru sortare

using namespace std;

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define TEST 5

namespace lab01
{

void bubbleSort(int* values, int n, Operation* opAsg, Operation* opCmp)
{
    // TODO: Bubble sort implementation here

    bool ok = false;
    int cnt = 0;
    do {
        ok = false;
        for (int i = 0; i < n - 1 - cnt; i++) {

            if (opCmp)
                opCmp->count();

            if (values[i] > values[i + 1]) {
                ok = true;

                if (opAsg)
                    opAsg->count(3);
                /// incrementul primeste 3 din cauza swap ului
                /// (este ca si cum as fi folosi un aux)

                swap(values[i], values[i + 1]);
            }
        }
        cnt++;
    } while (ok);


}

void selectionSort(int* values, int n, Operation* opAsg, Operation* opCmp)
{
    // TODO: Selection sort implementation here

    int mnPoz = 0;
    for (int i = 0; i < n - 1; i++) {
        mnPoz = i;

        for (int j = i + 1; j < n; j++) {

            if (opCmp)
                opCmp->count();

            if (values[j] < values[mnPoz])
                mnPoz = j;
        }

        if (values[mnPoz] != values[i]) {
            if (opAsg)
                opAsg->count(3);
            /// incrementul primeste 3 din cauza swap ului
            /// (este ca si cum as fi folosi un aux)

            swap(values[i], values[mnPoz]);
        }
    }
}

void insertionSort(int* values, int n, Operation* opAsg, Operation* opCmp)
{
    // TODO: Insertion sort implementation here

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

int binarySearchRecursiv(int* values, int val, int st, int dr, Operation* opAsg, Operation* opCmp) {
    if (dr >= st) {
        if (opCmp)
            opCmp->count();

        if (val > values[st])
            return st + 1;
        return st;
    }

    int mij = (st + dr) / 2;

    if (opCmp)
        opCmp->count();

    if (values[mij] == val)
        return mij + 1; /// cazurile in care am date distincte in values

    if (opCmp)
        opCmp->count();

    if (val > values[mij])
        return binarySearchRecursiv(values, val, mij + 1, dr, opAsg, opCmp);

    return binarySearchRecursiv(values, val, mij - 1, dr, opAsg, opCmp);
}

void binaryInsertionSort(int* values, int n, Operation* opAsg, Operation* opCmp)
{
    // TODO: Binary Insertion sort implementation here

    for (int i = 1; i < n; i++) {
        if (opAsg)
            opAsg->count();

        int val = values[i];
        int j = i - 1;
        int poz = binarySearchRecursiv(values, val, 0, j, opAsg, opCmp);
        /// in poz se cauta locatia unde trebuie inserat elemntul pentru sortare

        /// mutarea elementelor
        while (j >= poz) {
            if (opAsg)
                opAsg->count();

            values[j + 1] = values[j];
            j--;
        }

        if (opAsg)
            opAsg->count();

        values[j + 1] = val;
    }
}

void demonstrate(int size)
{
    // TODO: Write a small demo for each sorting method here

    int *v, *v1;
    v = (int*)malloc(size * sizeof(int));
    v1 = (int*)malloc(size * sizeof(int));

    FillRandomArray(v, size, 0, 20);

    /// salvez in v1 v pentru a folosi acelasi v si pentru restul sortarilor
    memcpy(v1, v, size * sizeof(int));

    cout << "inainte de sortare: \n";
    for (int i = 0; i < size; i++) {
        cout << v[i] << ' ';
    }
    cout <<'\n';

    insertionSort(v, size);

    cout << "dupa insertionSort: \n";
    for (int i = 0; i < size; i++) {
        cout << v[i] << ' ';
    }
    cout <<'\n';

    /// refac v
    memcpy(v, v1, size * sizeof(int));

    cout << "\ninainte de sortare: \n";
    for (int i = 0; i < size; i++) {
        cout << v[i] << ' ';
    }
    cout <<'\n';

    bubbleSort(v, size);

    cout << "dupa bubbleSort: \n";
    for (int i = 0; i < size; i++) {
        cout << v[i] << ' ';
    }
    cout <<'\n';

    /// refac v
    memcpy(v, v1, size * sizeof(int));

    cout << "\ninainte de sortare: \n";
    for (int i = 0; i < size; i++) {
        cout << v[i] << ' ';
    }
    cout <<'\n';

    selectionSort(v, size);

    cout << "dupa selectionSort: \n";
    for (int i = 0; i < size; i++) {
        cout << v[i] << ' ';
    }
    cout <<'\n';

    /// refac v
    memcpy(v, v1, size * sizeof(int));

    cout << "\ninainte de sortare: \n";
    for (int i = 0; i < size; i++) {
        cout << v[i] << ' ';
    }
    cout <<'\n';

    binaryInsertionSort(v, size);

    cout << "dupa binaryInsertionSort: \n";
    for (int i = 0; i < size; i++) {
        cout << v[i] << ' ';
    }
    cout <<'\n';
}

TEST_CASE("Direct sorting methods")
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

void performance(Profiler& profiler, AnalysisCase whichCase) {
    // TODO: Write your performance analysis here, as explained in the readme
    // You should use profiler.createOperation to pass the counter to each sorting function

    int v[MAX_SIZE], v1[MAX_SIZE];

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
                    FillRandomArray(v, size);
                    break;

                    /// cazul BEST - cand vectorul este sortat crescator
                case BEST:
                    FillRandomArray(v, size);
                    sort(v, v  + size);
                    break;

                    /// cazul WORST - cand vectorul este sortat descrescator
                case WORST:
                    FillRandomArray(v, size);
                    sort(v, v  + size, greater<>{});
                    break;
            }

            /// salvez v in v1 pentru a folosi acelasi vector pentru alte sortari
            memcpy(v1, v, size * sizeof(int));

            auto opInsertionAsg = profiler.createOperation("asg_insertion", size);
            auto opInsertionCmp = profiler.createOperation("cmp_insertion", size);
            insertionSort(v, size, &opInsertionAsg, &opInsertionCmp);

            /// refac v dupa insertionSort
            memcpy(v, v1, size * sizeof(int));

            auto opBubbleAsg = profiler.createOperation("asg_bubble", size);
            auto opBubbleCmp = profiler.createOperation("cmp_bubble", size);
            bubbleSort(v, size, &opBubbleAsg, &opBubbleCmp);

            /// refac v dupa bubbleSort
            memcpy(v, v1, size * sizeof(int));

            /// worst case pentru selection sort: n, 1, 2, ..., n - 2, n - 1
            /// trebuie modificat arrey ul v
            sort(v + 1, v + size);

            auto opSelectionAsg = profiler.createOperation("asg_selection", size);
            auto opSelectionCmp = profiler.createOperation("cmp_selection", size);
            selectionSort(v, size, &opSelectionAsg, &opSelectionCmp);

            /// refac v dupa selectionSort
            memcpy(v, v1, size * sizeof(int));

            auto opBinaryInsertionAsg = profiler.createOperation("asg_binaryInsertion", size);
            auto opBinaryInsertionCmp = profiler.createOperation("cmp_binaryInsertion", size);
            binaryInsertionSort(v, size, &opBinaryInsertionAsg, &opBinaryInsertionCmp);
        }

        if (whichCase == AVERAGE) {
            nrTest /= 5;
        }
    }

    profiler.divideValues("asg_insertion", TEST);
    profiler.divideValues("cmp_insertion", TEST);

    profiler.divideValues("asg_bubble", TEST);
    profiler.divideValues("cmp_bubble", TEST);

    profiler.divideValues("asg_selection", TEST);
    profiler.divideValues("cmp_selection", TEST);

    profiler.divideValues("asg_binaryInsertion", TEST);
    profiler.divideValues("cmp_binaryInsertion", TEST);

/*
    // comparatii sortari atribuiri si comparatii - pt cerinta 1.3.3

    profiler.createGroup("Atribuiri_Sortari", "asg_insertion", "asg_bubble", "asg_selection");
    profiler.createGroup("Comparatii_Sortari", "cmp_insertion", "cmp_bubble", "cmp_selection");

    profiler.createGroup("Atribuiri_Sortari+BinaryInsertion", "asg_insertion", "asg_bubble", "asg_selection", "asg_binaryInsertion");
    profiler.createGroup("Comparatii_Sortari+BinaryInsertion", "cmp_insertion", "cmp_bubble", "cmp_selection", "cmp_binaryInsertion");
*/

    /// suma totala operatii

    profiler.addSeries("Insertion_Series", "asg_insertion", "cmp_insertion");
    profiler.addSeries("Bubble_Series", "asg_bubble", "cmp_bubble");
    profiler.addSeries("Selection_Series", "asg_selection", "cmp_selection");
    profiler.addSeries("BinaryInsertion_Series", "asg_binaryInsertion", "cmp_binaryInsertion");

/*
    /// grupare serii - pentru graphs

    profiler.createGroup("Sortari_Group_Series","Insertion_Series", "Bubble_Series", "Selection_Series");

    profiler.createGroup("Sortari_Group_Series+BinaryInsertion","Insertion_Series", "Bubble_Series", "Selection_Series", "BinaryInsertion_Series");
*/
}

void benchmark(Profiler& profiler, AnalysisCase whichCase)
{
    // TODO: Write your benchmarks here, as explained in the readme
    // You should use profiler.startTimer/stopTimer for measuring and plotting measured time
}

} // namespace lab01

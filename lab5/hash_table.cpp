#include "hash_table.h"

#include "Profiler.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>

using namespace std;

// closed Hashing - open Addressing

// ca sa fie mai eficient la closed hashing open addressing se poate alege muterea sa fie cu pas mai mare (nu din 1 in 1)
// m nr prim apropiat de 10000 (9973 sau 10007)
// c1, c2 nr prime (se alege intre 3, 5 si 5, 7
// nu pot exista duplicate (22 de doua ori, de exemplu)
// daca exista valoare in tabel => returnez ca nu pot sa o adaug
// Cormen 11.4 - la pseudocod este cu h ul din lab
// alfa - cat de ocupata este hash table (cate celule au fost puse)
// alfa <= n/m -> bucla unde se verifica daca a fost inserat cu succes
// fara grafice - se nr cate celule au fost accesate
// valoare asteptata 1+alfa+alfa^2+...+alfa^n=(1-alfa^n)/(1-alfa)
// cand n -> inf => 1/(1-alfa) - pentru negasite
// 1+1 -> alfa(i)=i/m => 1/(1-alfa(i)) = 1/(m-1)/m = m/(m-i)
// dupa mai multe calcule, se duce la 1/alfa * ln(1/(1-alfa)) in medie, nr de accesari - pt gasite

// (asta se face doar pt un alfa (ex alfa = 0.95))
// cerinta 2 - un range mare de valori (0 si rand_max = 32767)
// din cele 3000 de elemente 1500 cu 1500 - trebuie sa fie in conflict
// lista de indici unici si random cu FIllRandomArray
// for j = 0 -> 1499
//      idE = nr.rand.in(j, n-1)
//      swap(val[idE], val[j])
//      search(val[j])

// pt negasite se genereaza nr mai mari fata de rand_max

// cerinta 4 - se umple pana la fil factor 0.99, si dupa se sterg elemente pana la 0.80 si fac operatiile
// se face ca mai sus dar se schimba search cu delete


int hashF(int key, int tbSize) {
    return key % tbSize;
}

int quatricProbe(int key, int i, int tbSize) {
    return (hashF(key, tbSize) + C1 * i + C2 * i * i) % tbSize;
}

void initHashTable(Cell *ht, int tbSize) {
    for (int i = 0; i < tbSize; i++)
        ht[i].state = EMPTY;
}

void freeHashTable(Cell **ht) {
    if (ht && *ht) {
        free(*ht);
        *ht = NULL;
    }
}

bool hashInsert( const Entry& entry, Cell *ht, int tbSize) {
    int i = 0;
    int numProbes = 0; // cnt nr max incercari - din cauza saltului i creste repede
    int firstAvailable = -1;

    while (numProbes < tbSize) {
        int poz = quatricProbe(entry.id, i, tbSize);
        if (ht[poz].state == EMPTY) {
            // gasit la prima poz valabila (practic id ul lui)
            if (firstAvailable == -1)
                firstAvailable = poz;
            break;
        }

        if (ht[poz].state == FILLED && entry.id == ht[poz].data.id)
            return false; // duplicat

        if (ht[poz].state == DELETED && firstAvailable == -1)
            firstAvailable = poz;

        i+=PROBE_STEP;
        numProbes++;
    }

    if (firstAvailable != -1) {
        ht[firstAvailable].state = FILLED;
        ht[firstAvailable].data = entry;
        return true;
    }

    return false;
}

Entry* hashSearch(Cell* ht, int tbSize, int key, int *accCnt) {
    int i = 0;
    int numProbes = 0;

    if (accCnt)
        *accCnt = 0;

    while (numProbes < tbSize) {
        int poz = quatricProbe(key, i, tbSize);

        if (accCnt)
            (*accCnt)++;

        if (ht[poz].state == EMPTY)
            return NULL;

        if (ht[poz].state == FILLED && ht[poz].data.id == key)
            return &ht[poz].data;

        i+=PROBE_STEP;
        numProbes++;
    }

    return NULL;
}

bool hashDelete(Cell *ht, int tbSize, int key) {
    int i = 0;
    int numProbes = 0;

    while (i < tbSize) {
        int poz = quatricProbe(key, i, tbSize);

        if (ht[poz].state == EMPTY)
            return false; // nu este nimic de sters

        if (ht[poz].state == FILLED && ht[poz].data.id == key) {
            ht[poz].state = DELETED;
            return true;
        }

        i+=PROBE_STEP;
        numProbes++;
    }

    return false;
}

void printHashTable(Cell *ht, int tbSize) {
    // setw - prntru aliniere
    // setw(5) pune spati goale in fata ca sa faca cuvantul de 5 caractere
    cout << "Index | State    | ID   | Name\n";
    cout << "------|----------|------|----------\n";
    for (int i = 0; i < tbSize; ++i) {
        if (ht[i].state == FILLED)
            cout << setw(5) << i << " | FILLED   | " << setw(4) << ht[i].data.id << " | " << ht[i].data.nume << "\n";
        else if (ht[i].state == DELETED)
            cout << setw(5) << i << " | DELETED  | " << setw(4) << "-" << " | -\n";
        }
    }


// pentru a pastra ACELASI set de date intre toate functiile de evaluare
static int* sharedInsertKeys = NULL;
static int* sharedNotFoundKeys = NULL;

void ensureSharedData() {
    if (sharedInsertKeys == NULL) {
        // 5 rulari * 9906 elemente (pt factor de 0.99) = 49530 elemente necesare
        // range-ul (1 - 100000) trebuie sa fie mai mare decat numarul de elemente cerut
        int totalInsert = 5 * 9906;
        sharedInsertKeys = (int*)malloc(totalInsert * sizeof(int));
        FillRandomArray(sharedInsertKeys, totalInsert, 1, 100000, true, UNSORTED);

        // 5 rulari * 1500 elemente negasite = 7500 elemente
        // range separat (150000 - 200000) pentru a garanta ca nu se suprapun
        int totalNotFound = 5 * 1500;
        sharedNotFoundKeys = (int*)malloc(totalNotFound * sizeof(int));
        FillRandomArray(sharedNotFoundKeys, totalNotFound, 150000, 200000, true, UNSORTED);
    }
}

void cleanupSharedData() {
    if (sharedInsertKeys) {
        free(sharedInsertKeys);
        sharedInsertKeys = NULL;
    }
    if (sharedNotFoundKeys) {
        free(sharedNotFoundKeys);
        sharedNotFoundKeys = NULL;
    }
}

void demonstrate() {
    int choice;
    cout << "1. Valori prestabilite (Size = 11)\n";
    cout << "2. Introducere manuala (Size si elemente personalizate)\n";
    cout << "Alege varianta: ";
    cin >> choice;

    int tbSize;
    if (choice == 1)
        tbSize = 11;
    else {
        cout << "Introdu dimensiunea tabelei: ";
        cin >> tbSize;
    }

    Cell *ht = (Cell*)malloc(tbSize * sizeof(Cell));
    initHashTable(ht, tbSize);

    if (choice == 1) {
        int keys[] = {10, 20, 30, 10 + tbSize, 20 + 2 * tbSize};
        for (int i = 0; i < 5; i++) {
            Entry e;
            e.id = keys[i];
            strcpy(e.nume, "Demo");
            hashInsert(e, ht, tbSize);
            cout << "S-a inserat cheia: " << e.id << "\n";
        }
    }
    else {
        int n;
        cout << "Cate elemente vrei sa inserezi? ";
        cin >> n;
        for (int i = 0; i < n; i++) {
            int val;
            cout << "Elementul " << i + 1 << ": ";
            cin >> val;

            Entry e;
            e.id = val;
            strcpy(e.nume, "Manual");

            if (hashInsert(e, ht, tbSize))
                cout << "S-a inserat cheia: " << e.id << "\n";
            else
                cout << "Eroare: Nu s-a putut insera cheia " << e.id << " (tabela plina sau duplicat)!\n";
        }
    }

    cout << "\nTabelul initial:\n";
    printHashTable(ht, tbSize);

    int searchKey;
    cout << "\nIntrodu o valoare pentru CAUTARE: ";
    cin >> searchKey;

    int acc = 0;
    Entry* found = hashSearch(ht, tbSize, searchKey, &acc);
    cout << "Rezultat cautare " << searchKey << ": " << (found != NULL ? "Gasit" : "Negasit");
    cout << " (Accesari: " << acc << ")\n";

    int deleteKey;
    cout << "\nIntrodu o valoare pentru STERGERE: ";
    cin >> deleteKey;

    cout << "Incercam stergerea cheii " << deleteKey << "...\n";
    if (hashDelete(ht, tbSize, deleteKey))
        cout << "Stergere reusita!\n";
    else
        cout << "Stergere esuata (elementul nu a fost gasit)!\n";

    cout << "\nTabelul dupa operatia de stergere:\n";
    printHashTable(ht, tbSize);

    acc = 0;
    found = hashSearch(ht, tbSize, deleteKey, &acc);
    cout << "\nCautam iar " << deleteKey << " dupa stergere: " << (found != NULL ? "Gasit" : "Negasit");
    cout << " (Accesari: " << acc << ")\n";

    freeHashTable(&ht);
}

void runEvaluation(double fillFactor, bool withDeletion) {
    ensureSharedData();

    int tbSize = 10007;
    int m = 3000;
    int targetElements = (int)(tbSize * fillFactor);

    int initialElements;
    if (withDeletion)
        initialElements = (int)(tbSize * 0.99); // pana la 99%
    else
        initialElements = targetElements; // fix cat trebuie

    double totalAvgFound = 0, totalAvgNotFound = 0;
    int globalMaxFound = 0, globalMaxNotFound = 0;

    for (int run = 0; run < 5; run++) {
        Cell *ht = (Cell*)malloc(tbSize * sizeof(Cell));
        initHashTable(ht, tbSize);

        //  o bucata de date specifica rularii curente din vectorii globali
        int* currentRunInsertKeys = sharedInsertKeys + (run * 9906);
        int* currentRunNotFoundKeys = sharedNotFoundKeys + (run * 1500);

        for (int i = 0; i < initialElements; i++) {
            Entry e;
            e.id = currentRunInsertKeys[i];
            strcpy(e.nume, "Test");
            hashInsert(e, ht, tbSize);
        }

        if (withDeletion) {
            int elementsToDelete = initialElements - targetElements;
            for (int i = 0; i < elementsToDelete; i++) {
                hashDelete(ht, tbSize, currentRunInsertKeys[initialElements - 1 - i]);
            }
            initialElements = targetElements;
        }

        // elemente gasite
        long long accFound = 0;
        int maxF = 0;

        // copie chei curente pentru a face swap pe ele fara sa fie stricat vectorul
        int* keysForSwap = (int*)malloc(initialElements * sizeof(int));
        for (int i = 0; i < initialElements; i++) {
            keysForSwap[i] = currentRunInsertKeys[i];
        }

        for (int j = 0; j < m / 2; j++) {
            int idE = j + (rand() % (initialElements - j));

            int temp = keysForSwap[j];
            keysForSwap[j] = keysForSwap[idE];
            keysForSwap[idE] = temp;

            int acc = 0;
            hashSearch(ht, tbSize, keysForSwap[j], &acc);
            accFound += acc;

            if (acc > maxF) {
                maxF = acc;
            }
        }
        free(keysForSwap);

        // elemenete negasite
        long long accNotFound = 0;
        int maxNF = 0;
        for (int i = 0; i < m / 2; i++) {
            int acc = 0;
            hashSearch(ht, tbSize, currentRunNotFoundKeys[i], &acc);
            accNotFound += acc;

            if (acc > maxNF)
                maxNF = acc;
        }

        totalAvgFound += (double)accFound / (m / 2);
        totalAvgNotFound += (double)accNotFound / (m / 2);

        if (maxF > globalMaxFound) globalMaxFound = maxF;
        if (maxNF > globalMaxNotFound) globalMaxNotFound = maxNF;

        freeHashTable(&ht);
    }

    cout << fixed << setprecision(2);
    cout << "| ";
    if (withDeletion)
        cout << setw(14) << "0.80 (del)";
    else
        cout << setw(14) << to_string(fillFactor).substr(0, 4);

    cout << " | " << setw(19) << (totalAvgFound / 5.0)
        << " | " << setw(18) << globalMaxFound
        << " | " << setw(23) << (totalAvgNotFound / 5.0)
        << " | " << setw(22) << globalMaxNotFound << " |\n";
}

void evaluateSingleFillFactor(double fillFactor) {
    cout << "\n| Filling factor | Avg. Effort (found) | Max Effort (found) | Avg. Effort (not found) | Max Effort (not found) |\n";
    cout << "|----------------|---------------------|--------------------|-------------------------|------------------------|\n";
    runEvaluation(fillFactor, false);
}

void evaluateAllFillFactors() {
    cout << "\n| Filling factor | Avg. Effort (found) | Max Effort (found) | Avg. Effort (not found) | Max Effort (not found) |\n";
    cout << "|----------------|---------------------|--------------------|-------------------------|------------------------|\n";
    double factors[] = {0.80, 0.85, 0.90, 0.95, 0.99};
    for (int i = 0; i < 5; i++) {
        runEvaluation(factors[i], false);
    }
}

void evaluateAfterDeletion() {
    cout << "\n| Filling factor | Avg. Effort (found) | Max Effort (found) | Avg. Effort (not found) | Max Effort (not found) |\n";
    cout << "|----------------|---------------------|--------------------|-------------------------|------------------------|\n";
    runEvaluation(0.80, true);

    // curatare dupa ultima evaluare (in ideea in care se testeaza un ordinea cerintelor de pe pdf ul de lab)
    // factor 0.95 -> toti factori de la 0.8 la 0.99 -> evaluare pana la 0.99 dupa stergere pana la 0.8
    cleanupSharedData();
}
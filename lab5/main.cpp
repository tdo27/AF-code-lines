/*
1. Teorie si implementare:

    Pentru rezolvarea problemei am folosit metoda Closed Hashing cu Open Addressing si verificare patratica. Aceasta metoda foloseste o formula prin care se calculeaza pozitia urmatoare de verificare, spre deosebire de verificarea liniara, unde pasul este mereu acelasi:
h(k, i) = (h'(k) + c1*i + c2*i^2) % m. (Cormen 11.4 + lab)

    Dimensiunea tabelei este 10007, un numar prim apropiat de 10000. Am ales aceasta valoare pentru a obtine o distributie cat mai buna a elementelor si pentru a reduce numarul de coliziuni. Si constantele c1 si c2 sunt numere prime (5 si 7).
Tabela nu permite introducerea aceleiasi valori de mai multe ori. Daca valoarea exista deja, inserarea nu mai este efectuata.
Pentru a imbunatati cautarea, am folosit un pas de verificare mai mare decat 1 (PROBE_STEP). Astfel, valoarea lui i creste cu acest pas la fiecare iteratie. Am introdus si variabila numProbes, care tine evidenta numarului de verificari efectuate. In acest fel, ne asiguram ca nu depasim numarul maxim de incercari, adica dimensiunea tabelei. Variabila i este folosita doar in calculul pozitiei, iar numProbes controleaza numarul efectiv de iteratii.

2. Analiza complexitatii (in functie de factorul de umplere alfa = n/m):

    Pentru cautarile in care elementul nu este gasit (unsuccessful search):
    Numarul mediu de verificari este estimat cu ajutorul sumei 1 + alfa + alfa^2 + ... + alfa^n. Pe masura ce n creste, aceasta suma se apropie de limita 1 / (1 - alfa), care reprezinta efortul mediu teoretic pentru o cautare nereusita.
    Pentru cautarile in care elementul este gasit (successful search):
    Se calculeaza numarul mediu de verificari necesare pentru gasirea elementelor, tinand cont de momentul in care fiecare dintre ele a fost inserat. Formula folosita pentru estimare este:
(1 / alfa) * ln(1 / (1 - alfa)).

3. Cum am facut testele (profiler.h):

    Alegerea elementelor gasite (1500 din 3000):
Pentru a alege aleator elementele care urmeaza sa fie cautate, am folosit algoritmul Fisher-Yates aplicat asupra cheilor deja inserate in tabela. Astfel, selectia este aleatorie si nu depinde de ordinea in care au fost introduse elementele.
    Alegerea elementelor negasite (1500 din 3000):
Am folosit functia FillRandomArray din Profiler pentru a genera valori dintr-un interval mai mare decat cel al elementelor inserate (de exemplu, valori peste 40.000). In acest fel, ne asiguram ca valorile generate nu se gasesc deja in tabela.

4. Testarea dupa stergere (Cerinta 4):

    Mai intai, tabela este umpluta pana cand factorul de umplere ajunge la 0.99. Dupa aceea, sunt sterse elemente pana cand factorul ajunge la 0.80.
Acest test arata ca elementele marcate ca DELETED pastreaza pozitiile prin care trece cautarea si pot influenta numarul de verificari. Din acest motiv, o tabela din care au fost sterse elemente poate avea o eficienta putin mai mica decat una care a fost umpluta direct pana la factorul 0.80.
*/

#include "hash_table.h"
#include <iostream>

using namespace std;

int main() {
    int optiune;

    do {
        cout << "\n   MENIU - TABELE DE DISPERSIE (HASH)    \n";
        cout << "1. Demo operatii tabela (citire marime)\n";
        cout << "2. Evaluare: Factor de umplere = 0.95\n";
        cout << "3. Evaluare: Toti factorii (0.80 -> 0.99)\n";
        cout << "4. Evaluare: Dupa stergere (0.99 -> 0.80)\n";
        cout << "0. Iesire\n";
        cout << "-----------------------------------------\n";
        cout << "Alege o optiune: ";
        cin >> optiune;

        switch (optiune) {
            case 1:
                demonstrate();
                break;
            case 2:
                cout << "\n--- RULARE EVALUARE (0.95) ---\n";
                evaluateSingleFillFactor(0.95);
                break;
            case 3:
                cout << "\n--- RULARE EVALUARE COMPLETA ---\n";
                evaluateAllFillFactors();
                break;
            case 4:
                cout << "\n--- RULARE EVALUARE DUPA STERGERE ---\n";
                evaluateAfterDeletion();
                break;
            case 0:
                cout << "Iesire din program...\n";
                break;
            default:
                cout << "Optiune invalida! Incearca din nou.\n";
                break;
        }
    } while (optiune != 0);

    return 0;
}
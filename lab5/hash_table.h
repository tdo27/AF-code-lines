#ifndef LAB5_HASH_TABLE_H
#define LAB5_HASH_TABLE_H

#define M 10007
#define C1 5
#define C2 7

const int PROBE_STEP = 3; // pentru salt ul de dispersare mai mare

typedef struct {
    int id;
    char nume[30];
}Entry;

enum CellState {EMPTY, DELETED, FILLED};

typedef struct {
    Entry data;
    CellState state;
}Cell;


int hashF(int key, int tbSize);
int quatricProbe(int key, int i, int tbSize);

void initHashTable(Cell *ht, int tbSize);
void freeHashTable(Cell **ht);

bool hashInsert(const Entry& entry, Cell *ht, int tbSize);
Entry* hashSearch(Cell* ht, int tbSize, int key, int *accCnt);
bool hashDelete(Cell *ht, int tbSize, int key);
void printHashTable(Cell *ht, int tbSize);

void demonstrate();
void runEvaluation(double fillFactor, bool withDeletion);
void evaluateSingleFillFactor(double fillFactor);
void evaluateAllFillFactors();
void evaluateAfterDeletion();


#endif
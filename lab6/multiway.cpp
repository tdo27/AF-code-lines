/*

R1 reprezinta arborele folosind un vector de parinti, unde fiecare element retine parintele nodului.
R2 foloseste o structura cu cheia nodului si lista de copii, iar R3 reprezinta arborele binar folosind primul copil si fratele din dreapta.

Se implementeaza afisarea arborilor folosind Pretty Print, unde nivelul fiecarui nod este evidentiat prin tab-uri.
De asemenea, se realizeaza transformarile R1 in R2 si R2 in R3.
Transformarea R1 in R2 se face prin numararea copiilor si construirea listelor de copii, iar transformarea R2 in R3 se face recursiv.
Ambele transformari au complexitatea O(n), deoarece fiecare nod este parcurs o singura data.

*/

#include "multiway.h"

#include "catch2.hpp"
#include <iostream>

#include <iomanip>
#include <cstdlib>


using namespace std;

// din cele 3 - se alege una (inordine, preordine, postordine)
// se face atat la iterativ cat si recursiv

// afisarile se pun - preordine - else la if d = 1
//                  - inordine - else la if d = 2
//                  - postordine - else la if d = 3

// pretty print de la R1 - O(n^2) - worst case

// T1 - trebuie in O(n)
//    - se face vector de contor copii => se poate initializa lista de array cu valori exacte
//    - se adauga copii
//    - 4 for uri de O(n)
//    - functie de eliberare arbore + creare de nod

// T2 - O(n)
//

namespace lab06
{
    struct MultiWayTreeNode
    {
        // TODO: define structure elements

        int key;
        MultiWayTreeNode** children;
        int nrChildren;
    };

    struct BinaryTreeNode // pentru R3
    {
        // TODO: define structure elements

        int key;
        BinaryTreeNode* left_child, *right_sibling;
    };

    void recursiveTraversal(TreeNode* root)
    {
        // TODO: Implement recursive traversal

        // inordine - SRD

        if (root == NULL)
            return;

        recursiveTraversal(root->left);

        cout << root->key << '\n';

        recursiveTraversal(root->right);
    }

    void iterativeTraversal(TreeNode* root)
    {
        // TODO: Implement iterative traversal

        int d = 1;
        TreeNode* node = root;

        do {
            if (d == 1) {
                if (node->left != NULL)
                    node = node->left;
                else d = 2;
            }

            if (d == 2) { // inordine
                cout << node->key << '\n';

                if (node->right != NULL) {
                    node = node->right;
                    d = 1;
                }
                else d = 3;
            }

            if (d == 3) {
                if (node->parent != NULL) {
                    if (node == node->parent->left) {
                        d = 2;
                    }
                    node = node->parent;
                }
            }
        }while (!(node == root && d == 3));
    }

    void demoPrettyPrintR1() {
        int parentArray[10] = {2, 7, 5, 2, 7, 7, -1, 5, 2};

        prettyPrintR1(parentArray, 8);
    }

    void afisR1(int nivel, int parent, const int* parentArray, int n) {
        for (int i = 0; i < n; i++)
            if (parentArray[i] == parent) {
                for (int j = 0; j < nivel; j++)
                    cout << '\t';
                cout  << i + 1 << '\n';
                afisR1(nivel + 1, i + 1, parentArray, n);
            }
    }

    void prettyPrintR1(const int* parentArray, int n)
    {
        // TODO: Implement pretty print for R1

        if (parentArray == NULL)
            cout << "Nu a fost construit R1";

        afisR1(0, -1, parentArray, n);
    }

    void afisR2(int nivel, MultiWayTreeNode *node) {
        if (node == NULL)
            return;

        for (int j = 0; j < nivel; j++)
            cout << '\t';
        cout << node->key << '\n';

        for (int i = 0; i < node->nrChildren; i++)
            afisR2(nivel + 1, node->children[i]);
    }

    void prettyPrintR2(MultiWayTreeNode* root)
    {
        // TODO: Implement pretty print for R2

        if (root == NULL)
            cout << "Nu a fost construit R2";

        afisR2(0, root);
    }

    void afisR3(int nivel, BinaryTreeNode* node) {
        if (node == NULL)
            return;

        for (int j = 0; j < nivel; j++)
            cout << '\t';
        cout << node->key << '\n';

        afisR3(nivel + 1, node->left_child);
        afisR3(nivel, node->right_sibling);
    }

    void prettyPrintR3(BinaryTreeNode* root)
    {
        // TODO: Implement pretty print for R3
        if (root == NULL)
            cout << "Nu a fost construit R3";

        afisR3(0, root);
    }

    MultiWayTreeNode* transformR1toR2(const int* parentArray, int n)
    {
        // TODO: Implement transformation from R1 to R2

        if (parentArray == NULL)
            return NULL;

        MultiWayTreeNode** nodes = (MultiWayTreeNode**)malloc(sizeof(MultiWayTreeNode*) * n);

        for (int i = 0; i < n; i++) {
            nodes[i] = (MultiWayTreeNode*)malloc(sizeof(MultiWayTreeNode));
            nodes[i]->key = i + 1;
            nodes[i]->nrChildren = 0;
            nodes[i]->children = NULL;
        }

        for (int i = 0; i < n; i++)
            if (parentArray[i] != -1) {
                int paIndex = parentArray[i] - 1;
                nodes[paIndex]->nrChildren++;
            }

        for (int i = 0; i < n; i++)
            if (nodes[i]->nrChildren > 0)
                nodes[i]->children = (MultiWayTreeNode**)malloc(sizeof(MultiWayTreeNode*) * nodes[i]->nrChildren);

        int* childIndex = (int*)calloc(n, sizeof(int));
        MultiWayTreeNode* root = NULL;

        for (int i = 0; i < n; i++)
            if (parentArray[i] != -1) {
                int paIndex = parentArray[i] - 1;
                nodes[paIndex]->children[childIndex[paIndex]] = nodes[i];
                childIndex[paIndex]++;
            }
        else root = nodes[i];

        free(childIndex);
        free(nodes); // nodes[i] practic fac parte din root
        // prin root = nodes[i] este ca si cum am schimbat "numele",
        // dar daca sterg cu un for nodes[i], sterg arborele

        return root;
    }

    BinaryTreeNode* transformR2toR3(MultiWayTreeNode* root)
    {
        // TODO: Implement transformation from R2 to R3

        if (root == NULL)
            return NULL;

        BinaryTreeNode* nodNou = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));

        nodNou->left_child = nodNou->right_sibling = NULL;
        nodNou->key = root->key;

        BinaryTreeNode* nodPrev = NULL;
        for (int i = 0; i < root->nrChildren; i++) {
            BinaryTreeNode* nodCurr = transformR2toR3(root->children[i]);

            if (i == 0)
                nodNou->left_child = nodCurr;
            else nodPrev->right_sibling = nodCurr;

            nodPrev = nodCurr;
        }

        return nodNou;
    }

    TEST_CASE("Tree Representations and Transformations")
    {
        // The example from the readme: Π = {2, 7, 5, 2, 7, 7, −1, 5, 2}
        const int parentArray[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
        const int n = sizeof(parentArray) / sizeof(parentArray[0]);

        SECTION("Transformation R1 to R2 and Pretty Print R2") {
            // TODO: Test transformR1toR2

            MultiWayTreeNode* r2_root = transformR1toR2(parentArray, n);

            REQUIRE(r2_root != NULL);
            REQUIRE(r2_root->key == 7);

            REQUIRE(r2_root->nrChildren == 3);
            REQUIRE(r2_root->children[0]->key == 2);
            REQUIRE(r2_root->children[1]->key == 5);
            REQUIRE(r2_root->children[2]->key == 6);

            MultiWayTreeNode* node2 = r2_root->children[0];
            REQUIRE(node2->nrChildren == 3);
            REQUIRE(node2->children[0]->key == 1);
            REQUIRE(node2->children[1]->key == 4);
            REQUIRE(node2->children[2]->key == 9);

            MultiWayTreeNode* node5 = r2_root->children[1];
            REQUIRE(node5->nrChildren == 2);
            REQUIRE(node5->children[0]->key == 3);
            REQUIRE(node5->children[1]->key == 8);

            MultiWayTreeNode* node6 = r2_root->children[2];
            REQUIRE(node6->nrChildren == 0);
        }

        SECTION("Transformation R2 to R3 and Pretty Print R3") {
            // TODO: Test transformR2toR3

            MultiWayTreeNode* r2_root = transformR1toR2(parentArray, n);
            BinaryTreeNode* r3_root = transformR2toR3(r2_root);

            REQUIRE(r3_root != NULL);
            REQUIRE(r3_root->key == 7);
            REQUIRE(r3_root->right_sibling == NULL);

            BinaryTreeNode* node2 = r3_root->left_child;
            REQUIRE(node2 != NULL);
            REQUIRE(node2->key == 2);

            BinaryTreeNode* node5 = node2->right_sibling;
            REQUIRE(node5 != NULL);
            REQUIRE(node5->key == 5);

            BinaryTreeNode* node6 = node5->right_sibling;
            REQUIRE(node6 != NULL);
            REQUIRE(node6->key == 6);
            REQUIRE(node6->right_sibling == NULL);

            BinaryTreeNode* node1 = node2->left_child;
            REQUIRE(node1 != NULL);
            REQUIRE(node1->key == 1);
            REQUIRE(node1->left_child == NULL);

            BinaryTreeNode* node4 = node1->right_sibling;
            REQUIRE(node4 != NULL);
            REQUIRE(node4->key == 4);

            BinaryTreeNode* node9 = node4->right_sibling;
            REQUIRE(node9 != NULL);
            REQUIRE(node9->key == 9);
            REQUIRE(node9->right_sibling == NULL);
        }
    }

} // namespace lab06

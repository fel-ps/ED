#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int v;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->v = v;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node* node, int v) {
    if (node == NULL) return createNode(v);
    if (v < node->v) node->left = insert(node->left, v);
    else if (v > node->v) node->right = insert(node->right, v);
    return node;
}

Node* search(Node* root, int v) {
    if (root == NULL || root->v == v) return root;
    if (root->v < v) return search(root->right, v);
    return search(root->left, v);
}

void freeTree(Node* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

unsigned long long measureExecutionTime(Node* root, int v) {
    clock_t start = clock();
    search(root, v);
    clock_t end = clock();
    return (end - start) * 1000000 / CLOCKS_PER_SEC;  // microseconds
}

int main() {
    FILE *file;
    int numExecutions = 5;
    int startSize = 100;
    int endSize = 10000;
    int increment = 100;

    file = fopen("bst_w_time.txt", "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    fprintf(file, "# Tamanho_do_Vetor Tempo_Total_de_Execucao\n");

    for (int size = startSize; size <= endSize; size += increment) {
        unsigned long long t_total = 0;

        for (int i = 0; i < numExecutions; i++) {
            Node* root = NULL;
            for (int j = 1; j <= size; j++) {
                root = insert(root, j);  // Inserção em ordem crescente, criando o pior caso
            }

            int searchv = size;  // Buscando o maior valor, que estará na folha
            t_total += measureExecutionTime(root, searchv);

            freeTree(root);
        }

        unsigned long long t_average = t_total / numExecutions;
        fprintf(file, "%d %llu\n", size, t_average);
    }

    fclose(file);

    printf("Tempos de execução no pior caso calculados e salvos em 'bst_w_time.txt'.\n");

    return 0;
}

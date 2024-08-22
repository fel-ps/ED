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

void generateRandomArray(int* array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = i + 1;  // Use a sorted array of unique values
    }
    // Shuffle the array to create randomness
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

unsigned long long measureExecutionTime(Node* root, int* searchKeys, int numSearches) {
    clock_t start = clock();
    for (int i = 0; i < numSearches; i++) {
        search(root, searchKeys[i]);
    }
    clock_t end = clock();
    return (end - start) * 1000000 / CLOCKS_PER_SEC;  // microseconds
}

int main() {
    FILE *file;
    int numSearches = 100;
    int startSize = 100;
    int endSize = 100000;
    int increment = 100;

    file = fopen("bst_a_time.txt", "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    fprintf(file, "# Tamanho_do_Vetor Tempo_Total_de_Execucao\n");

    srand(time(NULL));

    for (int size = startSize; size <= endSize; size += increment) {
        int *array = (int*)malloc(size * sizeof(int));
        if (array == NULL) {
            perror("Erro ao alocar memória");
            fclose(file);
            return 1;
        }

        generateRandomArray(array, size);

        unsigned long long totalTime = 0;

        for (int i = 0; i < 10; i++) {
            Node* root = NULL;
            for (int j = 0; j < size; j++) {
                root = insert(root, array[j]);
            }

            int *searchKeys = (int*)malloc(numSearches * sizeof(int));
            if (searchKeys == NULL) {
                perror("Erro ao alocar memória");
                freeTree(root);
                free(array);
                fclose(file);
                return 1;
            }

            // Pick random search keys from the tree
            for (int k = 0; k < numSearches; k++) {
                searchKeys[k] = array[rand() % size];
            }

            totalTime += measureExecutionTime(root, searchKeys, numSearches);

            free(searchKeys);
            freeTree(root);
        }

        unsigned long long averageTime = totalTime / 10;
        fprintf(file, "%d %llu\n", size, averageTime);

        free(array);
    }

    fclose(file);

    printf("Tempos de execução calculados e salvos em 'bst_a_time.txt'.\n");

    return 0;
}

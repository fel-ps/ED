#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura do nó da árvore AVL
typedef struct Node {
    int v;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// Função para obter a altura de um nó
int height(Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Função para calcular o valor máximo entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para criar um novo nó
Node* createNode(int v) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->v = v;
    node->left = node->right = NULL;
    node->height = 1;  // Novo nó é inicialmente adicionado como uma folha
    return node;
}

// Função para rotacionar à direita a subárvore com raiz no nó y
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Rotacionar
    x->right = y;
    y->left = T2;

    // Atualizar alturas
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Retornar nova raiz
    return x;
}

// Função para rotacionar à esquerda a subárvore com raiz no nó x
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Rotacionar
    y->left = x;
    x->right = T2;

    // Atualizar alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Retornar nova raiz
    return y;
}

// Função para obter o fator de balanceamento de um nó
int getBalance(Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Função para inserir um novo nó na árvore AVL
Node* insert(Node* node, int v) {
    if (node == NULL)
        return createNode(v);

    if (v < node->v)
        node->left = insert(node->left, v);
    else if (v > node->v)
        node->right = insert(node->right, v);
    else  // Chaves duplicadas não são permitidas
        return node;

    // Atualizar altura deste nó ancestral
    node->height = 1 + max(height(node->left), height(node->right));

    // Obter o fator de balanceamento deste nó ancestral
    int balance = getBalance(node);

    // Se o nó se tornou desbalanceado, então existem 4 casos

    // Caso de rotação simples à direita
    if (balance > 1 && v < node->left->v)
        return rightRotate(node);

    // Caso de rotação simples à esquerda
    if (balance < -1 && v > node->right->v)
        return leftRotate(node);

    // Caso de rotação dupla à direita
    if (balance > 1 && v > node->left->v) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Caso de rotação dupla à esquerda
    if (balance < -1 && v < node->right->v) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Função para buscar um valor na árvore AVL
Node* search(Node* root, int v) {
    if (root == NULL || root->v == v)
        return root;

    if (v < root->v)
        return search(root->left, v);

    return search(root->right, v);
}

// Função para liberar a memória da árvore
void freeTree(Node* node) {
    if (node == NULL)
        return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

// Função para gerar um array de números aleatórios únicos
void generateRandomArray(int* array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = i + 1;
    }
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Função para medir o tempo de execução das buscas
unsigned long long measureExecutionTime(Node* root, int* searchvs, int numSearches) {
    clock_t start = clock();
    for (int i = 0; i < numSearches; i++) {
        search(root, searchvs[i]);
    }
    clock_t end = clock();
    return (end - start) * 1000000 / CLOCKS_PER_SEC;  // microseconds
}

int main() {
    FILE *file;
    int numExecutions = 10;
    int numSearches = 100;
    int startSize = 100;
    int endSize = 100000;
    int increment = 100;

    file = fopen("avl_a_time.txt", "w");
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

        unsigned long long t_total = 0;

        for (int i = 0; i < numExecutions; i++) {
            Node* root = NULL;
            for (int j = 0; j < size; j++) {
                root = insert(root, array[j]);
            }

            int *searchvs = (int*)malloc(numSearches * sizeof(int));
            if (searchvs == NULL) {
                perror("Erro ao alocar memória");
                freeTree(root);
                free(array);
                fclose(file);
                return 1;
            }

            for (int k = 0; k < numSearches; k++) {
                searchvs[k] = array[rand() % size];
            }

            t_total += measureExecutionTime(root, searchvs, numSearches);

            free(searchvs);
            freeTree(root);
        }

        unsigned long long t_average = t_total / numExecutions;
        fprintf(file, "%d %llu\n", size, t_average);

        free(array);
    }

    fclose(file);

    printf("Tempos de execução calculados e salvos em 'avl_a_time.txt'.\n");

    return 0;
}

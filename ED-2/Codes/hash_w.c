#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TABLE_SIZE 100003  // Tamanho da tabela hash (um número primo para reduzir colisões)

// Estrutura para os nós da lista encadeada (usada para lidar com colisões)
typedef struct Node {
    int key;
    struct Node* next;
} Node;

// Função hash que sempre retorna o mesmo índice para simular o pior caso
int hash(int key) {
    return 0;  // Todas as chaves colidem no índice 0
}

// Função para inserir um elemento na tabela hash
void insert(Node* hashTable[], int key) {
    int index = hash(key);
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// Função para buscar um elemento na tabela hash
int search(Node* hashTable[], int key) {
    int index = hash(key);
    Node* node = hashTable[index];
    while (node != NULL) {
        if (node->key == key) {
            return 1;  // Elemento encontrado
        }
        node = node->next;
    }
    return 0;  // Elemento não encontrado
}

// Função para liberar a memória da tabela hash
void freeHashTable(Node* hashTable[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* node = hashTable[i];
        while (node != NULL) {
            Node* temp = node;
            node = node->next;
            free(temp);
        }
    }
}

// Função para medir o tempo de execução das buscas
unsigned long long measureExecutionTime(Node* hashTable[], int* searchKeys, int numSearches) {
    clock_t start = clock();
    for (int i = 0; i < numSearches; i++) {
        search(hashTable, searchKeys[i]);
    }
    clock_t end = clock();
    return (end - start) * 1000000 / CLOCKS_PER_SEC;  // microseconds
}

int main() {
    FILE *file;
    int numExecutions = 10;
    int numSearches = 100;
    int startSize = 100;
    int endSize = 10000;
    int increment = 100;

    file = fopen("hash_w_time.txt", "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    fprintf(file, "# Tamanho_da_Tabela Tempo_Medio_de_Execucao\n");

    srand(time(NULL));

    for (int size = startSize; size <= endSize; size += increment) {
        unsigned long long t_total = 0;

        for (int i = 0; i < numExecutions; i++) {
            Node* hashTable[TABLE_SIZE] = {NULL};

            // Inserir elementos na tabela hash
            for (int j = 0; j < size; j++) {
                int key = j;  // Usando valores únicos para simular o pior caso
                insert(hashTable, key);
            }

            // Gerar chaves de busca (metade existentes e metade não existentes)
            int *searchKeys = (int*)malloc(numSearches * sizeof(int));
            if (searchKeys == NULL) {
                perror("Erro ao alocar memória");
                freeHashTable(hashTable);
                fclose(file);
                return 1;
            }

            for (int k = 0; k < numSearches; k++) {
                if (k < numSearches / 2) {
                    // Chaves existentes
                    searchKeys[k] = k;
                } else {
                    // Chaves não existentes
                    searchKeys[k] = size + k;
                }
            }

            t_total += measureExecutionTime(hashTable, searchKeys, numSearches);

            free(searchKeys);
            freeHashTable(hashTable);
        }

        unsigned long long t_average = t_total / numExecutions;
        fprintf(file, "%d %llu\n", size, t_average);
    }

    fclose(file);

    printf("Tempos de execução calculados e salvos em hash_w_time.txt.\n");

    return 0;
}

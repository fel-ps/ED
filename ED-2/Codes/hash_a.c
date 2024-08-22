#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TABLE_SIZE 100003  // Tamanho da tabela hash (um número primo para reduzir colisões)

// Estrutura para os nós da lista encadeada (usada para lidar com colisões)
typedef struct Node {
    int v;
    struct Node* next;
} Node;

// Função hash simples (pode ser substituída por uma mais sofisticada)
int hash(int v) {
    return v % TABLE_SIZE;
}

// Função para inserir um elemento na tabela hash
void insert(Node* hashTable[], int v) {
    int index = hash(v);
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->v = v;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// Função para buscar um elemento na tabela hash
int search(Node* hashTable[], int v) {
    int index = hash(v);
    Node* node = hashTable[index];
    while (node != NULL) {
        if (node->v == v) {
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
unsigned long long measureExecutionTime(Node* hashTable[], int* searchvs, int numSearches) {
    clock_t start = clock();
    for (int i = 0; i < numSearches; i++) {
        search(hashTable, searchvs[i]);
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

    file = fopen("hash_a_time.txt", "w");
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
                int v = rand();
                insert(hashTable, v);
            }

            // Gerar chaves de busca (metade existentes e metade não existentes)
            int *searchvs = (int*)malloc(numSearches * sizeof(int));
            if (searchvs == NULL) {
                perror("Erro ao alocar memória");
                freeHashTable(hashTable);
                fclose(file);
                return 1;
            }

            for (int k = 0; k < numSearches; k++) {
                if (k < numSearches / 2) {
                    // Chaves existentes
                    searchvs[k] = rand();
                    insert(hashTable, searchvs[k]);
                } else {
                    // Chaves não existentes
                    searchvs[k] = rand() + size + 1;
                }
            }

            t_total += measureExecutionTime(hashTable, searchvs, numSearches);

            free(searchvs);
            freeHashTable(hashTable);
        }

        unsigned long long t_average = t_total / numExecutions;
        fprintf(file, "%d %llu\n", size, t_average);
    }

    fclose(file);

    printf("Tempos de execução calculados e salvos em hash_a_time.txt.\n");

    return 0;
}

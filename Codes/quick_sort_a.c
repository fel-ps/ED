#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

// Função para trocar dois elementos
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Função para particionar o vetor para o QuickSort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Função para realizar o QuickSort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Função para medir o tempo de execução em microssegundos
long long measureTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((long long)tv.tv_sec) * 1000000 + tv.tv_usec;
}

// Função para gerar um vetor aleatório de tamanho "size"
void generateRandomArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand();
    }
}

int main() {
    srand(time(NULL));  // Inicializa o gerador de números aleatórios
    int size = 10000;
    FILE *file = fopen("quick_a_time.txt", "w");  // Abre o arquivo para escrita

    for (int k = 100; k <= size; k += 10) {
        int *arr = (int *)malloc(k * sizeof(int));

        generateRandomArray(arr, k);  // Gera um vetor aleatório

        long long start_time = measureTime();  // Marca o tempo de início

        quickSort(arr, 0, k - 1);  // Executa o QuickSort

        long long end_time = measureTime();  // Marca o tempo de fim
        long long execution_time = end_time - start_time;  // Calcula o tempo de execução


        // Escreve no arquivo de texto
        fprintf(file, "%d %lld\n", k, execution_time);

        free(arr);  // Libera a memória alocada para o vetor
    }

    fclose(file);  // Fecha o arquivo

    return 0;
}
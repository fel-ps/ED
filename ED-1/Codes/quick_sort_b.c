#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void trocar(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int particionar(int arr[], int baixo, int alto) {
    int pivo = arr[alto];
    int i = (baixo - 1);

    for (int j = baixo; j < alto; j++) {
        if (arr[j] <= pivo) {
            i++;
            trocar(&arr[i], &arr[j]);
        }
    }
    trocar(&arr[i + 1], &arr[alto]);
    return (i + 1);
}

void quickSort(int arr[], int baixo, int alto) {
    if (baixo < alto) {
        int pi = particionar(arr, baixo, alto);
        quickSort(arr, baixo, pi - 1);
        quickSort(arr, pi + 1, alto);
    }
}

void generateBestCase(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i + 1;
    }
}

// Função para medir o tempo de execução em microssegundos
// long long measureTime() {
//     struct timeval tv;
//     gettimeofday(&tv, NULL);
//     return ((long long)tv.tv_sec) * 1000000 + tv.tv_usec;
// }

long long measureTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (long long)(time.tv_sec * 1000LL + time.tv_usec / 1000LL);  // Converte para milissegundos
}

int main() {
    FILE *file = fopen("quick_best_case_time.txt", "w");
    
    if (file == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        return 1;
    }
    
    fprintf(file, "# Tamanho_do_vetor Tempo_de_execucao(s)\n");

    int end_size = 10000;
    
    for (int size = 100; size <= end_size; size += 10) {
        int* arr = (int*)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Falha ao alocar memória.\n");
            fclose(file);
            return 1;
        }

        generateBestCase(arr, size);
        
        long long start = measureTime();
        quickSort(arr, 0, size - 1);
        long long end = measureTime();
        
        long long elapsed_time = end - start;
        
        fprintf(file, "%d %lld\n", size, elapsed_time);
        
        free(arr);
    }
    
    fclose(file);
    
    printf("Dados salvos em quick_best_case_time.txt.\n");

    return 0;
}

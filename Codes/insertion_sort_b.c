#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main() {
    int size, i;
    FILE *file;
    struct timeval start, end;
    long seconds, useconds;
    double cpu_time_used;

    // Abre o arquivo para escrita
    file = fopen("insertion_b_time.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Loop para diferentes tamanhos de vetor
    for (size = 100; size <= 10000; size += 10) {
        int *arr = (int *)malloc(size * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro ao alocar memória.\n");
            fclose(file);
            return 1;
        }

        // Melhor caso: vetor já ordenado
        for (i = 0; i < size; i++) {
            arr[i] = i + 1;
        }

        gettimeofday(&start, NULL);
        insertionSort(arr, size);
        gettimeofday(&end, NULL);

        // Calcula o tempo de execução em segundos
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        cpu_time_used = seconds + useconds / 1000000.0;

        // Escreve os dados no arquivo
        fprintf(file, "%d %f\n", size, cpu_time_used);

        free(arr);
    }

    // Fecha o arquivo
    fclose(file);

    return 0;
}

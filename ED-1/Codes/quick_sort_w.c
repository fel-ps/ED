#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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

long long getCurrentTime() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * 1000000LL + currentTime.tv_usec;
}

void generateDescendingArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
}

int main() {
    FILE* file = fopen("quick_worst_case_time.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.");
        return 1;
    }

    fprintf(file, "# Tamanho_do_vetor Pior_tempo_de_execucao_us\n");

    for (int vectorSize = 100; vectorSize <= 10000; vectorSize += 10) {
        int* arr = (int*)malloc(vectorSize * sizeof(int));
        if (arr == NULL) {
            printf("Erro ao alocar memória.\n");
            return 1;
        }

        generateDescendingArray(arr, vectorSize);

        long long startTime = getCurrentTime();
        quickSort(arr, 0, vectorSize - 1);
        long long endTime = getCurrentTime();

        long long worstTime = endTime - startTime;

        fprintf(file, "%d %lld\n", vectorSize, worstTime);

        free(arr);
    }

    fclose(file);

    return 0;
}

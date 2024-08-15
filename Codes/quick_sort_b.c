#include <stdio.h>
#include <stdlib.h>
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

int main() {
    FILE *file = fopen("quick_best_case_time.txt", "w");
    
    if (file == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        return 1;
    }
    
    fprintf(file, "# Tamanho_do_vetor Tempo_de_execucao(s)\n");

    int start_size = 100;
    int end_size = 10000;
    int increment = 10;
    
    for (int size = start_size; size <= end_size; size += increment) {
        int* arr = (int*)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Falha ao alocar memória.\n");
            fclose(file);
            return 1;
        }

        generateBestCase(arr, size);
        
        clock_t start = clock();
        quickSort(arr, 0, size - 1);
        clock_t end = clock();
        
        double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
        
        fprintf(file, "%d %lf\n", size, elapsed_time);
        
        free(arr);
    }
    
    fclose(file);
    
    printf("Dados salvos em quick_best_case_time.txt.\n");

    return 0;
}

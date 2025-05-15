#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int row;
    int col;
    int **matrix_a;
    int **matrix_b;
    int **result;
    int size;
} ThreadData;

void *multiply(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int sum = 0;

    for (int k = 0; k < data->size; k++) {
        sum += data->matrix_a[data->row][k] * data->matrix_b[k][data->col];
    }

    data->result[data->row][data->col] = sum;

    int *return_value = malloc(sizeof(int));
    *return_value = sum;

    free(data);  // Free dynamically allocated ThreadData
    pthread_exit(return_value);
}

int main() {
    int size;
    printf("Enter the size of the square matrices: ");
    scanf("%d", &size);

    int **matrix_a = malloc(size * sizeof(int *));
    int **matrix_b = malloc(size * sizeof(int *));
    int **result = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        matrix_a[i] = malloc(size * sizeof(int));
        matrix_b[i] = malloc(size * sizeof(int));
        result[i] = malloc(size * sizeof(int));
    }

    printf("Matrix A:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix_a[i][j] = i + j;
            printf("%d ", matrix_a[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrix B:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix_b[i][j] = i - j;
            printf("%d ", matrix_b[i][j]);
        }
        printf("\n");
    }

    int total_sum = 0;
    int total_threads = size * size;
    pthread_t *threads = malloc(total_threads * sizeof(pthread_t));
    int thread_idx = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            ThreadData *data = malloc(sizeof(ThreadData));
            data->row = i;
            data->col = j;
            data->matrix_a = matrix_a;
            data->matrix_b = matrix_b;
            data->result = result;
            data->size = size;

            if (pthread_create(&threads[thread_idx], NULL, multiply, data)) {
                fprintf(stderr, "Error creating thread\n");
                return 1;
            }
            thread_idx++;
        }
    }

    for (int i = 0; i < total_threads; i++) {
        int *res;
        pthread_join(threads[i], (void **)&res);
        total_sum += *res;
        free(res);
    }

    printf("\nResult Matrix:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    printf("\nTotal sum of all elements: %d\n", total_sum);

    for (int i = 0; i < size; i++) {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(result[i]);
    }
    free(matrix_a);
    free(matrix_b);
    free(result);
    free(threads);

    return 0;
}

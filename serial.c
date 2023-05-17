#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double calculate_mean(int *marks, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += marks[i];
    }
    return (double)sum / size;
}

double calculate_variance(int *marks, int size, double mean) {
    double variance = 0;
    for (int i = 0; i < size; i++) {
        variance += pow(marks[i] - mean, 2);
    }
    return variance / size;
}

int main() {
    int array_size;
    int *marks = NULL;
    clock_t start_time, end_time;
    double execution_time_ms;
    double mean, variance, standard_deviation;


    printf("Enter the size of the array: ");
    scanf("%d", &array_size);

    // Start measuring the execution time
    start_time = clock();

    marks = (int*)malloc(array_size * sizeof(int));


    printf("Enter the elements of the array: ");
    for (int i = 0; i < array_size; i++) {
        scanf("%d", &marks[i]);
    }


    mean = calculate_mean(marks, array_size);


    variance = calculate_variance(marks, array_size, mean);


    standard_deviation = sqrt(variance);

 
    printf("Mean: %.2f\n", mean);
    printf("Variance: %.2f\n", variance);
    printf("Standard Deviation: %.2f\n", standard_deviation);

    // Stop measuring the execution time
    end_time = clock();

    // Calculate the execution time in milliseconds
    execution_time_ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000;

    // Print the execution time
    printf("Execution Time: %.2f ms\n", execution_time_ms);
    
    free(marks);

    return 0;
}
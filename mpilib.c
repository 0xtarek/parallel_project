#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

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

int main(int argc, char *argv[]) {
    int num_processes, process_rank;
    int array_size;
    int *marks = NULL;
    int *local_marks = NULL;
    int local_sum = 0;
    int global_sum = 0;
    clock_t start_time, end_time;
    double execution_time_ms;
    double mean, local_mean, global_mean;
    double variance, local_variance, global_variance;
    double standard_deviation;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {

        printf("Enter the size of the array: ");
        scanf("%d", &array_size);
        // Start measuring the execution time
        start_time = clock();

        marks = (int*)malloc(array_size * sizeof(int));

  
        printf("Enter the elements of the array: ");
        for (int i = 0; i < array_size; i++) {
            scanf("%d", &marks[i]);
        }


        MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);


        local_marks = (int*)malloc((array_size / num_processes) * sizeof(int));
        MPI_Scatter(marks, array_size / num_processes, MPI_INT, local_marks,
                    array_size / num_processes, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);


        local_marks = (int*)malloc((array_size / num_processes) * sizeof(int));
        MPI_Scatter(marks, array_size / num_processes, MPI_INT, local_marks,
                    array_size / num_processes, MPI_INT, 0, MPI_COMM_WORLD);
    }


    for (int i = 0; i < array_size / num_processes; i++) {
        local_sum += local_marks[i];
    }

    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


    local_mean = (double)local_sum / (array_size / num_processes);
    MPI_Allreduce(&local_mean, &global_mean, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    mean = global_mean / num_processes;


    local_variance = calculate_variance(local_marks, array_size / num_processes, local_mean);


    MPI_Reduce(&local_variance, &global_variance, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


    if (process_rank == 0) {
        variance = global_variance / array_size;
        standard_deviation = sqrt(variance);

        printf("Mean: %.2f\n", mean);
        printf("Variance: %.2f\n", variance);
        printf("Standard Deviation: %.2f\n", standard_deviation);
    }
    // Stop measuring the execution time
    end_time = clock();

    // Calculate the execution time in milliseconds
    execution_time_ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000;

    // Print the execution time
    printf("Execution Time: %.2f ms\n", execution_time_ms);
    
    if (marks != NULL) {
        free(marks);
    }
    if (local_marks != NULL) {
        free(local_marks);
    }

    MPI_Finalize();

    return 0;
}


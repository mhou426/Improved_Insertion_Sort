#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
N = 1000
Improved Insertion Sort time: 0.000000 seconds
Original Insertion Sort time: 0.001000 seconds

Sorted.

N = 10000
Improved Insertion Sort time: 0.005000 seconds
Original Insertion Sort time: 0.014000 seconds

Sorted.

N = 100000
Improved Insertion Sort time: 0.087000 seconds
Original Insertion Sort time: 0.371000 seconds

Sorted.

N = 1000000
Improved Insertion Sort time: 1.609000 seconds

Sorted.
Original Insertion Sort time: 0.662000 seconds

Sorted.


N = 10000000
Improved Insertion Sort time: 52.139000 seconds

Sorted.
Original Insertion Sort time: 0.717000 seconds

Sorted.
*/

#define Num 10000000       // Size of the array
#define SortingVal 10   // Controls how sorted the input is (higher = more sorted)

// Global variables for block management
int m;                  // Block size (cube root of n)
int l;                  // Number of blocks (n/m)
int **block_data;       // 2D array for block storage
int *block_size;        // Array tracking sizes of each block
int *block_next;         // Array tracking block positions

void init_blocks(int n) {
    m = (int)ceil(sqrt(n));  // m = n^(1/3)
    l = (int)ceil((double)n / m);  // l = n/m

    // Allocate memory for blocks
    block_data = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        block_data[i] = (int *)malloc(2 * m * sizeof(int));
    }
    block_size = (int *)calloc(m, sizeof(int));
    block_next = (int *)malloc(m * sizeof(int));
}

void free_blocks() {
    for (int i = 1; i <= l; i++) {
        free(block_data[i]);
    }
    free(block_data);
    free(block_size);
    free(block_next);
}

void improved_insertion_sort(int A[], int n) {
    if (n <= 1) return;

    // int m = (int)ceil(sqrt(n));
    // // Allocate memory for blocks
    // int **block_data = (int **)malloc(m * sizeof(int *));
    // for (int i = 0; i < m; i++) {
    //     block_data[i] = (int *)malloc(2 * m * sizeof(int));
    // }
    // int *block_size = (int *)calloc(m, sizeof(int));
    // int *block_next = (int *)malloc(m * sizeof(int));  // Changed from block_left
    for (int i = 0; i < m; i++) block_next[i] = -1;

    // Initialize first block
    int first_block = 0;
    int last_block = 0;
    int free_block = 1;
    block_data[first_block][0] = A[0];
    block_size[first_block] = 1;

    for (int j = 1; j < n; j++) {
        int current = first_block;
        int prev = -1;
        
        // Find insertion point
        while (current != -1 && block_data[current][0] <= A[j]) {
            prev = current;
            current = block_next[current];
        }

        // If we didn't find a block, insert into last block
        if (prev == -1) prev = first_block;
        else if (current == -1) prev = last_block;
        else prev = (block_next[prev] == -1) ? last_block : prev;

        // Split block if needed
        if (block_size[prev] == 2 * m) {
            int new_block = free_block++;
            // Move second half to new block
            for (int i = 0; i < m; i++) {
                block_data[new_block][i] = block_data[prev][m + i];
            }
            block_size[prev] = m;
            block_size[new_block] = m;
            block_next[new_block] = block_next[prev];
            block_next[prev] = new_block;
            
            if (prev == last_block) last_block = new_block;
            
            // Choose which block to insert into
            if (block_data[new_block][0] <= A[j]) prev = new_block;
        }

        // Insert element
        int pos = block_size[prev];
        block_data[prev][pos] = A[j];
        block_size[prev]++;
        
        // Insertion sort within block
        while (pos > 0 && block_data[prev][pos-1] > block_data[prev][pos]) {
            int temp = block_data[prev][pos-1];
            block_data[prev][pos-1] = block_data[prev][pos];
            block_data[prev][pos] = temp;
            pos--;
        }
    }

    // Merge blocks back into array
    int i = 0;
    int current = first_block;
    while (current != -1) {
        for (int l = 0; l < block_size[current]; l++) {
            A[i++] = block_data[current][l];
        }
        current = block_next[current];
    }

    // // Free memory
    // for (int i = 0; i < m; i++) free(block_data[i]);
    // free(block_data);
    // free(block_size);
    // free(block_next);
}

// Standard Insertion Sort for comparison
void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// just to check if the output is fully sorted
void verifyArray(int arr[], int size) {
    for (int i = 0; i < size - 1; i++){
        //compare two adjacent
        if(arr[i] > arr[i+1]){
            printf("\nNot Sorted.\n");
            return;
        }
    }
    printf("\nSorted.\n");
}

int main() {
    srand(time(0));
    int *arr = (int *)malloc(Num * sizeof(int));
    int *duplicateArr = (int *)malloc(Num * sizeof(int));

    if (!arr || !duplicateArr) {
        printf("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    // CASE I: Generate revise - sorted list
    for (int i = 0; i < Num; i++) {
        arr[i] = i + 1;
        duplicateArr[i] = arr[i];
    }

    // CASE II: Partially shuffle the array
    for (int i = 0; i < Num/SortingVal; i++) {
        int index1 = rand() % Num;
        int index2 = rand() % Num;
        int temp = arr[index1];
        arr[index1] = arr[index2];
        arr[index2] = temp;
    }

    // Copy to duplicate array
    for (int i = 0; i < Num; i++) {
        duplicateArr[i] = arr[i];
    }

    // Test Standard Insertion Sort
    clock_t start = clock();
    insertion_sort(arr, Num);
    clock_t end = clock();
    double timeOriginal = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Original Insertion Sort time: %f seconds\n", timeOriginal);
    verifyArray(arr, Num);

    init_blocks(Num);

    // Test Improved Insertion Sort
    start = clock();
    improved_insertion_sort(arr, Num);
    end = clock();
    double timeRevised = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Improved Insertion Sort time: %f seconds\n", timeRevised);
    verifyArray(arr, Num);
    free_blocks();

    printf("Ratio: %f", timeRevised/timeOriginal);

    // Reset array
    for (int i = 0; i < Num; i++) {
        arr[i] = duplicateArr[i];
    }

    free(arr);
    free(duplicateArr);
    return 0;
}
/* A simplified learned index implementation:
 *
 * Skeleton code written by Jianzhong Qi, April 2023
 * Edited by: [Hanzhang Chen 1173968]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STAGE_NUM_ONE 1						  /* stage numbers */ 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */

#define DATASET_SIZE 100					  /* number of input integers */
#define DATA_OUTPUT_SIZE 10					  /* output size for stage 1 */

#define BS_NOT_FOUND (-1)					  /* used by binary search */
#define BS_FOUND 0

typedef int data_t; 				  		  /* data type */

/****************************************************************/

/* function prototypes */
void print_stage_header(int stage_num);
int cmp(data_t *x1, data_t *x2);
void swap_data(data_t *x1, data_t *x2);
void partition(data_t dataset[], int n, data_t *pivot,
		int *first_eq, int *first_gt);
void quick_sort(data_t dataset[], int n);
int binary_search(data_t dataset[], int lo, int hi, 
	data_t *key, int *locn);

void stage_one(data_t dataset[]);
void stage_two(data_t dataset[]);
int stage_three(data_t dataset[], data_t array_a[], data_t array_b[],
                data_t array_n[], int* max_err);
void stage_four(data_t dataset[], data_t array_a[], data_t array_b[],
                data_t array_n[], int count, int max_err);
/* add your own function prototypes here */
float check_ab(int dataset[], int i, int *a, int *b);
int max(int num1, int num2);
int min(int num1, int num2);
/****************************************************************/

/* main function controls a ll the action */
int
main(int argc, char *argv[]) {
	/* to hold all input data */
    
    data_t dataset[DATASET_SIZE];
    int max_err;
	// Define all arrays.
    int array_a[DATASET_SIZE];
    int array_b[DATASET_SIZE];
    int array_n[DATASET_SIZE];

	/* stage 1: read and sort the input */
    stage_one(dataset); 
	
	/* stage 2: compute the first mapping function */
    stage_two(dataset);
	
	/* stage 3: compute more mapping functions */ 
    int count = stage_three(dataset, array_a, array_b, array_n, &max_err);
	
	/* stage 4: perform exact-match queries */
    stage_four(dataset, array_a, array_b, array_n, count, max_err);
	
	/* all done; take some rest */
    return 0;
}

/****************************************************************/

/* add your code below; you can also modify the function return type 
   and parameter list 
*/

/* stage 1: read and sort the input */
void
stage_one(data_t dataset[]) {

	// Get the input numbers.
    int readnum = 0;
    while(readnum < 100){
        int num;
        scanf("%d", &num);
        dataset[readnum] = num;
        readnum += 1;
    }

	// Sort the inputs.
    quick_sort(dataset, readnum);
	// Print stage header.
    print_stage_header(STAGE_NUM_ONE);
    // Print the result.
    printf("First 10 numbers:");
    int i;
    for(i = 0; i < DATA_OUTPUT_SIZE; i++){
        printf(" %d", dataset[i]);
    }
    printf("\n\n");
}

/* stage 2: compute the first mapping function */
void
stage_two(data_t dataset[]) {
    
	// Define the values of a and b.
    int a = -dataset[0], b = dataset[1] - dataset[0];
    int i, pred, pred_error, max_error = -1, max_num;

	// Calculate the prediction error.
    for(i = 0; i < DATASET_SIZE; i++){
		// Consider the special condition.
        pred = ceil(1.0 * check_ab(dataset, 0, &a, &b));
        pred_error = abs(pred - i);
		// See if the prediction error is the maximum.
        if(pred_error > max_error){
            max_error = pred_error;
            max_num = i;
        }
    }

	// Print stage header.
    print_stage_header(STAGE_NUM_TWO);
	// Print the results.
    printf("Maximum prediction error: %d\n", max_error);
    printf("For key: %d\n", dataset[max_num]);
    printf("At position: %d\n", max_num);
    printf("\n");
}

/* stage 3: compute more mapping functions */ 
int
stage_three(data_t dataset[], data_t array_a[], data_t array_b[], 
        data_t array_n[], int* max_err) {

    int a, b;
    check_ab(dataset, 0, &a, &b);
	// Get the errm.
    scanf("%d", max_err);
	// Compare the prediction error with the maximum error.
    int i, pred, pred_error, num = 0;
    for(i = 2; i < DATASET_SIZE; i++){
        pred = ceil(check_ab(dataset[i], i, &a, &b));
        pred_error = abs(pred - i);
		// Update the numbers in arrays to save the datas we need.
        if(pred_error > *max_err){
			// See if there is just one element left.
            if(i == DATASET_SIZE - 2){
                array_a[num] = i + 1;
                array_b[num] = 0;
                array_n[num] = dataset[i + 1];
                num += 1;
                break;
            }
			// Add datas to arrays.
            array_a[num] = a;
            array_b[num] = b;
            array_n[num] = dataset[i - 1];
			// Consider the special condition.
            check_ab(dataset, i, &a, &b);
            num += 1;
        }
		// Add datas to arrays if there is just one elemnt left.
        if(i == DATASET_SIZE - 1){
            array_a[num] = a;
            array_b[num] = b;
            array_n[num] = dataset[i];
            num += 1;
        }
    }
	// Print stage header.
    print_stage_header(STAGE_NUM_THREE);
	// Print the results.
    printf("Target maximum prediction error: %d\n", *max_err);
    for(i = 0; i < num; i++){
        printf("Function %2d: a = %4d, b = %3d, max element = %3d\n", 
        i, array_a[i], array_b[i], array_n[i]);
    }
    printf("\n");
    return num;
}

/* stage 4: perform exact-match queries */
void
stage_four(data_t dataset[], data_t array_a[], data_t array_b[],
        data_t array_n[], int count, int max_err) {
    
	/* print stage header */
    print_stage_header(STAGE_NUM_FOUR);
    
    int key;
    while(scanf("%d", &key) != EOF){
        printf("Searching for %d:\n", key);
		// See if step 1 can be found.
        if(key < dataset[0] || key > dataset[DATASET_SIZE - 1]){
            printf("Step 1: not found!\n");
        } else{
            printf("Step 1: search key in data domain.\nStep 2:");
            int locn;
            binary_search(array_n, 0, count, &key, &locn);
            printf("\n");
            int a, b, lo, hi;
            a = array_a[locn];
            b = array_b[locn];
            double pred;
            pred = ceil(check_ab(key, 0, &a, &b));
			// Get the bigger one for hi.
            lo = max(0, (pred - max_err));
		    // Get the smaller one for lo.
            hi = min(DATASET_SIZE - 1, (pred + max_err))+1;
            printf("Step 3:");
            int result = binary_search(dataset, lo, hi, &key, &locn);
            if(result == 0){
                printf(" @ dataset[%d]!\n", locn);
            } else{
                printf(" not found!\n");
            }
        }
    }

    printf("\n");
}

/****************************************************************/
/* functions provided, adapt them as appropriate */

/* print stage header given stage number */
void
print_stage_header(int stage_num) {
    printf(STAGE_HEADER, stage_num);
}

/* data swap function used by quick sort, adpated from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/sortscaffold.c
*/
void
swap_data(data_t *x1, data_t *x2) {
    data_t t;
    t = *x1;
    *x1 = *x2;
    *x2 = t;
}

/* partition function used by quick sort, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/quicksort.c 
*/
void
partition(data_t dataset[], int n, data_t *pivot,
		int *first_eq, int *first_gt) {
    int next=0, fe=0, fg=n, outcome;

    while (next<fg) {
        if ((outcome = cmp(dataset+next, pivot)) < 0) {
            swap_data(dataset+fe, dataset+next);
            fe += 1;
            next += 1;
        } else if (outcome > 0) {
            fg -= 1;
            swap_data(dataset+next, dataset+fg);
        } else {
            next += 1;
        }
    }
	
    *first_eq = fe;
    *first_gt = fg;
    return;
}

/* quick sort function, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/quicksort.c 
*/
void
quick_sort(data_t dataset[], int n) {
    data_t pivot;
    int first_eq, first_gt;
    if (n<=1) {
        return;
    }
	/* array section is non-trivial */
    pivot = dataset[n/2]; // take the middle element as the pivot
    partition(dataset, n, &pivot, &first_eq, &first_gt);
    quick_sort(dataset, first_eq);
    quick_sort(dataset + first_gt, n - first_gt);
}

/* comparison function used by binary search and quick sort, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c 
*/
int
cmp(data_t *x1, data_t *x2) {
    return (*x1-*x2);
}

/* binary search between dataset[lo] and dataset[hi-1], adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c 
*/
int
binary_search(data_t dataset[], int lo, int hi, 
	    data_t *key, int *locn) {
    int mid, outcome;
	/* if key is in dataset, it is between dataset[lo] and dataset[hi-1] */
    if (lo>=hi) {
		// printf("lo>=hi occured!!!");
        *locn = lo;
        return BS_NOT_FOUND;
    }
    mid = (lo+hi)/2;
    printf(" %d", dataset[mid]);
    if ((outcome = cmp(key, dataset+mid)) < 0) {
        return binary_search(dataset, lo, mid, key, locn);
    } else if (outcome > 0) {
        return binary_search(dataset, mid+1, hi, key, locn);
    } else {
        *locn = mid;
        return BS_FOUND;
    }
}

// Choose the bigger number.
int
max(int num1, int num2){
    if(num1 > num2){
        return num1;
    } else{
        return num2;
    }
}
// Choose the smaller number.
int
min(int num1, int num2){
    if(num1 < num2){
        return num1;
    } else{
        return num2;
    }
}

// Check whethere b is 0, calculate the value of a and b.
float
check_ab(int dataset[], int i, int *a, int *b){
    if(dataset[i + 1] == dataset[i]){
        *a = i;
        *b = 0;
        return a;
    } else{
        *a = dataset[i + 1] * i - dataset[i] * (i + 1);
        *b = dataset[i + 1] - dataset[i];
        return (key + a) / (double)(b);
    }
}

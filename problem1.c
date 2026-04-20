/*
 * Activity Selection Problem — Greedy Method (C Implementation)
 * Selects the maximum number of mutually compatible (non-overlapping)
 * activities using the earliest-finish-time greedy strategy.
 *
 * Compile:  gcc -O2 -o problem1 problem1.c
 * Run:      ./problem1
 */

#include <stdio.h>
#include <stdlib.h>

// Data Structure
typedef struct {
    int start;
    int finish;
    int id;            /* original index (useful for reporting) */
} Activity;

// Comparator for qsort 
static int compare_by_finish(const void *a, const void *b) {
    const Activity *x = (const Activity *)a;
    const Activity *y = (const Activity *)b;
    if (x->finish != y->finish) return x->finish - y->finish;
    return x->start - y->start;        /* tiebreaker: earlier start first */
}

/*
 * Core Algorithm
 * selected[] is an output array (caller-allocated, size ≥ n)
 * Returns the number of activities selected.
 */
int activity_selection(Activity acts[], int n, Activity selected[]) {
    if (n <= 0) return 0;

    /* Step 1: Sort by finish time — O(n log n) */
    qsort(acts, n, sizeof(Activity), compare_by_finish);

    /* Step 2: Greedy linear scan — O(n) */
    int count = 0;
    selected[count++] = acts[0];
    int last_finish = acts[0].finish;

    for (int i = 1; i < n; i++) {
        if (acts[i].start >= last_finish) {    /* compatible? */
            selected[count++] = acts[i];
            last_finish = acts[i].finish;
        }
    }
    return count;
}

static void print_activities(const char *label, Activity a[], int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) {
        printf("(%d,%d)%s", a[i].start, a[i].finish, (i == n - 1) ? "" : ", ");
    }
    printf("]\n");
}

static void run_single_test(Activity input[], int n) {
    if (n <= 0) {
        printf("Input: []\nSelected: []\nCount: 0\n");
        return;
    }

    Activity *work = (Activity *)malloc(n * sizeof(Activity));
    Activity *selected = (Activity *)malloc(n * sizeof(Activity));
    if (!work || !selected) {
        printf("Memory allocation failed.\n");
        free(work);
        free(selected);
        return;
    }

    for (int i = 0; i < n; i++) work[i] = input[i];

    int count = activity_selection(work, n, selected);
    print_activities("Input", input, n);
    print_activities("Selected", selected, count);
    printf("Count: %d\n", count);

    free(work);
    free(selected);
}

// MAIN
int main(void) {
    Activity test[] = {
        {1,4,0},{3,5,1},{0,6,2},{5,7,3},{3,9,4},{5,9,5},
        {6,10,6},{8,11,7},{8,12,8},{2,14,9},{12,16,10}
    };
    int n = (int)(sizeof(test) / sizeof(test[0]));

    run_single_test(test, n);
    return 0;
}
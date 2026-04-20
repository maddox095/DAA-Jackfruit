/*
 * Edit Distance (Levenshtein Distance) - Dynamic Programming
 * ==========================================================
 * Allowed operations: insert, delete, replace
 * This program computes:
 * 1. Minimum edit distance
 * 2. Full DP table
 * 3. Sequence of operations
 *
 * Compile: gcc -O2 -o problem3 problem3.c
 * Run:     ./problem3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int min3(int a, int b, int c) {
    int m = a < b ? a : b;
    return m < c ? m : c;
}

static int **allocate_table(int rows, int cols) {
    int **table = (int **)malloc(rows * sizeof(int *));
    if (!table) return NULL;

    for (int i = 0; i < rows; i++) {
        table[i] = (int *)malloc(cols * sizeof(int));
        if (!table[i]) {
            for (int k = 0; k < i; k++) free(table[k]);
            free(table);
            return NULL;
        }
    }
    return table;
}

static void free_table(int **table, int rows) {
    if (!table) return;
    for (int i = 0; i < rows; i++) free(table[i]);
    free(table);
}

static void print_dp_table(int **dp, const char *x, const char *y) {
    int m = (int)strlen(x);
    int n = (int)strlen(y);

    printf("\nDP Table\n");
    printf("%4s", "");
    printf("%4s", "#");
    for (int j = 0; j < n; j++) {
        printf("%4c", y[j]);
    }
    printf("\n");

    for (int i = 0; i <= m; i++) {
        if (i == 0) printf("%4s", "#");
        else printf("%4c", x[i - 1]);

        for (int j = 0; j <= n; j++) {
            printf("%4d", dp[i][j]);
        }
        printf("\n");
    }
}

static void trace_operations(const char *x, const char *y, int **dp, int i, int j,
                             char ops[][160], int *count) {
    if (i == 0 && j == 0) return;

    if (i > 0 && j > 0 && x[i - 1] == y[j - 1]) {
        trace_operations(x, y, dp, i - 1, j - 1, ops, count);
        return;
    }

    if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + 1) {
        trace_operations(x, y, dp, i - 1, j - 1, ops, count);
        snprintf(ops[*count], 160,
                 "Replace '%c' with '%c' at position %d", x[i - 1], y[j - 1], i);
        (*count)++;
        return;
    }

    if (i > 0 && dp[i][j] == dp[i - 1][j] + 1) {
        trace_operations(x, y, dp, i - 1, j, ops, count);
        snprintf(ops[*count], 160,
                 "Delete '%c' at position %d", x[i - 1], i);
        (*count)++;
        return;
    }

    if (j > 0 && dp[i][j] == dp[i][j - 1] + 1) {
        trace_operations(x, y, dp, i, j - 1, ops, count);
        snprintf(ops[*count], 160,
                 "Insert '%c' before position %d", y[j - 1], i + 1);
        (*count)++;
        return;
    }
}

static void run_single_test(const char *x, const char *y) {
    int m = (int)strlen(x);
    int n = (int)strlen(y);

    int **dp = allocate_table(m + 1, n + 1);
    if (!dp) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (x[i - 1] == y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min3(dp[i - 1][j],     /* delete */
                                   dp[i][j - 1],     /* insert */
                                   dp[i - 1][j - 1]  /* replace */);
            }
        }
    }

    printf("X = \"%s\"\n", x);
    printf("Y = \"%s\"\n", y);
    printf("Minimum edit distance = %d\n", dp[m][n]);

    print_dp_table(dp, x, y);

    char ops[512][160];
    int op_count = 0;
    trace_operations(x, y, dp, m, n, ops, &op_count);

    printf("\nOperations\n");
    if (op_count == 0) {
        printf("Strings are already equal. No operations needed.\n");
    } else {
        for (int i = 0; i < op_count; i++) {
            printf("%d. %s\n", i + 1, ops[i]);
        }
    }

    free_table(dp, m + 1);
}

int main(void) {
    /*
     * Replace these two strings with any testcase you want.
     * The program will build the DP table and print the answer.
     */
    const char *X = "sunday";
    const char *Y = "saturday";

    run_single_test(X, Y);
    return 0;
}

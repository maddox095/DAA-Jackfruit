/*
 * Closest Pair of Points — Divide and Conquer  (C)
 * ==================================================
 * Recurrence : T(n) = 2T(n/2) + O(n log n)  =>  O(n log^2 n)
 * Brute force: O(n^2)
 *
 * NOTE: The true O(n log n) version pre-sorts by y and propagates
 *       that order through recursion. This implementation re-sorts
 *       only the strip at each level, giving O(n log^2 n) — still
 *       far superior to brute force and simpler to reason about.
 *
 * Compile:  gcc -O2 -lm -o problem2 problem2.c
 * Run:      ./problem2
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>


typedef struct {
    double x, y;
} Point;


static double dist(Point a, Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}


static int cmp_x(const void *a, const void *b) {
    double d = ((const Point *)a)->x - ((const Point *)b)->x;
    if (d < 0) return -1;
    if (d > 0) return  1;
    d = ((const Point *)a)->y - ((const Point *)b)->y;
    return (d < 0) ? -1 : (d > 0) ? 1 : 0;
}

static int cmp_y(const void *a, const void *b) {
    double d = ((const Point *)a)->y - ((const Point *)b)->y;
    return (d < 0) ? -1 : (d > 0) ? 1 : 0;
}

static double dmin(double a, double b) { return a < b ? a : b; }


double brute_force(Point pts[], int n) {
    double min_d = DBL_MAX;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            double d = dist(pts[i], pts[j]);
            if (d < min_d) min_d = d;
        }
    return min_d;
}

/* ────────────────────────────────────────────────────
 * Divide-and-Conquer  O(n log^2 n)
 *
 * Px : points sorted by x  (sub-array of size n)
 * n  : number of points
 *
 * Steps:
 *   1. Base case: n ≤ 3 → brute force
 *   2. Divide at median x into left (Px[0..mid-1])
 *      and right (Px[mid..n-1])
 *   3. Recurse on both halves
 *   4. Build strip of points within delta of the
 *      dividing line, sort strip by y, and scan
 *      each point against its ≤ 7 y-neighbors
 * ──────────────────────────────────────────────────── */
static double closest_rec(Point Px[], int n)
{
    /* ── Base case ── */
    if (n <= 3)
        return brute_force(Px, n);

    /* ── Divide ── */
    int mid = n / 2;
    double midX = Px[mid].x;

    /* ── Conquer ── */
    double dL = closest_rec(Px,       mid);
    double dR = closest_rec(Px + mid, n - mid);
    double delta = dmin(dL, dR);

    /* ── Combine: build strip ── */
    Point *strip = (Point *)malloc(n * sizeof(Point));
    int s = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(Px[i].x - midX) < delta)
            strip[s++] = Px[i];
    }

    /* Sort strip by y: O(s log s), s ≤ n  */
    qsort(strip, s, sizeof(Point), cmp_y);

    /*
     * For each point in strip, compare to subsequent
     * points whose y-distance < delta.
     * The packing argument guarantees at most 7
     * such neighbours (≤ 8 points in delta × 2*delta box),
     * so this loop is O(s) in total.
     */
    for (int i = 0; i < s; i++) {
        for (int j = i + 1; j < s && (strip[j].y - strip[i].y) < delta; j++) {
            double d = dist(strip[i], strip[j]);
            if (d < delta)
                delta = d;
        }
    }

    free(strip);
    return delta;
}

double closest_pair_dc(Point pts[], int n) {
    if (n <= 1) return DBL_MAX;
    if (n == 2) return dist(pts[0], pts[1]);

    /* Work on a copy (qsort is in-place) */
    Point *Px = (Point *)malloc(n * sizeof(Point));
    memcpy(Px, pts, n * sizeof(Point));

    /* Sort by x once: O(n log n) */
    qsort(Px, n, sizeof(Point), cmp_x);

    double result = closest_rec(Px, n);
    free(Px);
    return result;
}

static void print_points(const char *label, Point pts[], int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) {
        printf("(%.0f,%.0f)%s", pts[i].x, pts[i].y, (i == n - 1) ? "" : ", ");
    }
    printf("]\n");
}

static void run_single_test(Point input[], int n) {
    if (n <= 0) {
        printf("Input: []\nClosest distance: undefined\n");
        return;
    }

    Point *work = (Point *)malloc(n * sizeof(Point));
    if (!work) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < n; i++) work[i] = input[i];

    double answer = closest_pair_dc(work, n);
    print_points("Input", input, n);
    printf("Closest distance: %.6f\n", answer);

    free(work);
}

int main(void) {
    Point test[] = {
	{2,3}, {12,30}, {40,50}, {5,1}, {12,10}, {3,4}
    };
    int n = (int)(sizeof(test) / sizeof(test[0]));

    run_single_test(test, n);
    return 0;
}
Main Goal:

(!)[]

Select the maximum number of mutually compatible (non-overlapping) activities.

Each activity has:
- start time
- finish time
- id (for reference)

Two activities are compatible if:

next.start >= current.finish


How The Algorithm Works (Greedy - Earliest Finish Time)

1. Sort all activities by finish time (ascending).
2. Pick the first activity in this sorted list.
3. Scan left to right:
- if activity.start >= last_selected.finish, select it
- otherwise skip it
4. Continue until all activities are processed.

Why this choice works:
- Choosing the earliest finishing activity leaves maximum room for future activities.
- This local choice leads to a globally optimal result for maximum count.


Step-by-Step Example

Input:

(1,4), (3,5), (0,6), (5,7), (8,11), (12,16)

Sorted by finish:

(1,4), (3,5), (0,6), (5,7), (8,11), (12,16)

Selection process:
1. pick (1,4)
2. skip (3,5) because 3 < 4
3. skip (0,6) because 0 < 4
4. pick (5,7) because 5 >= 4
5. pick (8,11) because 8 >= 7
6. pick (12,16) because 12 >= 11

Final selected:

(1,4), (5,7), (8,11), (12,16)

Count = 4


Algorithm Time Complexity

Sorting: O(n log n)
Linear Scan: O(n)
Total: O(n log n)

Space:
- Additional selection array: O(n)
- Sorting by qsort is in-place on the input array, but may use internal stack space.


Correctness Intuition (Why Greedy Is Optimal Here)

For maximum number of activities, earliest-finish-time greedy is provably optimal.

Intuition:
- Suppose an optimal solution starts with some activity A.
- If greedy picks G with finish(G) <= finish(A), replacing A with G cannot reduce feasibility of later choices.
- So there exists an optimal solution that begins with greedy's first pick.
- Repeating the same argument for the remaining interval proves optimality.


Where This Greedy Strategy Fails

Important: it does not fail for this objective:

maximize number of non-overlapping activities.

It can fail if objective changes, for example:
- maximize total duration
- maximize total profit/weight (weighted interval scheduling)

Counterexample for maximize total duration (not count):

Activities: (1,2), (2,3), (1,10)

- Greedy by earliest finish picks (1,2) and (2,3):
  count = 2, total duration = 2
- Better duration solution picks (1,10):
  count = 1, total duration = 9

So greedy is correct for count, not for duration/profit objectives.


Copy-Paste Test Cases For problem1.c

Use with current main in problem1.c:
1. Replace only the Activity test[] block with one case below.
2. Keep this line unchanged:

```c
int n = (int)(sizeof(test) / sizeof(test[0]));
```

3. Compile and run.

```c
/* 1) Classic textbook example (expected count: 4) */
Activity test[] = {
    {1,4,0},{3,5,1},{0,6,2},{5,7,3},{3,9,4},{5,9,5},
    {6,10,6},{8,11,7},{8,12,8},{2,14,9},{12,16,10}
};

/* 2) No overlaps - all selected (expected count: 4) */
Activity test[] = {
    {0,1,0},{2,3,1},{4,5,2},{6,7,3}
};

/* 3) All overlap - only one selected (expected count: 1) */
Activity test[] = {
    {0,10,0},{1,10,1},{2,10,2},{3,10,3}
};

/* 4) Single activity (expected count: 1) */
Activity test[] = {
    {5,10,0}
};

/* 5) Touching intervals (compatible) (expected count: 4) */
Activity test[] = {
    {0,2,0},{2,4,1},{4,6,2},{6,8,3}
};

/* 6) One short blocks many long (expected count: 1) */
Activity test[] = {
    {0,1,0},{0,100,1},{0,200,2},{0,300,3}
};

/* 7) Nested - greedy picks shortest (expected count: 1) */
Activity test[] = {
    {1,2,0},{0,5,1},{0,10,2}
};

/* 8) Large gap then dense cluster (expected count: 3) */
Activity test[] = {
    {0,1,0},{100,101,1},{100,102,2},{100,103,3},{101,104,4}
};

/* 9) Objective-change demo (duration objective, not count):
   Greedy count = 2, but max total duration uses (1,10). */
Activity test[] = {
    {1,2,0},{2,3,1},{1,10,2}
};
```

Empty input case:

```c
run_single_test(NULL, 0);
```

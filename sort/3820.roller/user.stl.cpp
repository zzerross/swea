#include <cstdint>
#include <algorithm>

#define RAILS 200000
#define MOD(x) ((x) % 1000000007)

int* A;
int* B;
int P[RAILS];

bool slower(int i, int j) {
    return (int64_t) B[i] * (int64_t) (A[j] - 1) < (int64_t) B[j] * (int64_t) (A[i] - 1);
}

int MinRailSpeed(int N, int *a, int *b) {
    A = a, B = b;

    for (int i = 0; i < N; i++)
        P[i] = i;

    std::sort(P, P + N, slower);

    extern int CalcFinalSpeed(int N, int *a, int *b, int *p);
    return CalcFinalSpeed(N, a, b, P);
}

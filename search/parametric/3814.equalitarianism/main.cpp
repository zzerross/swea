#include <stdio.h>

#if 1 <= DEBUG
#define D1(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define D1(fmt, ...)
#endif

#define H 1000000000
#define S 100000

int T, N, K, A[S], B[S];

int abs(int i) {
    return i < 0 ? -i : i;
}

bool possible(int k, int limit) {
    for (int i = 0; i < N; i++)
        B[i] = A[i];

    for (int i = 1; i < N; i++) {
        int d = abs(B[i-1] - B[i]);
        int v = d - limit;
        if (0 < v) {
            if ((k -= v) < 0)
                return false;

            if (B[i-1] < B[i])
                B[i] -= v;
            else
                B[i-1] -= v;
        }
    }

    for (int i = N - 1; 0 < i; i--) {
        int d = abs(B[i-1] - B[i]);
        int v = d - limit;
        if (0 < v) {
            if ((k -= v) < 0)
                return false;

            if (B[i-1] < B[i])
                B[i] -= v;
            else
                B[i-1] -= v;
        }
    }

    return true;
}

int main() {
    scanf("%d", &T);

    for (int t = 1; t <= T; t++) {
        scanf("%d %d", &N, &K);

        for (int i = 0; i < N; i++)
            scanf("%d", &A[i]);

        int low = 1, high = H;

        while (low < high) {
            int middle = low + (high - low) / 2;

            if (possible(K, middle)) {
                high = middle;
            } else {
                low = middle + 1;
            }
        }

        printf("#%d %d\n", t, high);
    }

    return 0;
}

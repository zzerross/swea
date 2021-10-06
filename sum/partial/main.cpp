#include <stdio.h>

int main() {
    int T, N, A[200001] = { 0, };

    scanf("%d", &T);

    for (int m, i, t = 1; t <= T; t++) {
        scanf("%d", &N);

        for (m = 0, i = 1; i <= N; i++) {
            scanf("%d", &A[i]);

            if (A[i] < A[i-1] + A[i])
                A[i] += A[i-1];

            if (m < A[i])
                m = A[i];
        }

        printf("#%d %d\n", t, m);
    }

    return 0;
}

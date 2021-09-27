#include <cstdio>
#include <cstdint>

#define POLES 20

int T, N, L, R, I[POLES+1], P[POLES+1];
int64_t DP[POLES+1][POLES+1][POLES+1] = { 0, };

int main() {
    DP[1][1][1] = 1;

    for (int n = 2; n <= POLES; n++) {
        for (int l = 1; l <= POLES; l++) {
            for (int r = 1; r <= POLES; r++) {
                DP[n][l][r] += DP[n-1][l-1][r];
                DP[n][l][r] += DP[n-1][l][r-1];
                DP[n][l][r] += DP[n-1][l][r] * (n - 2);
            }
        }
    }

    scanf("%d", &T);

    for (int t = 1; t <= T; t++) {
        scanf("%d %d %d", &N, &L, &R);

        printf("#%d %ld\n", t, DP[N][L][R]);
    }

    return 0;
}

#include <cstdio>
#include <cstdint>

#if 1 <= DEBUG
#define D1(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define D1(fmt, ...)
#endif

#define POLES 20

int T, N, L, R, I[POLES+1], P[POLES+1];
int64_t DP[POLES+1][POLES+1][POLES+1] = { 0, };

int left(int s = POLES) {
    int c = 1;

    for (int i = 2; i <= s; i++)
        if (P[i-1] < P[i])
            c++;

    return c;
}

int right(int s = POLES) {
    int c = 1;

    for (int i = s - 1; 1 <= 1; i--)
        if (P[i] > P[i+1])
            c++;

    return c;
}

struct Count {
    int64_t l, r;
};

Count perm(int n, int f = 0, int s = POLES) {
    if (n < 1)
        return { left(), right() };

    for (int i = 1; i <= s; i++) {
        if (f & (1 << i))
            continue;

        P[n] = i;

        Count c = perm(n - 1, f | (1 << i));

        DP[n][c.l][c.r] += 1;
    }

    return { 0, 0 };
}

void dump(int s = POLES) {
#if 1 <= DEBUG
    for (int n = 1; n <= s; n++) {
        D1("n=%d\n", n);

        for (int l = 1; l <= n; l++) {
            D1("%2d: ", l);

            for (int r = 1; r <= n; r++)
                D1("%5ld ", DP[n][l][r]);

            D1("\n");
        }

        D1("\n");
    }
#endif
}

int main() {
    setbuf(stdout, NULL);

    perm(POLES);
    dump();

    scanf("%d", &T);

    for (int t = 1; t <= T; t++) {
        scanf("%d %d %d", &N, &L, &R);

#if TEST
        if (t != TEST)
            continue;
#endif

        for (int i = 0; i <= N; i++)
            I[i] = i;

        printf("#%d %ld\n", t, DP[N][L][R]);
    }

    return 0;
}

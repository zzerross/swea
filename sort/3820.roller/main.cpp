#include <iostream>
#include <stdio.h>

using namespace std;

const int MAXN = 200000;

int CalcFinalSpeed(int N, int *a, int *b, int *p) {
    int v = 1;

    for (int i = 0; i < N; i++) {

        int CurRail = p[i];

        v = (int)(((long long)(a[CurRail])*v + b[CurRail]) % 1000000007);
    }

    return v;
}
extern int MinRailSpeed(int N, int *a, int *b);

int a[MAXN], b[MAXN];

int main(void) {
    setbuf(stdout, NULL);

    int tc = 1, TestCase;
    for (scanf("%d", &TestCase); tc <= TestCase; tc++) {

        int N; scanf("%d", &N);

        for (int i = 0; i < N; i++) {
            scanf("%d%d", &a[i], &b[i]);
        }

#if TEST
        if (tc != TEST)
            continue;
#endif

        int answer = MinRailSpeed(N, a, b);

        printf("#%d %d\n", tc, answer);
    }
    return 0;
}

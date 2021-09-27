#include <cstdint>
#include <algorithm>

#define RAILS 200000

#define MOD(x) ((x) % 1000000007)

int min(const int& i, const int& j) {
    return i < j ? i : j;
}

bool slow(int64_t a1, int64_t b1, int64_t a2, int64_t b2) {
    return b1 * (a2 - 1) < b2 * (a1 - 1);
}

void sort(int s, int* pa, int* pb, int* a) {
    int b[s];

    for (int n = 1; n < s; n *= 2) {
        for (int i, j, p = 0; p < s;) {
            int l = p;
            int m = min(p += n, s) - 1;
            int h = min(p += n, s) - 1;

            for (i = l; i <= h; i++)
                b[i] = a[i];

            for (i = l, j = m + 1; i <= m && j <= h;)
                a[l++] = slow(pa[b[i]], pb[b[i]], pa[b[j]], pb[b[j]]) ? b[i++] : b[j++];

            while (i <= m)
                a[l++] = b[i++];

            while (j <= h)
                a[l++] = b[j++];
        }
    }
}

int MinRailSpeed(int N, int *a, int *b) {
    static int p[RAILS];

    for (int i = 0; i < N; i++)
        p[i] = i;

    sort(N, a, b, p);

    extern int CalcFinalSpeed(int N, int *a, int *b, int *p);
    return CalcFinalSpeed(N, a, b, p);
}

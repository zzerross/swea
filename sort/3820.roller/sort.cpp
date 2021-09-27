#include <cstdio>
#include <cstdint>
#include <algorithm>

#if 1 <= DEBUG
#define D1(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define D1(fmt, ...)
#endif

using namespace std;

#define RAILS 200000

#define MOD(x) ((x) % 1000000007)
extern int CalcFinalSpeed(int N, int *a, int *b, int *p);

int MinRailSpeed(int N, int *a, int *b) {
        int answer = 0;
            return answer;
}

struct Rail {
    uint64_t a, b;

    bool operator<(const Rail& o) const {
        return b*(o.a - 1) < o.b*(a - 1);
    }
};

template <typename T, int S>
struct Array {
    T buf[S];
    int len;

    T& operator[](int i) {
        return buf[i];
    }

    T* begin() {
        return buf;
    }

    T* end() {
        return buf + len;
    }
};

int main() {
    int tests;
    Array<Rail, RAILS> rails;

    scanf("%d", &tests);

    for (int t = 1; t <= tests; t++) {
        scanf("%d", &rails.len);

        for (int i = 0; i < rails.len; i++)
            scanf("%lu %lu", &rails[i].a, &rails[i].b);

#if TEST
        if (t != TEST)
            continue;
#endif

        sort(rails.begin(), rails.end());

#if 1 <= DEBUG
        D1("%8s: \n", __func__);

        for (int i = 0; i < rails.len; i++)
            D1("%8s: %lu %lu\n", "", rails[i].a, rails[i].b);
#endif

        uint64_t velocity = 1;

        for (int i = 0; i < rails.len; i++)
            velocity = MOD(MOD(rails[i].a * velocity) + rails[i].b);

        printf("#%d %lu\n", t, velocity);
    }

    return 0;
}

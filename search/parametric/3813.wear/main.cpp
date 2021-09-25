#include <cstdio>

#define BLOCKS 200000
#define LEVEL  200000

template <typename T, int S>
struct Array {
    T buf[S];
    int len;

    T& operator[](const int i) {
        return buf[i];
    }
};

Array<int, BLOCKS> gLevels, gChunks;

bool possible(int level) {
    for (int l = 0, count = 0, chunk = 0; l < gLevels.len; l++) {
        count = gLevels[l] <= level ? count + 1 : 0;

        if (gChunks[chunk] <= count) {
            count = 0;

            if (gChunks.len <= ++chunk)
                return true;
        }
    }

    return false;
}

int main() {
    int tests;

    scanf("%d", &tests);

    for (int t = 1; t <= tests; t++) {
        scanf("%d %d", &gLevels.len, &gChunks.len);

        for (int i = 0; i < gLevels.len; i++)
            scanf("%d", &gLevels[i]);

        for (int i = 0; i < gChunks.len; i++)
            scanf("%d", &gChunks[i]);

#if TEST
        if (t != TEST)
            continue;
#endif

        int low = 0, high = LEVEL;

        while (low < high) {
            int middle = low + (high - low) / 2;

            if (possible(middle))
                high = middle;
            else
                low = middle + 1;
        }

        printf("#%d %d\n", t, high);
    }

    return 0;
}

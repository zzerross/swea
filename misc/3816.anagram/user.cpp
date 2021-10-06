#define ALPHA ('z' - 'a' + 1)

int c1[ALPHA] = { 0, };
int c2[ALPHA] = { 0, };

static int* Count(int* c, char* s, int n) {
    for (register int i = 0; i < ALPHA; i++)
        c[i] = 0;

    for (int i = 0; i < n; i++)
        c[s[i] - 'a']++;

    return c;
}

static bool IsSame(int* a, int* b) {
    for (register int i = 0; i < ALPHA; i++)
        if (a[i] != b[i])
            return false;

    return true;
}

int FindAnagram(int l1, char* s1, int l2, char* s2) {
    Count(c1, s1, l1);

    int r = 0;

    for (register int i = 0; i <= l2 - l1; i++)
        r += IsSame(c1, Count(c2, &s2[i], l1));

    return r;
}

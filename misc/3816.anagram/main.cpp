#include <iostream>
#include <stdio.h>

using namespace std;

const int MAXN = 100000;

extern int FindAnagram(int Len1, char *S1, int Len2, char *S2);

char S1[MAXN + 5];
char S2[MAXN + 5];

int main(void) {
    setbuf(stdout, NULL);

    int TestCase; for (scanf("%d", &TestCase); TestCase--;) {

        scanf("%s", S1);
        scanf("%s", S2);

        //L1 = Length of S1, L2 = Length of S2
        int L1 = 0, L2 = 0;
        for (; S1[L1]; L1++);
        for (; S2[L2]; L2++);

        int answer = FindAnagram(L1, S1, L2, S2);

        static int tc = 0;
        printf("#%d %d\n", ++tc, answer);
    }
    return 0;
}

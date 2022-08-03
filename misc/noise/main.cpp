#include <memory.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

#define MAXC 5000

using namespace std;

extern void encode(char QRC[100][100], char SRC[100]);
extern void decode(char GRY[100][100], char DST[100]);

static char QRC[MAXC][100][100];
static char IMG[MAXC][200][200];
static char GRY[MAXC][100][100];
static char SRC[MAXC][100];
static char DST[MAXC][100];

static int seed = 113;

static int pseudo_rand() {
  seed = (214013 * seed + 2531011);
  return (seed >> 16) & 0x7FFF;
}

void img2gray(int i) {
  for (register int y = 0; y < 100; y++)
    for (register int x = 0; x < 100; x++) {
      GRY[i][y][x] = 0;
      GRY[i][y][x] += IMG[i][y * 2 + 0][x * 2 + 0];
      GRY[i][y][x] += IMG[i][y * 2 + 0][x * 2 + 1];
      GRY[i][y][x] += IMG[i][y * 2 + 1][x * 2 + 0];
      GRY[i][y][x] += IMG[i][y * 2 + 1][x * 2 + 1];
    }
}

void makeSource() {
  for (register int c = 0; c < MAXC; c++)
    for (register int l = 0; l < 100; l++)
      SRC[c][l] = 'A' + (pseudo_rand() % 26);
}

void addNoise() {
  for (register int c = 0; c < MAXC; c++)
    for (register int y = 0; y < 200; y++)
      for (register int x = 0; x < 200; x++)
        IMG[c][y][x] = pseudo_rand() % 2 == 0 ? 0 : 1;

  for (register int c = 0; c < MAXC; c++) {
    register int offsetx = pseudo_rand() % 101;
    register int offsety = pseudo_rand() % 101;

    for (register int y = 0; y < 100; y++)
      for (register int x = 0; x < 100; x++)
        IMG[c][offsety + y][offsetx + x] = QRC[c][y][x] == 0 ? 0 : 1;
  }

  for (register int c = 0; c < MAXC; c++) {
    for (register int l = 0; l < 13000; l++)
      IMG[c][pseudo_rand() % 200][pseudo_rand() % 200] = 1;

    img2gray(c);
  }
}

int main(void) {
  makeSource();

  char TMP[100];
  for (register int c = 0; c < MAXC; c++) {
    memcpy(TMP, SRC[c], sizeof(SRC[0]));
    encode(QRC[c], TMP);
  }

  addNoise();

  clock_t TIME = clock();

  for (register int c = 0; c < MAXC; c++) decode(GRY[c], DST[c]);

  TIME = (clock() - TIME) / (CLOCKS_PER_SEC / 1000);

  int FAIL = 0;

  for (register int c = 0; c < MAXC; c++) {
    FAIL += memcmp(SRC[c], DST[c], 100) == 0 ? 0 : 1;
    //    if (memcmp(SRC[c], DST[c], 100)) {
    //      for (int i = 0; i < 100; i++) printf("%c", SRC[c][i]); printf("\n");
    //      for (int i = 0; i < 100; i++) printf("%c", DST[c][i]); printf("\n");
    //    }
  }

  if ((FAIL * 10000 + TIME) < 20000)
    cout << "PASS" << endl;
  else
    cout << "FAIL" << endl;

  return 0;
}

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>

#include <ctime>
#include <iostream>
#include <memory>

using namespace std;

#if 1 <= kDEBUG
#define D1(fmt, ...) \
  printf("%10s:%03d" fmt "\n", __func__, __LINE__, ##__VA_ARGS__)
#define IN1(fmt, ...) D1("> " fmt, ##__VA_ARGS__);
#define OUT1(fmt, ...) D1("< " fmt, ##__VA_ARGS__);
#endif

#ifndef D1
#define D1(fmt, ...)
#define IN1(fmt, ...)
#define OUT1(fmt, ...)
#endif

int seed = 18043000;
int pseudo_rand(void) {
  seed = seed * 214013 + 2531011;
  return (seed >> 16) & 0x7FFF;
}

template <typename T, int S>
struct Array {
  T buf[S];
  constexpr static int size = S;

  T& operator[](int i) { return buf[i]; }
};

class Cells : public Array<Array<int, CELLS>, CELLS> {};

extern void do_alloc(int h2b[kHOMES], int by[kBASES], int bx[kBASES],
                     int hy[kHOMES], int hx[kHOMES]);

struct Cell {
  int y, x;

  Cell() { Init(); }

  void Init() {
    y = Randomize();
    x = Randomize();
  }

  static int Randomize() { return pseudo_rand() % CELLS; }
};

class Test {
 public:
  Test() : cells_{} {
    IN1();

    for (int b = 0; b < kBASES; b++) {
      Cell c;
      while (cells_[c.y][c.x]) c.Init();

      cells_[c.y][c.x] = b + 1;
      by_[b] = c.y;
      bx_[b] = c.x;
    }

    for (int h = 0; h < kHOMES; h++) {
      Cell c;
      while (cells_[c.y][c.x]) c.Init();

      cells_[c.y][c.x] = -1;
      hy_[h] = c.y;
      hx_[h] = c.x;

      h2b_[h] = h % kBASES;
    }

    do_alloc(h2b_, by_, bx_, hy_, hx_);

    OUT1();
  }

  long long Loss() {
    long long loss = 0;
    int allocs[kBASES] = {
        0,
    };

    for (int h = 0; h < kHOMES; h++) {
      if (cells_[hy_[h]][hx_[h]] != -1) {
        loss += kLOSS;
        continue;
      }
      cells_[hy_[h]][hx_[h]] = 0;

      int b = h2b_[h];
      if (b < 0) {
        loss += kLOSS;
      } else if (kBASES <= b) {
        loss += kLOSS;
      } else if (kALLOCS <= allocs[b]) {
        loss += kLOSS;
      } else {
        loss += abs(by_[b] - hy_[h]) + abs(bx_[b] - hx_[h]);
        allocs[b]++;
      }
    }

    for (int b = 0; b < kBASES; b++) {
      if (cells_[by_[b]][bx_[b]] != b + 1) return kLOSS * kHOMES;

      cells_[by_[b]][bx_[b]] = 0;
    }

    return loss;
  }

 private:
  int by_[kBASES];
  int bx_[kBASES];

  int hy_[kHOMES];
  int hx_[kHOMES];

  int h2b_[kHOMES];

  Cells cells_;
};

int main(void) {
  long long loss = 0;

  for (int tc = 1; tc--; loss += make_shared<Test>()->Loss())
    ;

  cout << (loss <= 2490000000 ? "PASS" : "LOSS") << ": " << loss << endl;

  return 0;
}

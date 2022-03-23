#define _CRT_SECURE_NO_WARNINGS

#include <memory>

using namespace std;

int Rand(void) {
  static int seed = 18043000;

  seed = seed * 214013 + 2531011;

  return (seed >> 16) & 0x7FFF;
}

template <typename T, int S>
struct Array {
  T buf[S];
  constexpr static int size = S;

  T& operator[](int i) { return buf[i]; }
};

class Cells : public Array<Array<int, kCells>, kCells> {};

extern void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases],
                     int hy[kHomes], int hx[kHomes]);

struct Cell {
  int y, x;

  Cell() { Init(); }

  void Init() {
    y = Rand();
    x = Rand();
  }

  static int Rand() { return ::Rand() % kCells; }
};

class Test {
 public:
  Test() : cells_{} {
    IN1();

    InitBases();
    InitHomes();
    Dump();

    do_alloc(h2b_, by_, bx_, hy_, hx_);

    OUT1();
  }

  long long Loss() {
    long long loss = 0;
    int allocs[kBases]{};

    for (int h = 0; h < kHomes; h++) {
      if (cells_[hy_[h]][hx_[h]] != -1) {
        loss += kLoss;
        continue;
      }
      cells_[hy_[h]][hx_[h]] = 0;

      int b = h2b_[h];
      if (b < 0) {
        loss += kLoss;
      } else if (kBases <= b) {
        loss += kLoss;
      } else if (kAllocs <= allocs[b]) {
        loss += kLoss;
      } else {
        loss += abs(by_[b] - hy_[h]) + abs(bx_[b] - hx_[h]);
        allocs[b]++;
      }
    }

    for (int b = 0; b < kBases; b++) {
      if (cells_[by_[b]][bx_[b]] != b + 1) {
        return kLoss * kHomes;
      }

      cells_[by_[b]][bx_[b]] = 0;
    }

    return loss;
  }

 private:
  void Dump() {
    for (int y = 0; y < kCells; y++) {
      D1();
      for (int x = 0; x < kCells; x++) {
        if (cells_[y][x]) {
          P1("%2d ", cells_[y][x]);
        } else {
          P1(".. ");
        }
      }
      P1("\n");
    }
  }

  void InitBases() {
    IN1();

    for (int b = 0; b < kBases; b++) {
      Cell c;
      while (cells_[c.y][c.x]) {
        c.Init();
      }

      cells_[c.y][c.x] = b + 1;
      by_[b] = c.y;
      bx_[b] = c.x;
    }

    OUT1();
  }

  void InitHomes() {
    IN1();

    for (int h = 0; h < kHomes; h++) {
      Cell c;
      while (cells_[c.y][c.x]) {
        c.Init();
      }

      cells_[c.y][c.x] = -1;
      hy_[h] = c.y;
      hx_[h] = c.x;

      h2b_[h] = h % kBases;
    }

    OUT1();
  }

  int by_[kBases];
  int bx_[kBases];

  int hy_[kHomes];
  int hx_[kHomes];

  int h2b_[kHomes];

  Cells cells_;
};

int main(void) {
  setbuf(stdout, NULL);

  long long loss = 0;

  for (int tc = 1; tc--;) {
    loss += make_shared<Test>()->Loss();
  }

  cout << (loss <= 2490000000 ? "PASS" : "LOSS") << ": " << loss << endl;

  return 0;
}

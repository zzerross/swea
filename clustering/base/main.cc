#define _CRT_SECURE_NO_WARNINGS

#include <memory>

#if DEBUG
#else
#define P1(fmt, ...)
#define D1(fmt, ...)
#define IN1(fmt, ...)
#define OUT1(fmt, ...)
#endif

#if INPUT
#else
constexpr static int kCells = 3000;
constexpr static int kBases = 4;
constexpr static int kHomes = 110000;
constexpr static int kAllocs = 30000;
constexpr static int kLoss = 6000LL;
#endif

constexpr static long long kPass = 2490000000LL;

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
      if (!IsBase(b)) {
        loss += kLoss;
      } else if (kAllocs <= allocs[b]) {
        loss += kLoss;
      } else {
        loss += Distance(h, b);
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
  bool IsBase(int b) { return 0 <= b && b <= kBases; }

  long long Distance(int h, int b) {
    return abs(by_[b] - hy_[h]) + abs(bx_[b] - hx_[h]);
  }

  void Dump() {
#if 2 <= DEBUG
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
#endif
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

  printf("MAX : %10lld\n", kPass);
  printf("INIT: %10lld\n", 199720088LL);
  printf("%4s: %10lld\n", loss <= kPass ? "PASS" : "LOSS", loss);

  return 0;
}

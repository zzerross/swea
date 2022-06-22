#define _CRT_SECURE_NO_WARNINGS

#if PRINT
#else
#define A(expr, ...) (expr)
#define PR(lvl, fmt, ...)
#define PL(lvl, fmt, ...)
#define PI(fmt, ...)
#define PO(fmt, ...)
#endif

#if INPUT
#else
constexpr static int kCells = 3000;
constexpr static int kQouter = 750;
constexpr static int kHalf = 1500;
constexpr static int kThreeQ = 2250;
constexpr static int kBases = 4;
constexpr static int kHomes = 110000;
//                             9000000;
//                             2250000;
constexpr static int kAllocs = 30000;
constexpr static int kLoss = 6000LL;
#endif

constexpr static int kRate = INPUT ? 1 : 150;

constexpr static int kTests = 20;
constexpr static long long kPass = 2490000000LL / 20 * kTests;

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

struct Cell {
  int y, x;

  Cell() { Init(); }

  void Init() {
    y = Rand();
    x = Rand();
  }

  static int Rand() { return ::Rand() % kCells; }
};

int by_[kBases];
int bx_[kBases];
int hy_[kHomes];
int hx_[kHomes];
int h2b_[kHomes];
Cells cells_;
Cells map_;

static bool IsBase(int b) { return 0 <= b && b < kBases; }

static inline long long Distance(int h, int b) {
  return abs(by_[b] - hy_[h]) + abs(bx_[b] - hx_[h]);
}

static long long Loss() {
  long long loss = 0;
  int allocs[kBases]{};

  for (int h = 0; h < kHomes; h++) {
    if (A(cells_[hy_[h]][hx_[h]] != -1)) {
      loss += kLoss;
      continue;
    }
    cells_[hy_[h]][hx_[h]] = 0;

    int b = h2b_[h];
    if (A(!IsBase(b))) {
      loss += kLoss;
    } else if (A(kAllocs <= allocs[b])) {
      loss += kLoss;
    } else {
      auto d = Distance(h, b);
      loss += d;
      allocs[b]++;
      if ((kCells / 2) < d) {
        PL(3, "allocs[%d]=%5d d=%10lld loss=%10lld", b, allocs[b], d, loss);
      }
    }
  }

  for (int b = 0; b < kBases; b++) {
    if (A(cells_[by_[b]][bx_[b]] != b + 1)) {
      return kLoss * kHomes;
    }

    cells_[by_[b]][bx_[b]] = 0;
  }

  return loss;
}

void Set(int y, int x, int b) { map_[y][x] = b; }

static int Get(int y, int x) {
  int c[kBases]{};

  for (int i = 0; i < kRate; i++) {
    for (int j = 0; j < kRate; j++) {
      int m = map_[y + i][x + j];
      if (0 < m) {
        return m;
      } else if (m < 0) {
        c[-m - 1]++;
      }
    }
  }

  int b = 0;
  for (int i = 1; i < kBases; i++) {
    if (c[b] < c[i]) {
      b = i;
    }
  }

  return -(b + 1);
}

void Dump(const char* s) {
#if 2 <= PRINT
  PR(2, "%10s:\n", s);
  for (int y = 0; y < kCells; y += kRate) {
    PR(2, "%10d: ", y);
    for (int x = 0; x < kCells; x += kRate) {
      if (0 < Get(y, x)) {
        PR(2, "+%d ", Get(y, x));
      } else if (Get(y, x) < 0) {
        PR(2, "%d ", Get(y, x));
      } else {
        PR(2, ".. ");
      }
    }
    PR(2, "\n");
  }
#endif
}

static void InitBases() {
  PI();

  for (int b = 0; b < kBases; b++) {
    Cell c;
    while (cells_[c.y][c.x]) {
      c.Init();
    }

    cells_[c.y][c.x] = b + 1;
    by_[b] = c.y;
    bx_[b] = c.x;

    Set(c.y, c.x, cells_[c.y][c.x]);
  }

  PO();
}

static void InitHomes() {
  PI();

  for (int h = 0; h < kHomes; h++) {
    Cell c;
    while (cells_[c.y][c.x]) {
      c.Init();
    }

    cells_[c.y][c.x] = -1;
    hy_[h] = c.y;
    hx_[h] = c.x;

    h2b_[h] = h % kBases;

    Set(c.y, c.x, -(h2b_[h] + 1));
  }

  PO();
}

static void Init() {
  PI();

#if 2 <= PRINT
  for (int y = 0; y < kCells; y++) {
    for (int x = 0; x < kCells; x++) {
      Set(y, x, 0);
    }
  }
#endif

  InitBases();
  InitHomes();
  Dump(__func__);

  PO();
}

int main(void) {
  setbuf(stdout, NULL);
  extern void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases],
                       int hy[kHomes], int hx[kHomes]);

  long long loss = 0;

  for (int t = 0; t < kTests; t++) {
    Init();

    do_alloc(h2b_, by_, bx_, hy_, hx_);

    auto l = Loss();
    PL(2, "l=%10lld", l);
    loss += l;
  }

  printf("MAX : %10lld\n", kPass);
  printf("INIT: %10lld\n", 199720088LL);
  printf("%4s: %10lld\n", loss <= kPass ? "PASS" : "FAIL", loss);

  return 0;
}

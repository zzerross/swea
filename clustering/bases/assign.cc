#if DEBUG
extern void Dump(const char* s);
extern int Set(int y, int x, int b);
#else
#define Dump(s)
#define Set(y, x, b)
#endif

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
constexpr static int kBases = 4;
constexpr static int kHomes = 110000;
constexpr static int kAllocs = 30000;
constexpr static int kLoss = 6000LL;
#endif

constexpr static int kDistance = kCells * 2;

static int* by_;
static int* bx_;

static int* hy_;
static int* hx_;

static int* h2b_;

static inline int Abs(int i) { return 0 <= i ? i : -i; }

static inline long long Distance(int h, int b) {
  return Abs(hy_[h] - by_[b]) + Abs(hx_[h] - bx_[b]);
}

int Min(int a, int b) { return a < b ? a : b; }
int Max(int a, int b) { return a > b ? a : b; }

bool Assign() {
  PI();

  unsigned long long u{};
  int a[kBases]{};

  for (int h = 0; h < kHomes; h++) {
    auto min = Distance(h, h2b_[h]);
    if (kAllocs - 1 <= a[h2b_[h]]) {
      min = kDistance;
    }

    for (int b = 0; b < kBases; b++) {
      if (kAllocs - 1 <= a[b]) {
        continue;
      }

      auto d = Distance(h, b);
      if (d < min) {
        min = d;
        u++;
        h2b_[h] = b;

        Set(hy_[h], hx_[h], -(b + 1));
      }
    }

    a[h2b_[h]]++;
  }

  Dump(__func__);
  PL(1, "a{%d %d %d %d} < %d", a[0], a[1], a[2], a[3], kAllocs);
  PO("u=%llu", u);
  return !!u;
}

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

  Assign();
}

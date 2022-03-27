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

struct Bound {
  int y0, y1, x0, x1;

  Bound() : y0(kCells), y1(0), x0(kCells), x1(0) {}
} bounds_[kBases];

static inline int Abs(int i) { return 0 <= i ? i : -i; }

static inline long long Distance(int h, int b) {
  return Abs(hy_[h] - by_[b]) + Abs(hx_[h] - bx_[b]);
}

constexpr static int kQouter = kCells / 4;
constexpr static int kQouter3 = kQouter * 3;

static void Init() {
  Set(by_[0], bx_[0], 0);
  by_[0] = kQouter;
  bx_[0] = kQouter;
  Set(by_[0], bx_[0], 1);

  Set(by_[1], bx_[1], 0);
  by_[1] = kQouter;
  bx_[1] = kQouter3;
  Set(by_[1], bx_[1], 2);

  Set(by_[2], bx_[2], 0);
  by_[2] = kQouter3;
  bx_[2] = kQouter;
  Set(by_[2], bx_[2], 3);

  Set(by_[3], bx_[3], 0);
  by_[3] = kQouter3;
  bx_[3] = kQouter3;
  Set(by_[3], bx_[3], 4);

  Dump(__func__);
}

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

int Min(int a, int b) { return a < b ? a : b; }
int Max(int a, int b) { return a > b ? a : b; }

void Update() {
  PI();

  for (int h = 0; h < kHomes; h++) {
    auto& b = bounds_[h2b_[h]];

    b.y0 = Min(hy_[h], b.y0);
    b.y1 = Max(hy_[h], b.y1);
    b.x0 = Min(hx_[h], b.x0);
    b.x1 = Max(hx_[h], b.x1);
  }

  for (int i = 0; i < kBases; i++) {
    auto& b = bounds_[i];

    Set(by_[i], bx_[i], 0);

    by_[i] = (b.y0 + b.y1) / 2;
    bx_[i] = (b.x0 + b.x1) / 2;

    PL(1, "%2d %2d %2d %2d:%2d %2d", b.y0, b.y1, b.x0, b.x1, by_[i], bx_[i]);
    Set(by_[i], bx_[i], i + 1);
  }

  Dump(__func__);
  PO();
}

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

#if UPDATE
  for (Init(); Assign(); Update())
    ;
#else
  while (Assign())
    ;
#endif
}

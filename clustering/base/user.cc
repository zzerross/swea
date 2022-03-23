#if DEBUG
extern void Dump();
#else
#define Dump()
#endif

#ifndef P1
#define P1(fmt, ...)
#define D1(fmt, ...)
#define IN1(fmt, ...)
#define OUT1(fmt, ...)
#endif

#ifndef P2
#define P2(fmt, ...)
#define D2(fmt, ...)
#define IN2(fmt, ...)
#define OUT2(fmt, ...)
#endif

#if INPUT
#else
constexpr static int kCells = 3000;
constexpr static int kBases = 4;
constexpr static int kHomes = 110000;
constexpr static int kAllocs = 30000;
constexpr static int kLoss = 6000LL;
#endif

static int* by_;
static int* bx_;

static int* hy_;
static int* hx_;

static int* h2b_;

static inline int Abs(int i) { return 0 <= i ? i : -i; }

static inline long long Distance(int h, int b) {
  return Abs(hy_[h] - by_[b]) + Abs(hx_[h] - bx_[b]);
}

bool Assign() {
  IN1();

  unsigned long long u{};

  for (int h = 0; h < kHomes; h++) {
    auto d0 = Distance(h, h2b_[h]);

    for (int b = 0; b < kBases; b++) {
      auto d1 = Distance(h, b);
      if (d1 < d0) {
        D2("h=%5d b=%d(%5lld)->%d(%5lld)", h, h2b_[h], d0, b, d1);
        d0 = d1;
        h2b_[h] = b;
        u++;
      }
    }
  }

  OUT1("u=%llu", u);
  return !!u;
}

int Min(int a, int b) { return a < b ? a : b; }
int Max(int a, int b) { return a > b ? a : b; }

void Update() {
  struct Bound {
    int y0, y1, x0, x1;

    Bound() : y0(kCells), y1(0), x0(kCells), x1(0) {}
  } bounds[kBases];

  for (int h = 0; h < kHomes; h++) {
    auto& b = bounds[h2b_[h]];

    b.y0 = Min(hy_[h], b.y0);
    b.y1 = Max(hy_[h], b.y1);
    b.x0 = Min(hx_[h], b.x0);
    b.x1 = Max(hx_[h], b.x1);
  }

  for (int i = 0; i < kBases; i++) {
    auto& b = bounds[i];

    by_[i] = (b.y0 + b.y1) / 2;
    bx_[i] = (b.x0 + b.x1) / 2;
  }
}

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

  while (Assign()) {
    Update();
  }
}

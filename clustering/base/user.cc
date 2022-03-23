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

static int* by_;
static int* bx_;

static int* hy_;
static int* hx_;

static int* h2b_;

long long Distance(int h, int b) {
  return abs(hy_[h] - by_[b]) + abs(hx_[h] - bx_[b]);
}

bool Assign() {
  IN1();

  unsigned long long u{};

  for (int h = 0, d; h < kHomes; h++) {
    d = Distance(h, h2b_[h]);

    for (int b = 0; b < kBases; b++) {
      if (Distance(h, b) < d) {
        h2b_[h] = b;
        u++;
      }
    }
  }

  OUT1("u=%llu", u);
  return !!u;
}

void Update() {}

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

  while (Assign()) {
    Update();
  }
}

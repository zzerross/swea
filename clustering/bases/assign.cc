#if PRINT
extern void Dump(const char* s);
extern void Set(int y, int x, int b);
#else
static void Dump(const char* s){};
static void Set(int y, int x, int b){};

#define A(expr, ...) (expr)
#define PR(lvl, fmt, ...)
#define PL(lvl, fmt, ...)
#define PI(fmt, ...)
#define PO(fmt, ...)
#endif

#define I2B(i) ((i) + 1)
#define B2I(b) ((b)-1)

#if INPUT
#else
constexpr static int kCells = 3000;
constexpr static int kBases = 4;
constexpr static int kHomes = 110000;
constexpr static int kAllocs = 30000;
constexpr static int kLoss = 6000LL;
constexpr static int kCenter = 1500;
#endif

constexpr static int kDistance = kCells * 2;

static int* by_;
static int* bx_;

static int* hy_;
static int* hx_;

static int* h2b_;

static int coners[kBases];

static inline int Abs(int i) { return i < 0 ? -i : i; }

static inline long long Distance(int y1, int x1, int y2, int x2) {
  return Abs(y1 - y2) + Abs(x1 - x2);
}

static inline long long Distance(int h, int b) {
  return Abs(hy_[h] - by_[b]) + Abs(hx_[h] - bx_[b]);
}

int Min(int a, int b) { return a < b ? a : b; }
int Max(int a, int b) { return a > b ? a : b; }

int NearstBase(int y, int x, int& assigned) {
  PI();

  int min_d = kDistance;
  int min_b = 0;

  for (int b = 0; b < kBases; b++) {
    if (assigned & (1 << b)) {
      continue;
    }

    int d = Distance(y, x, by_[b], bx_[b]);
    PL(2, "d=%5d min_d=%5d b=%+d", d, min_d, I2B(b));
    if (d < min_d) {
      min_d = d;
      min_b = b;
    }
  }

  assigned |= (1 << min_b);

  PO("y=%5d x=%5d assigned=0x%02x b=%d", y, x, assigned, I2B(min_b));

  return min_b;
}

static void Init() {
  int assigned = 0;
  int b;

  coners[0] = NearstBase(0, 0, assigned);
  coners[1] = NearstBase(0, kCells - 1, assigned);
  coners[2] = NearstBase(kCells - 1, 0, assigned);
  coners[3] = NearstBase(kCells - 1, kCells - 1, assigned);

#if 2 <= PRINT
  for (int i = 0; i < kBases; i++) {
    int b = coners[i];
    PL(2, "coners[%d] b=%+d {%5d,%5d}", i, I2B(i), by_[b], bx_[b]);
  }
#endif
}

static int Assign(int h) {
  if (hy_[h] < kCenter) {
    return coners[hx_[h] < kCenter ? 0 : 1];
  } else {
    return coners[hx_[h] < kCenter ? 2 : 3];
  }
}

static void Assign() {
  PI();

  for (int h = 0; h < kHomes; h++) {
    h2b_[h] = Assign(h);
    Set(hy_[h], hx_[h], -(h2b_[h] + 1));
  }

  Dump(__func__);
  PO();
}

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

  Init();
  Assign();
}

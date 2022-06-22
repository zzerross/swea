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
long long sum_[kBases];
int bases_[kBases];
int nhomes_[kBases];
int homes_[kBases][kHomes];

static inline int Abs(int i) { return 0 <= i ? i : -i; }

static inline long long Distance(int h, int b) {
  return Abs(hy_[h] - by_[b]) + Abs(hx_[h] - bx_[b]);
}

#if 2 <= PRINT
#define DumpAllocs()                                                     \
  {                                                                      \
    for (int i = 0; i < kBases; i++) {                                   \
      PL(2, "sum_[%d]=%8lld allocs_[%d]=%d", bases_[i], sum_[bases_[i]], \
         bases_[i], allocs_[bases_[i]]);                                 \
    }                                                                    \
  }
#else
#define DumpAllocs()
#endif

static int Rand(void) {
  static int seed = 18043000;
  seed = seed * 214013 + 2531011;
  return (seed >> 16) & 0x7FFF;
}

bool IsOverflow(int* allocs_) {
  for (int b = 0; b < kBases; b++)
    if (kAllocs < allocs_[b]) return true;
  return false;
}

size_t RandomSwap(auto allocs_, auto sum_) {
  size_t swapped = 0;
  for (bool u = true; u; swapped++) {
    u = false;

    int h0 = Rand() % kHomes;
    int h1 = Rand() % kHomes;
    if (h0 == h1) continue;

    int b0 = h2b_[h0];
    int b1 = h2b_[h1];
    if (b0 == b1) continue;

    int d0 = Distance(h0, b0);
    int d1 = Distance(h1, b1);

    long long s0 = 0;
    long long s1 = 0;
    for (int b = 0; b < kBases; b++) {
      s0 += sum_[b];

      if (b == b0) {
        s1 -= sum_[b0];
        s1 += sum_[b1];
      } else if (b == b1) {
        s1 += sum_[b0];
        s1 -= sum_[b1];
      } else {
        s1 += sum_[b];
      }
    }
    if (s0 <= s1) continue;

    h2b_[h0] = b1;
    h2b_[h1] = b0;
    u = true;

    PL(2, "swapped=%lu u=%d %d->%d %d->%d\n", swapped, u, b0, b1, allocs_[b0],
       allocs_[b1]);
  }

  return swapped;
}

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

  int allocs_[kBases] = {};
  for (int h = 0; h < kHomes; h++) {
    allocs_[h2b_[h]]++;
  }

  for (int h = 0; h < kHomes; h++) {
    sum_[h2b_[h]] += Distance(h, h2b_[h]);
  }

  auto swapped = RandomSwap(allocs_, sum_);

  PO("allocs_{%d %d %d %d} < %d swapped=%lu", allocs_[0], allocs_[1],
     allocs_[2], allocs_[3], kAllocs, swapped);
}

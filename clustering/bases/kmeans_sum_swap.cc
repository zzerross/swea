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
int allocs_[kBases];

static inline int Abs(int i) { return 0 <= i ? i : -i; }

int Min(int a, int b) { return a < b ? a : b; }
int Max(int a, int b) { return a > b ? a : b; }

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

int _min(const int& i, const int& j) { return i < j ? i : j; }

template <typename T>
void Sort(T* a, T* e, auto P) {
  int s = e - a;
  T b[s];
  for (int n = 1; n < s; n *= 2) {
    for (int i, j, p = 0; p < s;) {
      int l = p;
      int m = _min(p += n, s) - 1;
      int h = _min(p += n, s) - 1;

      for (i = l; i <= h; i++) b[i] = a[i];

      for (i = l, j = m + 1; i <= m && j <= h;)
        a[l++] = P(b[i], b[j]) ? b[i++] : b[j++];

      while (i <= m) a[l++] = b[i++];
      while (j <= h) a[l++] = b[j++];
    }
  }
}

void Assign() {
  PI();

  for (int b = 0; b < kBases; b++) {
    allocs_[b] = 0;
  }

  for (int h = 0; h < kHomes; h++) {
    auto m = kDistance;

    for (int b = 0; b < kBases; b++) {
      auto d = Distance(h, b);
      if (d < m) {
        m = d;
        h2b_[h] = b;
      }
    }

    Set(hy_[h], hx_[h], -(h2b_[h] + 1));
    allocs_[h2b_[h]]++;
  }

  Dump(__func__);
  PO("allocs_{%d %d %d %d} < %d", allocs_[0], allocs_[1], allocs_[2],
     allocs_[3], kAllocs);
}

void Sum() {
  for (int b = 0; b < kBases; b++) {
    sum_[b] = 0;
  }

  for (int h = 0; h < kHomes; h++) {
    sum_[h2b_[h]] += Distance(h, h2b_[h]);
  }

  for (int i = 0; i < kBases; i++) {
    bases_[i] = i;
  }

  Sort(bases_, bases_ + kBases,
       [](int b1, int b2) { return allocs_[b1] > allocs_[b2]; });

  DumpAllocs();
}

void Sort() {
  for (int b = 0; b < kBases; b++) {
    nhomes_[b] = 0;
  }

  for (int h = 0; h < kHomes; h++) {
    int b = h2b_[h];
    homes_[b][nhomes_[b]++] = h;
  }

  for (int b = 0; b < kBases; b++) {
    Sort(homes_[b], homes_[b] + nhomes_[b],
         [b](int h1, int h2) { return Distance(h1, b) > Distance(h2, b); });
  }
}

void Normalize() {
  for (int i = 0; i < kBases; i++) {
    int s = bases_[i];

    for (int k = 0; k < nhomes_[s] && kAllocs <= allocs_[s]; k++) {
      int h = homes_[s][k];
      int d = kBases;
      int m = kDistance;

      for (int j = i + 1; j < kBases; j++) {
        int c = bases_[j];
        if (kAllocs <= allocs_[c]) continue;

        int t = Distance(h, c);
        if (m < t) continue;

        m = t;
        d = c;
      }

      if (kBases <= d) continue;

      allocs_[s]--;
      allocs_[d]++;
      sum_[s] -= m;
      sum_[d] += m;
      h2b_[h] = d;
      Set(hy_[h], hx_[h], -(d + 1));

      PL(3, "allocs_[%d]=%d->allocs_[%d]=%d", s, allocs_[s], d, allocs_[d]);
    }
    PL(3, "    [%d]=%8lld        [%d]=%d", s, sum_[s], s, allocs_[s]);
  }
  DumpAllocs();
}

bool RandomMove() {
  bool moved = false;
  do {
    int h = Rand() % kHomes;
    int b0 = h2b_[h];
    int b1 = Rand() % kBases;
    if (b0 == b1) continue;
    if (kAllocs <= allocs_[b1]) continue;

    int d0 = Distance(h, b0);
    int d1 = Distance(h, b1);
    if (d0 <= d1) continue;

    allocs_[b0]--;
    allocs_[b1]++;
    h2b_[h] = b1;
    moved = true;

    PL(2, "moved=%d %d->%d %d->%d\n", moved, b0, b1, allocs_[b0], allocs_[b1]);
  } while (0);

  return moved;
}

size_t Overflows() {
  size_t overflows = 0;
  for (int b = 0; b < kBases; b++) overflows += Max(0, allocs_[b] - kAllocs);
  return overflows;
}

bool SwapRandomly(auto allocs_, auto sum_) {
  bool swapped = false;

  do {
    int h0 = Rand() % kHomes;
    int h1 = Rand() % kHomes;
    if (h0 == h1) continue;

    int b0 = h2b_[h0];
    int b1 = h2b_[h1];
    if (b0 == b1) continue;

    int d0 = Distance(h0, b0);
    int d1 = Distance(h1, b1);

    long long s0 = Distance(h0, h2b_[h0]) + Distance(h1, h2b_[h1]);
    long long s1 = Distance(h0, h2b_[h1]) + Distance(h1, h2b_[h0]);

    PL(1, "s0=%10lld", s0);
    PL(1, "s1=%10lld", s1);
    PL(1, "  =%10lld %s", s1 - s0, s0 <= s1 ? "" : "swap");
    if (s0 <= s1) continue;

    h2b_[h0] = b1;
    h2b_[h1] = b0;
    swapped = true;

    PL(2, "swapped=%d %d->%d %d->%d\n", swapped, b0, b1, allocs_[b0],
       allocs_[b1]);
  } while (0);

  return swapped;
}

void SwapRandomly() {
  // while (0 < Overflows()) RandomMove();

  for (int t = 0; t < kHomes * 100; t++) {
    SwapRandomly(allocs_, sum_);
  }
}

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

  Assign();
  Sum();
  Sort();
  Normalize();
  SwapRandomly();
}

// FAIL: 2622542560

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

bool Assign() {
  PI();

  unsigned long long u{};

  for (int b = 0; b < kBases; b++) {
    allocs_[b] = 0;
  }

  for (int h = 0; h < kHomes; h++) {
    auto min = Distance(h, h2b_[h]);

    for (int b = 0; b < kBases; b++) {
      auto d = Distance(h, b);
      if (d < min) {
        min = d;
        u++;
        h2b_[h] = b;

        Set(hy_[h], hx_[h], -(b + 1));
      }
    }

    allocs_[h2b_[h]]++;
  }

  Dump(__func__);
  PO("allocs_{%d %d %d %d} < %d u=%llu", allocs_[0], allocs_[1], allocs_[2],
     allocs_[3], kAllocs, u);
  return !!u;
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

  sort(bases_, bases_ + kBases,
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
    sort(homes_[b], homes_[b] + nhomes_[b],
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

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

  Assign();
  Sum();
  Sort();
  Normalize();
}

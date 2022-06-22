#include <algorithm>
#include <array>
#include <bitset>
#include <iterator>
#include <list>
#include <numeric>
using namespace std;

#if PRINT
extern void Dump(const char* s);
extern int Set(int y, int x, int b);
#else
#define Dump(s)
#define Set(y, x, b)

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
constexpr static int kAllocs = 30000 - 1;
constexpr static int kLoss = 6000LL;
#endif

constexpr static int kNear = 2000;
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

int Next(bitset<kHomes>& assigned_, array<int, kHomes>& homes, int& next) {
  for (; next < kHomes && homes[next] < kHomes; next++) {
    if (!assigned_[homes[next]]) {
      assigned_[homes[next]] = true;
      return next;
    }
  }
  return -1;
}

static int Rand(void) {
  static int seed = 18043000;
  seed = seed * 214013 + 2531011;
  return (seed >> 16) & 0x7FFF;
}

bool RandomMove(auto allocs_) {
  int h = Rand() % kHomes;
  int b0 = h2b_[h];
  int b1 = Rand() % kBases;
  if (b0 == b1) return false;
  if (kAllocs <= allocs_[b1]) return false;

  int d0 = Distance(h, b0);
  int d1 = Distance(h, b1);
  if (d0 <= d1) return false;

  allocs_[b0]--;
  allocs_[b1]++;
  h2b_[h] = b1;

  PL(2, "%d->%d %d->%d(%d) %d->%d\n", b0, b1, d0, d1, d1 - d0, allocs_[b0],
     allocs_[b1]);

  return true;
}

bool RandomSwap(auto allocs_, auto sum_) {
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

    PL(3, "s0=%10lld", s0);
    PL(3, "s1=%10lld", s1);
    PL(3, "  =%10lld %s", s1 - s0, s0 <= s1 ? "" : "swap");
    if (s0 <= s1) continue;

    h2b_[h0] = b1;
    h2b_[h1] = b0;
    swapped = true;

    PL(2, "swapped=%d %d->%d %d->%d\n", swapped, b0, b1, allocs_[b0],
       allocs_[b1]);
  } while (0);

  return swapped;
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

constexpr size_t kDistances = kBases * kHomes;
struct Dist {
  int home;
  int base;
  long long distance;
};
Dist distances_[kDistances] = {};
Dist* overs_[kDistances] = {};

size_t Overflows(auto allocs_) {
  size_t overflows = 0;
  for (int b = 0; b < kBases; b++) overflows += Max(0, allocs_[b] - kAllocs);
  return overflows;
}

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

  int allocs_[kBases] = {};

  for (int b = 0; b < kBases; b++) {
    allocs_[b] = 0;
  }

  for (int i = 0, h = 0; h < kHomes; h++) {
    for (int b = 0; b < kBases; b++) {
      distances_[i++] = {h, b, Distance(h, b)};
    }
  }
  Sort(distances_, distances_ + kDistances,
       [](auto& i, auto& j) { return i.distance < j.distance; });

  bool assigned_[kHomes] = {};

  size_t overs = 0;
  for (int i = 0; i < kDistances; i++) {
    auto& d = distances_[i];
    if (assigned_[d.home]) continue;
    if (kAllocs <= allocs_[d.base]) {
      overs_[overs++] = &d;
    }

    assigned_[d.home] = true;
    h2b_[d.home] = d.base;
    allocs_[d.base]++;
  }

  size_t overflows = Overflows(allocs_);

  PO("allocs_{%d %d %d %d} < %d overflows=%ld", allocs_[0], allocs_[1],
     allocs_[2], allocs_[3], kAllocs, overflows);

  for (int o = overs - 1; 0 <= o; o--) {
    auto m = kDistance;
    auto d = overs_[o];
    auto n = kBases;

    for (int b = 0; b < kBases; b++) {
      if (b == d->base) continue;
      if (kAllocs <= allocs_[b]) continue;

      if (Distance(d->home, b) < m) {
        n = b;
      }
    }

    h2b_[d->home] = n;
    --allocs_[d->base];
    ++allocs_[n];
  }

  PO("allocs_{%d %d %d %d} < %d", allocs_[0], allocs_[1], allocs_[2],
     allocs_[3], kAllocs);

  long long sum_[kBases] = {};

  for (int b = 0; b < kBases; b++) {
    sum_[b] = 0;
  }

  for (int h = 0; h < kHomes; h++) {
    sum_[h2b_[h]] += Distance(h, h2b_[h]);
  }

  for (int nswapped = 0; nswapped < kHomes || 0 < Overflows(allocs_);) {
    RandomMove(allocs_);

    if (RandomSwap(allocs_, sum_)) {
      nswapped = 0;
    } else {
      nswapped++;
    }
  }

  PO("allocs_{%d %d %d %d} < %d", allocs_[0], allocs_[1], allocs_[2],
     allocs_[3], kAllocs);
}

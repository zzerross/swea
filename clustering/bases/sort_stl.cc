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

int Next(bitset<kHomes>& assigned, array<int, kHomes>& homes, int& next) {
  for (; next < kHomes && homes[next] < kHomes; next++) {
    if (!assigned[homes[next]]) {
      assigned[homes[next]] = true;
      return next;
    }
  }
  return -1;
}

void do_alloc(int h2b[kHomes], int by[kBases], int bx[kBases], int hy[kHomes],
              int hx[kHomes]) {
  h2b_ = h2b, by_ = by, bx_ = bx, hy_ = hy, hx_ = hx;

  array<array<int, kHomes>, kBases> table;
  for (int b = 0; b < table.size(); b++) {
    auto& a = table[b];
    iota(begin(a), end(a), 0);
    sort(begin(a), end(a),
         [b](int i, int j) { return Distance(i, b) < Distance(j, b); });
  }

  bitset<kHomes> assigned;
  int nexts[kBases]{};
  int allocs[kBases]{};

  for (int b = 0; assigned.count() < kNear; b = (++b % kBases)) {
    auto& row = table[b];

    if (Next(assigned, row, nexts[b]) < 0) continue;

    int h = row[nexts[b]++];
    h2b_[h] = b;
    allocs[b]++;
    Set(hy_[h], hx_[h], -(b + 1));
  }

  for (int h = 0; h < kHomes; h++) {
    if (assigned[h]) continue;

    int m = kDistance;
    int b = 0;

    for (int c = 0; c < kBases; c++) {
      if (kAllocs <= allocs[c]) continue;

      auto d = Distance(h, c);
      if (d < m) {
        m = d;
        b = c;
      }
    }

    h2b_[h] = b;
    allocs[b]++;
    Set(hy_[h], hx_[h], -(b + 1));
  }

  Dump(__func__);
}

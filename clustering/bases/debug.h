#pragma once

#include <cassert>
#include <iostream>
using namespace std;

#define P0(fmt, ...) printf(fmt, ##__VA_ARGS__)

#if PRINT
#define A(expr, ...)              \
  ({                              \
    bool r;                       \
    if (expr) PL(0, __VA_ARGS__); \
    assert(!(r = (expr)));        \
    r;                            \
  })

#define PR(lvl, fmt, ...) \
  if (lvl <= PRINT) P0(fmt, ##__VA_ARGS__)

#define PL(lvl, fmt, ...) \
  PR(lvl, "%10s:%03d: " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

#define PI(fmt, ...) PL(1, "> " fmt, ##__VA_ARGS__)
#define PO(fmt, ...) PL(1, "< " fmt, ##__VA_ARGS__)
#endif

#if INPUT
constexpr static int kCells = 30;
constexpr static int kBases = 4;
constexpr static int kHomes = 110;
constexpr static int kAllocs = 30;
constexpr static int kLoss = 6000LL;
#endif

#pragma once

#include <iostream>
using namespace std;

#define P0(fmt, ...) printf(fmt, ##__VA_ARGS__)

#if 1 <= DEBUG
#define P1(fmt, ...) P0(fmt, ##__VA_ARGS__)
#define D1(fmt, ...) P1("%10s:%03d: " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)
#define IN1(fmt, ...) D1("> " fmt, ##__VA_ARGS__)
#define OUT1(fmt, ...) D1("< " fmt, ##__VA_ARGS__)
#endif

#if 2 <= DEBUG
#define P2(fmt, ...) P1(fmt, ##__VA_ARGS__)
#define D2(fmt, ...) D1(fmt, ##__VA_ARGS__)
#define IN2(fmt, ...) IN1(fmt, ##__VA_ARGS__)
#define OUT2(fmt, ...) OUT1(fmt, ##__VA_ARGS__)
#endif


#if INPUT
constexpr static int kCells = 30;
constexpr static int kBases = 4;
constexpr static int kHomes = 110;
constexpr static int kAllocs = 30;
constexpr static int kLoss = 6000LL;
#endif

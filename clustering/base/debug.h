#pragma once

#include <iostream>
using namespace std;

#define P0(fmt, ...) printf("%10s:%03d" fmt, __func__, __LINE__, ##__VA_ARGS__)

#if 1 <= kDEBUG
#define P1(fmt, ...) P0(fmt, ##__VA_ARGS__)
#define D1(fmt, ...) P1(": " fmt, ##__VA_ARGS__)
#define IN1(fmt, ...) D1("> " fmt "\n", ##__VA_ARGS__)
#define OUT1(fmt, ...) D1("< " fmt "\n", ##__VA_ARGS__)
#endif

#ifndef D1
#define P1(fmt, ...)
#define D1(fmt, ...)
#define IN1(fmt, ...)
#define OUT1(fmt, ...)
#endif

#if kDEBUG
constexpr static int kCells = 30;
constexpr static int kBases = 1;
constexpr static int kHomes = 1100;
constexpr static int kAllocs = 300;
constexpr static int kLoss = 60LL;
#else
constexpr static int kCells = 3000;
constexpr static int kBases = 4;
constexpr static int kHomes = 110000;
constexpr static int kAllocs = 30000;
constexpr static int kLoss = 6000LL;
#endif

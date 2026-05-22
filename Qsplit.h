#pragma once
#include "common.h"

void addSequence(bitmask_t mins, bitmask_t maxs);

bitmask_t determineType(bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q, bitmask_t N2);

void QSplitA(bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q);

void QSplitB(bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q);

void QSplitC(bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q);

void QSplitD(bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q);

void QSplit(char oABCD, bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q);

void printSequence(Sequence S);

Sequence Range_QQ(int k, bitmask_t x1, bitmask_t y1, bitmask_t x2, bitmask_t y2);
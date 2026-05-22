#pragma once
#include <stdio.h>
#include <iostream>
#include <cmath> 
#include <vector>
#include <bitset>
#include <chrono>
#include <string>
#include <algorithm>
#include <queue>
#include <random>
#include <fstream>
#include <iomanip>
#include <numeric>

using namespace std;
typedef long long bitmask_t;
typedef long halfmask_t;
typedef std::vector<bitmask_t> Row;
typedef vector<vector<bitmask_t>> TwoDArray;

typedef std::pair<bitmask_t, bitmask_t> Segment;
// 定义一个序列
typedef std::vector<Segment> Sequence;


#define getOneBitByPos(X,bit) ((X>>bit) & 0x01)  
#define last_k_Zero(n,k) (n & ((~0U) << k))
#define getLowKBits(X,k) (X & ((1 << k) - 1))
#define lsb32_idx(v) (N_bit[((uint32_t)((v & -v) * 0x077CB531U)) >> 27])


static const int N_bit[32] = { 0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9 };
static const int M_bit[32] = { 0,  9,  1, 10, 13, 21,  2, 29, 11, 14, 16, 18, 22, 25,  3, 30, 8, 12, 20, 28, 15, 17, 24,  7, 19, 27, 23,  6, 26,  5,  4, 31 };


//编码
//the mappings from L1-coord to its L1-code  从L1坐标到其L1代码的映射
extern char CHM[4][2][2];

//the mappings from L1-coord to its Ln-state 从L1坐标到其Ln状态的映射
extern char CSM[4][2][2];


class Point {
public:
    float x;
    float y;
    long long xi;
    long long yi;
    long long curveval;

    // 默认构造函数
    Point() : x(0), y(0), xi(0), yi(0), curveval(0) {}

    Point(float xval, float yval) : x(xval), y(yval), xi(0), yi(0), curveval(0) {}

};


class window {
public:
    int x1;
    int y1;
    int x2;
    int y2;
    window(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

    // 打印Window的坐标
    void printCoordinates() const {
        std::cout << "\tx1:" << x1 << "\ty1:" << y1 << "\tx2:" << x2 << "\ty2: " << y2 << std::endl;
    }
};

// 结构体：用于存储每个二维数组的当前行信息
struct HeapNode {
    const vector<bitmask_t>* row;  // 指向当前行
    size_t arrayIdx;               // 该行所属的二维数组索引
    size_t rowIdx;                 // 该行在二维数组中的索引

    HeapNode(const vector<bitmask_t>* r, size_t aIdx, size_t rIdx)
        : row(r), arrayIdx(aIdx), rowIdx(rIdx) {
    }
};

// 比较器：按行首元素排序
struct Compare {
    bool operator()(const HeapNode& a, const HeapNode& b) {
        return a.row->front() > b.row->front();  // 小顶堆，首元素小的优先
    }
};

std::vector<Point> readPointsFromFile(const std::string& filename);


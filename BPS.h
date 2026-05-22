#pragma once
#include "common.h" 

bitmask_t encode(int x, int y, int order);

// 生成随机点
std::vector<Point> generateRandomPoints(int n);

// 按x排序赋值
void sortPointsX(std::vector<Point>& points);

// 按y排序得到（xi,yi）
void sortPointsYi(std::vector<Point>& points, int n);

// 按x排序得到（xi,yi）
void sortPointsXi(std::vector<Point>& points);


// 打印点
void printPoints(const std::vector<Point>& points, const std::string& title);


int middle32_num(int num1, int num2);

int BPS(int n, bool border, int N_limit, int k, int exp, int L_k);

void handle_change(int old_val, int new_val, bool is_pop, const std::vector<Point>& arr, std::vector<long long>& arr_push, std::vector<long long>& arr_pop, int fixed1, int fixed2, bool is_x_axis, int N_limit);

void window_move(window& W, int k, const std::vector<Point>& arr_xi, const std::vector<Point>& arr_yi, std::vector<long long>& arr_push, std::vector<long long>& arr_pop, int N_limit, int exp, int l_k);

void printVector(const std::vector<long long>& vec);

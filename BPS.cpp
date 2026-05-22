#include "EPS.h"
#include "common.h" 
#include "Qsplit.h"
#include <cstring> 


bitmask_t encode(int x, int y, int order) {

    char nType = 0;
    bitmask_t resKey = 0;
    int iterStartPos = order - 1;
    unsigned bitX = 0, bitY = 0;
    for (int i = iterStartPos; i >= 0; i--)
    {
        bitX = getOneBitByPos(x, i);
        bitY = getOneBitByPos(y, i);
        resKey = (resKey << 2) | CHM[nType][bitX][bitY];
        nType = CSM[nType][bitX][bitY];
    }
    return  resKey;
}

// 生成随机点
std::vector<Point> generateRandomPoints(int n) {
    std::vector<Point> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < n; ++i) {
        float xval = dis(gen);
        float yval = dis(gen);
        points.emplace_back(xval, yval);
    }
    return points;
}

// 按x排序赋值
void sortPointsX(std::vector<Point>& points) {
    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
        });
    for (size_t i = 0; i < points.size(); ++i) points[i].xi = i;
}

// 按y排序得到（xi,yi）
void sortPointsYi(std::vector<Point>& points, int n) {
    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
        });
    for (size_t i = 0; i < points.size(); ++i) {
        points[i].yi = i;
        points[i].curveval = encode(points[i].xi, i, n);
    }
}

// 按x排序得到（xi,yi）
void sortPointsXi(std::vector<Point>& points) {
    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.xi < b.xi;
        });
}


// 打印点
void printPoints(const std::vector<Point>& points, const std::string& title) {
    std::cout << title << ":\n";
    for (const auto& point : points) {
        std::cout << "Point(x: " << point.x << ", y: " << point.y << ", xi: " << point.xi << ", yi: " << point.yi << ", curveval: " << point.curveval << ")\n";
    }
    std::cout << std::endl;
}


int middle32_num(int num1, int num2) {
    int v = num1 ^ num2;
    v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16;
    v = M_bit[(v * 0x07C4ACDDU) >> 27];
    return  getLowKBits(num1, v) ? num2 >> v << v : num1;
}



//参数为边框索引值，先定外扩exp=1/内缩exp=0，再定层级次数, 看是什么边框(左下broder=0,右上broder=1),阶数k
//BPS固定迁越次数L_k，往大走还是往小走由exp决定
int BPS(int n, bool border, int N_limit, int k, int exp, int L_k) {
    int temp_n;
    if (border != exp) {//往小走
        for (int i = 0; i < L_k; i++) {
            temp_n = last_k_Zero(n, lsb32_idx(n) + 1);
            if ((temp_n > 0 && !border) || (temp_n > N_limit && border)) n = temp_n;//左下往小走或右上往小走
        }
    }
    else {
        int N_l = 1 << k;//往大走
        for (int i = 0; i < L_k; i++) {
            temp_n = n + (1 << lsb32_idx(n));
            if ((temp_n < N_l && border) || (temp_n < N_limit && !border)) n = temp_n;//右上往大走或左下往大走
        }
    }
    return n;
}

void handle_change(int old_val, int new_val, bool is_pop, const std::vector<Point>& arr, std::vector<long long>& arr_push, std::vector<long long>& arr_pop, int fixed1, int fixed2, bool is_x_axis, int N_limit) {
    if (old_val == new_val) return;

    int start = std::max(0, std::min(old_val, new_val));
    int end = std::min(N_limit, std::max(old_val, new_val));
    bool cond = new_val < old_val;

    for (int i = start; i < end; ++i) {
        if (is_x_axis) {
            if (arr[i].yi >= fixed1 && arr[i].yi < fixed2) {
                (cond == is_pop ? arr_pop : arr_push).push_back(arr[i].curveval);
            }
        }
        else {
            if (arr[i].xi >= fixed1 && arr[i].xi < fixed2) {
                (cond == is_pop ? arr_pop : arr_push).push_back(arr[i].curveval);
            }
        }
    }
}

void window_move(window& W, int k, const std::vector<Point>& arr_xi, const std::vector<Point>& arr_yi, std::vector<long long>& arr_push, std::vector<long long>& arr_pop, int N_limit, int exp, int l_k) {
    int x1_old = W.x1;

    //输出初始窗口的坐标
    W.printCoordinates();
    

    W.x1 = BPS(W.x1, 0, W.x2, k, exp, l_k);
    handle_change(x1_old, W.x1, true, arr_xi, arr_push, arr_pop, W.y1, W.y2, true, N_limit);

    int x2_old = W.x2;
    W.x2 = BPS(W.x2, 1, W.x1, k, exp, l_k);
    handle_change(x2_old, W.x2, false, arr_xi, arr_push, arr_pop, W.y1, W.y2, true, N_limit);

    int x_low = W.x1, x_high = W.x2;
    int y1_old = W.y1;
    W.y1 = BPS(W.y1, 0, W.y2, k, exp, l_k);
    if (y1_old < W.y1) {
        x_low = x1_old > W.x1 ? x1_old : W.x1;
        x_high = x2_old < W.x2 ? x2_old : W.x2;
    }
    handle_change(y1_old, W.y1, true, arr_yi, arr_push, arr_pop, x_low, x_high, false, N_limit);

    int y2_old = W.y2; x_low = W.x1, x_high = W.x2;
    W.y2 = BPS(W.y2, 1, W.y1, k, exp, l_k);
    if (y2_old > W.y2) {
        x_low = x1_old > W.x1 ? x1_old : W.x1;
        x_high = x2_old < W.x2 ? x2_old : W.x2;
    }
    handle_change(y2_old, W.y2, false, arr_yi, arr_push, arr_pop, x_low, x_high, false, N_limit);

    W.printCoordinates();
}

// 定义一个结构体来同时返回结果和耗时
struct MatchResult {
    std::vector<bitmask_t> data;
    double duration_us;
};

MatchResult find_by_heuristic_guess_2d(const std::vector<bitmask_t>& a, const std::vector<Segment>& b) {
    std::vector<bitmask_t> result;
    size_t max_limit = a.size() / 3;
    result.resize(max_limit);

    // 获取裸指针
    bitmask_t* dest_ptr = result.data();
    bitmask_t* base_ptr = result.data();

    auto search_it = a.begin();
    const auto a_end = a.end();

    auto start9 = std::chrono::high_resolution_clock::now();

    for (const auto& seg : b) {
        if (search_it == a_end) break;
        if (*search_it < seg.first) {
            long long diff = seg.first - *search_it;
            auto step = std::min((ptrdiff_t)(diff + 1LL), a_end - search_it);
            search_it = std::lower_bound(search_it, search_it + step, seg.first);
        }

        if (search_it == a_end) break;
        if (*search_it > seg.second) continue;

        //auto limit = std::min((ptrdiff_t)(seg.second - seg.first + 2LL), a_end - search_it);
        //auto end_it = std::upper_bound(search_it, search_it + limit, seg.second);
        auto end_it = std::upper_bound(search_it, a_end, seg.second);

        size_t count = end_it - search_it;
        if (count > 0) {
            std::memcpy(dest_ptr, &(*search_it), count * sizeof(bitmask_t));
            dest_ptr += count;
            search_it = end_it;
        }
    }

    //截断多余空间
    result.resize(dest_ptr - base_ptr);
    auto end9 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration9 = end9 - start9;
    return { result, duration9.count() };
}

void process_pop(Sequence& S, std::vector<bitmask_t>& arr_pop) {
    if (S.empty() || arr_pop.empty()) return;
    std::sort(arr_pop.begin(), arr_pop.end());
    Sequence newS;
    newS.reserve(S.size());

    size_t i = 0, j = 0; // i for S, j for arr_pop
    while (i < S.size()) {
        bitmask_t a = S[i].first;
        bitmask_t b = S[i].second;
        while (j < arr_pop.size() && arr_pop[j] < a) j++;

        // 处理当前区间内的所有 pop 元素
        std::vector<bitmask_t> pops_in_range;
        while (j < arr_pop.size() && arr_pop[j] <= b) {
            pops_in_range.push_back(arr_pop[j]);
            j++;
        }

        if (pops_in_range.empty()) {
            newS.push_back({ a, b });
        }
        else {
            bitmask_t prev = a;
            for (bitmask_t x : pops_in_range) {
                if (x > prev)
                    newS.push_back({ prev, x - 1 });
                prev = x + 1;
            }
            if (prev <= b)
                newS.push_back({ prev, b });
        }

        ++i;
    }
    S.swap(newS);
}

void process_push(Sequence& S, const std::vector<bitmask_t>& arr_push) {
    if (arr_push.empty()) return;

    std::vector<bitmask_t> a = arr_push;
    std::sort(a.begin(), a.end());

    std::vector<Segment> newSegs;
    newSegs.reserve(a.size());

    bitmask_t L = a[0], R = a[0];
    for (size_t i = 1; i < a.size(); ++i) {
        if (a[i] == R + 1) {
            R = a[i];
        }
        else {
            newSegs.emplace_back(L, R);
            L = R = a[i];
        }
    }
    newSegs.emplace_back(L, R);

    Sequence result;
    result.reserve(S.size() + newSegs.size());

    size_t i = 0, j = 0;
    while (i < S.size() && j < newSegs.size()) {
        const auto& s = S[i];
        const auto& t = newSegs[j];

        if (s.second + 1 < t.first) {
            result.push_back(s);
            i++;
        }
        else if (t.second + 1 < s.first) {
            result.push_back(t);
            j++;
        }
        else {
            // 区间相交或相邻 → 合并
            bitmask_t left = std::min(s.first, t.first);
            bitmask_t right = std::max(s.second, t.second);

            // 合并 S 中可能重叠的区间
            i++;
            while (i < S.size() && S[i].first <= right + 1) {
                right = std::max(right, S[i].second);
                i++;
            }
            // 合并 newSegs 中可能重叠的区间
            j++;
            while (j < newSegs.size() && newSegs[j].first <= right + 1) {
                right = std::max(right, newSegs[j].second);
                j++;
            }

            result.emplace_back(left, right);
        }
    }

    // 4. 添加剩余区间
    while (i < S.size()) result.push_back(S[i++]);
    while (j < newSegs.size()) result.push_back(newSegs[j++]);

    S.swap(result);
}


// 函数定义
void printVector(const std::vector<long long>& vec) {
    for (long long element : vec) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}


int getElementCount(const TwoDArray& arr) {
    int total = 0;
    for (const auto& row : arr) {
        total += row.size();
    }
    return total;
}

// 1. 更新实验结果结构体，包含所有要求的指标
struct ExperimentResult {
    std::string datasetName;
    double lambda;
    double selectivity;
    double avg_total_time;
    double avg_seg_orig;
    double avg_seg_eps;
    double avg_red_seg;
    double avg_window_shift;
    double avg_scan_cost;
    double avg_comp_rate;
    double avg_query_rate;
};

/**/
int main() {
    // 2. 定义本地数据集路径及其对应的 K_val 值
    std::vector<std::pair<std::string, int>> local_datasets = {
        {"D:/share/data/learnedbench-master/uniform_20m_2_1.txt", 25},
        {"D:/share/data/learnedbench-master/gaussian_20m_2_1.txt", 25},
        {"D:/share/data/learnedbench-master/lognormal_20m_2_1.txt", 25}
    };

    // 3. 准备 CSV 输出文件
    std::ofstream outFile("BPS_Experiment_Results1.csv");
    outFile << "数据集,方向,迁越次数,窗口占比,查询时间,原始分段数量,EPS分段数量,减少的分段数量,震荡区间,冗余数量,分段压缩率%,数据查询率%" << std::endl;

    // 5. 处理本地数据集
    for (const auto& dataset : local_datasets) {
        const std::string& filePath = dataset.first;
        int default_K_val = dataset.second;

        std::string datasetName = filePath.substr(filePath.find_last_of('/') + 1);
        std::cout << "\n>>> 正在处理本地数据集: " << datasetName << " <<<" << std::endl;

        std::vector<Point> arr_x = readPointsFromFile(filePath);
        if (arr_x.empty()) continue;

        int K_val = default_K_val; // 使用默认 K_val
        int num_points = arr_x.size();
        std::cout << "K_val: " << K_val << ", Number of Points: " << num_points << std::endl;

        // --- 执行秩空间映射预处理 ---
        sortPointsX(arr_x);
        std::vector<Point> arr_y = arr_x;
        sortPointsYi(arr_y, K_val);
        arr_x = arr_y;
        sortPointsXi(arr_x);

        std::vector<bitmask_t> a;
        a.reserve(num_points);
        for (const auto& p : arr_x) a.push_back(p.curveval);
        std::sort(a.begin(), a.end());

        // 实验参数（与生成数据集相同）
        std::vector<int> lambdas = { 1,2,3,4,5,6 };
        std::vector<double> selectivities = { 0.001 ,0.005, 0.01, 0.05, 0.1, 0.2 };
        int iterations = 50;

        auto generateValidWindow = [&](double selectivity) {
            long long L_eff = num_points;
            double target_area = (double)L_eff * L_eff * selectivity;
            int side = static_cast<int>(std::sqrt(target_area));
            int x1 = rand() % (L_eff - side);
            int y1 = rand() % (L_eff - side);
            return window(x1, y1, x1 + side, y1 + side);
            };

        // 实验循环
        for (double sel : selectivities) {
            std::vector<window> fixed_windows;
            for (int i = 0; i < iterations; ++i) fixed_windows.push_back(generateValidWindow(sel));

            // --- 预计算 S0 ---
            std::vector<int> s0_sizes;
            for (int i = 0; i < iterations; ++i) {
                s0_sizes.push_back(Range_QQ(K_val, fixed_windows[i].x1, fixed_windows[i].y1, fixed_windows[i].x2, fixed_windows[i].y2).size());
            }

            //纯外迁越和纯内迁越分别测试一次
            for (int BS = 0; BS < 2; BS++)
            {
                for (int lb : lambdas) {
                    double sum_time = 0, sum_eps = 0, sum_red = 0, sum_shift = 0, sum_cost = 0, sum_comp = 0, sum_query_rate = 0;
                    double sum_orig = 0;

                    for (int i = 0; i < iterations; ++i) {
                        window W_orig = fixed_windows[i];
                        window W_eps = W_orig;
                        int seg_orig = s0_sizes[i];

                        std::vector<long long> arr_push, arr_pop;
                        auto start_opt = std::chrono::high_resolution_clock::now();

                        // 1. EPS 变换
                        window_move(W_eps, K_val, arr_x, arr_y, arr_push, arr_pop, num_points, BS, lb);

                        double current_shift = std::abs(W_eps.x1 - W_orig.x1) + std::abs(W_eps.x2 - W_orig.x2) +
                            std::abs(W_eps.y1 - W_orig.y1) + std::abs(W_eps.y2 - W_orig.y2);

                        // 2. 生成 S1
                        Sequence S1 = Range_QQ(K_val, W_eps.x1, W_eps.y1, W_eps.x2, W_eps.y2);
                        int seg_eps_count = S1.size();

                        // 3. 修正
                        process_pop(S1, arr_pop);
                        process_push(S1, arr_push);

                        auto end_opt = std::chrono::high_resolution_clock::now();
                        double opt_duration = std::chrono::duration<double, std::micro>(end_opt - start_opt).count();

                        // 4. 查找
                        MatchResult m_res = find_by_heuristic_guess_2d(a, S1);

                        // 指标累加
                        sum_time += (opt_duration + m_res.duration_us);
                        sum_orig += seg_orig;
                        sum_eps += seg_eps_count;
                        sum_red += (seg_orig - seg_eps_count);
                        sum_shift += current_shift;
                        sum_cost += (arr_push.size() + arr_pop.size());
                        sum_comp += (seg_orig > 0) ? (double)(seg_orig - seg_eps_count) / seg_orig : 0;
                        sum_query_rate += (double)m_res.data.size() / num_points;
                    }

                    // 写入 CSV 文件
                    outFile << datasetName << ","
                        << BS << ","
                        << lb << ","
                        << sel * 100 << "%,"
                        << sum_time / iterations << ","
                        << sum_orig / iterations << ","
                        << sum_eps / iterations << ","
                        << sum_red / iterations << ","
                        << sum_shift / iterations << ","
                        << sum_cost / iterations << ","
                        << (sum_comp / iterations) * 100.0 << ","
                        << (sum_query_rate / iterations) * 100.0 << std::endl;
                }
            }

            std::cout << datasetName << " [窗口 " << sel * 100 << "%] 测试完毕。" << std::endl;
        }
    }

    outFile.close();
    std::cout << "\n所有实验已完成！结果保存在 EPS_Experiment_Results.csv 中。" << std::endl;
    return 0;
}


//int BPSS(int n, bool border, int N_limit, int k, int exp, int L_k) {
//    if (border != exp) {//往小走
//        for (int i = 0; i < L_k; i++) {
//            if ((n > 0 && !border) || (n > N_limit && border))
//                n = last_k_Zero(n, lsb32_idx(n) + 1);//左下往小走或右上往小走
//        }
//    }
//    else {
//        int N_l = 1 << k;//往大走
//        for (int i = 0; i < L_k; i++) {
//            if ((n < N_l && border) || (n < N_limit && !border))
//                n = n + (1 << lsb32_idx(n));//右上往大走或左下往大走
//        }
//    }
//    return n;
//}
//
//int main()
//{ 
//    int n = 12;
//    std::cout << n << std::endl;
//    n = BPSS(n, true, 1, 25, 0, 1);
//    std::cout << n << std::endl;
//    return 0;
//}
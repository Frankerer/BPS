#include "common.h" 


//编码
//the mappings from L1-coord to its L1-code  从L1坐标到其L1代码的映射
extern char CHM[4][2][2] = { 0,1,3,2,0,3,1,2,2,3,1,0,2,1,3,0 };

//the mappings from L1-coord to its Ln-state 从L1坐标到其Ln状态的映射
extern char CSM[4][2][2] = { 1,0,3,0,0,2,1,1,2,1,2,3,3,3,0,2 };


std::vector<Point> readPointsFromFile(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "错误: 无法打开文件 " << filename << std::endl;
        std::cerr << "请检查文件路径是否正确，或者文件是否存在。" << std::endl;
        exit(1); // 无法读取数据则直接退出
    }

    std::cout << "正在从 " << filename << " 读取数据..." << std::endl;


    double x, y;
    // 循环读取每一行的两个数值
    while (infile >> x >> y) {
        points.emplace_back(x, y);
    }

    infile.close();
    std::cout << "读取完成，共加载 " << points.size() << " 个点。" << std::endl;
    return points;
}
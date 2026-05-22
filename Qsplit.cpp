#include "Qsplit.h"
Sequence S;

void QSplit(char oABCD, bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q);

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void addSequence(bitmask_t mins, bitmask_t maxs) {
	// 如果序列为空，直接添加新段
	if (S.empty()) {
		S.push_back(std::make_pair(mins, maxs));
	}
	else {
		// 获取序列中的最后一个段
		Segment& lastSegment = S.back();

		// 如果最后一个段和新段是连续的，合并它们
		if (lastSegment.second + 1 == mins) {
			lastSegment.second = maxs;
		}
		else {
			// 否则，直接添加新段
			S.push_back(std::make_pair(mins, maxs));
		}
	}
}


bitmask_t determineType(bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q, bitmask_t N2) {
	bitmask_t mid = N2;
	if (x < mid) {
		if (y < mid) {//窗口左下角点在左下角，
			if (x + q <= mid) {
				if (y + p <= mid) { //窗口右上角点在左下角 
					return 7;
				}
				else {    //窗口右上角点在左上角 
					return 3;
				}
			}
			else {
				if (y + p <= mid) { //窗口右上角点在右下角 
					return 2;
				}
				else {    //窗口右上角点在右上角 
					return 0;
				}
			}
		}
		else {//起点左上角 
			if (x + q <= mid) {
				return 5;
			}
			else {
				return 1;
			}
		}
	}
	else {
		if (y < mid) {//起点在右下角 
			if (y + p <= mid) {
				return 8;
			}
			else {
				return 4;
			}
		}
		else {  //起点在右上角 
			return 6;
		}
	}
}


void QSplitA(bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q) {
	bitmask_t N2 = T / 2;
	bitmask_t N4 = N2 * N2;
	int type = determineType(x, y, p, q, N2);
	switch (type) {
	case 0: {
		QSplit('B', N2, mino, x, y, N2 - y, N2 - x);
		QSplit('A', N2, mino + N4, x, 0, y + p - N2, N2 - x);
		QSplit('A', N2, mino + N4 * 2, 0, 0, y + p - N2, x + q - N2);
		QSplit('D', N2, mino + N4 * 3, 0, y, N2 - y, x + q - N2);
		break;
	}
	case 1: {
		QSplit('A', N2, mino + N4, x, y - N2, p, N2 - x);
		QSplit('A', N2, mino + N4 * 2, 0, y - N2, p, x + q - N2);
		break;
	}
	case 2: {
		QSplit('B', N2, mino, x, y, p, N2 - x);
		QSplit('D', N2, mino + N4 * 3, 0, y, p, x + q - N2);
		break;
	}
	case 3: {
		QSplit('B', N2, mino, x, y, N2 - y, q);
		QSplit('A', N2, mino + N4, x, 0, y + p - N2, q);
		break;
	}
	case 4: {
		QSplit('A', N2, mino + N4 * 2, x - N2, 0, y + p - N2, q);
		QSplit('D', N2, mino + N4 * 3, x - N2, y, N2 - y, q);
		break;
	}
	case 5: {
		QSplit('A', N2, mino + N4, x, y - N2, p, q);
		break;
	}
	case 6: {
		QSplit('A', N2, mino + N4 * 2, x - N2, y - N2, p, q);
		break;
	}
	case 7: {
		QSplit('B', N2, mino, x, y, p, q);
		break;
	}
	case 8: {
		QSplit('D', N2, mino + N4 * 3, x - N2, y, p, q);
		break;
	}
	}
}

void QSplitB(bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q) {
	bitmask_t N2 = T / 2;
	bitmask_t N4 = N2 * N2;
	int type = determineType(x, y, p, q, N2);
	switch (type) {
	case 0: {
		QSplit('A', N2, mino, x, y, N2 - y, N2 - x);
		QSplit('B', N2, mino + N4, 0, y, N2 - y, x + q - N2);
		QSplit('B', N2, mino + N4 * 2, 0, 0, y + p - N2, x + q - N2);
		QSplit('C', N2, mino + N4 * 3, x, 0, y + p - N2, N2 - x);
		break;
	}
	case 1: {
		QSplit('B', N2, mino + N4 * 2, 0, y - N2, p, x + q - N2);
		QSplit('C', N2, mino + N4 * 3, x, y - N2, p, N2 - x);
		break;
	}
	case 2: {
		QSplit('A', N2, mino, x, y, p, N2 - x);
		QSplit('B', N2, mino + N4 * 1, 0, y, p, x + q - N2);
		break;
	}
	case 3: {
		QSplit('A', N2, mino, x, y, N2 - y, q);
		QSplit('C', N2, mino + N4 * 3, x, 0, y + p - N2, q);
		break;
	}
	case 4: {
		QSplit('B', N2, mino + N4, x - N2, y, N2 - y, q);
		QSplit('B', N2, mino + N4 * 2, x - N2, 0, y + p - N2, q);
		break;
	}
	case 5: {
		QSplit('C', N2, mino + N4 * 3, x, y - N2, p, q);
		break;
	}
	case 6: {
		QSplit('B', N2, mino + N4 * 2, x - N2, y - N2, p, q);
		break;
	}
	case 7: {
		QSplit('A', N2, mino, x, y, p, q);
		break;
	}
	case 8: {
		QSplit('B', N2, mino + N4, x - N2, y, p, q);
		break;
	}
	}
}

void QSplitC(bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q) {
	bitmask_t N2 = T / 2;
	bitmask_t N4 = N2 * N2;
	int type = determineType(x, y, p, q, N2);
	switch (type) {
	case 0: {
		QSplit('D', N2, mino, 0, 0, y + p - N2, x + q - N2);
		QSplit('C', N2, mino + N4, 0, y, N2 - y, x + q - N2);
		QSplit('C', N2, mino + N4 * 2, x, y, N2 - y, N2 - x);
		QSplit('B', N2, mino + N4 * 3, x, 0, y + p - N2, N2 - x);
		break;
	}
	case 1: {
		QSplit('D', N2, mino, 0, y - N2, p, x + q - N2);
		QSplit('B', N2, mino + N4 * 3, x, y - N2, p, N2 - x);
		break;
	}
	case 2: {
		QSplit('C', N2, mino + N4, 0, y, p, x + q - N2);
		QSplit('C', N2, mino + N4 * 2, x, y, p, N2 - x);
		break;
	}
	case 3: {
		QSplit('C', N2, mino + N4 * 2, x, y, N2 - y, q);
		QSplit('B', N2, mino + N4 * 3, x, 0, y + p - N2, q);
		break;
	}
	case 4: {
		QSplit('D', N2, mino, x - N2, 0, y + p - N2, q);
		QSplit('C', N2, mino + N4, x - N2, y, N2 - y, q);
		break;
	}
	case 5: {
		QSplit('B', N2, mino + N4 * 3, x, y - N2, p, q);
		break;
	}
	case 6: {
		QSplit('D', N2, mino, x - N2, y - N2, p, q);
		break;
	}
	case 7: {
		QSplit('C', N2, mino + N4 * 2, x, y, p, q);
		break;
	}
	case 8: {
		QSplit('C', N2, mino + N4, x - N2, y, p, q);
		break;
	}
	}
}

void QSplitD(bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q) {
	bitmask_t N2 = T / 2;
	bitmask_t N4 = N2 * N2;
	int type = determineType(x, y, p, q, N2);
	switch (type) {
	case 0: {
		QSplit('C', N2, mino, 0, 0, y + p - N2, x + q - N2);
		QSplit('D', N2, mino + N4, x, 0, y + p - N2, N2 - x);
		QSplit('D', N2, mino + N4 * 2, x, y, N2 - y, N2 - x);
		QSplit('A', N2, mino + N4 * 3, 0, y, N2 - y, x + q - N2);
		break;
	}
	case 1: {
		QSplit('C', N2, mino, 0, y - N2, p, x + q - N2);
		QSplit('D', N2, mino + N4, x, y - N2, p, N2 - x);
		break;
	}
	case 2: {
		QSplit('D', N2, mino + N4 * 2, x, y, p, N2 - x);
		QSplit('A', N2, mino + N4 * 3, 0, y, p, x + q - N2);
		break;
	}
	case 3: {
		QSplit('D', N2, mino + N4, x, 0, y + p - N2, q);
		QSplit('D', N2, mino + N4 * 2, x, y, N2 - y, q);
		break;
	}
	case 4: {
		QSplit('C', N2, mino, x - N2, 0, y + p - N2, q);
		QSplit('A', N2, mino + N4 * 3, x - N2, y, N2 - y, q);
		break;
	}
	case 5: {
		QSplit('D', N2, mino + N4, x, y - N2, p, q);
		break;
	}
	case 6: {
		QSplit('C', N2, mino, x - N2, y - N2, p, q);
		break;
	}
	case 7: {
		QSplit('D', N2, mino + N4 * 2, x, y, p, q);
		break;
	}
	case 8: {
		QSplit('A', N2, mino + N4 * 3, x - N2, y, p, q);
		break;
	}
	}
}


void QSplit(char oABCD, bitmask_t T, bitmask_t mino, bitmask_t x, bitmask_t y, bitmask_t p, bitmask_t q) {
	if (T == p && T == q) {
		addSequence(mino, mino + T * T - 1);
	}
	else {
		switch (oABCD) {
		case 'A': {
			QSplitA(T, mino, x, y, p, q);
			break;
		}
		case 'B': {
			QSplitB(T, mino, x, y, p, q);
			break;
		}
		case 'C': {
			QSplitC(T, mino, x, y, p, q);
			break;
		}
		case 'D': {
			QSplitD(T, mino, x, y, p, q);
			break;
		}
		}
	}
}

// 打印序列的函数
void printSequence(Sequence S) {
	//    for (const auto &segment : S) {
	//        std::cout << "(" << segment.first << ", " << segment.second << ") ";
	//    }
	//    std::cout << std::endl;
	//    
		// 使用 auto 关键字获取迭代器的类型
	int i = 0;
	for (auto it = S.begin(); it != S.end(); ++it) {
		// 'it' 是一个迭代器，指向当前元素，通过 it-> 来访问元素
		// 例如，it->first 访问 Segment 中的第一个元素，即起始点
		// it->second 访问 Segment 中的第二个元素，即结束点
		std::cout << "Start: " << it->first << ", End: " << it->second << "  i  =" << i++ << std::endl;
	}

}


Sequence Range_QQ(int k, bitmask_t x1, bitmask_t y1, bitmask_t x2, bitmask_t y2) {
	// 获取当前时间点
	S.clear();
	//auto start_time = std::chrono::high_resolution_clock::now();
	//输入数据（hilbert方向，hilbert边长，最小hilbert阶，窗口x,窗口y,窗口高，窗口宽） 
	QSplit('A', 1 << k, 0, x1, y1, y2 - y1, x2 - x1);

	// 获取结束时间点
	//auto end_time = std::chrono::high_resolution_clock::now();

	// 计算执行时间（微秒）
	//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	//std::cout << "总分段数量：" << S.size() << std::endl;

	// 输出执行时间
	//std::cout << "Execution time: " << duration.count() << " 微秒" << std::endl;
	//printSequence(S);

	return S;

}
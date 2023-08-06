
#include "eXCalc.h"
#include <iostream>
#include <random>
#include <time.h>
#include <iomanip>
#include <vector>
/**
 * @brief 尝试升级一次
 * 
 * @return true 成功了
 * @return false 失败了
 */
#define SENSITIVITY 1000
bool Try(float win_rate) {
	std::random_device seed;
	std::ranlux48 engine(seed());
	std::uniform_int_distribution<> distrib(0,SENSITIVITY);
	return win_rate*SENSITIVITY > distrib(seed);
}

void TryInit() {

}

/**
 * @brief 模拟一次的模型
 * 
 * @param start_level 开始等级
 * @param target_level 目标等级
 * @param win_rate 升级概率
 * @return int 到达target_level所花费次数
 */
int Modle(int start_level, int target_level, float win_rate) {
	int now_level = start_level;
	int try_cnt = 0;
	int up_cnt = 0;
	int down_cnt = 0;
	while (now_level < target_level) {
		if (Try(win_rate)) {
			up_cnt ++;
			now_level ++;
		} else {
			down_cnt ++;
			now_level --;
			if (now_level < start_level) {
				now_level = start_level;
			}
		}
		try_cnt ++;
	}
	// std::cout 	<< "try_cnt = "<< try_cnt
	// 			<< "\tup_cnt = "<< up_cnt 
	// 			<< "\tdown_cnt = "<< down_cnt 
	// 			<< "\ttrue_win_rate ="<< std::setprecision(5) <<  (float)up_cnt/try_cnt*100 << " %" 
	// 			<< std::endl;
	return try_cnt;
}

int main(int argc, char const* argv[]) {

	int try_times = 100000;
	int start_level = 10;
	int target_level = 15;
	float win_rate = 0.2f;

	std::cout << "\ntry_times : ";
	std::cin >> try_times;
	std::cout << "start_level : ";
	std::cin >> start_level;
	std::cout << "target_level : ";
	std::cin >> target_level;
	std::cout << "win_rate (%) : ";
	std::cin >> win_rate;
	win_rate /= 100;
	std::vector<int> list_cnt;
	for (int i=0; i<try_times; i++) {
		list_cnt.push_back(Modle(10,15,win_rate));
		if (i%(try_times/100) == 0) {
			std::cout 	<< "calculate  : "<< std::fixed << std::setprecision(0) << (float) i /try_times * 100 << " %" 
						<< std::endl;
		}
	}
	int sum = std::accumulate(list_cnt.begin(), list_cnt.end(), 0.0);
	float mean = (float) sum / list_cnt.size();
	std::cout 	<< "Done!" << std::endl
				<< std::fixed << std::setprecision(2) << "mean = "<< mean
				<< std::endl;
	std::cin.get();
	return 0;
}

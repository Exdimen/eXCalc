
#include "eXCalc.h"
#include <iostream>
#include <random>
#include <time.h>
#include <iomanip>
#include <vector>
#include <chrono>
#include <thread>

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

void SleepMs(int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
void WaitInput() {
	std::cin.sync();
	std::cin.get();
}
void ManualTry() {
	WaitInput();
}
/**
 * @brief 模拟一次的模型
 * 
 * @param start_level 开始等级
 * @param target_level 目标等级
 * @param win_rate 升级概率
 * @param is_display 是否显示过程
 * @return int 到达target_level所花费次数
 */
#define DELAY_TIMES 3
int Modle(int start_level, int target_level, float win_rate, int now_level, bool is_display, int manual = 0) {
	if (is_display) {
		std::cout 	<< "---------------------------------"<< std::endl
					<< "Start!" << std::endl << "\t";
		for (int i=start_level; i<target_level; i++) {
			std::cout << "\t" << i;
		}
		std::cout << std::endl;
	}
	// int now_level = start_level;
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
		if (is_display) {
			std::cout << try_cnt << "\t";
			for (int i=0; i<now_level-start_level; i++) {
				std::cout << "\t";
			}
			std::cout << "@";
			SleepMs(10);
			int delta_level = target_level - now_level;
			if (delta_level < DELAY_TIMES) {
				SleepMs((DELAY_TIMES-delta_level)*500);
			}
			if (delta_level <= manual && delta_level != 0) {
				std::cout << "Try!";
				ManualTry();
			}
			std::cout << std::endl;
		}
	}
	if (is_display) {
		std::cout 	<< "---------------------------------"<< std::endl
					<< "Total Try : \t"<< try_cnt
					<< "\nSuccess : \t"<< up_cnt 
					<< "\nFailed : \t"<< down_cnt 
					<< "\nSuccess Frequency : \t"<< std::setprecision(5) <<  (float)up_cnt/try_cnt*100 << " %" 
					<< std::endl;
					
	}
	return try_cnt;
}

int main(int argc, char const* argv[]) {

	int try_times = 1;
	int start_level = 10;
	int target_level = 15;
	int now_level = 10;
	float win_rate = 20.0f;

	std::cout << "\nTry Times : ";
	std::cin >> try_times;
	std::cout << "Start Level : ";
	std::cin >> start_level;
	std::cout << "Now Level : ";
	std::cin >> now_level;
	std::cout << "Target Level : ";
	std::cin >> target_level;
	std::cout << "Win Rate (%) : ";
	std::cin >> win_rate;

	win_rate /= 100;
	std::vector<int> list_cnt;

	if (try_times == 1) {
		// 	手气不错模式
		int manual_num = 0;
		std::cout << "\nManual Num : ";
		std::cin >> manual_num;
		while(1) {
			list_cnt.push_back(Modle(start_level,target_level,win_rate,now_level,true,manual_num));
			SleepMs(500);
			std::cout << "\nTry Again?(y/n) : ";
			if (std::getchar() == 'n') {	
				break;
			}
			std::cin.sync();
		}
	} else {
		// 求期望模式
		for (int i=0; i<try_times; i++) {
			list_cnt.push_back(Modle(start_level,target_level,win_rate,now_level,false));
			if (try_times >= 100 && (i+1)%(try_times/100) == 0) {
				std::cout 	<< "calculate  : "<< std::fixed << std::setprecision(0) << (float) i /try_times * 100 << " %" 
							<< std::endl;
			}
		}
	}


	int sum = std::accumulate(list_cnt.begin(), list_cnt.end(), 0.0);
	float mean = (float) sum / list_cnt.size();
	std::cout 	<< "Done!" << std::endl
				<< std::fixed << std::setprecision(2) << "mean = "<< mean
				<< std::endl;
	WaitInput();
	return 0;
}

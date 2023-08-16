
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
#define REINFORCE_DELTA_LEVEL 2
CoreCnt Modle(int start_level, int target_level, float win_rate, int now_level, bool is_reinforce, bool is_display, int manual = 0) {
	if (is_display) {
		std::cout 	<< "---------------------------------"<< std::endl
					<< "Start!" << std::endl << "\t";
		for (int i=start_level; i<target_level; i++) {
			std::cout << "\t" << i;
		}
		std::cout << std::endl;
	}
	// int now_level = start_level;

	CoreCnt output;

	output.try_cnt = 0;
	output.reinforce_cnt = 0;

	int up_cnt = 0;
	int down_cnt = 0;

	int delta_level;
	float final_win_rate;
	while (now_level < target_level) {
		// 使用强化剂
		delta_level = target_level - now_level;
		final_win_rate = win_rate;
		if (is_reinforce && delta_level <= 4) {
			final_win_rate += 0.05f;
			output.reinforce_cnt ++;
		}
		if (Try(final_win_rate)) {
			up_cnt ++;
			now_level ++;
		} else {
			down_cnt ++;
			now_level --;
			if (now_level < start_level) {
				now_level = start_level;
			}
		}
		output.try_cnt ++;
		if (is_display) {
			std::cout << output.try_cnt << "\t";
			for (int i=0; i<now_level-start_level; i++) {
				std::cout << "\t";
			}
			std::cout << "@";
			SleepMs(10);
			delta_level = target_level - now_level;
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
					<< "Total Try : \t"<< output.try_cnt
					<< "\nSuccess : \t"<< up_cnt 
					<< "\nFailed : \t"<< down_cnt 
					<< "\nReinforce Cnt : " << output.reinforce_cnt
					<< "\nSuccess Frequency : \t"<< std::setprecision(5) <<  (float)up_cnt/output.try_cnt*100 << " %" 
					<< std::endl;
					
	}
	return output;
}

int main(int argc, char const* argv[]) {

	int try_times = 1;
	int start_level = 10;
	int target_level = 15;
	int now_level = 10;
	float win_rate = 20.0f;
	bool is_reinforce = false;


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
	std::cout << "Is Enable Reinforce?(y) : ";
	std::cin.sync();
	if (std::cin.get() == 'y') {
		std::cout << "Enable Reinforce" << std::endl;
		is_reinforce = true;
	}
	win_rate /= 100;

	std::vector<int> trycnt_list;
	std::vector<int> reinforcecnt_list;
	if (try_times == 1) {
		// 	手气不错模式
		int manual_num = 0;
		std::cout << "\nManual Num : ";
		std::cin >> manual_num;
		while(1) {
			CoreCnt output = Modle(start_level,target_level,win_rate,now_level,is_reinforce,true,manual_num);
			trycnt_list.push_back(output.try_cnt);
			reinforcecnt_list.push_back(output.reinforce_cnt);
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
			CoreCnt output = Modle(start_level,target_level,win_rate,now_level,is_reinforce,false);
			trycnt_list.push_back(output.try_cnt);
			reinforcecnt_list.push_back(output.reinforce_cnt);
			if (try_times >= 100 && (i+1)%(try_times/100) == 0) {
				std::cout 	<< "Calculate  : "<< std::fixed << std::setprecision(0) << (float) i /try_times * 100 << " %" 
							<< std::endl;
			}
		}
	}


	int trycnt_sum = std::accumulate(trycnt_list.begin(), trycnt_list.end(), 0.0);
	float trycnt_mean = (float) trycnt_sum / trycnt_list.size();
	int  reinforcecnt_sum = std::accumulate(reinforcecnt_list.begin(), reinforcecnt_list.end(), 0.0);
	float  reinforcecnt_mean = (float)  reinforcecnt_sum / reinforcecnt_list.size();
	std::cout 	<< "Done!" << std::endl
				<< std::fixed << std::setprecision(2) << "Try Cnt Mean : "<< trycnt_mean
				<< std::endl
				<< std::fixed << std::setprecision(2) << "Reinforce Cnt Mean : "<< reinforcecnt_mean
				<< std::endl;
	WaitInput();
	return 0;
}

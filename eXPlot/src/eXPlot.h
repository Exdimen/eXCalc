// eXPlot.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>

const int N = 100000;

enum PlotType {delta_mode, absoulte_mode};
struct PlotData {
	float t[4][N];		// 0 - us; 1 - ms; 2 - s; 3 - min
	float x[N];
	float y[N];
	float p[N];
	float q[N];
	float plot_t[N];	// 显示的时间轴
	float plot_x[N];
	float plot_y[N];
	float plot_p[N];
	float plot_q[N];
    int chennal_num;
};
extern PlotData pdata;
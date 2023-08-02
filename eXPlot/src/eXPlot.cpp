#include "App.h"
#include <stdio.h>

#include <queue>
#include <thread>
#include <random>
#include "implot_internal.h"


#include "SerialInterface.h"
#include "eXPlot.h"
#include "ConnectInterface.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) 
# pragma execution_character_set("utf-8") 
#endif

std::queue<int> qData;

// itas109::CSerialPort serial_port;
// SerialInterface serial_interface(&serial_port);

PlotData pdata = {0};

struct ImLearn : public App {

	using App::App;

	ImFont* cn_font;

	void Start() override {
	}

	void Update() override {
		
		ImPlotContext& gp = *ImPlot::GetCurrentContext();
		ImPlotPlot& plot = *gp.CurrentPlot;

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(GetWindowSize(), ImGuiCond_Always);


		ImGui::Begin("Screen", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

		ImGui::Text("端口号：");
		ImGui::SameLine();
			//设置下拉列表框 & 枚举选项
		static std::string com = "NULL";
		std::vector<SerialPortInfo> serialList;
		ImGui::SetNextItemWidth(300);
		if (ImGui::BeginCombo(" ", com.c_str())) {
			// 获取串口列表
			serialList = CSerialPortInfo::availablePortInfos();
			// EnumSerial(serialList);
			
			if (serialList.size() == 0) ImGui::Selectable("NULL", false);
			else {
				for (int i = 0; i < serialList.size(); ++i) {
					bool isSelect = serialList[i].portName == com;
					if (ImGui::Selectable(serialList[i].portName, isSelect)) {
						com = serialList[i].portName;
					}
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PushItemWidth(300);
		ImGui::SameLine();
		static int baud_rate = 115200;
		ImGui::Text("波特率：");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		ImGui::InputInt("  ", &baud_rate, 100, 1000);
		ImGui::PushItemWidth(100);
		ImGui::SameLine();
		// 点击连接按钮事件
		std::string strState;
		if (serial_port->isOpen()) {
			ImGui::SameLine();
			if (ImGui::Button("断开")) {
				serial_port->close();
				serial_port->disconnectReadEvent();
			}
		}
		else {
			if (ImGui::Button("连接")) {
				serial_port->init(com.c_str(),baud_rate);
				serial_port->open();
				serial_port->connectReadEvent(serial_interface);
			}
		}

		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * 0.85f);
		ImGui::Text("eXPlot V0.0.2 Exdimen");

		// 状态栏标题行
		{ float delta_line = 0.15f;
			ImGui::Text("串口状态");
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * delta_line);
			ImGui::Text("Tx字节数");
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * delta_line*2);
			ImGui::Text("Rx字节数");
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * delta_line*3);
			ImGui::Text("发送数据速率Kbps");
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * delta_line*4);
			ImGui::Text("接收数据速率Kbps");


		// 状态栏状态行
			if (serial_port->isOpen()) {
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Connect");
			}
			else {
				ImGui::TextDisabled("Disconnect");
			}
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * delta_line);
			ImGui::Text("%d",serial_interface->get_tx_cnt());
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * delta_line*2);
			ImGui::Text("%d",serial_interface->get_rx_cnt());
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * delta_line*3);
			ImGui::Text("%.2f",serial_interface->get_tx_speed());
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * delta_line*4);
			ImGui::Text("%.2f",serial_interface->get_rx_speed());
		}
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * 0.85f);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

		ImGui::Separator();
		static int plot_N = 1000;
		static int time_scale = 1;		// 时间单位
		static bool is_stop = false;
		static bool is_x_autofit = true;
		static bool is_y_autofit = true;

		static char x_label_buf[32] = "x(t)";
		static char y_label_buf[32] = "y(t)";
		static char p_label_buf[32] = "p(t)";
		static char q_label_buf[32] = "q(t)";
		static int plot_mode = 0;			// 1 - 时域模式 2 - 空间向量模式
		ImGui::BeginChild("PlotParam", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.2f, -1));{

			if (ImGui::CollapsingHeader("数据设置")) {
				ImGui::InputText("通道x", x_label_buf, IM_ARRAYSIZE(x_label_buf));
				ImGui::InputText("通道y", y_label_buf, IM_ARRAYSIZE(y_label_buf));
				ImGui::InputText("通道p", p_label_buf, IM_ARRAYSIZE(p_label_buf));
				ImGui::InputText("通道q", q_label_buf, IM_ARRAYSIZE(q_label_buf));
				ImGui::Separator();
			}

			static bool is_delta_mode = false;
			if (ImGui::CollapsingHeader("图像设置")) {
				ImGui::SetNextItemWidth(100);
				ImGui::Combo("<- 显示模式", &plot_mode, "时域\0空间向量");
				ImGui::PushItemWidth(100);
				if (plot_mode == 0) {
					ImGui::Checkbox("相对模式", &is_delta_mode);
				}
				ImGui::Separator();
			}
			ImGui::Text("显示数据数：");
			const int min_plot_N = 2;
			const int max_plot_N = N;
			ImGui::SliderScalar(" ", ImGuiDataType_S32, &plot_N,  &min_plot_N, &max_plot_N, NULL, ImGuiSliderFlags_Logarithmic);
			ImGui::InputInt("  ", &plot_N, min_plot_N, max_plot_N);
			ImGui::SetNextItemWidth(100);
			ImGui::Combo("<- 时间单位", &time_scale, "us\0ms\0s\0min");
			ImGui::PushItemWidth(100);


			ImGui::Text("自动调节：");
			static bool is_all_autofit = true;
			static bool last_is_all_autofit = is_all_autofit;
			static bool last_is_x_autofit = is_x_autofit;
			static bool last_is_y_autofit = is_y_autofit;
			ImGui::Checkbox("时间轴", &is_x_autofit);
			ImGui::Checkbox("数据轴", &is_y_autofit);
			ImGui::Checkbox("所有轴", &is_all_autofit);
			if (is_x_autofit != last_is_x_autofit 
				|| is_y_autofit != last_is_y_autofit) {
				if (!is_x_autofit || !is_y_autofit) {
					is_all_autofit = false;
				}
				if (is_x_autofit && is_y_autofit) {
					is_all_autofit = true;
				}
			}
			else if (is_all_autofit != last_is_all_autofit) {
				if (is_all_autofit) {
					is_x_autofit = true;
					is_y_autofit = true;
				} else {
					is_x_autofit = false;
					is_y_autofit = false;
				}
				
			}
			last_is_all_autofit = is_all_autofit;
			last_is_x_autofit = is_x_autofit;
			last_is_y_autofit = is_y_autofit;


			ImGui::Text("坐标系：");

			if (ImGui::Button("清除数据")) {
				for (int i=0; i < N; i++) {
					pdata.x[i] = 0;
					pdata.y[i] = 0;
					pdata.p[i] = 0;
					pdata.q[i] = 0;
					for (int j=0; j<4; j++) {
						pdata.t[j][i] = 0;
					}
				}
			}
			ImGui::SameLine();
			static bool is_black_style = true;
			if (is_black_style) {
				if (ImGui::Button("亮色模式")) {
					ImPlot::StyleColorsLight();
					is_black_style = false;
				}
			} else {
				if (ImGui::Button("暗色模式")) {
					ImPlot::StyleColorsDark();
					is_black_style = true;
				}
			}
			
			if (is_stop) {
				if (ImGui::Button("继续",ImVec2(ImGui::GetWindowContentRegionWidth(), 30))) {
					is_stop = false;
				}
			} else {
				// 数据处理
				switch (plot_mode) {
					case 0:	{	// 时域模式 
						for (int i=0; i< plot_N; i++) {
							if (pdata.chennal_num >= 1) {
								pdata.plot_x[i] = pdata.x[i];
							}
							if (pdata.chennal_num >= 2) {
								pdata.plot_y[i] = pdata.y[i];
							}
							if (pdata.chennal_num >= 3) {
								pdata.plot_p[i] = pdata.p[i];
							}
							if (pdata.chennal_num >= 4) {
								pdata.plot_q[i] = pdata.q[i];
							}
							if (is_delta_mode) {
								// 时间轴相对模式
								pdata.plot_t[i] = pdata.t[time_scale][i] - pdata.t[time_scale][0];
							} else {
								// 时间轴绝对模式
								pdata.plot_t[i] = pdata.t[time_scale][i];
							}

						}
						break;
					}
					case 1: {	//空间向量
						pdata.plot_x[0] = 0;
						pdata.plot_y[0] = 0;
						pdata.plot_p[0] = 0;
						for (int i = N-1; i > 0; i--) {
							pdata.x[i] = pdata.x[i-1];
							pdata.y[i] = pdata.y[i-1];
							pdata.p[i] = pdata.p[i-1];
						}
						for (int i = 1; i < plot_N; i++) {
							pdata.plot_x[i] = -pdata.p[i-1];
							pdata.plot_y[i] = -pdata.x[i-1];
							pdata.plot_p[i] = -pdata.y[i-1];
						}
						break;						
					}
				}
				if (ImGui::Button("暂停",ImVec2(ImGui::GetWindowContentRegionWidth(), 30))) {
					is_stop = true;
				}


			}

		} ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("PlotMain", ImVec2(0, -1)); {


			// plot waveforms
			if (ImPlot::BeginPlot("Data",ImVec2(-1,-1))) {
				ImPlotAxisFlags x_axis_param;
				ImPlotAxisFlags y_axis_param;
				if (is_x_autofit) {
					x_axis_param = ImPlotAxisFlags_AutoFit;
				} else {
					x_axis_param = ImPlotAxisFlags_None;
				}
				if (is_y_autofit) {
					y_axis_param = ImPlotAxisFlags_AutoFit;
				} else {
					y_axis_param = ImPlotAxisFlags_None;
				}
				switch (plot_mode) {
					case 0:	{	// 时域模式 
						switch (time_scale) {
							case 0:
								ImPlot::SetupAxes("Time [us]","Signal", x_axis_param, y_axis_param);
								break;
							case 1:
								ImPlot::SetupAxes("Time [ms]","Signal", x_axis_param, y_axis_param);
								break;
							case 2:
								ImPlot::SetupAxes("Time [s]","Signal", x_axis_param, y_axis_param);
								break;
							case 3:
								ImPlot::SetupAxes("Time [min]","Signal", x_axis_param, y_axis_param);
								break;
						}
						ImPlot::GetStyle().LineWeight = 2;
						ImPlot::SetupLegend(ImPlotLocation_NorthEast);

						// ImHasFlag& plot = 
						if (pdata.chennal_num >= 1) {
							ImPlot::PlotLine(x_label_buf, pdata.plot_t, pdata.plot_x, plot_N, 0);
						}
						if (pdata.chennal_num >= 2) {
							ImPlot::PlotLine(y_label_buf, pdata.plot_t, pdata.plot_y, plot_N, 0);
						}
						if (pdata.chennal_num >= 3) {
							ImPlot::PlotLine(p_label_buf, pdata.plot_t, pdata.plot_p, plot_N, 0);
						}
						if (pdata.chennal_num >= 4) {
							ImPlot::PlotLine(q_label_buf, pdata.plot_t, pdata.plot_q, plot_N, 0);
						}
						break;
					}
					case 1: {	//空间向量
						ImPlot::SetupAxes("A","B", x_axis_param, y_axis_param);
												ImPlot::GetStyle().LineWeight = 2;
						ImPlot::SetupLegend(ImPlotLocation_NorthEast);
						ImPlot::PlotLine("X-Y", pdata.plot_x, pdata.plot_y, plot_N, 0);
						ImPlot::PlotLine("X-Z", pdata.plot_x, pdata.plot_p, plot_N, 0);
						ImPlot::PlotLine("Y-Z", pdata.plot_y, pdata.plot_p, plot_N, 0);
						break;
					}
				}



				ImPlot::EndPlot();
			}
		} ImGui::EndChild();
		ImGui::End();
	}
};



void App(int argc, char const* argv[]) {
	ImLearn app("eXPlot", 1920, 1080, argc, argv);
	app.Run();
}


int main(int argc, char const* argv[]) {
	std::thread connect(Connect);
	std::thread app(App, argc, argv);
	connect.join();
	app.join();
	return 0;
}

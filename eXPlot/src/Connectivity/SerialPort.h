
#ifndef _SERIAL_20190304_H
#define _SERIAL_20190304_H
 
#include <string>
#include <Windows.h>
#include <stdint.h>
#include <queue>
#include <mutex>

class CSerialPort
{
    public:
    CSerialPort();
    ~CSerialPort();
    
    public:
    void setConfig(const std::string& portNum, DWORD baudRate, BYTE byteSize, BYTE parityBit, BYTE stopBit);
    bool openComm(); // 打开串口
    void closeComm(); // 关闭串口
    bool writeToComm(char* pdata); // 发送数据
    bool readFromComm(uint8_t* buffer);
    bool readFromComm(char buffer[], DWORD dwLength); // 读取数据
    
    void SetRxCntPlus() {
        std::lock_guard<std::mutex> lock(mtx);
        rx_cnt ++;
    }
    void UpdateRxSpeed();
    public:
        // 线程共享资源
        bool m_bOpen;       // 串口开关标志
        int rx_cnt;
        int tx_cnt;
        bool command_close; // 关闭命令
        float rx_speed;     // 接收速度 kbps
        std::mutex mtx;     // 互斥锁
    private:
    
    HANDLE m_hComm; // 通信设备
    std::string m_portNum; // 串口号
    DWORD m_dwBaudRate; // 波特率
    BYTE m_byteSize; // 数据位
    BYTE m_parityBit; // 校验位
    BYTE m_stopBit; // 停止位
    
    private:
   
    enum BufferSize
    {
        MIN_BUFFER_SIZE = 256,
        BUFFER_SIZE = 512,
        MAX_BUFFER_SIZE = 1024
    };
    
    // 设置串口号
    void setPortNum(const std::string& portNum)
    {
        this->m_portNum = portNum;
    }
    // 设置波特率
    void setBaudRate(const uint32_t baudRate)
    {
        this->m_dwBaudRate = baudRate;
    }
    // 设置数据位
    void setByteSize(const uint8_t byteSize)
    {
        this->m_byteSize = byteSize;
    }
    // 设置检验位
    void setParityBit(const uint8_t parityBit)
    {
        this->m_parityBit = parityBit;
    }
    // 设置停止位
    void setStopBit(const uint8_t stopBit)
    {
        this->m_stopBit = stopBit;
    }
    void resetCnt() {
        std::lock_guard<std::mutex> lock(mtx);
        tx_cnt = 0;
        rx_cnt = 0;
    }
    // 获取串口号
    std::string getPortNum() { return m_portNum; }
    // 获取波特率
    uint32_t getBaudRate() { return m_dwBaudRate; }
    // 获取数据位
    uint8_t getByteSize() { return m_byteSize; }
    // 获取检验位
    uint8_t getParityBit() { return m_parityBit; }
    // 获取停止位
    uint8_t getStopBit() { return m_stopBit; }
    int getRxCnt() {
        std::lock_guard<std::mutex> lock(mtx);
        return rx_cnt;
    }
    // 

};

static void EnumSerial(std::vector<std::string>& serialList) {
	HKEY hKey;
	LPCTSTR data_Set = TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");
	long ret0 = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey));
	if (ret0 != ERROR_SUCCESS)	return;

	DWORD Name[100];
	UCHAR szPortName[100];
	LONG  Status = 1;
	DWORD dwIndex = 0;
	DWORD dwName;
	DWORD dwSizeofPortName;
	DWORD Type;

	for (DWORD i = 0;; ++i) {
		dwSizeofPortName = dwName = 100;
		Status = RegEnumValue(hKey, i, (LPSTR) Name, &dwName, NULL, &Type, szPortName, &dwSizeofPortName);
		if ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA)) {
			std::string z = "";
			for (int i = 0; i < dwSizeofPortName; i += 1) {
				z += szPortName[i];
			}
			serialList.push_back(z);
		}
		else break;
	}
	RegCloseKey(hKey);
}
#endif

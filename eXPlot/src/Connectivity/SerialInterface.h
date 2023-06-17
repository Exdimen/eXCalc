
// #ifndef _SERIAL_20190304_H
// #define _SERIAL_20190304_H
 
// #include <string>
// #include <Windows.h>
// #include <stdint.h>
// #include <queue>
// #include <mutex>

// class CSerialPort
// {
//     public:
//     CSerialPort();
//     ~CSerialPort();
    
//     public:
//     void setConfig(const std::string& portNum, DWORD baudRate, BYTE byteSize, BYTE parityBit, BYTE stopBit);
//     bool openComm(); // 打开串口
//     void closeComm(); // 关闭串口
//     bool writeToComm(char* pdata); // 发送数据
//     bool readFromComm(uint8_t* buffer);
//     bool readFromComm(char buffer[], DWORD dwLength); // 读取数据
    
//     void SetRxCntPlus() {
//         std::lock_guard<std::mutex> lock(mtx);
//         rx_cnt ++;
//     }
//     void UpdateRxSpeed();
//     public:
//         // 线程共享资源
//         bool m_bOpen;       // 串口开关标志
//         int rx_cnt;
//         int tx_cnt;
//         bool command_close; // 关闭命令
//         float rx_speed;     // 接收速度 kbps
//         std::mutex mtx;     // 互斥锁
//     private:
    
//     HANDLE m_hComm; // 通信设备
//     std::string m_portNum; // 串口号
//     DWORD m_dwBaudRate; // 波特率
//     BYTE m_byteSize; // 数据位
//     BYTE m_parityBit; // 校验位
//     BYTE m_stopBit; // 停止位
    
//     private:
   
//     enum BufferSize
//     {
//         MIN_BUFFER_SIZE = 256,
//         BUFFER_SIZE = 512,
//         MAX_BUFFER_SIZE = 1024
//     };
    
//     // 设置串口号
//     void setPortNum(const std::string& portNum)
//     {
//         this->m_portNum = portNum;
//     }
//     // 设置波特率
//     void setBaudRate(const uint32_t baudRate)
//     {
//         this->m_dwBaudRate = baudRate;
//     }
//     // 设置数据位
//     void setByteSize(const uint8_t byteSize)
//     {
//         this->m_byteSize = byteSize;
//     }
//     // 设置检验位
//     void setParityBit(const uint8_t parityBit)
//     {
//         this->m_parityBit = parityBit;
//     }
//     // 设置停止位
//     void setStopBit(const uint8_t stopBit)
//     {
//         this->m_stopBit = stopBit;
//     }
//     void resetCnt() {
//         std::lock_guard<std::mutex> lock(mtx);
//         tx_cnt = 0;
//         rx_cnt = 0;
//     }
//     // 获取串口号
//     std::string getPortNum() { return m_portNum; }
//     // 获取波特率
//     uint32_t getBaudRate() { return m_dwBaudRate; }
//     // 获取数据位
//     uint8_t getByteSize() { return m_byteSize; }
//     // 获取检验位
//     uint8_t getParityBit() { return m_parityBit; }
//     // 获取停止位
//     uint8_t getStopBit() { return m_stopBit; }
//     int getRxCnt() {
//         std::lock_guard<std::mutex> lock(mtx);
//         return rx_cnt;
//     }
//     // 

// };
#pragma once
#include <SerialPort.h>
#include <string>
#include <vector>
#include <sigslot.h>
#include <chrono>
#include <stdint.h>
#include <folly/ProducerConsumerQueue.h>
using namespace itas109;
using namespace std;
static void EnumSerial(std::vector<std::string>& serialList) {
    CSerialPort port;
    for (int i = 0; i <= 255; i++)
    {
        std::string portname = "/dev/ttyS" + std::to_string(i);
        port.init(portname.c_str());
        if (port.isOpen())
        {
            serialList.push_back(portname);
            port.close();
        }
    }
}

namespace edm_comm {
class SerialInterface : public sigslot::has_slots<> {
public:
    SerialInterface (CSerialPort* _serial_port) {
        rx_len = 0;
        rx_cnt = 0;
        last_rx_cnt = 0;
        rx_speed = 0.0f;
        serial_port = _serial_port;
        last_speedcalc_timestamp = chrono::system_clock::now();
    }

    void OnReceiveMessage() {
        if (!serial_port)
            return;
        char read_str[1024];
        rx_len = serial_port->readAllData(read_str);
        if (rx_len > 0) {
            for (int i=0; i<rx_len; i++) {
                str.write(read_str[i]);
            }
            rx_cnt += rx_len;
        }
    }

    bool SerialRecevice(uint8_t* data) {
        if (str.isEmpty())
            return false;
        str.read((char &)*data);
        return true;
    }

    void UpdateRxSpeed() {
        chrono::system_clock::time_point now = chrono::system_clock::now();
        chrono::duration<double, milli> delta_time = now - last_speedcalc_timestamp;
        if (delta_time.count() < 50) 
            return;
        rx_speed = (rx_cnt - last_rx_cnt)*10 / (float)1000.0f / delta_time.count(); // None 8 1速度下
        last_speedcalc_timestamp = chrono::system_clock::now();
        last_rx_cnt = rx_cnt;
    }

    int get_rx_cnt() {return rx_cnt;}
    float get_rx_speed() {return rx_speed;}
private:
    CSerialPort* serial_port;
    folly::ProducerConsumerQueue<char> str{1024};
    // char str[1024];
    int rx_len;
    int rx_cnt;
    float rx_speed;     // 接收速度 kbps
    chrono::system_clock::time_point last_speedcalc_timestamp;
    int last_rx_cnt;
    
};
}

// #endif

#pragma once
#include <CSerialPort/SerialPort.h>
#include <string>
#include <vector>
#include <chrono>
#include <stdint.h>
#include <folly/ProducerConsumerQueue.h>
using namespace itas109;
using namespace std;

namespace edm_comm {
class SerialInterface : public CSerialPortListener {
public:
    SerialInterface (CSerialPort* _serial_port) {
        rx_len = 0;
        rx_cnt = 0;
        last_rx_cnt = 0;
        rx_speed = 0.0f;
        serial_port = _serial_port;
        last_speedcalc_timestamp = chrono::system_clock::now();
    }

    void onReadEvent(const char *portName, unsigned int readBufferLen) {
        if (!serial_port)
            return;
        if (readBufferLen == 0)
            return;
        char *data = new char[readBufferLen];
        rx_len = serial_port->readData(data, readBufferLen);
        if (rx_len > 0) {
            for (int i=0; i<rx_len; i++) {
                str.write(data[i]);
            }
            rx_cnt += rx_len;
        }
        delete[] data;
        data = nullptr;
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
        if (delta_time.count() < 300) 
            return;
        rx_speed = (float) (rx_cnt - last_rx_cnt)*10 / delta_time.count(); // None 8 1速度下
        last_speedcalc_timestamp = chrono::system_clock::now();
        last_rx_cnt = rx_cnt;
    }

    int get_rx_cnt() {return rx_cnt;}
    float get_rx_speed() {return rx_speed;}
private:
    CSerialPort* serial_port;
    folly::ProducerConsumerQueue<char> str{1024};
    int rx_len;
    int rx_cnt;
    float rx_speed;     // 接收速度 kbps
    chrono::system_clock::time_point last_speedcalc_timestamp;
    int last_rx_cnt;
    
};
}

// #endif

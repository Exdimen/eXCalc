#pragma once
#include <CSerialPort/SerialPort.h>
#include <string>
#include <vector>
#include <chrono>
#include <stdint.h>
// #include <folly/ProducerConsumerQueue.h>

#include "ring_buffer.h"

using namespace itas109;
using namespace std;

namespace edm_comm {
class SerialInterface : public CSerialPortListener {
public:
    SerialInterface (CSerialPort* _serial_port) {
        rx_len = 0;
        rx_cnt = 0;
        tx_cnt = 0;
        last_rx_cnt = 0;
        last_tx_cnt = 0;
        rx_speed = 0.0f;
        tx_speed = 0.0f;
        serial_port = _serial_port;
        last_speedcalc_timestamp = chrono::system_clock::now();

        RingBuf_Init(&rx_buffer_handle, (uint8_t*) &rx_buffer, 1024);
    }

    void onReadEvent(const char *portName, unsigned int readBufferLen) {
        if (!serial_port)
            return;
        if (readBufferLen == 0)
            return;
        uint8_t *data = new uint8_t[readBufferLen];
        rx_len = serial_port->readData(data, readBufferLen);
        if (rx_len > 0) {
            RingBuf_Put(&rx_buffer_handle, data, rx_len);
            rx_cnt += rx_len;
        }
        delete[] data;
        data = nullptr;
    }

    bool SerialRecevice(uint8_t* data) {
        uint16_t fb_len;
        fb_len = RingBuf_Get(&rx_buffer_handle, data, 1);
        if (fb_len == 0) {
            return false;
        }
        return true;
    }
    bool SerialTransmit(uint8_t* data, uint16_t len) {
        if (!serial_port)
            return false;
        int ret = serial_port->writeData(data, len);
        if (ret != -1) {
            tx_cnt += ret;
            return true;
        }
        return false;
    }

    void UpdateRxSpeed() {
        chrono::system_clock::time_point now = chrono::system_clock::now();
        chrono::duration<double, milli> delta_time = now - last_speedcalc_timestamp;
        if (delta_time.count() < 300) 
            return;
        rx_speed = (float) (rx_cnt - last_rx_cnt)*10 / delta_time.count(); // None 8 1速度下
        tx_speed = (float) (tx_cnt - last_tx_cnt)*10 / delta_time.count(); // None 8 1速度下
        last_speedcalc_timestamp = chrono::system_clock::now();
        last_rx_cnt = rx_cnt;
        last_tx_cnt = tx_cnt;
    }
    int get_tx_cnt() {return tx_cnt;}
    int get_rx_cnt() {return rx_cnt;}
    float get_rx_speed() {return rx_speed;}
    float get_tx_speed() {return tx_speed;}

private:
    CSerialPort* serial_port;
    // folly::ProducerConsumerQueue<char> str{1024};
    T_RingBuffer rx_buffer_handle;
    uint8_t rx_buffer[1024];

    int rx_len;
    int rx_cnt;
    int tx_cnt;
    float rx_speed;     // 接收速度 kbps
    float tx_speed;     // 发送速率 kbps
    chrono::system_clock::time_point last_speedcalc_timestamp;
    int last_rx_cnt;
    int last_tx_cnt;
    
};
}

// #endif

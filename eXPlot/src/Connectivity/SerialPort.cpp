// #include "stdafx.h"
#include "serialport.h"
#include<windows.h>
#include <thread>
#include <chrono>
using namespace std::chrono;
CSerialPort::CSerialPort()
{
    m_bOpen = false;
    rx_cnt = 0;
    tx_cnt = 0;
    command_close = false;
    rx_speed = 0;
}
 
CSerialPort::~CSerialPort()
{
 
}
 
void CSerialPort::setConfig(const std::string& portNum, DWORD baudRate, BYTE byteSize, BYTE parityBit, BYTE stopBit)
{
    m_portNum = (portNum);
    m_dwBaudRate = (baudRate);
    m_byteSize = (byteSize);
    m_parityBit = (parityBit);
    m_stopBit = (stopBit);
    m_bOpen = (false);
}
 
// 打开串口成功,返回 true
 
bool CSerialPort::openComm()
{
    if(m_bOpen)
        return true;
 
    m_hComm = CreateFileA(m_portNum.c_str(),
        GENERIC_READ | GENERIC_WRITE, //允许读和写
        0, //独占方式
        NULL, OPEN_EXISTING, //打开而不是创建
        0, //同步方式
        NULL
        );
 
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        char szBuf[1024] = { 0 };
        sprintf_s(szBuf, "Open %s failed, Error code: %d", m_portNum.c_str(), GetLastError());
        MessageBox(NULL, szBuf, "Warnning", MB_OK);
 
        return false;
 
    }
    else
    {
        DCB dcb;
        SetupComm(m_hComm, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE); // 设置读写缓冲区大小
        GetCommState(m_hComm, &dcb);
        dcb.BaudRate = m_dwBaudRate;
        dcb.ByteSize = m_byteSize;
        dcb.Parity = m_parityBit;
        dcb.StopBits = m_stopBit;
 
        if (!SetCommState(m_hComm, &dcb))
        {
            char szBuf[1024] = { 0 };
            wsprintf(szBuf, "Com config failed, Error code: %d", GetLastError());
            MessageBox(NULL, szBuf, "ERROR", MB_OK);
 
            return false;
        }
 
    }
 
    //在读写串口前，用 PurgeComm 函数清空缓冲区
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT | PURGE_TXABORT);
 
    m_bOpen = true;
    
    return true;
 
}
 
// 关闭串口
 
void CSerialPort::closeComm()
{
    m_bOpen = false;
    Sleep(200);
    CloseHandle(m_hComm);

}
void CSerialPort::UpdateRxSpeed() {
    static const int delta_t = 100;
    static time_point<high_resolution_clock> last_timestamp = high_resolution_clock::now();
    static int last_rx_cnt = rx_cnt;
    if ( duration_cast<milliseconds>(high_resolution_clock::now() - last_timestamp).count() > delta_t) {
        rx_speed = (float) (rx_cnt - last_rx_cnt) * 8 / delta_t;
        last_timestamp = high_resolution_clock::now();
        last_rx_cnt = rx_cnt;
    }



}


// 向串口发送数据
bool CSerialPort::writeToComm(char* pData)
{
    DWORD dwLength = strlen(pData);
    DWORD dwError = 0;
    if (ClearCommError(m_hComm, &dwError, NULL) && dwError > 0)
    {
        PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR);
    }
 
    DWORD dwTx = 0;
    BOOL ret = FALSE;
    ret = WriteFile(m_hComm, pData, dwLength, &dwTx, NULL);
 
    if (ret == FALSE)
    {
        char szBuf[1024] = { 0 };
        sprintf_s(szBuf, "Tx Error Code: %d", GetLastError());
        MessageBox(NULL, szBuf, "Error", MB_OK);
 
        return false;
    }
 
    return true;
 
 
}
 
// 从串口中读取数据
bool CSerialPort::readFromComm(uint8_t* buffer) {
    return readFromComm((char*) buffer, 1);
}
bool CSerialPort::readFromComm(char buffer[], DWORD dwLength)
{

    COMSTAT comStat;
    DWORD dwError = 0;
    if (ClearCommError(m_hComm, &dwError, &comStat) && dwError > 0)
    {
        PurgeComm(m_hComm, PURGE_RXABORT | PURGE_RXCLEAR);
    }
 
    DWORD dwRx = 0; // 读入的字节数
    BOOL ret = FALSE;
    char szTmp[4] = { 0 };
    int sizeOfBytes = sizeof(szTmp);

    ret = ReadFile(m_hComm, buffer, dwLength, &dwRx, NULL); // 读入数据
    if (ret == TRUE)
    {
        if (dwRx == 0)
            return false;
        std::lock_guard<std::mutex> lock(mtx);
        rx_cnt += dwRx;
        return true;

    }
    else
    {
        char szBuf[1024] = { 0 };
        wsprintf(szBuf, "Rx Error Code: %d", GetLastError());
        // MessageBox(NULL, szBuf, "Error", MB_OK);
        return false;
    }
}
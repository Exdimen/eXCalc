/*
 * Connect.h
 *
 *  Created on: 2022年11月4日
 *      Author: Exdimen
 */

#ifndef UARTCONNECT_H_
#define UARTCONNECT_H_

#include "stdint.h"
#include "Message.h"
typedef bool (* UartRecevice)(uint8_t* );				// 串口接收读取函数
typedef bool (* UartTransmit)(uint8_t* , uint16_t);	// 串口发送函数
class UartConnect {
public:
	enum ConnectType {
		RX_AND_TX = 0,
		RX,
		TX };
protected:
	/**
	 * @brief 通信类型
	 * 
	 */
	ConnectType cont_type;
	/**
	 * @brief 帧结构
	 * 
	 */
	const uint8_t header = 0x00;	//帧头
	uint8_t seq;					//包序号
	uint8_t address;				//地址
	uint8_t msg_id;
	uint8_t payload[300];			//载荷
	uint16_t payload_size;			//载荷长度
	uint8_t protocol_size;			//协议帧除载荷外总长

	uint8_t tx_payload[300];		// 发送帧头载荷

	/**
	 * @brief 解析函数需要的成员变量
	 * 
	 */
	enum {
		HEADER=0x00,
		SEQ,
		ADDRESS,
		MSGID,
		PAYLOADSIZE,
		PAYLOAD,
		CHECK };
	uint8_t state;
	uint8_t payload_cnt;
	uint8_t crc[4];
	uint8_t crc_len;
	uint8_t crc_cnt;
	uint8_t rx_buffer[300];		// 接受组成缓冲区
	uint16_t rx_cnt;			// 接收缓冲区计数
	uint8_t tx_buffer[300];		// 发送缓冲区
	uint8_t tx_cnt;				// 发送缓冲区计数

	bool is_complete;			// 是否完成接收或发送准备
	bool is_initalized;			// 是否完成初始化
	UartRecevice read_func;		// Rx读取
	uint8_t read_data;
	UartTransmit transmit_func;	// Tx发送

	/**
	 * @brief 收发的消息
	 * 
	 */
	msg::Message* rx_msg; 
	msg::Message* tx_msg;
	
	virtual bool Check()=0;			// 检验校验位
	virtual uint8_t GenerateCheck() {return 0;}// 生成校验位 返回校验位数
	virtual bool Parse()=0;		// 解析函数
	virtual void Decode()=0;	// 解码函数
	virtual void Encode()=0;	// 编码函数
	virtual void Send() {};		// 发送函数（参考MessageMotorSend）
	virtual void TxUpdate()=0;	
	void RxUpdate();

	void SetReadFunc(UartRecevice _read_func) {read_func = _read_func;}
	void SetTransmitFunc(UartTransmit _transmit_func) {transmit_func = _transmit_func;}
public:
	UartConnect();
	virtual ~UartConnect();

	void Init(UartRecevice read_func);
	void Init(UartTransmit transmit_func);
	virtual bool LaunchSend(msg::Message* msg) {return false;}
	virtual void Update();
};


#endif /* UARTCONNECT_H_ */

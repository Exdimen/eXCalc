/*
 * ring_buffer.h
 *
 *  Created on: 2022年7月5日
 *      Author: Exdimen
 */

#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

//Note: not need lock for just one producer / one consumer
//need mutex to protect for multi-producer / multi-consumer

typedef struct _ringBuffer {
    uint8_t *bufferPtr;
    uint16_t bufferSize;

    uint16_t readIndex;
    uint16_t writeIndex;
} T_RingBuffer;

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void RingBuf_Init(T_RingBuffer *pthis, uint8_t *pBuf, uint16_t bufSize);
uint16_t RingBuf_Put(T_RingBuffer *pthis, const uint8_t *pData, uint16_t dataLen);
uint16_t RingBuf_Get(T_RingBuffer *pthis, uint8_t *pData, uint16_t dataLen);
uint16_t RingBuf_GetUnusedSize(T_RingBuffer *pthis);

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* INC_RING_BUFFER_H_ */

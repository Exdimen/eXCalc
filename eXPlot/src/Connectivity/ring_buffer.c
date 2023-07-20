#include "ring_buffer.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define RINGBUF_MIN(a, b) (((a)<(b))?(a):(b))

/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief Cut buffer size to power of 2, in order to increase the convenience of get and put operating of buffer.
 * @param bufSize Original buffer size.
 * @return Buffer size after handling.
 */
static uint16_t RingBuf_CutBufSizeToPowOfTwo(uint16_t bufSize)
{
    uint16_t i = 0;

    while ((1 << (++i)) <= bufSize);
    return (uint16_t) (1 << (--i));
}

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Ring buffer initialization.
 * @param pthis Pointer to ring buffer structure.
 * @param pBuf Pointer to data buffer.
 * @param bufSize Size of data buffer.
 * @return None.
 */
void RingBuf_Init(T_RingBuffer *pthis, uint8_t *pBuf, uint16_t bufSize)
{
    pthis->readIndex = 0;
    pthis->writeIndex = 0;
    pthis->bufferPtr = pBuf;
    pthis->bufferSize = RingBuf_CutBufSizeToPowOfTwo(bufSize);
}

/**
 * @brief Put a block of data into ring buffer.
 * @param pthis Pointer to ring buffer structure.
 * @param pData Pointer to data to be stored.
 * @param dataLen Length of data to be stored.
 * @return Length of data to be stored.
 */
uint16_t RingBuf_Put(T_RingBuffer *pthis, const uint8_t *pData, uint16_t dataLen)
{
    uint16_t writeUpLen;

    dataLen = RINGBUF_MIN(dataLen, (uint16_t) (pthis->bufferSize - pthis->writeIndex + pthis->readIndex));

    //fill up data
    writeUpLen = RINGBUF_MIN(dataLen, (uint16_t) (pthis->bufferSize - (pthis->writeIndex & (pthis->bufferSize - 1))));
    memcpy(pthis->bufferPtr + (pthis->writeIndex & (pthis->bufferSize - 1)), pData, writeUpLen);

    //fill begin data
    memcpy(pthis->bufferPtr, pData + writeUpLen, dataLen - writeUpLen);

    pthis->writeIndex += dataLen;

    return dataLen;
}

/**
 * @brief Get a block of data from ring buffer.
* @param pthis Pointer to ring buffer structure.
* @param pData Pointer to data to be read.
* @param dataLen Length of data to be read.
* @return Length of data to be read.
 */
uint16_t RingBuf_Get(T_RingBuffer *pthis, uint8_t *pData, uint16_t dataLen)
{
    uint16_t readUpLen;

    dataLen = RINGBUF_MIN(dataLen, (uint16_t) (pthis->writeIndex - pthis->readIndex));

    //get up data
    readUpLen = RINGBUF_MIN(dataLen, (uint16_t) (pthis->bufferSize - (pthis->readIndex & (pthis->bufferSize - 1))));
    memcpy(pData, pthis->bufferPtr + (pthis->readIndex & (pthis->bufferSize - 1)), readUpLen);

    //get begin data
    memcpy(pData + readUpLen, pthis->bufferPtr, dataLen - readUpLen);

    pthis->readIndex += dataLen;

    return dataLen;
}

/**
 * @brief Get unused size of ring buffer.
 * @param pthis Pointer to ring buffer structure.
 * @return Unused size of ring buffer.
 */
uint16_t RingBuf_GetUnusedSize(T_RingBuffer *pthis)
{
    return (uint16_t) (pthis->bufferSize - pthis->writeIndex + pthis->readIndex);
}

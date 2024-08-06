/*
 * USART����ͨ��
 * ---------------------------------------------------
 * ����: �������������
 * Email: kyle.xing@fashionstar.com.hk
 * ����ʱ��: 2021-03-16
 * ---------------------------------------------------
 */
#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h" 
#include "stdio.h"
#include "string.h"
#include "ring_buffer.h"

#define USART_RECV_BUF_SIZE 2048
#define USART_SEND_BUF_SIZE 2048

// UART Enable
#define UART4_ENABLE 1
#define USART2_ENABLE 0
#define USART3_ENABLE 0


// ����1��ز���
#if UART4_ENABLE
    // ������
    #define UART4_BAUDRATE 115200
#endif

// ����2��ز���
#if USART2_ENABLE
    #define USART2_BAUDRATE 115200
    // ʱ�Ӷ���
    #define USART2_CLK RCC_APB1Periph_USART2
    #define USART2_APBxClkCmd RCC_APB1PeriphClockCmd
    #define USART2_GPIO_CLK RCC_APB2Periph_GPIOA
    #define USART2_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd
    // GPIO���Ŷ���
    #define USART2_TX_GPIO_PORT GPIOA
    #define USART2_TX_GPIO_PIN GPIO_Pin_2
    #define USART2_RX_GPIO_PORT GPIOA
    #define USART2_RX_GPIO_PIN  GPIO_Pin_3
#endif

// ����3��ز���
#if USART3_ENABLE
    #define USART3_BAUDRATE 115200
    // ʱ�Ӷ���
    #define USART3_CLK RCC_APB1Periph_USART3
    #define USART3_APBxClkCmd RCC_APB1PeriphClockCmd
    #define USART3_GPIO_CLK RCC_APB2Periph_GPIOB
    #define USART3_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd
    // GPIO���Ŷ���
    #define USART3_TX_GPIO_PORT GPIOB
    #define USART3_TX_GPIO_PIN GPIO_Pin_10
    #define USART3_RX_GPIO_PORT GPIOB
    #define USART3_RX_GPIO_PIN  GPIO_Pin_11
#endif

typedef struct
{  
    USART_TypeDef *pUSARTx;
    // ���Ͷ˻�����
    RingBufferTypeDef *sendBuf;
		// ���ն˻�����
    RingBufferTypeDef *recvBuf;
} Usart_DataTypeDef;

//memset(&gc_block, 0, sizeof(parser_block_t))
#if UART4_ENABLE
    extern Usart_DataTypeDef uart4;
    extern uint8_t uart4SendBuf[USART_SEND_BUF_SIZE+1];
	extern uint8_t uart4RecvBuf[USART_RECV_BUF_SIZE+1];
	extern RingBufferTypeDef uart4SendRingBuf;
	extern RingBufferTypeDef uart4RecvRingBuf;
extern UART_HandleTypeDef UART4_InitStructure;
#endif

#if USART2_ENABLE
	extern uint8_t usart2SendBuf[USART_SEND_BUF_SIZE+1];
	extern uint8_t usart2RecvBuf[USART_RECV_BUF_SIZE+1];
	extern RingBufferTypeDef usart2SendRingBuf;
	extern RingBufferTypeDef usart2RecvRingBuf;
	extern Usart_DataTypeDef usart2;

#endif

#if USART3_ENABLE
    extern uint8_t usart3SendBuf[USART_SEND_BUF_SIZE+1];
	extern uint8_t usart3RecvBuf[USART_RECV_BUF_SIZE+1];
	extern RingBufferTypeDef usart3SendRingBuf;
	extern RingBufferTypeDef usart3RecvRingBuf;
	extern Usart_DataTypeDef usart3;
#endif

// ���ô���
void Usart_Init(void);
// �����ֽ�
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
// �����ֽ�����
void Usart_SendByteArr(USART_TypeDef *pUSARTx, uint8_t *byteArr, uint16_t size);
// �����ַ���
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);
// �����ڷ��ͻ�����������ȫ������ȥ
void Usart_SendAll(Usart_DataTypeDef *usart);
#endif

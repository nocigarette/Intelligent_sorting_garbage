/*
 * USART串口通信
 * ---------------------------------------------------
 * 作者: 阿凯爱玩机器人
 * Email: kyle.xing@fashionstar.com.hk
 * 更新时间: 2021-03-16
 * ---------------------------------------------------
 */
#include "myusart.h"

#if UART4_ENABLE
uint8_t uart4SendBuf[USART_SEND_BUF_SIZE + 1];
uint8_t uart4RecvBuf[USART_RECV_BUF_SIZE + 1];
RingBufferTypeDef uart4SendRingBuf;
RingBufferTypeDef uart4RecvRingBuf;
Usart_DataTypeDef uart4;
UART_HandleTypeDef UART4_InitStructure;
#endif

#if USART2_ENABLE
uint8_t usart2SendBuf[USART_SEND_BUF_SIZE + 1];
uint8_t usart2RecvBuf[USART_RECV_BUF_SIZE + 1];
RingBufferTypeDef usart2SendRingBuf;
RingBufferTypeDef usart2RecvRingBuf;
Usart_DataTypeDef usart2;
#endif

#if USART3_ENABLE
uint8_t usart3SendBuf[USART_SEND_BUF_SIZE + 1];
uint8_t usart3RecvBuf[USART_RECV_BUF_SIZE + 1];
RingBufferTypeDef usart3SendRingBuf;
RingBufferTypeDef usart3RecvRingBuf;
Usart_DataTypeDef usart3;
#endif

void Usart_Init(void)
{
    // GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    // 串口配置结构体

#if UART4_ENABLE
    // 赋值结构体usart指针
    uart4.pUSARTx = UART4;
    // 初始化缓冲区(环形队列)
    RingBuffer_Init(&uart4SendRingBuf, USART_SEND_BUF_SIZE, uart4SendBuf);
    RingBuffer_Init(&uart4RecvRingBuf, USART_RECV_BUF_SIZE, uart4RecvBuf);
    uart4.recvBuf = &uart4RecvRingBuf;
    uart4.sendBuf = &uart4SendRingBuf;

    __HAL_RCC_GPIOC_CLK_ENABLE(); //使能GPIOA时钟

    __HAL_RCC_UART4_CLK_ENABLE(); //使能USART1时钟

    // USART1端口配置
    GPIO_InitStructure.Pin = GPIO_PIN_11 | GPIO_PIN_10; // GPIOA9与GPIOA10
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;         //复用功能
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pull = GPIO_PULLUP; //上拉
    GPIO_InitStructure.Alternate = GPIO_AF8_UART4;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PA9，PA10

    // USART1 初始化设置
    UART4_InitStructure.Instance = UART4;
    UART4_InitStructure.Init.BaudRate = UART4_BAUDRATE;      //波特率设置
    UART4_InitStructure.Init.WordLength = UART_WORDLENGTH_8B; //字长为8位数据格式
    UART4_InitStructure.Init.StopBits = UART_STOPBITS_1;      //一个停止位
    UART4_InitStructure.Init.Parity = UART_PARITY_NONE;       //无奇偶校验位
    UART4_InitStructure.Init.HwFlowCtl = UART_HWCONTROL_NONE; //无硬件数据流控制
    UART4_InitStructure.Init.Mode = UART_MODE_TX_RX;          //收发模式
    UART4_InitStructure.Init.OverSampling = UART_OVERSAMPLING_16;

    //__HAL_UART_ENABLE(&UART4_InitStructure);

    HAL_NVIC_SetPriority(UART4_IRQn, 0, 1); //设置优先级
    HAL_NVIC_EnableIRQ(UART4_IRQn);         //使能串口中断
    HAL_UART_Init(&UART4_InitStructure);    //初始化串口1
    HAL_UART_Receive_IT(&UART4_InitStructure, (uint8_t *)&uart4SendBuf[0], 1);
		
		
#endif
#if USART2_ENABLE

    // 赋值结构体usart指针
    usart2.pUSARTx = USART2;
    // 初始化缓冲区(环形队列)
    RingBuffer_Init(&usart2SendRingBuf, USART_SEND_BUF_SIZE, usart2SendBuf);
    RingBuffer_Init(&usart2RecvRingBuf, USART_RECV_BUF_SIZE, usart2RecvBuf);
    usart2.recvBuf = &usart2RecvRingBuf;
    usart2.sendBuf = &usart2SendRingBuf;

    // 赋值结构体usart指针
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟

    //串口2引脚复用映射
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // GPIOA2复用为USART2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // GPIOA3复用为USART2

    // USART2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // GPIOA2与GPIOA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //初始化PA2，PA3

    // USART2 初始化设置
    USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;                           //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式
    USART_Init(USART2, &USART_InitStructure);                                       //初始化串口2

    USART_Cmd(USART2, ENABLE); //使能串口 2

    USART_ClearFlag(USART2, USART_FLAG_TC);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启接受中断

    // Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器、

#endif

#if USART3_ENABLE
    // 赋值结构体usart指针
    usart3.pUSARTx = USART3;
    // 初始化缓冲区(环形队列)
    RingBuffer_Init(&usart3SendRingBuf, USART_SEND_BUF_SIZE, usart3SendBuf);
    RingBuffer_Init(&usart3RecvRingBuf, USART_RECV_BUF_SIZE, usart3RecvBuf);
    usart3.recvBuf = &usart3RecvRingBuf;
    usart3.sendBuf = &usart3SendRingBuf;

    // 使能USART时钟
    USART3_APBxClkCmd(USART3_CLK, ENABLE);
    // 使能GPIO时钟
    USART3_GPIO_APBxClkCmd(USART3_GPIO_CLK, ENABLE);
    // 配置串口USART的发送管脚 TX
    GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStructure);

    // 配置串口USART的接收管脚 RX
    GPIO_InitStructure.GPIO_Pin = USART3_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStructure);

    // Usart NVIC 配置串口外部中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化NVIC寄存器

    // USART 初始化设置
    USART_InitStructure.USART_BaudRate = USART3_BAUDRATE;                           //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式
    // 初始化串口
    USART_Init(USART3, &USART_InitStructure);
    //开启串口接收中断 IT: Interupt
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    // 使能串口
    USART_Cmd(USART3, ENABLE);
#endif
}

/* 发送单个字节 */
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
    /* 发送一个字节到USART */
    HAL_UART_Transmit(&UART4_InitStructure, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    // USART_SendData(pUSARTx, ch);
    /* 等待发送寄存器为空 */
}

/* 发送8位的字节流 */
void Usart_SendByteArr(USART_TypeDef *pUSARTx, uint8_t *byteArr, uint16_t size)
{
    uint16_t bidx;
    for (bidx = 0; bidx < size; bidx++)
    {
        Usart_SendByte(pUSARTx, byteArr[bidx]);
    }
}

/* 发送字符串 */
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
    uint16_t sidx = 0;
    do
    {
        Usart_SendByte(pUSARTx, (uint8_t)(*(str + sidx)));
        sidx++;
    } while (*(str + sidx) != '\0');
}

// 将串口发送缓冲区的内容全部发出去
void Usart_SendAll(Usart_DataTypeDef *usart)
{
    uint8_t value;
    while (RingBuffer_GetByteUsed(usart->sendBuf))
    {
        value = RingBuffer_Pop(usart->sendBuf);
        // printf("Usart_SendAll pop: %d", value);
        Usart_SendByte(usart->pUSARTx, value);
    }
}
#if UART4_ENABLE
// 定义串口中断处理函数
// void USART1_IRQHandler(void)
//{
//    uint8_t ucTemp;
//    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//    {
//        // 从串口读取一个字符
//     ucTemp = USART_ReceiveData(USART1);
//        // 新的字符添加到串口的环形缓冲队列中
//        RingBuffer_Push(uart4.recvBuf, ucTemp);
//    }
//}
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB

#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

// 注: 不定义这个函数会报错
void _ttywrch(int ch)
{
}

// 重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{

    HAL_UART_Transmit(&UART4_InitStructure, (uint8_t *)&ch, 1, 1000);

    return (ch);
    // while ((uart4.pUSARTx->SR & 0X40) == 0)
    // {
    // }
    // /* 发送一个字节数据到串口 */
    // USART_SendData(uart4.pUSARTx, (uint8_t)ch);
    // /* 等待发送完毕 */
    // // while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
    // return (ch);
}
#endif
#endif

#if USART2_ENABLE
// 定义串口中断处理函数
void USART2_IRQHandler(void)
{
    uint8_t ucTemp;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        // 从串口读取一个字符
        ucTemp = USART_ReceiveData(usart2.pUSARTx);
        // 新的字符添加到串口的环形缓冲队列中
        RingBuffer_Push(usart2.recvBuf, ucTemp);
    }
}
#endif

#if USART3_ENABLE
void USART3_IRQHandler(void)
{
    uint8_t ucTemp;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        // 从串口读取一个字符
        ucTemp = USART_ReceiveData(usart3.pUSARTx);
        // 新的字符添加到串口的环形缓冲队列中
        RingBuffer_Push(usart3.recvBuf, ucTemp);
    }
}
#endif

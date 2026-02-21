#include"stm32f10x.h"
#include"stdio.h"
#include"stdarg.h"
#include"Serial.h"

/*
1.开启时钟
2.配置引脚TX复用输出RX输入
3.配置USART
4.配置波特率

*/
uint8_t Serial_RxData;
uint8_t Serial_RxFlag = 0;
uint8_t Serial_TXPacket[256];
uint8_t Serial_RXPacket[256];
char Serial_RXString[256];
/**
 * @brief    串口初始化
 * @param    无
 * @retval   无
 * 
 */
void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//开启USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOA时钟
    //配置引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //配置USART
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;//接收和发送
    USART_Init(USART1,&USART_InitStructure);
    //配置中断
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能接收中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置中断优先级分组
    //配置NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//配置USART1中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断
    NVIC_Init(&NVIC_InitStructure);//初始化

    //使能USART1
    USART_Cmd(USART1,ENABLE);//使能USART1
}
/**
 * @brief  发送一个字节
 * 
 * @param byte  要发送的字节
 */
void Serial_SendByte(uint8_t byte)
{
    USART_SendData(USART1,byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

/**
 * @brief 发送数组
 * 
 * @param array   数组
 * @param len    数组长度
 */
void Serial_SendArray(uint8_t *array,uint16_t len)
{
    for(uint16_t i = 0;i < len;i++)
    {
        Serial_SendByte(array[i]);
    }
}

/**
 * @brief 发送字符串
 * 
 * @param str  字符串
 */
void Serial_SendString(char *str)
{
    while(*str)
    {
        Serial_SendByte(*str++);
    }
}
/**
 * @brief 发送数字
 * 
 * @param num  数字
 */
void Serial_SendNum(uint32_t num)
{
    char str[10] = {0};
    sprintf(str,"%d",num);
    Serial_SendString(str);
}
/**
 * @brief 格式化输出
 * 
 * @param fmt 
 * @param ... 
 */
void Serial_Printf(char *fmt,...)
{
    char str[100] = {0};
    va_list ap;
    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    va_end(ap);
    Serial_SendString(str);
}
//
int fputc(int ch,FILE *f){

	
    Serial_SendByte((uint8_t)ch);
	return ch;
	
}





//
uint8_t Serial_GetFlag(void)
{
    if(Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}
/**
 * @brief   获取接收数据
 * 
 * @return uint8_t 
 */
uint8_t Serial_GetRxData(void)
{
    return Serial_RxData;
}

/**
 * @brief 发送数据包   
 * 
 * @param len 
 */
void Serial_SendPacket(uint16_t len)
{
    Serial_SendByte(0xFF);
    Serial_SendArray(Serial_TXPacket,len);
    Serial_SendByte(0xFE);
}

#if SERIAL_RX_MODE == 1

uint8_t RxStart=0xEF;//数据表头
uint8_t RxEnd=0xFE;//数据表尾
uint8_t RXNum=3; //数据长度
void USART1_IRQHandler(void)
{
    static uint8_t RxSate = 0;
     static uint8_t pRxSate = 0;
    if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART1);
     if(RxSate == 0)
        {
            if(RxData == RxStart)
            {
                RxSate = 1;
                pRxSate=0;
            }
        }
        else if(RxSate == 1)
        {
            Serial_RXPacket[pRxSate++] = RxData;
            if(pRxSate == RXNum){
                RxSate = 2;
            }
        }
        else if(RxSate == 2)
        {
            if(RxData == RxEnd)
            {
                RxSate = 0;
                Serial_RxFlag= 1;

            }
          
        }
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

#elif SERIAL_RX_MODE == 2

void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0;//接收状态
    static uint8_t pRxSate = 0;//接收数据包位置
    if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART1);
     if(RxState == 0)
        {
            if(RxData == '@')
            {
                RxState = 1;
                pRxSate=0;
            }
        }
        else if(RxState == 1)
        {   if(RxData=='\r')
            {
                RxState = 2;
                
            }
            else
            {
           Serial_RXString[pRxSate++] = RxData;		//将数据存入数据包数组的指定位置
        
            }
        }
        else if(RxState == 2)
        {
            if(RxData == '\n')
            {
                RxState = 0;
                Serial_RxFlag= 1;
                Serial_RXPacket[pRxSate++] ='\0';

            }
          
        }
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除USART1的RXNE标志位
    }
}

#else
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
	{
		Serial_RxData = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量
        
		Serial_RxFlag = 1;										//置接收标志位变量为1
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//清除USART1的RXNE标志位
																//读取数据寄存器会自动清除此标志位
																//如果已经读取了数据寄存器，也可以不执行此代码
	}
}


#endif
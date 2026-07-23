#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(KEY_RCC, ENABLE);
    
    // 行线 H1~H4 (PF0~PF3) 配置为推挽输出
    GPIO_InitStructure.GPIO_Pin = H1 | H2 | H3 | H4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);

    // 列线 L1~L4 (PF4~PF7) 配置为上拉输入
    GPIO_InitStructure.GPIO_Pin = L1 | L2 | L3 | L4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);

    // 默认让所有行输出高电平
    GPIO_SetBits(KEY_GPIO_PORT, H1 | H2 | H3 | H4);
}

u8 KEY_Scan(void)
{
    static u8 key_up = 1;  
    u8 key = 0;

    // 先把所有行拉低，用来检测是否有任意按键按下
    GPIO_ResetBits(KEY_GPIO_PORT, H1 | H2 | H3 | H4);
    delay_us(5); // 给电平一点反应时间

    if (key_up && (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L1) == 0 || 
                   GPIO_ReadInputDataBit(KEY_GPIO_PORT, L2) == 0 || 
                   GPIO_ReadInputDataBit(KEY_GPIO_PORT, L3) == 0 || 
                   GPIO_ReadInputDataBit(KEY_GPIO_PORT, L4) == 0))
    {
        delay_ms(10); // 软件消抖
        key_up = 0;   // 锁死单次触发标志
        
        // --- 开始逐行扫描 ---
        
        // 1. 仅拉低第1行 (H1)，其余行拉高
        GPIO_ResetBits(KEY_GPIO_PORT, H1);
        GPIO_SetBits(KEY_GPIO_PORT, H2 | H3 | H4);
        delay_us(5); // 核心：微秒延时，等待高频电平稳定
        if      (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L1) == 0) key = 1;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L2) == 0) key = 2;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L3) == 0) key = 3;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L4) == 0) key = 4;

        // 2. 仅拉低第2行 (H2)，其余行拉高
        GPIO_ResetBits(KEY_GPIO_PORT, H2);
        GPIO_SetBits(KEY_GPIO_PORT, H1 | H3 | H4);
        delay_us(10);
        if      (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L1) == 0) key = 5;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L2) == 0) key = 6;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L3) == 0) key = 7;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L4) == 0) key = 8;

        // 3. 仅拉低第3行 (H3)，其余行拉高
        GPIO_ResetBits(KEY_GPIO_PORT, H3);
        GPIO_SetBits(KEY_GPIO_PORT, H1 | H2 | H4);
        delay_us(10);
        if      (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L1) == 0) key = 9;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L2) == 0) key = 10;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L3) == 0) key = 11;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L4) == 0) key = 12;

        // 4. 仅拉低第4行 (H4)，其余行拉高
        GPIO_ResetBits(KEY_GPIO_PORT, H4);
        GPIO_SetBits(KEY_GPIO_PORT, H1 | H2 | H3);
        delay_us(10);
        if      (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L1) == 0) key = 13;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L2) == 0) key = 14;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L3) == 0) key = 15;
        else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L4) == 0) key = 16;
    }
    // 当所有列线恢复高电平，说明手抬起了
    else if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, L1) == 1 && 
             GPIO_ReadInputDataBit(KEY_GPIO_PORT, L2) == 1 && 
             GPIO_ReadInputDataBit(KEY_GPIO_PORT, L3) == 1 && 
             GPIO_ReadInputDataBit(KEY_GPIO_PORT, L4) == 1)
    {
        key_up = 1; // 释放标志
    }

    // 扫描结束后，恢复行线全高
    GPIO_SetBits(KEY_GPIO_PORT, H1 | H2 | H3 | H4);
    return key;


}



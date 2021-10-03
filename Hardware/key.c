#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                                         //结构体定义
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE); //开启GPIOA GPIOD时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;                                    //IO设置为PD2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                //设置成上拉输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);                                       //初始化GPIOD2

    //初始化 WK_UP-->GPIOA.0	  下拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);        //初始化GPIOA.0
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下   S4 PA.0
//2，KEY1按下   S3 PD.2
//注意此函数有响应优先级,KEY0>KEY1!!
// u8 KEY_Scan(u8 mode)
// {
//     static u8 key_up = 1; //按键按松开标志
//     if (mode)
//         key_up = 1; //支持连按
//     if (key_up && (KEY0 == 1 || KEY1 == 0))
//     {
//         delay_ms(10); //去抖动
//         key_up = 0;
//         if (KEY1 == 0)
//             return KEY1_PRES;
//         else if (KEY0 == 1)
//             return KEY0_PRES;
//     }
//     else if (KEY0 == 0 && KEY1 == 1)
//         key_up = 1;
//     return 0; // 无按键按下
// }

u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1; //按键按松开标志
	if (mode)
		key_up = 1; //支持连按
	if (key_up && (KEY0 == 0 ||  WK_UP == 1))
	{
		delay_ms(10); //去抖动
		key_up = 0;
		if (KEY0 == 0)
			return KEY0_PRES;
		else if (WK_UP == 1)
			return WKUP_PRES;
	}
	else if (KEY0 == 1  && WK_UP == 0)
		key_up = 1;
	return 0; // 无按键按下
}u8 KEY_Scan2(u8 mode)
{
	static u8 key_up2 = 1; //按键按松开标志
	if (mode)
		key_up2 = 1; //支持连按
	if (key_up2 &&  (WK_UP == 1))
	{
		delay_ms(10); //去抖动
		key_up2 = 0;
		if (WK_UP == 1)
			return WKUP_PRES;
	}
	else if (WK_UP == 0)
		key_up2 = 1;
	return 0; // 无按键按下
}




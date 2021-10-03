#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"

//KEY0按下   S4
//KEY1按下   S3
// #define KEY0 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) //宏定义 PA1 PD2输出 为采集数据做准备
// #define KEY1 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)

// #define KEY0_PRES 1 //KEY0按下
// #define KEY1_PRES 2 //KEY1按下

#define KEY0 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)  //读取按键0 
#define WK_UP GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) //读取按键3(WK_UP)  

#define KEY0_PRES 1 //KEY0按下  
#define WKUP_PRES 3 //KEY_UP按下(即WK_UP/KEY_UP)

void KEY_Init(void); //IO初始化
u8 KEY_Scan(u8);     //按键扫描函数
u8 KEY_Scan2(u8);     //按键扫描函数

#endif

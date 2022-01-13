#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "timer.h"
#include "key.h"
#include "dac8552.h"
#include "led.h"
#include "gui.h"
#include "tft.h"

int main(void)
{
  float temp = 0;
  float getcounter = 0;
  int wave_flag = 0;
  // 0——start——————————timer———65536  计数器计数的是start->timer这一段的值
  //DAC输出方式：wave_start+wave_timer，首先从起步部分开始计数，此时为最小的负电压，
  //然后计数满整个定时器设定值，最大值便为正电压，从而实现正负电压输出。

  delay_init();
  uart_init(115200);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  KEY_Init();
  LED_Init();
  Lcd_Init();
  DAC8552_GPIO_Init();
  wave_init();
  //  while (1)
  //  {
  //    printf("zifuchuan:%d\r\n", atoi(zifu));
  //    printf("zifuchuana:%.4f\r\n", atof(str));
  //    delay_ms(500);
  //  }

  while (1)
  {
    if (strstr(USART_RX_BUF, "dac0,") && (USART_RX_STA & 0x8000))
    {
      if (strstr(USART_RX_BUF, "voltage set to") != NULL)
      {
        wave_voltage = atof(USART_RX_BUF);
        printf("wave_voltage=%.2f\r\n", wave_voltage);
        change_flag = 1;
        USART_RX_STA = 0;
				USART_RX_BUF[199]=0;
      }
      else if (strstr(USART_RX_BUF, "wave set to") != NULL)
      {
        tri_or_sin = atoi(USART_RX_BUF);
				if(tri_or_sin)
				{printf("wave:sin\r\n");}
        else {printf("wave:tri\r\n");}
        wave_flag = 1;
        USART_RX_STA = 0;
				USART_RX_BUF[199]=0;
      }
      else if (strstr(USART_RX_BUF, "freq set to") != NULL)
      {
        wave_frequency = atoi(USART_RX_BUF);
        printf("wave_frequency:%d\r\n", wave_frequency);
        change_flag = 1;
        USART_RX_STA = 0;
				USART_RX_BUF[199]=0;
      }
    }
		else if(strstr(USART_RX_BUF, "IDN") && (USART_RX_STA & 0x8000))
		{
			printf("123456\n");
			USART_RX_STA = 0;
			USART_RX_BUF[199]=0;
		}
    if (change_flag)
    {
      wave_timer = wave_voltage * 65536 / 5;
      wave_fre_div = wave_timer * wave_frequency;
      wave_fre_div = (36000000 / wave_fre_div) - 1;
      wave_start = (5 - wave_voltage) * 65536 / 10;
      TIM3_init(wave_timer, (int)wave_fre_div);

      GUI_sprintf_hzstr16x(115, 40, "    ", Red, White);
      GUI_number(115, 40, wave_voltage, Red, White);
      GUI_sprintf_hzstr16x(125, 40, ".", Red, White);
      GUI_number(135, 40, wave_voltage * 10 - (int)wave_voltage * 10, Red, White);
      GUI_sprintf_hzstr16x(130, 60, "  ", Red, White);
      GUI_number(130, 60, wave_frequency, Red, White);

      change_flag = 0;
    }
    if (tri_or_sin && wave_flag) //输出正弦波
    {
      GUI_sprintf_hzstr16x(110, 20, "   ", Red, White);
      GUI_sprintf_hzstr16x(110, 20, "Sin", Red, White);
    }
    else if (tri_or_sin == 0 && wave_flag) //输出三角波
    {
      GUI_sprintf_hzstr16x(110, 20, "   ", Red, White);
      GUI_sprintf_hzstr16x(110, 20, "Tri", Red, White);
    }
    if (tri_or_sin)
    {
      //首先使用定时器的数据转化为/PI/2到PI/2的弧度值
      //再将弧度值通过sin计算
      //再将sin值转为寄存器值0——timer
      //最后加上start
      temp = TIM_GetCounter(TIM3);
      temp = (temp * PI) / (wave_timer);
      temp = temp - PI / 2;
      temp = sin(temp);
      getcounter = (wave_timer / 2) * temp + (wave_timer / 2) + 300;
      getcounter += wave_start;
      wave_flag = 0;
    }
    else
    {
      getcounter = TIM_GetCounter(TIM3) + wave_start;
      wave_flag = 0;
    }
    DAC8552_Write_CHA(getcounter);
  }
}

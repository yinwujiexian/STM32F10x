#include <stdio.h>
#include <math.h>
#include "main.h"
#include "timer.h"
#include "key.h"
#include "dac8552.h"
#include "led.h"
#include "gui.h"
#include "tft.h"

int main(void)
{
  int tri_fre = 10;
  double tri_fre_div = 0;

  delay_init();
  uart_init(115200);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  KEY_Init();
  LED_Init();
  Lcd_Init();
  DAC8552_GPIO_Init();
  //默认输出三角波：±2.8V，10Hz
  TIM3_init(36700, 97); //arr:设置自动重装载寄存器周期的值 //psc:设置时钟频率除数的预分值

  TFT_CS(0);        //打开LCD片选使能
  GUI_Clear(White); //清屏
  GUI_sprintf_hzstr16x(10, 0, "Tri_Frequency S3+ S4-", Red, White);
  GUI_sprintf_hzstr16x(10, 20, "Tri_Frequency:", Red, White);
  GUI_sprintf_hzstr16x(150, 20, "Hz", Red, White);
  GUI_sprintf_hzstr16x(10, 60, "PC.0----SYNC", Red, White);
  GUI_sprintf_hzstr16x(10, 80, "PC.2----SCLK", Red, White);
  GUI_sprintf_hzstr16x(10, 100, "PC.4----DIN", Red, White);

  while (1)
  {
    GUI_number(130, 20, (int)tri_fre, Red, White);

    if (KEY_Scan(0) == KEY0_PRES)
    {
      tri_fre += 1;
      if (tri_fre <= 0)
        tri_fre = 10;
      else if (tri_fre >= 99)
        tri_fre = 10;
      GUI_sprintf_hzstr16x(130, 20, "  ", Red, White);
      GUI_number(130, 20, tri_fre, Red, White);
      tri_fre_div = 367 * tri_fre;
      tri_fre_div = (360000 / tri_fre_div) - 1;
      printf("tri_fre_div=%d\r\n", (int)tri_fre_div);
      TIM3_init(36700, (int)tri_fre_div);
    }
    else if (KEY_Scan2(0) == WKUP_PRES)
    {
      tri_fre = tri_fre - 1;
      if (tri_fre <= 0)
        tri_fre = 10;
      else if (tri_fre >= 99)
        tri_fre = 10;
      GUI_sprintf_hzstr16x(130, 20, "  ", Red, White);
      GUI_number(130, 20, tri_fre, Red, White);
      tri_fre_div = 367 * tri_fre;
      tri_fre_div = (360000 / tri_fre_div) - 1;
      printf("tri_fre_div=%d\r\n", (int)tri_fre_div);
      TIM3_init(36700, (int)tri_fre_div);
    }

    DAC8552_Write_CHA(TIM_GetCounter(TIM3) + 14417);
    DAC8552_Write_CHB(TIM_GetCounter(TIM3) + 14417);
  }
}

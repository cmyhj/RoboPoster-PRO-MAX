
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/SDRAM/sdram.h"
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TOUCH/touch.h"
#include "./bitmap.h"
#include "./BSP/Applications/Applications.h"
#include "string.h"
int mode = 0;
int chesscolor = 0; // 0:白棋 1:黑棋
int val = 0;
uint8_t turn = 0;
uint8_t CenterTurn = 0;
uint8_t state = 0;//0xf0 交换黑白棋 0xf1黑胜 0xf2白胜 0xf3平局
extern uint8_t Screen_Chess[9];
extern uint8_t Chess_info[10][9];
int menuflag = 1;

uint8_t uart_receive[5];
uint8_t uart_save[5];

#define Yi_Er_place_x 320+96
#define Yi_Er_place_y (64+128)/2
#define COLOR_PLACE_X 405
#define COLOR_PLACE_y 192

#define TASK_PLACE_1 100+128
#define TASK_PLACE_2 100+128+128
#define TASK_PLACE_3 100+128+128+128

#define TASK_PLACE(x) (100+128*(x+1))

int main(void)
{
  sys_stm32_clock_init(360, 25, 2, 8); /* ����ʱ��,180Mhz */
  delay_init(180);                     /* ��ʱ��ʼ�� */
  usart_init(90, 115200);              /* ��ʼ��USART */
  led_init();                          /* ��ʼ��LED */
  sdram_init();                        /* ��ʼ��SDRAM */
  lcd_init();                          /* ��ʼ��LCD */
  key_init();                          /* ��ʼ������ */
  tp_dev.init();                       /* ��������ʼ�� */
  lcd_display_dir(1);                  /* ��ʾ���� */
  if (lcddev.dir)
    tp_dev.touchtype = 1;
  else
    tp_dev.touchtype = 0;
  
  block_t *sequence,process;
  
  lcd_clear(CHIENESE_RED);
  sequence->color = GOLDEN;
  sequence->position.x = 0;
  sequence->position.y = 0;
  sequence->size = 32;
  sequence->str = "Welcome to USMART";
 
	block_draw(sequence);
  display_bitmap_on_lcd(nuaa,408,64,GOLDEN,100,0);
  display_bitmap_on_lcd(gong_chuang_sai_xiao_sai,160,32,GOLDEN,100+410,32);
  display_bitmap_on_lcd(ckyf,64,64,GOLDEN,100+410+200,0);
  display_bitmap_on_lcd(ren_wu,128,64,GOLDEN,100,(64+128)/2);
  display_bitmap_on_lcd(picking_what,512,64,GOLDEN,150,192);

  // display_bitmap_on_lcd(stage_1,7*64,64,GOLDEN,200,320);
  //  display_bitmap_on_lcd(hong,64,64,BLACK,COLOR_PLACE_X,COLOR_PLACE_y);
  //  display_bitmap_on_lcd(hong,64,64,RED,TASK_PLACE_1,96);
  //     display_bitmap_on_lcd(hong,64,64,RED,TASK_PLACE_2,96);
  //        display_bitmap_on_lcd(hong,64,64,RED,TASK_PLACE_3,96);
	 display_bitmap_on_lcd(arrow,64,64,GOLDEN,TASK_PLACE_1+64,96);
   	display_bitmap_on_lcd(arrow,64,64,GOLDEN,TASK_PLACE_2+64,96);

  while(1)
  {
    if(USART_Receive()&&memcmp(uart_receive,uart_save,sizeof(uart_receive)))
    {
			memcpy(uart_save,uart_receive,sizeof(uart_receive));
			lcd_clear(CHIENESE_RED);
      block_draw(sequence);
  display_bitmap_on_lcd(nuaa,408,64,GOLDEN,100,0);
  display_bitmap_on_lcd(gong_chuang_sai_xiao_sai,160,32,GOLDEN,100+410,32);
  display_bitmap_on_lcd(ckyf,64,64,GOLDEN,100+410+200,0);
  display_bitmap_on_lcd(ren_wu,128,64,GOLDEN,100,(64+128)/2);
  display_bitmap_on_lcd(picking_what,512,64,GOLDEN,150,192);
	 display_bitmap_on_lcd(arrow,64,64,GOLDEN,TASK_PLACE_1+64,96);
   	display_bitmap_on_lcd(arrow,64,64,GOLDEN,TASK_PLACE_2+64,96);
      for(int i = 0; i < 3; i++)
      {
        switch (uart_receive[i])
        {
        case 1:
          display_bitmap_on_lcd(hong,64,64,RED,TASK_PLACE(i),96);
          break;
        case 2:
          display_bitmap_on_lcd(lv,64,64,GREEN,TASK_PLACE(i),96);
          break;
        case 3:
          display_bitmap_on_lcd(lan,64,64,BLUE,TASK_PLACE(i),96);
          break;
        
        default:
          break;
        }
      }
      switch (uart_receive[3])
      {
        case 1:
          display_bitmap_on_lcd(stage_1,7*64,64,GOLDEN,200,320);
          break;
        case 2:
          display_bitmap_on_lcd(stage_2,8*64,64,GOLDEN,200,320);
          break;
        case 3:
          display_bitmap_on_lcd(stage_3,7*64,64,GOLDEN,200,320);
          break;
        case 4:
          display_bitmap_on_lcd(stage_4,8*64,64,GOLDEN,200,320);
          break;
        default:
          break;
      }
      switch (uart_receive[4])
      {
        case 1:
          display_bitmap_on_lcd(hong,64,64,RED,COLOR_PLACE_X,COLOR_PLACE_y);
          break;
        case 2:
          display_bitmap_on_lcd(lv,64,64,GREEN,COLOR_PLACE_X,COLOR_PLACE_y);
          break;
        case 3:
          display_bitmap_on_lcd(lan,64,64,BLUE,COLOR_PLACE_X,COLOR_PLACE_y);
          break;
        default:
          break;
      }
    }
    else{
      
    }
    

  }


}
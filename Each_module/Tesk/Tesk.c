#include "Variable.h"

extern int16_t V_yaw;
extern int16_t V_r;
extern int16_t t123;
extern uint8_t receive_USART1[4];
extern uint8_t Transmit_USART6[3];

void Test_Task(void const * argument)
{
  for(;;)
  {
	if(t123)
	{
		t123 = 0;
	}
  }
}

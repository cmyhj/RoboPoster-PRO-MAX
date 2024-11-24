#include "Variable.h"

extern int16_t V_yaw;
extern int16_t V_r;

void Test_Task(void const * argument)
{

//	Sink_plate;
//    osDelay(15100);
//	Claw;
//	osDelay(1000);
//	Un_Sink;
//	osDelay(1000);
////	V_yaw=1000;
////	osDelay(1000);
////	V_yaw=0;
////	osDelay(1000);
//	V_r=5000;
//	osDelay(1000);
//	V_r=0;
//	osDelay(1000);
//	Sink_ground;
//    osDelay(1000);
//	Un_Claw;
//	osDelay(1000);
//	Un_Sink;
//	osDelay(1000);
//	Un_Sink;
//	V_r=-5000;
//	osDelay(1000);
//	V_r=0;
//	osDelay(1000);
////	V_yaw=-1000;
////	osDelay(1000);
////	V_yaw=0;
////	osDelay(1000);
////	V_yaw=1000;
////	osDelay(1000);
  for(;;)
  {
	Claw;
	  osDelay(1000);
	Un_Sink;
	  osDelay(1000);
//	Sink_plate;
//    osDelay(1000);
//	Claw;
//	osDelay(1000);
//	Un_Sink;
//	osDelay(1000);
//	  V_r=500;
//	  osDelay(2000);
//	V_yaw=1000;
//	osDelay(1000);
  }
}

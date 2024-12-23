#include "Variable.h"

extern int16_t V_yaw;
extern int16_t V_r;
extern int16_t V_x;
extern int16_t V_y;
extern int16_t flag[4];
extern int16_t v[6];
extern int16_t t1;
extern int16_t t_screen;
extern moto_info_t motor_yaw_info;
extern Motor_measure_t Motor_measure[5];
extern uint8_t Transmit_USART6[3];

void Chassis_Task(void const * argument)
{
  /* USER CODE BEGIN Chassis_Task */
  /* Infinite loop */
  for(;;)
  {
	CAN_cmd_chassis(v[0]*flag[0],v[1]*flag[1],v[2]*flag[2],v[3]*flag[3]);
	CAN_cmd_gimbal(v[5],v[4],0,0);
	Servo_Set();
    osDelay(1);
  }
  /* USER CODE END Chassis_Task */
}

void PID_Task(void const * argument)
{
	while(1)
	{
		if(t1)
		{
			for(int i=0;i<=3;i++)
			{
				v[i]=pid_calc(&M2006_speed_pid[i],pid_calc(&M2006_angle_pid[i],a[i],Motor_measure[i].total_ecd),Motor_measure[i].speed_rpm);
			}
			v[5]=pid_calc(&M6020_yaw_speed_pid,pid_calc(&M6020_yaw_angle_pid,a[i],motor_yaw_info.rotor_angle),motor_yaw_info.rotor_speed);
			osDelay(2);
		}
	}
}

void Transmit_Task(void const * argument)
{
  /* USER CODE BEGIN Transmit_Task */
  /* Infinite loop */
  for(;;)
  {
	if(t_screen)
	{
		HAL_UART_Transmit(&huart6,Transmit_USART6,4,1000);
	}
    osDelay(1);
  }
  /* USER CODE END Transmit_Task */
}



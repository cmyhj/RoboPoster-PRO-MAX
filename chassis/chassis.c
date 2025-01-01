#include "Variable.h"

void Chassic_Task(void const * argument)
{
  /* USER CODE BEGIN Chassis_Task */
  /* Infinite loop */
  for(;;)
  {
	CAN_cmd_chassis(v[0]*flag[0],v[1]*flag[1],v[2]*flag[2],v[3]*flag[3]);
	CAN_cmd_gimbal(v[5],v[4],0,0);
	a[4] = r / 15 * 36 * 8191;
	Servo_Set();
    osDelay(1);
  }
  /* USER CODE END Chassis_Task */
}

void PID_Task(void const * argument)
{
	while(1)
	{
		if(t_stop)
		{
			v[0]=pid_calc(&M2006_speed_pid_stop,pid_calc(&M2006_angle_pid_stop,C_x,C_x),Motor_measure[0].speed_rpm);
			v[2]=pid_calc(&M2006_speed_pid_stop,pid_calc(&M2006_angle_pid_stop,C_x,C_x),Motor_measure[2].speed_rpm);
			v[1]=pid_calc(&M2006_speed_pid_stop,pid_calc(&M2006_angle_pid_stop,C_y,C_y),Motor_measure[1].speed_rpm);
			v[3]=pid_calc(&M2006_speed_pid_stop,pid_calc(&M2006_angle_pid_stop,C_y,C_y),Motor_measure[3].speed_rpm);
		}
		if(t_move)
		{
			V_x = pid_calc(&M2006_angle_pid_x,Target_x,C_x);
			V_y = pid_calc(&M2006_angle_pid_y,Target_y,C_y);
			
			v[0]=pid_calc(&M2006_speed_pid[0],V_x/10*K,Motor_measure[0].speed_rpm*2*3.1415926f/60/36*50);
			v[2]=pid_calc(&M2006_speed_pid[2],V_x/10*K,Motor_measure[2].speed_rpm*2*3.1415926f/60/36*50);
			
			v[1]=pid_calc(&M2006_speed_pid[1],V_y/10*K,Motor_measure[1].speed_rpm*2*3.1415926f/60/36*50);
			v[3]=pid_calc(&M2006_speed_pid[0],V_y/10*K,Motor_measure[3].speed_rpm*2*3.1415926f/60/36*50);
		}
		if(t1)	v[4]=pid_calc(&M2006_speed_pid[4],pid_calc(&M2006_angle_pid_r,a[4],Motor_measure[4].total_ecd),Motor_measure[4].speed_rpm);
		else v[4] = 0;
		v[5] = pid_calc(&M6020_yaw_speed_pid,pid_calc(&M6020_yaw_angle_pid,a[5],motor_yaw_info.rotor_angle),motor_yaw_info.rotor_speed);
		osDelay(2);
		C_x = (Motor_measure[0].total_ecd + Motor_measure[2].total_ecd) / 36 / 8190 * 2 * 3.1415926f * 50 / 2;
		C_x = (Motor_measure[1].total_ecd + Motor_measure[2].total_ecd) / 36 / 8190 * 2 * 3.1415926f * 50 / 2;
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
		Transmit_USART8[0] = 'c';
		Transmit_USART8[1] = 'k';
		Transmit_USART8[2] = 'y';
		Transmit_USART8[3] = 'f';
		Transmit_USART8[4] = Test[0+(t_test-1)*3];
		Transmit_USART8[5] = Test[1+(t_test-1)*3];
		Transmit_USART8[6] = Test[2+(t_test-1)*3];
		Transmit_USART8[7] = t_now;
		Transmit_USART8[8] = t_test;
		HAL_UART_Transmit(&huart8,Transmit_USART8,9,1000);
	}
	if(t_GM65)
	{
		for(int i = 0;i <= 5; i++ )
		{
			if(receive_USART8[i] == 0x31)	Test[i] = 1;
			if(receive_USART8[i] == 0x32)	Test[i] = 2;
			if(receive_USART8[i] == 0x33)	Test[i] = 3;
		}
		t_GM65 = 0;
	}
    osDelay(1);
  }
  /* USER CODE END Transmit_Task */
}



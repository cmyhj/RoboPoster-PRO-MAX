#include "Variable.h"

void Chassic_Task(void const * argument)
{
  /* USER CODE BEGIN Chassis_Task */
  /* Infinite loop */
  for(;;)
  {
	CAN_cmd_chassis(v[0],v[1],v[2],v[3]);
	CAN_cmd_gimbal(v[5],v[4],0,0);
	a[4] = r / 15 * 36 * 8191;
	Servo_Set();
    osDelay(1);
  }
  /* USER CODE END Chassis_Task */
}



void PID_Task(void const * argument)
{
//	t_move = 1;
//	t_x = 1;
//	V_x = 200;
//	V_y = 200;	
	while(1)
	{
		if(t_move)
		{

			V_x = pid_calc(&M2006_angle_pid_x,Target_x,C_x);
			V_y = pid_calc(&M2006_angle_pid_y,Target_y,C_y);			
			
			if(t_x){
			v[0]=pid_calc(&M2006_speed_pid[0],V_x,Speed[0]);
			v[3]=pid_calc(&M2006_speed_pid[3],-V_x,Speed[3]);}
			else{			
				v[0] = 0;
				v[3] = 0;
			}
			
			if(t_y){
			v[1]=pid_calc(&M2006_speed_pid[1],V_y,Speed[1]);
			v[2]=pid_calc(&M2006_speed_pid[2],-V_y,Speed[2]);}
			else{
				v[1] = 0;
				v[2] = 0;
			}
		}
		else
		{
			v[0]=pid_calc(&M2006_speed_pid_stop,pid_calc(&M2006_angle_pid_stop,C_x,C_x),Motor_measure[0].speed_rpm);
			v[2]=pid_calc(&M2006_speed_pid_stop,pid_calc(&M2006_angle_pid_stop,C_x,C_x),Motor_measure[2].speed_rpm);
			v[1]=pid_calc(&M2006_speed_pid_stop,pid_calc(&M2006_angle_pid_stop,C_y,C_y),Motor_measure[1].speed_rpm);
			v[3]=pid_calc(&M2006_speed_pid_stop,pid_calc(&M2006_angle_pid_stop,C_y,C_y),Motor_measure[3].speed_rpm);
		}
		if(t_r)	v[4]=pid_calc(&M2006_speed_pid[4],pid_calc(&M2006_angle_pid_r,r,mil[4]),Speed[4]);
		else v[4] = 0;
		if(t_theta) v[5] = pid_calc(&M6020_yaw_speed_pid,pid_calc(&M6020_yaw_angle_pid,Angle_yaw,Theta),rad);
		else v[5] = 0;  
		osDelay(2);
		C_x = (mil[0] - mil[3])/ 2;
		C_y = (mil[1] - mil[2])/ 2;
		
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
	}
	if(t_OpenMV)
	{
		Transmit_USART6[0] = 'C';
		Transmit_USART6[1] = 'K';
		if(t_low)
			Transmit_USART6[2] = 49;
		else Transmit_USART6[2] = 48;
		Transmit_USART6[3] = 'Y';
		Transmit_USART6[4] = 'F';
		HAL_UART_Transmit(&huart6,Transmit_USART6,5,1000);
	}
	mil[0] = Motor_measure[0].total_ecd * 3.1415926f * 50 / 36 / 8190; 
	mil[1] = Motor_measure[1].total_ecd * 3.1415926f * 50 / 36 / 8190;  
	mil[2] = Motor_measure[2].total_ecd * 3.1415926f * 50 / 36 / 8190; 
	mil[3] = Motor_measure[3].total_ecd * 3.1415926f * 50 / 36 / 8190;  
	mil[4] = Motor_measure[4].total_ecd * 16.0f / 36 / 8190; 
	Theta  = 360.0f * motor_yaw_info.rotor_angle / 8190;
	Speed[0] = Motor_measure[0].speed_rpm * 3.1415926f * 50 / 36;
	Speed[1] = Motor_measure[1].speed_rpm * 3.1415926f * 50 / 36;
	Speed[2] = Motor_measure[2].speed_rpm * 3.1415926f * 50 / 36;
	Speed[3] = Motor_measure[3].speed_rpm * 3.1415926f * 50 / 36;
	Speed[4] = Motor_measure[4].speed_rpm * 16/ 36;
	rad = motor_yaw_info.rotor_speed / 36;
	
	if(t_plant == 2)
			{
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);
			}
			if(t_plant == 1)
			{
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);
			}
			if(t_plant == 3)
			{
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
			}
		
			osDelay(1);
		}
  /* USER CODE END Transmit_Task */
}



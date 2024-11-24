#include "Variable.h"

extern int16_t V_yaw;
extern int16_t V_r;

void Chassis_Task(void const * argument)
{
  /* USER CODE BEGIN Chassis_Task */
  /* Infinite loop */
  for(;;)
  {
	CAN_cmd_chassis(Motor_measure_2006[0].Output,Motor_measure_2006[1].Output,Motor_measure_2006[2].Output,Motor_measure_2006[3].Output);
	CAN_cmd_gimbal(Motor_measure_6020.set_voltage,Motor_measure_2006[4].Output,0, 0);
	Servo_Set();
    osDelay(1);
  }
  /* USER CODE END Chassis_Task */
}

void PID_Task(void const * argument)
{
	for(int i = 0;i++;i<5)
	{
		Motor_measure_2006[i].Output=pid_calc(&M2006_speed_pid[i],pid_calc(&M2006_angle_pid[i],A_r,Motor_measure_2006[i].total_ecd),Motor_measure_2006[i].speed_rpm);
    }
	Motor_measure_6020.set_voltage=pid_calc(&M6020_yaw_speed_pid, pid_calc(&M6020_yaw_angle_pid, A_yaw*22.7528, Motor_measure_6020.rotor_angle), Motor_measure_6020.rotor_speed);
	osDelay(40);
}



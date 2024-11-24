#include "Variable.h"

extern int16_t V_yaw;
extern int16_t V_r;

void Chassis_Task(void const * argument)
{
  /* USER CODE BEGIN Chassis_Task */
  /* Infinite loop */
  for(;;)
  {
//	CAN_cmd_chassis(0,0,0,0);
	CAN_cmd_gimbal(V_yaw,V_r,0, 0);
	Servo_Set();
    osDelay(1);
  }
  /* USER CODE END Chassis_Task */
}

void PID_Task(void const * argument)
{
	Motor_measure_2006[4].Output=pid_calc(&M2006_speed_pid[4],pid_calc(&M2006_angle_pid[4],A_r,Motor_measure_2006[4].total_ecd),Motor_measure_2006[4].speed_rpm);
    Motor_measure_6020.set_voltage=pid_calc(&M6020_yaw_speed_pid, pid_calc(&M6020_yaw_angle_pid, A_yaw*22.7528, Motor_measure_6020.rotor_angle), Motor_measure_6020.rotor_speed);
	osDelay(40);
}



#include "AllInclude.h"

void ChassisPIDCalculate(void const * argument)
{
    //init
	chassis_init(&chassis_move);

    //sequence:mode_set,feedback,calculate&control,send&control
    while(1)
    {
        gyro_update(&chassis_move);
		if(safe_flag==0)
		{
			chassis_move.chassis_mode=CHASSIS_STOP;
			
			chassis_feedback_update(&chassis_move);
			
			osDelay(10);	
		}
		else
		{
			if(chassis_move.chassis_mode==CHASSIS_STOP)
			{
				chassis_move.chassis_mode=CHASSIS_MOVE_AND_ROTATE;
			}
			
			//as it's fname
			chassis_feedback_update(&chassis_move);
			//calculate the position pid
			chassis_set_control(&chassis_move);
			//in speedpid to current
			chassis_control_loop(&chassis_move);
			
			Set_Wheel_Speed(chassis_move.motor_chassis[0].give_current, 
							chassis_move.motor_chassis[1].give_current,
							chassis_move.motor_chassis[2].give_current,
							chassis_move.motor_chassis[3].give_current);
			
 
			
		osDelay(2);
		}
    }
}
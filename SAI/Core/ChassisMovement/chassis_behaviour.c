#include "chassis_behaviour.h"
#include "AllInclude.h"
#include "math.h"
float IR_V_x=0,IR_V_y=0;

float float_fabs(float value)
{
	if(value>=0) return value;
	else return -value;
}


/*
这一个函数主要是判断我们需要用哪种模式
*/
void chassis_behaviour_control_set(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector)
{

    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_vector == NULL)
    {
        return;
    }
		if (chassis_move_vector->chassis_mode == CHASSIS_MOVE_AND_ROTATE)
    {
        chassis_move_and_rotate_control(vx_set, vy_set, wz_set, chassis_move_vector);
    }
		else if (chassis_move_vector->chassis_mode == CHASSIS_INFRARED)
    {
        chassis_infrared_control(vx_set, vy_set, wz_set, chassis_move_vector);
    }
    else if (chassis_move_vector->chassis_mode == CHASSIS_STOP)
    {
        chassis_stop_control(vx_set, vy_set, wz_set, chassis_move_vector);
    }
}



/**
  * @brief          底盘平动+旋转
  * @author         RM
  * @param[in]      vx_set前进的速度,正值 前进速度， 负值 后退速度
  * @param[in]      vy_set左右的速度,正值 左移速度， 负值 右移速度
  * @param[in]      wz_set底盘设置的旋转速度,正值 顺时针旋转，负值 逆时针旋转
  * @param[in]      chassis_move_and_rotate_vector底盘数据
	* @param[in]			平动环+陀螺仪角度环，入口参数分别为chassis->x_set,y_set,gyro_set，改这三个即可控制三个环
  * @retval         返回空
  */

void chassis_move_and_rotate_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_vector == NULL)
    {
        return;
    }

		*vx_set = PID_calc(&chassis_move_vector->motor_distance_pid,chassis_move_vector->x,chassis_move_vector->x_set);
		*vy_set = PID_calc(&chassis_move_vector->motor_distance_pid,chassis_move_vector->y,chassis_move_vector->y_set);

		
		//保证车在斜向平移时沿直线运动
		float v_total=(*vx_set)*(*vx_set)+(*vy_set)*(*vy_set);
		if(v_total>1)
		{
			float rate=sqrt(v_total)/1;
			*vx_set=*vx_set/rate;
			*vy_set=*vy_set/rate;
		}	
		
		/***************************************************************************/
		
		//三套pid控制旋转
		if(float_fabs(chassis_move_vector->x_set-chassis_move_vector->x)>=0.05f)
		{
			*wz_set = PID_calc(&chassis_move_vector->motor_move_X_gyro_pid,chassis_move_vector->gyro,chassis_move_vector->gyro_set);
		}
		else if(float_fabs(chassis_move_vector->y_set-chassis_move_vector->y)>=0.05f)
		{
			*wz_set = PID_calc(&chassis_move_vector->motor_move_Y_gyro_pid,chassis_move_vector->gyro,chassis_move_vector->gyro_set);
		}
		else 
		{
			*wz_set = PID_calc(&chassis_move_vector->motor_gyro_pid,chassis_move_vector->gyro,chassis_move_vector->gyro_set);
		}

		chassis_move_vector->chassis_mode=CHASSIS_MOVE_AND_ROTATE;
}
static void  chassis_infrared_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_vector == NULL)
    {
        return;
    }
		*wz_set = PID_calc(&chassis_move_vector->motor_gyro_pid,chassis_move_vector->gyro,chassis_move_vector->gyro_set);
//		*vx_set = PID_calc(&chassis_move_vector->motor_infrared_pid,chassis_move_vector->vx,IR_V_x);
//    *vy_set = PID_calc(&chassis_move_vector->motor_infrared_pid,chassis_move_vector->vy,IR_V_y);
		*vx_set=IR_V_x;
		*vy_set=IR_V_y;
		chassis_move_vector->chassis_mode=CHASSIS_INFRARED;
}
/**
  * @brief          底盘无力
  * @author         RM
  * @param[in]      chassis_move_vector底盘数据
	* @param[in]    	无力的状态也是测试的状态，要是想尝试什么功能可以丢这里
  * @retval         返回空
  */
void chassis_stop_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_vector == NULL)
    {
        return;
    }
		chassis_move_vector->chassis_mode=CHASSIS_STOP;
}

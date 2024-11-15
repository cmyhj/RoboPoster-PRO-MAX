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
��һ��������Ҫ���ж�������Ҫ������ģʽ
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
  * @brief          ����ƽ��+��ת
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�,��ֵ ǰ���ٶȣ� ��ֵ �����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�,��ֵ �����ٶȣ� ��ֵ �����ٶ�
  * @param[in]      wz_set�������õ���ת�ٶ�,��ֵ ˳ʱ����ת����ֵ ��ʱ����ת
  * @param[in]      chassis_move_and_rotate_vector��������
	* @param[in]			ƽ����+�����ǽǶȻ�����ڲ����ֱ�Ϊchassis->x_set,y_set,gyro_set�������������ɿ���������
  * @retval         ���ؿ�
  */

void chassis_move_and_rotate_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_vector == NULL)
    {
        return;
    }

		*vx_set = PID_calc(&chassis_move_vector->motor_distance_pid,chassis_move_vector->x,chassis_move_vector->x_set);
		*vy_set = PID_calc(&chassis_move_vector->motor_distance_pid,chassis_move_vector->y,chassis_move_vector->y_set);

		
		//��֤����б��ƽ��ʱ��ֱ���˶�
		float v_total=(*vx_set)*(*vx_set)+(*vy_set)*(*vy_set);
		if(v_total>1)
		{
			float rate=sqrt(v_total)/1;
			*vx_set=*vx_set/rate;
			*vy_set=*vy_set/rate;
		}	
		
		/***************************************************************************/
		
		//����pid������ת
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
  * @brief          ��������
  * @author         RM
  * @param[in]      chassis_move_vector��������
	* @param[in]    	������״̬Ҳ�ǲ��Ե�״̬��Ҫ���볢��ʲô���ܿ��Զ�����
  * @retval         ���ؿ�
  */
void chassis_stop_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_vector == NULL)
    {
        return;
    }
		chassis_move_vector->chassis_mode=CHASSIS_STOP;
}

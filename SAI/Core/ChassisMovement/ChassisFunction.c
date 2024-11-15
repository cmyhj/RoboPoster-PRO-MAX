#include "ChassisFunction.h"
#include "ChassisVariable.h"


int8_t stir_round_count=0;
float limit_xspeed_set=0;
float limit_yspeed_set=0;
float limit_wspeed_set=0;
int cnty=0;
/*
限制最大最小值
*/
float float_constrain(float Value, float minValue, float maxValue)
{
    if (Value < minValue)
        return minValue;
    else if (Value > maxValue)
        return maxValue;
    else
        return Value;
}

void chassis_init(chassis_move_t *chassis_move_init)
{
    chassis_move_init->chassis_mode=CHASSIS_MOVE_AND_ROTATE;
	/*
		把4个2006电机的数据的结构体指针传给chassis_move结构体中的数组
	*/
	for(uint8_t i=0;i<4;i++)
	{
		chassis_move_init->motor_chassis[i].chassis_motor_measure=Motor_measure+i;
	}
	
	
	/*
		初始化速度PID(电流环
		先初始化3个系数
		再通过PID_init初始化具体参数，并限制最大最小值
	*/
	const static float motor_speed_pid[3] = {M2006_MOTOR_SPEED_PID_KP, M2006_MOTOR_SPEED_PID_KI, M2006_MOTOR_SPEED_PID_KD};
	for (uint8_t i = 0; i < 4; i++)
  	{
		PID_init(&chassis_move_init->motor_speed_pid[i], PID_POSITION, motor_speed_pid, M2006_MOTOR_SPEED_PID_MAX_OUT, M2006_MOTOR_SPEED_PID_MAX_IOUT);
	}
	chassis_move.motor_speed_pid[0].Kp=16000;//8000
	chassis_move.motor_speed_pid[0].Ki=300;//150
	chassis_move.motor_speed_pid[0].Kd=10000;//5000
	
	
	chassis_move.motor_speed_pid[1].Kp=16400;//8200
	chassis_move.motor_speed_pid[1].Ki=330;//165
	chassis_move.motor_speed_pid[1].Kd=10000;//5000
	
	chassis_move.motor_speed_pid[2].Kp=16000;//8000
	chassis_move.motor_speed_pid[2].Ki=340;//170
	chassis_move.motor_speed_pid[2].Kd=10200;//5100
	
	chassis_move.motor_speed_pid[3].Kp=17000;//8500
	chassis_move.motor_speed_pid[3].Ki=320;//160
	chassis_move.motor_speed_pid[3].Kd=10400;//5200
	
	chassis_move.motor_speed_pid[0].deadband=0.0001;
	chassis_move.motor_speed_pid[1].deadband=0.0001;
	chassis_move.motor_speed_pid[2].deadband=0.0001;
	chassis_move.motor_speed_pid[3].deadband=0.0001;
	/*
		初始化平动PID(里程环
		先初始化3个系数
		通过PID_init初始化具体参数，并限制最大最小值
		注意平动环的distance和distance_set是在chassis_move_init结构体里面
		distance_set是入口参数，你改这个就行了，前后和左右看vx和vy的正负就行
	*/
	const static float motor_distance_pid[3] = {M2006_MOTOR_DISTANCE_PID_KP, M2006_MOTOR_DISTANCE_PID_KI, M2006_MOTOR_DISTANCE_PID_KD};
	PID_init(&chassis_move_init->motor_distance_pid, PID_POSITION, motor_distance_pid, M2006_MOTOR_DISTANCE_PID_MAX_OUT, M2006_MOTOR_DISTANCE_PID_MAX_IOUT);
//	chassis_move_init->x=0;
//	chassis_move_init->y=0;
	chassis_move_init->x_set=0;
	chassis_move_init->y_set=0;
	
	/*
		初始化陀螺仪角度环
		先初始化3个系数
		通过PID_init初始化具体参数，并限制最大最小值
		注意角度环的gyro,gyro_Dbuf,gyro_set,last_gyro是在chassis_move_init结构体里面,在chassis_feedback_update里面更新的
		distance_set是入口参数，你改这个就行了，前后和左右看vx和vy的正负就行
	*/		 
	const static float motor_gyro_pid[3] = {M2006_MOTOR_GYRO_PID_KP, M2006_MOTOR_GYRO_PID_KI, M2006_MOTOR_GYRO_PID_KD};
	PID_init(&chassis_move_init->motor_gyro_pid, PID_POSITION, motor_gyro_pid, M2006_MOTOR_GYRO_PID_MAX_OUT, M2006_MOTOR_GYRO_PID_MAX_IOUT);
	
	const static float motor_move_X_gyro_pid[3] = {M2006_MOTOR_MOVE_X_GYRO_PID_KP, M2006_MOTOR_MOVE_X_GYRO_PID_KI, M2006_MOTOR_MOVE_X_GYRO_PID_KD};
	PID_init(&chassis_move_init->motor_move_X_gyro_pid, PID_POSITION, motor_move_X_gyro_pid, M2006_MOTOR_MOVE_GYRO_PID_MAX_OUT, M2006_MOTOR_MOVE_GYRO_PID_MAX_IOUT);
	
	const static float motor_move_Y_gyro_pid[3] = {M2006_MOTOR_MOVE_Y_GYRO_PID_KP, M2006_MOTOR_MOVE_Y_GYRO_PID_KI, M2006_MOTOR_MOVE_Y_GYRO_PID_KD};
	PID_init(&chassis_move_init->motor_move_Y_gyro_pid, PID_POSITION, motor_move_Y_gyro_pid, M2006_MOTOR_MOVE_GYRO_PID_MAX_OUT, M2006_MOTOR_MOVE_GYRO_PID_MAX_IOUT);
	
	chassis_move_init->gyro=0.0f;
	chassis_move_init->gyro_set=0.0f;
	chassis_move_init->last_gyro=0.0f;
	
	const static float  motor_infrared_pid[3] = {M2006_MOTOR_INFRARED_PID_KP, M2006_MOTOR_INFRARED_PID_KI, M2006_MOTOR_INFRARED_PID_KD};
	PID_init(&chassis_move_init->motor_infrared_pid, PID_POSITION, motor_infrared_pid, M2006_MOTOR_INFRAREDd_PID_MAX_OUT, M2006_MOTOR_INFRARED_PID_MAX_IOUT);
//	chassis_move_init->vx=0;
//	chassis_move_init->vy=0;
	
	/*
		这里主要是初始化下x,y,z的最大最小值
	*/
	chassis_move_init->vx_max_speed = +NORMAL_MAX_CHASSIS_SPEED_X;
	chassis_move_init->vx_min_speed = -NORMAL_MAX_CHASSIS_SPEED_X;

	chassis_move_init->vy_max_speed = +NORMAL_MAX_CHASSIS_SPEED_Y;
	chassis_move_init->vy_min_speed = -NORMAL_MAX_CHASSIS_SPEED_Y;
		
	chassis_move_init->wz_max_speed = +NORMAL_MAX_CHASSIS_SPEED_Z;
  	chassis_move_init->wz_min_speed = -NORMAL_MAX_CHASSIS_SPEED_Z;
	
	/*
		这个函数就是用来解算和更新各项电机传回来的数据的
	*/
	chassis_feedback_update(chassis_move_init);
}


void chassis_feedback_update(chassis_move_t *chassis_move_update)
{
	if(chassis_move_update == NULL)
	{
		return ;
	}
	for(uint8_t i=0;i<4;i++)
	{
		//更新电机速度，加速度是速度的PID微分
		chassis_move_update->motor_chassis[i].speed = M2006_MOTOR_RPM_TO_VECTOR * chassis_move_update->motor_chassis[i].chassis_motor_measure->speed_rpm;
	}
	chassis_move_update->x=(+chassis_move_update->motor_chassis[0].chassis_motor_measure->total_ecd
													-chassis_move_update->motor_chassis[1].chassis_motor_measure->total_ecd
													-chassis_move_update->motor_chassis[2].chassis_motor_measure->total_ecd
													+chassis_move_update->motor_chassis[3].chassis_motor_measure->total_ecd)
													/4.0f *M2006_MOTOR_ECD_TO_DISTANCE;
	chassis_move_update->y=(+chassis_move_update->motor_chassis[0].chassis_motor_measure->total_ecd
													+chassis_move_update->motor_chassis[1].chassis_motor_measure->total_ecd
													-chassis_move_update->motor_chassis[2].chassis_motor_measure->total_ecd
													-chassis_move_update->motor_chassis[3].chassis_motor_measure->total_ecd)
													/4.0f *M2006_MOTOR_ECD_TO_DISTANCE/1.13f;
		/*
		更新陀螺仪角度数据
		*/
		gyro_update(chassis_move_update);
		
		
		 //更新底盘纵向速度 x， 平移速度y，旋转速度wz，坐标系为右手系
		chassis_move_update->vx = ( + chassis_move_update->motor_chassis[0].speed 
																- chassis_move_update->motor_chassis[1].speed 
																- chassis_move_update->motor_chassis[2].speed 
																+ chassis_move_update->motor_chassis[3].speed)
																* MOTOR_SPEED_TO_CHASSIS_SPEED_VX;
    chassis_move_update->vy = ( + chassis_move_update->motor_chassis[0].speed 
																+ chassis_move_update->motor_chassis[1].speed 
																- chassis_move_update->motor_chassis[2].speed 
																- chassis_move_update->motor_chassis[3].speed)
																* MOTOR_SPEED_TO_CHASSIS_SPEED_VY;
    chassis_move_update->wz = (	- chassis_move_update->motor_chassis[0].speed 
																- chassis_move_update->motor_chassis[1].speed 
																- chassis_move_update->motor_chassis[2].speed 
																- chassis_move_update->motor_chassis[3].speed)
																* MOTOR_SPEED_TO_CHASSIS_SPEED_WZ/CHASSIS_WZ_SET_SCALE;
}


/*
记录陀螺仪进行了几次角度突变，突变几次就在角度上加上多少个360°
*/
//TODO:Gryo is not inside
void gyro_update(chassis_move_t *chassis_gyro_update)
{
	chassis_gyro_update->last_gyro=chassis_gyro_update->current_gyro;
	chassis_gyro_update->current_gyro=0;
	if(chassis_gyro_update->current_gyro-chassis_gyro_update->last_gyro<-200)
	{
		stir_round_count++;
	}
	else if(chassis_gyro_update->current_gyro-chassis_gyro_update->last_gyro>200)
	{
		stir_round_count--;
	}
	if(stir_round_count==0)
	{
		chassis_gyro_update->gyro=0;
	}
	else if(stir_round_count>0)
	{
		chassis_gyro_update->gyro=0+360*stir_round_count;
	}
	else if(stir_round_count<0)
	{
		chassis_gyro_update->gyro=0+360*stir_round_count;
	}
}

void chassis_set_control(chassis_move_t *chassis_move_control)
{
	if (chassis_move_control == NULL)
  {
		return;
  }	
	//get three control set-point, 获取三个控制设置值
	float vx_set = 0.0f, vy_set = 0.0f, wz_set = 0.0f;
	chassis_behaviour_control_set(&vx_set,&vy_set, &wz_set,chassis_move_control); 
	if(chassis_move_control->chassis_mode==CHASSIS_MOVE_AND_ROTATE)
	{		 
		
		chassis_move_control->vx_set=vx_set;
		chassis_move_control->vy_set=vy_set;
		chassis_move_control->wz_set=wz_set; 
		 //if speed is over limit speed
		 //the function is to control the cart begin moving slowly!
		// if(chassis_move_control->vx - chassis_move_control->vx_set>SLOWSTART_MINDIS_V||chassis_move_control->vx - chassis_move_control->vx_set<-SLOWSTART_MINDIS_V)
		// {
		// 	limit_xspeed_set+=(chassis_move_control->vx_set*SLOWSTART_VX_K);
		// 	chassis_move_control->vx_set=limit_xspeed_set;
			
		// 	cntx=0; 
		// }		 
		// else
		// {
		// 	cntx++;
		// }
		// if(cntx>10)
		// {
		// 	cntx=0;
		// 	limit_xspeed_set=0;
		// }

		if(chassis_move_control->vy-chassis_move_control->vy_set>SLOWSTART_MINDIS_V||chassis_move_control->vy-chassis_move_control->vy_set<-SLOWSTART_MINDIS_V)
		{	 
			limit_yspeed_set+=(chassis_move_control->vy_set*SLOWSTART_VY_K);
			chassis_move_control->vy_set=limit_yspeed_set;
			
			cnty=0;	 
		}		 
		else
		{
			cnty++;
		}				 
		if(cnty>10)
		{
			limit_yspeed_set=0;
			cnty=0;
		}		  
		 
//		if(chassis_move_control->wz-chassis_move_control->wz_set>SLOWSTART_MINDIS_W||chassis_move_control->wz-chassis_move_control->wz_set<-SLOWSTART_MINDIS_W)
//		{ 
//			limit_wspeed_set+=(chassis_move_control->wz_set*SLOWSTART_WZ_K);
//			chassis_move_control->wz_set=limit_wspeed_set;
//			
//			cnty=0;	 
//		}		 
//		else
//		{
//			cntw++;
//		}				 
//		if(cntw>10)
//		{
//			limit_wspeed_set=0;
//			cntw=0;
//		}	
		
		
		chassis_move_control->vx_set = float_constrain(chassis_move_control->vx_set, chassis_move_control->vx_min_speed, chassis_move_control->vx_max_speed);
		chassis_move_control->vy_set = float_constrain(chassis_move_control->vy_set, chassis_move_control->vy_min_speed, chassis_move_control->vy_max_speed);
		chassis_move_control->wz_set = float_constrain(chassis_move_control->wz_set, chassis_move_control->wz_min_speed, chassis_move_control->wz_max_speed);
	}
	else if(chassis_move_control->chassis_mode==CHASSIS_INFRARED)
	{
		chassis_move_control->vx_set=vx_set;
		chassis_move_control->vy_set=vy_set;
		chassis_move_control->wz_set=wz_set;
		chassis_move_control->vx_set = float_constrain(chassis_move_control->vx_set, chassis_move_control->vx_min_speed, chassis_move_control->vx_max_speed);
		chassis_move_control->vy_set = float_constrain(chassis_move_control->vy_set, chassis_move_control->vy_min_speed, chassis_move_control->vy_max_speed);
		chassis_move_control->wz_set = float_constrain(chassis_move_control->wz_set, chassis_move_control->wz_min_speed, chassis_move_control->wz_max_speed);
	}
	else if(chassis_move_control->chassis_mode==CHASSIS_STOP)
	{
//		safe_flag=0;
	}
}

/*
麦克纳姆轮运动分解函数
*/
 void chassis_vector_to_mecanum_wheel_speed(const float vx_set, const float vy_set, const float wz_set, float wheel_speed[4])
{
	wheel_speed[0] = +( + vx_set + vy_set - CHASSIS_WZ_SET_SCALE  * wz_set);
    wheel_speed[1] = -( + vx_set - vy_set + CHASSIS_WZ_SET_SCALE  * wz_set);
    wheel_speed[2] = -( + vx_set + vy_set + CHASSIS_WZ_SET_SCALE  * wz_set);
    wheel_speed[3] = +( + vx_set - vy_set - CHASSIS_WZ_SET_SCALE  * wz_set);
}

void chassis_control_loop(chassis_move_t *chassis_move_control_loop)
{
		//计算轮子控制最大速度，并限制其最大速度
		chassis_control_loop_limit(chassis_move_control_loop);

		//计算pid，赋值电流值
    for (uint8_t i = 0; i < 4; i++)
    {	
			chassis_move_control_loop->motor_chassis[i].give_current = (int16_t)PID_calc(&chassis_move_control_loop->motor_speed_pid[i],
													chassis_move_control_loop->motor_chassis[i].speed,chassis_move_control_loop->motor_chassis[i].speed_set);
    }
}


//限制四轮最大转速
void chassis_control_loop_limit(chassis_move_t *chassis)
{
//    float max_speed = 0.0f; //四轮最大数
//		float	rate = 0.0f;//限速比
//    float temp = 0.0f;
    uint8_t i = 0;
		float wheel_speed[4]={0,0,0,0};
			
    //底盘目标速度转轮速
    chassis_vector_to_mecanum_wheel_speed(chassis->vx_set,chassis->vy_set, chassis->wz_set, wheel_speed);
		//找4轮最大速 超速则4轮同比降速
    for (i = 0; i < 4; i++)
    {
        chassis->motor_chassis[i].speed_set = wheel_speed[i];
//				if(chassis->motor_chassis[i].speed_set>=0) temp =  chassis->motor_chassis[i].speed_set;
//				if(chassis->motor_chassis[i].speed_set<0)  temp = -chassis->motor_chassis[i].speed_set;
//        if (float_fabs(max_speed) < float_fabs(temp))
//        {
//            max_speed = temp;
//        }
    }

//    if (float_fabs(max_speed) > float_fabs(MAX_WHEEL_SPEED))
//    {
//        rate = MAX_WHEEL_SPEED / float_fabs(max_speed);
//        for (i = 0; i < 4; i++)
//        {
//            chassis->motor_chassis[i].speed_set *= rate;
//        }
//    }
		
}

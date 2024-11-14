#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "pid.h"
#include "stdint.h"
#include "bsp_can.h"

//��λ(cm)
#define WHEEL_DISTANCE_TO_CENTER_X 8.0f
#define WHEEL_DISTANCE_TO_CENTER_Y 12.648f
#define WHEEL_RADIUS 3.81f


#define NORMAL_MAX_CHASSIS_SPEED_X 1.0f
#define NORMAL_MAX_CHASSIS_SPEED_Y 1.0f
#define NORMAL_MAX_CHASSIS_SPEED_Z 2.0f


//m2006ת��ת���ɵ����ٶ�(m/s)�ı�����
#define M2006_MOTOR_RPM_TO_VECTOR (2*3.1415926f*WHEEL_RADIUS/100/60/36)


#define MOTOR_SPEED_TO_CHASSIS_SPEED_VX 0.25f
#define MOTOR_SPEED_TO_CHASSIS_SPEED_VY 0.25f
#define MOTOR_SPEED_TO_CHASSIS_SPEED_WZ 0.25f

#define MOTOR_DISTANCE_TO_CENTER 0.2f


//�������̵������ٶ�
#define MAX_WHEEL_SPEED 1.0f

#define CHASSIS_PID_MAX_OUT 8000.0f
#define CHASSIS_PID_MAX_IOUT 3000.0f

/*
�ٶ�PID����
*/
#define M2006_MOTOR_SPEED_PID_KP 18000.0f
#define M2006_MOTOR_SPEED_PID_KI 200.0f
#define M2006_MOTOR_SPEED_PID_KD 2300.0f
#define M2006_MOTOR_SPEED_PID_MAX_OUT MAX_MOTOR_CAN_CURRENT
#define M2006_MOTOR_SPEED_PID_MAX_IOUT 5000.0f

/*
 ����PID����
*/
#define M2006_MOTOR_DISTANCE_PID_KP 2.4f
#define M2006_MOTOR_DISTANCE_PID_KI 0.000004f
#define M2006_MOTOR_DISTANCE_PID_KD 0.0000012f
#define M2006_MOTOR_DISTANCE_PID_MAX_OUT 1500.0f
#define M2006_MOTOR_DISTANCE_PID_MAX_IOUT 0.8f

/*
�����ǽǶ�PID����
*/
#define M2006_MOTOR_GYRO_PID_KP 0.3f
#define M2006_MOTOR_GYRO_PID_KI 0.0f
#define M2006_MOTOR_GYRO_PID_KD 2.1f
#define M2006_MOTOR_GYRO_PID_MAX_OUT 2.0f
#define M2006_MOTOR_GYRO_PID_MAX_IOUT 2.0f
#define MAX_MOTOR_CAN_CURRENT 15000.0f

#define M2006_MOTOR_MOVE_X_GYRO_PID_KP 0.15f
#define M2006_MOTOR_MOVE_X_GYRO_PID_KI 0.0f
#define M2006_MOTOR_MOVE_X_GYRO_PID_KD 3.2f
#define M2006_MOTOR_MOVE_Y_GYRO_PID_KP 0.1f
#define M2006_MOTOR_MOVE_Y_GYRO_PID_KI 0.0f
#define M2006_MOTOR_MOVE_Y_GYRO_PID_KD 3.2f
#define M2006_MOTOR_MOVE_GYRO_PID_MAX_OUT MAX_MOTOR_CAN_CURRENT
#define M2006_MOTOR_MOVE_GYRO_PID_MAX_IOUT 200.0f

//m2006ת�����ת����������·�̵ı�����m)
#define M2006_MOTOR_ECD_TO_DISTANCE (2*3.1415926f*WHEEL_RADIUS/8192/36/100)
#define CHASSIS_WZ_SET_SCALE ((WHEEL_DISTANCE_TO_CENTER_X+WHEEL_DISTANCE_TO_CENTER_Y)/100)

/*
�������
*/
#define SLOWSTART_VX_K 0.03f
#define SLOWSTART_VY_K 0.02f
#define SLOWSTART_WZ_K 0.0004f
#define SLOWSTART_MINDIS_V 0.30f
#define SLOWSTART_MINDIS_W 0.02f


typedef struct
{
	Motor_measure_t *chassis_motor_measure;
	
	float accel;
	float accel_angle;
	
	float speed;
	
	float speed_set;
	float angle_set;
	
	int16_t give_current;
}chassis_motor_t;


/*
	��Ϊģʽ��
	0 ����״̬
	1 ���̿�������
*/
//typedef enum
//{
//	CHASSIS_STOP,
//	CHASSIS_MOVE_AND_ROTATE,
//}chassis_mode_e;

typedef enum
{
	CHASSIS_STOP,
	CHASSIS_MOVE_AND_ROTATE,
	CHASSIS_INFRARED,
	
}chassis_mode_e;


typedef struct
{
	chassis_mode_e chassis_mode;//�����˶�ģʽ
	chassis_motor_t motor_chassis[4];//4�����̵������
	
	pid_type_def motor_speed_pid[4];//���̵���ٶ�pid
	pid_type_def motor_distance_pid;//���̵��ƽ��pid
	pid_type_def motor_gyro_pid;
	pid_type_def motor_move_X_gyro_pid;
	pid_type_def motor_move_Y_gyro_pid;
	
	pid_type_def motor_infrared_pid;
	
	float vx;//x�����ٶ�
	float vy;//y�����ٶ�
	float wz;//��ת�ٶ�
	
	float vx_set;//���õ�x�ٶ�
	float vy_set;//���õ�y�ٶ�
	float wz_set;//���õ���ת�ٶ�
	
	
	float x;
	float y;
	
	float x_set;
	float y_set;
	
	float gyro_set;
	float gyro;
	float current_gyro;
	float last_gyro;
	int Dis_fm_set;
	
	float vx_max_speed; 	//ǰ����������ٶ� 
  float vx_min_speed;  	//���˷�������ٶ� 
  float vy_max_speed;  	//��������ٶ� 
  float vy_min_speed;  	//�ҷ�������ٶ�
	float wz_max_speed;		//��ת����ٶ�
	float wz_min_speed;		//��ת��С�ٶ�
	
}chassis_move_t;

extern void chassis_behaviour_control_set(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector);
extern void chassis_move_and_rotate_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector);
extern void chassis_stop_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector);

float float_constrain(float Value, float minValue, float maxValue);
void chassis_init(chassis_move_t *chassis_move_init);
void chassis_feedback_update(chassis_move_t *chassis_move_update);
void gyro_update(chassis_move_t *chassis_gyro_update);
void chassis_set_control(chassis_move_t *chassis_move_control);
void chassis_vector_to_mecanum_wheel_speed(const float vx_set, const float vy_set, const float wz_set, float wheel_speed[4]);
void chassis_control_loop(chassis_move_t *chassis_move_control_loop);
void chassis_control_loop_limit(chassis_move_t *chassis);

#endif

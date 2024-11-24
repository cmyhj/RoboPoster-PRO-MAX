#ifndef _Variable_H
#define _Variable_H

#include "chassis.h"
#include "FreeRTOS.h"
#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
#include "bsp_can.h" 
#include "pid.h"
#include "Servo.h"
#include "stdint.h"
#include "Tesk.h"
#include "stdint.h"
#include "can.h"


extern int16_t v[6];
extern int16_t a[6];
extern int16_t i;
extern int16_t Angle1;
extern int16_t Angle2;
extern int16_t flag[4];
extern int16_t V_yaw;
extern int16_t V_r;
extern int16_t A_yaw;
extern int16_t A_r;
extern uint8_t rx_data[8];

extern Motor_measure_t Motor_measure_2006[5];
extern Motor_measure_y Motor_measure_6020;

extern pid_struct_t M6020_yaw_speed_pid;
extern pid_struct_t M6020_yaw_angle_pid;
extern pid_struct_t M2006_speed_pid[5];
extern pid_struct_t M2006_angle_pid[5];

extern CAN_TxHeaderTypeDef CANx_tx_message;

#define 	Claw 			Angle2=70
#define 	Un_Claw 		Angle2=-60
#define 	Sink_plate 		Angle1=-50
#define 	Sink_ground 	Angle1=60
#define 	Un_Sink 		Angle1=-160

#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))

#define		CAN_2006_M1_ID	0x201
#define		CAN_2006_M2_ID	0x202
#define		CAN_2006_M3_ID	0x203
#define		CAN_2006_M4_ID	0x204
#define		CAN_6020_ID		0x205
#define		CAN_2006_M5_ID	0x206

#define motor_can1 0x200 
#define motor_can2 0x1FF

#endif

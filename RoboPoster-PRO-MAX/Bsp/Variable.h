#ifndef _Variable_H
#define _Variable_H

#include "chassis.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
#include "can.h"
#include "gpio.h"
#include "bsp_can.h" 
#include "pid.h"
#include "Servo.h"
#include "stdint.h"
#include "Tesk.h"
#include "stdio.h"
#include "math.h"

extern int16_t v[6];
extern int32_t a[6];
extern int16_t i;
extern int16_t Angle1;
extern int16_t Angle2;
extern int16_t flag[4];
extern int16_t V_yaw;
extern int16_t V_r;
extern int16_t A_yaw;
extern int16_t A_r;
extern uint8_t rx_data[8];
extern uint16_t can_flag1[4];

extern uint8_t K;

extern float Target_x;
extern float Target_y;

extern float C_x; 
extern float C_y; 

extern moto_info_t motor_yaw_info;
extern Motor_measure_t Motor_measure[5];
extern pid_struct_t M6020_yaw_speed_pid;
extern pid_struct_t M6020_yaw_angle_pid;

extern pid_struct_t M2006_speed_pid[5];
extern pid_struct_t M2006_angle_pid_x;
extern pid_struct_t M2006_angle_pid_y;
extern pid_struct_t M2006_angle_pid_r;
extern pid_struct_t M2006_speed_pid_stop;
extern pid_struct_t M2006_angle_pid_stop;

extern uint8_t bil[6];
extern int16_t V_x;
extern int16_t V_y;

extern int16_t t1;
extern int16_t t2;
extern int16_t t123;
extern int16_t t_stop;
extern int16_t t_move;
extern int16_t t_test;
extern int16_t t_now;
extern int16_t t_x;
extern int16_t t_y;
extern int16_t t_r;
extern int16_t t_theta;
extern float Speed[5];
extern float rad;
extern float Angle_yaw;
extern float quit_x;
extern float quit_y;
extern float t_low;

extern uint8_t receive_USART8[7];
extern uint8_t Transmit_USART8[9];
extern uint8_t receive_USART6[10];
extern uint8_t Transmit_USART6[5];
extern int16_t t_stop;
extern int16_t t_move; 
extern int16_t t_screen;
extern int16_t t_OpenMV;
extern int16_t t_GM65;
extern uint8_t Test[6];
extern int16_t Get_cc[6];
extern float mil[5];
extern float Theta;
extern int16_t Target[4];
extern float r;
extern int16_t t_plant;


extern CAN_TxHeaderTypeDef CANx_tx_message;

#define     Claw            Angle2=105;osDelay(500)
#define     Un_Claw         Angle2=140;osDelay(500)
#define     Sink_plate      Angle1=-50;osDelay(500)
#define     Sink_firstplat  Angle1=-60;osDelay(500)
#define     Sink_ground     Angle1=60;osDelay(500)
#define     Sink_material   Angle1=-110;osDelay(500)
#define     Un_Sink         Angle1=-160;osDelay(500)

#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))

#define        CAN_2006_M1_ID    0x201
#define        CAN_2006_M2_ID    0x202
#define        CAN_2006_M3_ID    0x203
#define        CAN_2006_M4_ID    0x204
#define        CAN_6020_ID        0x205
#define        CAN_2006_M5_ID    0x206

#define motor_can1 0x200 
#define motor_can2 0x1FF

#endif


#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "Variable.h"

typedef struct
{
	int16_t	 	speed_rpm;
    int16_t  	torque_current;
	int16_t     Output;
    uint8_t  	temp;
	int16_t 	ecd;				//abs angle range:[0,8191]
	int16_t 	last_ecd;	        //abs angle range:[0,8191]
	int32_t		round_cnt;
	int32_t		total_ecd;
}Motor_measure_t;


typedef struct
{
    uint16_t can_id;//电机ID
    int16_t  set_voltage;//设定的电压值
    uint16_t rotor_angle;//机械角度
    int16_t  rotor_speed;//转速
    int16_t  torque_current;//扭矩电流
    uint8_t  temp;//温度
}Motor_measure_y;


void can_filter_init(void);
void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_gimbal(int16_t yaw, int16_t Vr, int16_t rev1, int16_t rev2);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

#endif

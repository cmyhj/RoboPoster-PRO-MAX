#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "stdint.h"
#include "can.h"

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


#define motor_can1 0x200 
#define motor_can2 0x1FF

void can_filter_init(void);
void Set_Wheel_Speed(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
void Set_Stir_Speed(int16_t motor5);
void Motor_measure_fun(Motor_measure_t *ptr,uint8_t* RX_buffer);

extern Motor_measure_t Motor_measure[14];

#endif

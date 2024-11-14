/**
  ******************************************************************************
  * @file    motor.h
  * @author  Karolance Future
  * @version V2.1.0
  * @date    2021/08/29
  * @brief   Header file of motor.c
  ******************************************************************************
  * @attention
  *
  * 此文件只适用于如下电机及其电子调速器:
	*
	*   Motor   *  Governor  U  Receive         * Firmware                * MotorCANID * Notes
	*           |                               |                         |            |
	*   RM3508  |  C620      24 [-16384,+16384] |                         | [1:8]      |
	*   RM2006  |  C610      24 [-10000,+10000] |                         | [1:8]      |
	*           |  820R      12 [-8000 ,+8000 ] | RM2006_V1_2_4_x.bin.enc | [1:4]      |
	*           |                               | RM2006_V1_2_5_x.bin.enc | [1:7]      |
  	*  RM3510  |  820R      24 [-16384,+16384] | RM3510_V1_0_0_1.bin.enc | [1:4]      | Factory configuration
	*           |                               | RM3510_V1_2_4_x.bin.enc | [1:4]      |
	*           |                               | RM3510_V1_2_5_x.bin.enc | [1:7]      |
	*   GM6020  |   /        24 [-30000,+30000] |                         | [1:7]      |
	*           |                               |                         |            |
	*           *                               *                         *            *
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_H__
#define __MOTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes and Defines ------------------------------------------------------*/
#include "stdint.h"

typedef enum
{
	Motor_CANID_1FF  = 0x1FF,
  	Motor_CANID_200  = 0x200,
	Motor_CANID_2FF  = 0x2FF,
	
	Motor_CANID_1    = 0x201,
	Motor_CANID_2    = 0x202,
	Motor_CANID_3    = 0x203,
	Motor_CANID_4    = 0x204,
	Motor_CANID_5    = 0x205,
	Motor_CANID_6    = 0x206,
	Motor_CANID_7    = 0x207,
	Motor_CANID_8    = 0x208,

} Motor_CANID_TypeDef;

/* 电调反馈报文 */
typedef struct
{
	int16_t Angle;
	int16_t Speed;
	int16_t Current;
	uint8_t Temperature;
} Motor_Governor_TypeDef;

/* 处理后的信息 */
typedef struct
{
	int16_t LastAngle;
	int64_t TotalAngle;
	uint8_t Calibration;
	int16_t OffsetAngle;
	int32_t RoundCnt;
} Motor_Data_TypeDef;

typedef struct
{
	Motor_CANID_TypeDef    CANID;
  	int16_t                GivenValue;  //向电调发送的信息
	Motor_Governor_TypeDef Governor;
	Motor_Data_TypeDef     Data;
	Motor_Data_TypeDef	   InitData;
} Motor_TypeDef;


void Motor_Reset(Motor_TypeDef *ptr);
void Motor_SetCANID(Motor_TypeDef *ptr, Motor_CANID_TypeDef CANID);
void Motor_UpdateGovernor(Motor_TypeDef *ptr, uint8_t *DataArray);
void Motor_UpdateTotalAngle1(Motor_TypeDef *ptr);
void Motor_UpdateTotalAngle2(Motor_TypeDef *ptr);
void Motor_UpdateData(Motor_TypeDef *ptr, uint8_t *DataArray);


#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_H__ */

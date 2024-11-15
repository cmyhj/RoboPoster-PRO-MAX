/**
 * @file Motor.c
 * @author DCY
 * @brief 基于DJI库函数，适配麦轮
 * @version 0.1
 * @date 2024-07-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* Includes and Defines ------------------------------------------------------*/
#include "motor.h"
// #define USE_UpdateTotalAngle1
#define USE_UpdateTotalAngle2
#define abs(x)	( (x>0) ? (x) : (-x) )


/* Functions -----------------------------------------------------------------*/
/*==============================================================================
              ##### 初始化函数 #####
  ==============================================================================
    [..]  该部分提供如下函数:
		  (+) 电机数据初始化函数 Motor_Reset
      (+) 电机CANID设置函数 Motor_SetCANID
*/
void Motor_Reset(Motor_TypeDef *ptr)
{
	ptr->GivenValue       = 0;
	ptr->Data.LastAngle   = 0;
	ptr->Data.TotalAngle  = 0;
	ptr->Data.Calibration = 0;
	ptr->Data.RoundCnt    = 0;
	ptr->Data.OffsetAngle = 0;
}

void Motor_SetCANID(Motor_TypeDef *ptr, Motor_CANID_TypeDef CANID)
{
	ptr->CANID = CANID;
}

/*==============================================================================
              ##### 数据更新函数 #####
  ==============================================================================
    [..]  该部分提供如下函数:
		  (+) 译码电调反馈报文函数 Motor_UpdateGovernor
		  (+) 记录旋转总圈数函数1 Motor_UpdateTotalAngle1
      (+) 记录旋转总圈数函数2 Motor_UpdateTotalAngle2
      (+) 更新电机所有数据函数 Motor_UpdateData
*/
/**
 * @brief 译码电调反馈报文函数
 * @param ptr 电机指针
 * @param DataArray 反馈报文数组
 */
void Motor_UpdateGovernor(Motor_TypeDef *ptr, uint8_t *DataArray)
{
	ptr->Governor.Angle       = ((uint16_t)DataArray[0]) << 8 | DataArray[1];
	ptr->Governor.Speed       = ((uint16_t)DataArray[2]) << 8 | DataArray[3];
	ptr->Governor.Current     = ((uint16_t)DataArray[4]) << 8 | DataArray[5];
	ptr->Governor.Temperature = DataArray[6];
}
/**
 * @brief 记录旋转总圈数函数1
 * 
 */
#if defined USE_UpdateTotalAngle1
	void Motor_UpdateTotalAngle1(Motor_TypeDef *ptr)
	{
		if(ptr->Data.Calibration == 0)
		{
			ptr->Data.OffsetAngle = ptr->Governor.Angle;
			ptr->Data.Calibration = 1;
		}
		else
		{
			if(ptr->Governor.Angle - ptr->Data.LastAngle >  4096) ptr->Data.RoundCnt--;
			if(ptr->Governor.Angle - ptr->Data.LastAngle < -4096) ptr->Data.RoundCnt++;
		}
		ptr->Data.TotalAngle = ptr->Data.RoundCnt * (int64_t)8192 + ptr->Governor.Angle - ptr->Data.OffsetAngle;
		/*电调上电角度不是0，修正误差*/
	}
#elif defined USE_UpdateTotalAngle2
	void Motor_UpdateTotalAngle2(Motor_TypeDef *ptr)
	{
		int32_t res1 = 0,res2 = 0;
		if(ptr->Data.Calibration == 0)
		{
			ptr->Data.LastAngle = ptr->Governor.Angle;
			ptr->Data.Calibration = 1;
		}
		res1 = ptr->Governor.Angle - ptr->Data.LastAngle + ((ptr->Governor.Angle < ptr->Data.LastAngle) ? 8192 : -8192);
		res2 = ptr->Governor.Angle - ptr->Data.LastAngle;
		ptr->Data.TotalAngle += abs(res1) < abs(res2) ? res1 : res2;
	}
#endif
/**
 * @brief 更新电机所有数据函数
 * 
 * @param ptr 
 * @param DataArray 
 */
void Motor_UpdateData(Motor_TypeDef *ptr, uint8_t *DataArray)
{
	ptr->Data.LastAngle = ptr->Governor.Angle;
	
	Motor_UpdateGovernor(ptr, DataArray);
	
	#if defined USE_UpdateTotalAngle1
		Motor_UpdateTotalAngle1(ptr);
	#elif defined USE_UpdateTotalAngle2
		Motor_UpdateTotalAngle2(ptr);
	#endif
}

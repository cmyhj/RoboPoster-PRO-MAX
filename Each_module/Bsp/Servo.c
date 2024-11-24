#include "Variable.h"

void Servo_Init()
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); 
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}

void Servo_Set()
{	
	uint32_t Duty;
	Duty = Angle1 * 1000 / 180 + 1500;
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, Duty);
	Duty = Angle2 * 1000 / 180 + 1500;
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, Duty);
}

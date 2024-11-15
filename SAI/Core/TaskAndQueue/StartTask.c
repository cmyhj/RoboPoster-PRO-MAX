#include "AllInclude.h"


extern osThreadId ChassisPIDCalHandle;
extern osThreadId ScrewPIDCalHandle;
extern osThreadId StartHandle;
extern osThreadId ConnectHandle;
extern osThreadId DebugHandle;
extern osMessageQId PositionHandle;


void Can1ChassisMotorFilterConfig()
{
	CAN_FilterTypeDef CAN1_Filter;

	CAN1_Filter.FilterIdHigh = 0x201 << 5;	   // 0x201
	CAN1_Filter.FilterIdLow = 0x200 << 5;	   // 0x202
	CAN1_Filter.FilterMaskIdHigh = 0x201 << 5; // 0x203
	CAN1_Filter.FilterMaskIdLow = 0x200 << 5;  // 0x204
	CAN1_Filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN1_Filter.FilterBank = 0;
	CAN1_Filter.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN1_Filter.FilterScale = CAN_FILTERSCALE_16BIT;
	CAN1_Filter.FilterActivation = ENABLE;

	if (HAL_CAN_ConfigFilter(&hcan1, &CAN1_Filter) != HAL_OK)
	{
		Error_Handler();
	}
}

void StartTask(void const * argument)
{
    vTaskSuspend(ChassisPIDCalHandle);
    vTaskSuspend(ScrewPIDCalHandle);
    vTaskSuspend(ConnectHandle);
    vTaskSuspend(DebugHandle);

    /*Chassis init*/
	Can1ChassisMotorFilterConfig();


    vTaskResume(ChassisPIDCalHandle);
    osDelay(20);
    vTaskResume(ScrewPIDCalHandle);
    osDelay(20);
    vTaskResume(ConnectHandle);
    osDelay(20);
    vTaskResume(DebugHandle);
    osDelay(20);

    osDelay(500);
    vTaskDelete(StartHandle);
    
}
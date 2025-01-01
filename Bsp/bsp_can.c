#include "Variable.h"

void can_filter_init(void)
{

    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void Motor_measure_fun(Motor_measure_t *ptr,uint8_t* RX_buffer)
{	
	ptr->ecd = (uint16_t)((RX_buffer)[0] << 8 | (RX_buffer)[1]); 	//????§Ö???
	ptr->speed_rpm = (uint16_t)((RX_buffer)[2] << 8 | (RX_buffer)[3]);	//??????(rpm)
	ptr->given_current = (uint16_t)((RX_buffer)[4] << 8 | (RX_buffer)[5]);	//??????
	ptr->temperate = (RX_buffer)[6];         
	if(ptr->ecd - ptr->last_ecd > 4096)				
		ptr->round_cnt --;								
	else if(ptr->ecd - ptr->last_ecd < -4096)		
		ptr->round_cnt ++;
	ptr->total_ecd = ptr->round_cnt * 8192 + ptr->ecd;	
	ptr->last_ecd = ptr->ecd;
	ptr->ecd = 0;
//	ptr->total_ecd = ptr->total_ecd / 36 / 8192 * 2 * 3.1415926 * 25 ;
}

void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
	CAN_TxHeaderTypeDef chassis_tx_message;
	uint8_t	chassis_can_send_data[8]; 
    chassis_tx_message.StdId = motor_can1;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = motor2 >> 8;
    chassis_can_send_data[3] = motor2;
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;
    HAL_CAN_AddTxMessage(&hcan1, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}


void CAN_cmd_gimbal(int16_t yaw, int16_t Vr, int16_t rev1, int16_t rev2)
{
    uint32_t send_mail_box;
	 CAN_TxHeaderTypeDef gimbal_tx_message;
	uint8_t	gimbal_can_send_data[8]; 
    gimbal_tx_message.StdId = motor_can2;
    gimbal_tx_message.IDE = CAN_ID_STD;
    gimbal_tx_message.RTR = CAN_RTR_DATA;
    gimbal_tx_message.DLC = 0x08;
    gimbal_can_send_data[0] = (yaw >> 8);
    gimbal_can_send_data[1] = yaw;
    gimbal_can_send_data[2] = (Vr >> 8);
    gimbal_can_send_data[3] = Vr;
    gimbal_can_send_data[4] = (rev1 >> 8);
    gimbal_can_send_data[5] = rev1;
    gimbal_can_send_data[6] = (rev2 >> 8);
    gimbal_can_send_data[7] = rev2;
    HAL_CAN_AddTxMessage(&hcan2, &gimbal_tx_message, gimbal_can_send_data, &send_mail_box);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1)
	{		
		CAN_RxHeaderTypeDef RX_Header;
		uint8_t RX_BUFFER[8];
		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RX_Header,RX_BUFFER);	
		static uint8_t i = 0;	
		i = RX_Header.StdId - 0x201;	
		Motor_measure_fun(&Motor_measure[i], RX_BUFFER);
	}	
	if(hcan->Instance == CAN2)
	{
		CAN_RxHeaderTypeDef RX_Header;
		uint8_t RX_BUFFER[8];
		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RX_Header,RX_BUFFER);	
		switch(RX_Header.StdId)
		{
			case 0x205:
			{
				motor_yaw_info.rotor_angle    = ((RX_BUFFER[0] << 8) | RX_BUFFER[1]);
				motor_yaw_info.rotor_speed    = ((RX_BUFFER[2] << 8) | RX_BUFFER[3]);
				motor_yaw_info.torque_current = ((RX_BUFFER[4] << 8) | RX_BUFFER[5]);
				motor_yaw_info.temp           =   RX_BUFFER[6];
				break;
			}
			case 0x206:
			{
				Motor_measure_fun(&Motor_measure[4], RX_BUFFER);
				break;
			}
		}
	}
	static uint16_t tt = 0;
	tt++;
	if(tt >= 500)
	{
		tt=0;
		HAL_GPIO_TogglePin(GPIOH,GPIO_PIN_11);
	}

}



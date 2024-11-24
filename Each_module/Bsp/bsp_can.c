#include "Variable.h"

extern int16_t flag[4];

extern Motor_measure_t Motor_measure_2006[5];
extern Motor_measure_y Motor_measure_6020;
extern uint8_t rx_data[8];

extern CAN_HandleTypeDef hcan1;

void can_filter_init(void)
{

    CAN_FilterTypeDef can_filter_st;	                //����������ṹ��
	can_filter_st.FilterActivation = ENABLE;			//ENABLEʹ�ܹ�����
  can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;	//���ù�����ģʽ--��ʶ������λģʽ
  can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;	//��������λ�� 32 λ
  can_filter_st.FilterIdHigh = 0x0000;				//ID��λ
  can_filter_st.FilterIdLow = 0x0000;					//ID��λ
  can_filter_st.FilterMaskIdHigh = 0x0000;			//�����������λ
  can_filter_st.FilterMaskIdLow = 0x0000;				//�����������λ
    
  can_filter_st.FilterBank = 0;						//��������-˫CAN��ָ��0~27
  can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;	//������������� FIFO
  HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);		//HAL�����ù���������
  HAL_CAN_Start(&hcan1);								//ʹ��CAN1������
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);//ʹ��CAN�ĸ����ж�
 
    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void Motor_measure_fun(Motor_measure_t *ptr,uint8_t* RX_buffer)
{
	ptr->last_ecd = ptr->ecd;		
	ptr->ecd = (uint16_t)((RX_buffer)[0] << 8 | (RX_buffer)[1]); 	//ת�ӻ�е�Ƕ�
	ptr->speed_rpm = (uint16_t)((RX_buffer)[2] << 8 | (RX_buffer)[3]);	//ת��ת��(rpm)
	ptr->torque_current = (uint16_t)((RX_buffer)[4] << 8 | (RX_buffer)[5]);	//ת�ص���
	ptr->temp = (RX_buffer)[6];         
	if(ptr->ecd - ptr->last_ecd > 4096)				
		ptr->round_cnt --;								
	else if(ptr->ecd - ptr->last_ecd < -4096)		
		ptr->round_cnt ++;
	ptr->total_ecd = ptr->round_cnt * 8192 + ptr->ecd;	
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
    HAL_CAN_AddTxMessage(&hcan1, &gimbal_tx_message, gimbal_can_send_data, &send_mail_box);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
	if(hcan==&hcan1)
	{
		if(rx_header.StdId==CAN_6020_ID)
		{
			Motor_measure_6020.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);
			Motor_measure_6020.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);
			Motor_measure_6020.torque_current = ((rx_data[4] << 8) | rx_data[5]);
			Motor_measure_6020.temp           =   rx_data[6];
		}	
		if(rx_header.StdId==CAN_2006_M5_ID)
		{
			Motor_measure_fun(&Motor_measure_2006[4], rx_data);
			Motor_measure_2006[4].total_ecd=0;
			Motor_measure_2006[4].ecd=0;
			Motor_measure_2006[4].last_ecd=0;
			Motor_measure_2006[4].round_cnt=0;
		}	
		if(rx_header.StdId==CAN_2006_M4_ID)
		{
			Motor_measure_fun(&Motor_measure_2006[3], rx_data);
			Motor_measure_2006[3].total_ecd=0;
			Motor_measure_2006[3].ecd=0;
			Motor_measure_2006[3].last_ecd=0;
			Motor_measure_2006[3].round_cnt=0;
		}
		if(rx_header.StdId==CAN_2006_M3_ID)
		{
			Motor_measure_fun(&Motor_measure_2006[2], rx_data);
			Motor_measure_2006[2].total_ecd=0;
			Motor_measure_2006[2].ecd=0;
			Motor_measure_2006[2].last_ecd=0;
			Motor_measure_2006[2].round_cnt=0;
		}
		if(rx_header.StdId==CAN_2006_M2_ID)
		{
			Motor_measure_fun(&Motor_measure_2006[1], rx_data);
			Motor_measure_2006[1].total_ecd=0;
			Motor_measure_2006[1].ecd=0;
			Motor_measure_2006[1].last_ecd=0;
			Motor_measure_2006[1].round_cnt=0;
		}
		if(rx_header.StdId==CAN_2006_M1_ID)
		{
			Motor_measure_fun(&Motor_measure_2006[0], rx_data);
			Motor_measure_2006[0].total_ecd=0;
			Motor_measure_2006[0].ecd=0;
			Motor_measure_2006[0].last_ecd=0;
			Motor_measure_2006[0].round_cnt=0;
		}
		static uint16_t tt = 0;
		tt++;
		if(tt >= 50)
		{
			HAL_GPIO_TogglePin(GPIOH,GPIO_PIN_11);
			tt=0;
		}		
	}
}

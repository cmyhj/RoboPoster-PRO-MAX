#include "bsp_can.h"
#include "stdint.h"
#include "main.h"
#include "cmsis_os.h"
#include "stdbool.h"

uint16_t can_flag1[4];
int16_t flag[4]={0};


Motor_measure_t Motor_measure[14];
CAN_TxHeaderTypeDef CANx_tx_message;


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
	
	can_filter_st.SlaveStartFilterBank = 14;   //˫CANģʽ�¹涨CAN������ģʽ�Ĺ��������䣬�ӹ�����Ϊ14
 	can_filter_st.FilterBank = 14;						//��������-˫CAN��ָ��0~27
 	HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);		//HAL�����ù���������
 	HAL_CAN_Start(&hcan2);								//ʹ��CAN2������
 	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);//ʹ��CAN�ĸ����ж�

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

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RX_Header;	//����֡֡ͷ
	uint8_t RX_BUFFER[8];	//���մ������֡���ݵ�����
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RX_Header,RX_BUFFER);	
	
	static uint8_t i = 0;
	if(hcan==&hcan1)
	{		
		i = RX_Header.StdId - 0x201;	
		Motor_measure_fun(&Motor_measure[i], RX_BUFFER);
	  if(flag[i]!=0)
		{
			if(can_flag1[i]!=0&&i<4)
			{
				Motor_measure[i].total_ecd=0;
				Motor_measure[i].ecd=0;
				Motor_measure[i].last_ecd=0;
				Motor_measure[i].round_cnt=0;
				can_flag1[i]=0;
			}
		}	
		// if (RX_Header.StdId == Plate_Motor.CANID)
 		// Motor_UpdateData(&Plate_Motor, RX_BUFFER);
		flag[i]++;
	}
	else if(hcan == &hcan2)
	{
		//...
	}
}

void Set_motor_cmd(CAN_HandleTypeDef *hcan,uint32_t STDID,int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
	uint32_t send_mail_box;	
	uint8_t CANx_send_data[8];
	CANx_tx_message.StdId = STDID;			
	CANx_tx_message.IDE = CAN_ID_STD;		
  	CANx_tx_message.RTR = CAN_RTR_DATA;	
  	CANx_tx_message.DLC = 0x08;			
	
	CANx_send_data[0] = motor1 >> 8;		
	CANx_send_data[1] = motor1;
	CANx_send_data[2] = motor2 >> 8;
	CANx_send_data[3] = motor2;
	CANx_send_data[4] = motor3 >> 8;
	CANx_send_data[5] = motor3;
	CANx_send_data[6] = motor4 >> 8;
	CANx_send_data[7] = motor4;

	HAL_CAN_AddTxMessage(hcan,&CANx_tx_message, CANx_send_data,&send_mail_box);	
}


/**
  * @brief          send control current of motor (0x201, 0x202, 0x203, 0x204 ,0x205)
  * @param[in]      motor1: (0x201) 2006 motor control current, range [-10000,10000] 
  * @param[in]      motor2: (0x202) 2006 motor control current, range [-10000,10000] 
  * @param[in]      motor3: (0x203) 2006 motor control current, range [-10000,10000] 
  * @param[in]      motor4: (0x204) 2006 motor control current, range [-10000,10000] 
  * @param[in]      motor5: (0x205) 2006 motor control current, range [-10000,10000] 
	* @retval         none
  */
/**
  * @brief          ���͵�����Ƶ���(0x201,0x202,0x203,0x204 ,0x205)
  * @param[in]      motor1: (0x201) 2006������Ƶ���, ��Χ [-10000,10000]
  * @param[in]      motor2: (0x202) 2006������Ƶ���, ��Χ [-10000,10000]
  * @param[in]      motor3: (0x203) 2006������Ƶ���, ��Χ [-10000,10000]
  * @param[in]      motor4: (0x204) 2006������Ƶ���, ��Χ [-10000,10000]
  * @param[in]      motor5: (0x205) 2006������Ƶ���, ��Χ [-10000,10000]
  * @retval         none
  */
void Set_Wheel_Speed(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
	Set_motor_cmd(&hcan1,motor_can1,motor1,motor2,motor3,motor4);
}

void Set_Stir_Speed(int16_t motor5)
{
	Set_motor_cmd(&hcan1,motor_can2,motor5,0,0,0);
}


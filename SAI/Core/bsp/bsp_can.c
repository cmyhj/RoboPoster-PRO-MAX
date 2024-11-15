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
	CAN_FilterTypeDef can_filter_st;	                //定义过滤器结构体
	can_filter_st.FilterActivation = ENABLE;			//ENABLE使能过滤器
	can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;	//设置过滤器模式--标识符屏蔽位模式
	can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;	//过滤器的位宽 32 位
	can_filter_st.FilterIdHigh = 0x0000;				//ID高位
	can_filter_st.FilterIdLow = 0x0000;					//ID低位
	can_filter_st.FilterMaskIdHigh = 0x0000;			//过滤器掩码高位
	can_filter_st.FilterMaskIdLow = 0x0000;				//过滤器掩码低位
		
	can_filter_st.FilterBank = 0;						//过滤器组-双CAN可指定0~27
	can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;	//与过滤器组管理的 FIFO
	HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);		//HAL库配置过滤器函数
	HAL_CAN_Start(&hcan1);								//使能CAN1控制器
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);//使能CAN的各种中断
	
	can_filter_st.SlaveStartFilterBank = 14;   //双CAN模式下规定CAN的主从模式的过滤器分配，从过滤器为14
 	can_filter_st.FilterBank = 14;						//过滤器组-双CAN可指定0~27
 	HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);		//HAL库配置过滤器函数
 	HAL_CAN_Start(&hcan2);								//使能CAN2控制器
 	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);//使能CAN的各种中断

}

void Motor_measure_fun(Motor_measure_t *ptr,uint8_t* RX_buffer)
{
	ptr->last_ecd = ptr->ecd;		
	ptr->ecd = (uint16_t)((RX_buffer)[0] << 8 | (RX_buffer)[1]); 	//转子机械角度
	ptr->speed_rpm = (uint16_t)((RX_buffer)[2] << 8 | (RX_buffer)[3]);	//转子转速(rpm)
	ptr->torque_current = (uint16_t)((RX_buffer)[4] << 8 | (RX_buffer)[5]);	//转矩电流
	ptr->temp = (RX_buffer)[6];         
	if(ptr->ecd - ptr->last_ecd > 4096)				
		ptr->round_cnt --;								
	else if(ptr->ecd - ptr->last_ecd < -4096)		
		ptr->round_cnt ++;
	ptr->total_ecd = ptr->round_cnt * 8192 + ptr->ecd;	
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RX_Header;	//数据帧帧头
	uint8_t RX_BUFFER[8];	//接收存放数据帧数据的数组
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
  * @brief          发送电机控制电流(0x201,0x202,0x203,0x204 ,0x205)
  * @param[in]      motor1: (0x201) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor2: (0x202) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor3: (0x203) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor4: (0x204) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor5: (0x205) 2006电机控制电流, 范围 [-10000,10000]
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


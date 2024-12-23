#include "Variable.h"

int16_t i=0;
int16_t v[6]={0};
int16_t a[6];
int16_t Angle1=0;
int16_t Angle2=0;
int16_t flag[4]={0};
int16_t V_yaw = 0;
int16_t V_r = 0;
int16_t A_yaw = 0;
int16_t A_r = 0;
uint8_t rx_data[8];
uint16_t can_flag1[4];
uint8_t bil[6]={0};
int16_t V_x = 0;
int16_t V_y = 0;
int16_t t1 = 0;
int16_t t123 = 0;
uint8_t receive_USART1[4];
uint8_t Transmit_USART6[3];
int16_t t_screen = 0;

moto_info_t motor_yaw_info;
Motor_measure_t Motor_measure[5];

pid_struct_t M6020_yaw_speed_pid;
pid_struct_t M6020_yaw_angle_pid;

pid_struct_t M2006_speed_pid[5];
pid_struct_t M2006_angle_pid[5];

CAN_TxHeaderTypeDef CANx_tx_message;

#define M2006_MOTOR_RPM_TO_VECTOR (2*3.1415926f*WHEEL_RADIUS/100/60/36)


#define MOTOR_SPEED_TO_CHASSIS_SPEED_VX 0.25f
#define MOTOR_SPEED_TO_CHASSIS_SPEED_VY 0.25f
#define MOTOR_SPEED_TO_CHASSIS_SPEED_WZ 0.25f

#define MOTOR_DISTANCE_TO_CENTER 0.2f


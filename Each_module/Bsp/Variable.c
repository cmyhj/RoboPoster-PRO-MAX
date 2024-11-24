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

Motor_measure_t Motor_measure_2006[5];
Motor_measure_y Motor_measure_6020;

pid_struct_t M6020_yaw_speed_pid;
pid_struct_t M6020_yaw_angle_pid;

pid_struct_t M2006_speed_pid[5];
pid_struct_t M2006_angle_pid[5];

CAN_TxHeaderTypeDef CANx_tx_message;


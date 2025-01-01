#include "Variable.h"
 
void pid_init(pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max)
{
  pid->kp      = kp;
  pid->ki      = ki;
  pid->kd      = kd;
  pid->i_max   = i_max;
  pid->out_max = out_max;
}
 
 
float pid_calc(pid_struct_t *pid, float ref, float fdb)
{
  pid->ref = ref;
  pid->fdb = fdb;
  pid->err[1] = pid->err[0];
  pid->err[0] = pid->ref - pid->fdb;
  
  pid->p_out  = pid->kp * pid->err[0];
  pid->i_out += pid->ki * pid->err[0];
  pid->d_out  = pid->kd * (pid->err[0] - pid->err[1]);
  LIMIT_MIN_MAX(pid->i_out, -pid->i_max, pid->i_max);
  
  pid->output = pid->p_out + pid->i_out + pid->d_out;
  LIMIT_MIN_MAX(pid->output, -pid->out_max, pid->out_max);
  return pid->output;
}
 
void PID_init()
{
	pid_init(&M6020_yaw_speed_pid, 300, 1, 2, 30000, 30000);
	pid_init(&M6020_yaw_angle_pid, 0.5, 0, 0, 300, 8000);
	
	pid_init(&M2006_speed_pid[0], 1, 0, 0, 10000, 10000);
	pid_init(&M2006_speed_pid[1], 1, 0, 0, 10000, 10000);
	pid_init(&M2006_speed_pid[2], 1, 0, 0, 10000, 10000);
	pid_init(&M2006_speed_pid[3], 1, 0, 0, 10000, 10000);
	
	pid_init(&M2006_angle_pid_x, 0.5, 0, 0, 300, 8000);
	pid_init(&M2006_angle_pid_y, 0.5, 0, 0, 300, 8000);
	
	pid_init(&M2006_speed_pid_stop, 1, 0, 0, 10000, 10000);
	pid_init(&M2006_angle_pid_stop, 1, 0, 0, 300, 8000);
	
	pid_init(&M2006_speed_pid[4], 0.5, 0, 0, 5000, 5000);
	pid_init(&M2006_angle_pid_r, 0.5, 0, 0, 300, 8000);
}

void Slow_up()
{
//	V_x/10*K - Motor_measure[0].speed_rpm*2*3.1415926f/60/36*50;
//	V_x/10*K - Motor_measure[2].speed_rpm*2*3.1415926f/60/36*50;	
//	V_y/10*K - Motor_measure[1].speed_rpm*2*3.1415926f/60/36*50;
//	V_y/10*K - Motor_measure[3].speed_rpm*2*3.1415926f/60/36*50;
}

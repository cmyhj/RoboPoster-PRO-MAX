#include "Variable.h"
 
extern pid_struct_t M6020_yaw_speed_pid;
extern pid_struct_t M6020_angle_speed_pid;
extern pid_struct_t M2006_speed_pid[5];
extern pid_struct_t M2006_angle_pid[5];
 
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
	pid_init(&M6020_yaw_angle_pid, 0.5, 0, 0, 30, 100);
	
	pid_init(&M2006_speed_pid[0], 1, 0, 0, 30000, 30000);
	pid_init(&M2006_angle_pid[0], 1, 0, 0, 30, 100);
	pid_init(&M2006_speed_pid[1], 1, 0, 0, 30000, 30000);
	pid_init(&M2006_angle_pid[1], 1, 0, 0, 30, 100);
	pid_init(&M2006_speed_pid[2], 1, 0, 0, 30000, 30000);
	pid_init(&M2006_angle_pid[2], 1, 0, 0, 30, 100);
	pid_init(&M2006_speed_pid[3], 1, 0, 0, 30000, 30000);
	pid_init(&M2006_angle_pid[3], 1, 0, 0, 30, 100);
	
	pid_init(&M2006_speed_pid[4], 1, 0, 0, 30000, 30000);
	pid_init(&M2006_angle_pid[4], 1, 0, 0, 30, 100);
}

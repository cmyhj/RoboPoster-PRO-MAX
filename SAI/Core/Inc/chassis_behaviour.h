#ifndef __CHASSIS_BEHAVIOUR_H
#define __CHASSIS_BEHAVIOUR_H

#include "stdio.h"
#include "pid.h"
#include "chassis.h"

void chassis_behaviour_control_set(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector);
void chassis_move_and_rotate_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector);
void chassis_stop_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector);
static void  chassis_infrared_control(float *vx_set, float *vy_set, float *wz_set, chassis_move_t *chassis_move_vector);

#endif

#ifndef __CHASSISFUCTION_H__
#define __CHASSISFUCTION_H__

#include "AllInclude.h"

float float_constrain(float Value, float minValue, float maxValue);
void chassis_init(chassis_move_t *chassis_move_init);
void chassis_feedback_update(chassis_move_t *chassis_move_update);
void gyro_update(chassis_move_t *chassis_gyro_update);
void chassis_set_control(chassis_move_t *chassis_move_control);
void chassis_vector_to_mecanum_wheel_speed(const float vx_set, const float vy_set, const float wz_set, float wheel_speed[4]);
void chassis_control_loop(chassis_move_t *chassis_move_control_loop);
void chassis_control_loop_limit(chassis_move_t *chassis);


#endif // !__CHASSISFUCTION_H__

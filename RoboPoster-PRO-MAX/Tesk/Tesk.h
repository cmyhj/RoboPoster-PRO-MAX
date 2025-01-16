#ifndef __Tesk_H
#define __Tesk_H

#include "Variable.h"

void Test_Task(void const * argument);
void Car_move_x(int x);
void Car_move_y(int y);
void Car_r(float x);
void Car_t(float x);
//void Car_ground_1(int Test);
//void Car_ground_2( int Test);
void Car_get(void);
void Car_put_first(void);
void Car_put_second(void);
void Car_put_stack_task(void);
void Car_put_stack(int i);
void Car_take_first(void);
void choose_plate(int i);
void Car_take_stack(void);


#endif

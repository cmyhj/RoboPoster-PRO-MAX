#include "Variable.h"
#define car_r_distance 120
#define car_r_angle_distance 160
#define car_r_angle 30
void Test_Task(void const *argument)
{
	while (1)
	{
		if (1)
		{

			Car_t(120);

			Un_Claw;
			Un_Sink;
			// 读取二维码
			// while(1);
			t_GM65 = 1;
			while (Test[0] == 0)
			{
				osDelay(1);
			}
			t_GM65 = 0;
			Car_stack_material_task();
			while (1)
				;
			// alltask:
			//			Car_t(120);
			//		  Car_move_x(80);
			//		  Car_move_y(650);
			// 读取二维码
			//

			// Car_move_y(800);
			// 任务1
			// 取物料函数
			// Car_get();
			//
			// Car_move_y(-400);
			// Car_move_x(2020);
			// 任务1
			// 放取物料函数在粗加工区
			//		Car_put_first();
			//		Car_take_first();
			//		//
			//		Car_move_y(900);
			//		Car_move_x(-900);
			//		//任务1
			//		//放物料函数至暂存区
			//		Car_put_second();
			//		//
			//		Car_move_x(-970);
			//		Car_move_y(-500);
			//		//任务2
			//			for (int i=0;i<3;i++)//renewtask
			//			{
			//				Test[i]=Test[i+3];
			//			}
			//		//取物料函数
			//		Car_get();
			//		//
			//		Car_move_y(-400);
			//		Car_move_x(2020);
			//		//任务2
			//		//放取物料函数在粗加工区
			//		Car_put_first();
			//		Car_take_first();
			//		//
			//		Car_move_y(900);
			//		Car_move_x(-900);
			//		//任务2
			//		//放物料（堆叠）函数
			//		Car_stack_material();
			//		//
			//		Car_move_x(-970);
			//		Car_move_y(-2100);
			//		Car_move_x(-80);

			// Car_ground();
			// t_stop = 1;
			// t_theta = 1;
			// Angle_yaw = 120;
			// osDelay(1000);
			// t_stop = 0;
			// t_move = 1;
			// Car_move_x(90);
			// Car_move_y(650);
			// osDelay(1000);
			// t_theta = 1;
			// Angle_yaw = 30;
			// Car_move_y(800);
			// t_move = 0;
			// t_stop = 1;
			// //取物料
			// t_stop = 0;
			// t_move = 1;
			// Car_move_y(-400);
			// Car_move_x(1750);
			// //放物料
			// //取物料
			// Car_move_y(830);
			// Car_move_x(-810);
			// //放物料
			// Car_move_x(-940);
			// Car_move_y(-507.5);
			// Car_move_y(800);
			// t_move = 0;
			// t_stop = 1;
			// //取物料
			// t_stop = 0;
			// t_move = 1;
			// Car_move_y(-400);
			// Car_move_x(1750);
			// //放物料
			// //取物料
			// Car_move_y(830);
			// Car_move_x(-810);

			// r = 50;
			// 	osDelay(2000);
			// 	r = 100;
			// 	while(abs(r-mil[4])>0.5)
			// 	{
			// 		osDelay(1);
			// 	}

			// t_move = 1;
			// 	r += 100;
			// 	while(abs(mil[4]-r)>1) {}
			// Un_Sink;
			// t_OpenMV =1;
			// 		quit_y = Get_cc[1] ;
			// 		quit_x = Get_cc[0];
			// while((abs(quit_y/Get_cc[5])>0.1)&&(abs(quit_x/Get_cc[5])>0.1))
			// {
			// quit_x = Get_cc[0];
			// quit_y = Get_cc[1];
			// 	r += quit_y/Get_cc[5]*10;
			// 	while(abs(mil[4]-r)>1) {}
			// 	Target_y += quit_y/Get_cc[5]*10;
			// 	while(abs(Target_y-C_y)>0.1) {}
			// 	osDelay(1);
			// //		  t123=0;
			// }
			// Sink_ground;
			// osDelay(1);
			// t_low = 1;
			// 		quit_y = Get_cc[1] ;
			// 		quit_x = Get_cc[0];
			// while((abs(quit_y/Get_cc[5])>0.1)&&(abs(quit_x/Get_cc[5])>0.1))
			// {
			// quit_x = Get_cc[0];
			// quit_y = Get_cc[1];
			// 	r += quit_y/Get_cc[5]*10;
			// 	while(abs(mil[4]-r)>1) {}
			// 	Target_y += quit_y/Get_cc[5]*10;
			// 	while(abs(Target_y-C_y)>0.1) {}
			// 	osDelay(1);
			// //		  t123=0;
			// }
			// int x_r,y_r;
			// x_r = Target_y;
			// y_r = mil[4];
			// 	Car_r(0,0);
			// Sink_plate;
			// osDelay(1000);
			// Claw;
			// osDelay(1000);
			// Un_Sink;
			// osDelay(1000);
			// Car_r(y_r,0);
			// Sink_ground;
			// osDelay(1000);

			// Un_Claw;
			// osDelay(1000);
			// Un_Sink;
			// osDelay(1000);
			// Car_r(0,0);
			//   t123=0;
		}
	}
}

void Car_move_x(int x)
{
	t_move = 1;
	t_x = 1;
	Target_x += x;
	while (fabsf(Target_x - C_x) > 1)
	{
		osDelay(1);
	}
	t_x = 0;
	osDelay(1);
	t_move = 0;
}

void Car_move_y(int y)
{
	t_move = 1;
	t_y = 1;
	Target_y += y;
	while (fabsf(Target_y - C_y) > 1)
	{
		osDelay(1);
	}
	t_y = 0;
	osDelay(1);
	t_move = 0;
}

void Car_r(float x)
{
	r += x;
	t_r = 1;
	while (fabs(r - mil[4]) > 0.5)
	{
		osDelay(1);
	}
	t_r = 0;
}

void Car_t(float x)
{
	Angle_yaw = x;
	t_theta = 1;
	while (fabsf(Angle_yaw - Theta) > 1)
	{
		osDelay(1);
	}
}

// void Car_ground_1( int Test)
//{
//	int y_r;
//	t_move = 1;
//	t_y = 1;
////	t_x = 1;
//	t_r = 1;
//	t_low = 0;
//	t_OpenMV = 1;
//	osDelay(100);
//	quit_x = 10.0f * Get_cc[0] / Get_cc[5];
//	quit_y = 10.0f * Get_cc[1] / Get_cc[5];
//	Car_move_y(quit_x);
//	Car_r(quit_y);
//	Sink_ground;
//	t_low = 1;
//	osDelay(1000);
//	quit_x = 10.0f * Get_cc[0] / Get_cc[5];
//	quit_y = 10.0f * Get_cc[1] / Get_cc[5];
//	Car_move_y(quit_x);
//	Car_r(quit_y);
//	Car_r(-r);
//	Sink_plate;
//	osDelay(1000);
//	Claw;
//	osDelay(1000);
//	Un_Sink;
//	osDelay(1000);
//	Car_r(y_r);
//	Sink_ground;
//	osDelay(1000);
//	Un_Claw;
//	osDelay(1000);
//	Un_Sink;
//	osDelay(1000);
//	Car_r(-r);
//}

// void Car_ground_2( int Test)
//{
//	int y_r;
//	t_move = 1;
////	t_y = 1;
//	t_x = 1;
//	t_r = 1;
//	t_low = 0;
//	t_OpenMV = 1;
//	osDelay(100);
//	quit_x = 10.0f * Get_cc[0] / Get_cc[5];
//	quit_y = 10.0f * Get_cc[1] / Get_cc[5];
//	Car_move_x(-quit_x);
//	Car_r(quit_y);
//	Sink_ground;
//	t_low = 1;
//	osDelay(1000);
//	quit_x = 10.0f * Get_cc[0] / Get_cc[5];
//	quit_y = 10.0f * Get_cc[1] / Get_cc[5];
//	Car_move_x(-quit_x);
//	Car_r(quit_y);
//	Car_r(-r);
//	Sink_plate;
//	osDelay(1000);
//	Claw;
//	osDelay(1000);
//	Un_Sink;
//	osDelay(1000);
//	Car_r(y_r);
//	Sink_ground;
//	osDelay(1000);
//	Un_Claw;
//	osDelay(1000);
//	Un_Sink;
//	osDelay(1000);
//	Car_r(-r);
//}

void Car_get()
{

	for (int8_t i = 0; i < 3; i++)
	{
		Un_Claw;
		Un_Sink;
		Car_r(car_r_distance);

		choose_plate(Test[i]);
		t_OpenMV = 1;
		osDelay(100);
		while (Get_cc[3] == 0 && Get_cc[2] == 0)
			;
		while (fabsf(1.0f * Get_cc[2]) > 10 || fabsf(1.0f * Get_cc[3]) > 10)
		{ // 物料不在中心或物料非任务目标
			osDelay(1);
			if (fabsf(10.0f * Get_cc[3] / Get_cc[5]) < 20)
			{
				t_move = 1;
				t_y = 1;
				quit_x = 10.0f * Get_cc[2] / Get_cc[5];
				Car_move_y(-quit_x);
			}
			else if (fabsf(10.0f * Get_cc[2] / Get_cc[5]) < 20)
			{
				t_move = 1;
				t_x = 1;
				quit_y = 10.0f * Get_cc[3] / Get_cc[5];
				Car_move_x(-quit_y);
			}
		}
		while (fabsf(10.0f * Get_cc[2] / Get_cc[5]) > 10 || fabsf(10.0f * Get_cc[3] / Get_cc[5]) > 10 || Get_cc[4] != Test[i])
		{
			osDelay(1);
		}
		Sink_firstplat;
		Claw;
		Un_Sink;
		Car_r(-r);
		Sink_plate;
		Un_Claw;
		Un_Sink;
	}
}

void Car_put_first()
{
	Un_Claw;
	Un_Sink;
	Car_r(car_r_distance);
	t_low = 0;
	t_OpenMV = 1;
	osDelay(100);
	quit_x = 10.0f * Get_cc[0] / Get_cc[5];
	quit_y = 10.0f * Get_cc[1] / Get_cc[5];
	Car_move_y(quit_x);
	t_y = 0;
	t_x = 1;
	Car_move_x(quit_y);
	Sink_ground;
	t_low = 1;
	osDelay(1000);
	quit_x = 10.0f * Get_cc[0] / Get_cc[5];
	quit_y = 10.0f * Get_cc[1] / Get_cc[5];
	Car_move_y(quit_x);
	Car_move_x(quit_y);
	Un_Claw;
	Un_Sink;
	Car_r(-r);
	for (int8_t i = 0; i < 3; i++)
	{
		Car_put_material(Test[i]);
	}
}
void Car_put_second()
{
	Un_Claw;
	Un_Sink;
	Car_r(car_r_distance);
	t_low = 0;
	t_OpenMV = 1;
	osDelay(100);
	quit_x = 10.0f * Get_cc[0] / Get_cc[5];
	quit_y = 10.0f * Get_cc[1] / Get_cc[5];
	Car_move_x(-quit_x);
	Car_move_y(quit_y);
	Sink_ground;
	t_low = 1;
	osDelay(1000);
	quit_x = 10.0f * Get_cc[0] / Get_cc[5];
	quit_y = 10.0f * Get_cc[1] / Get_cc[5];
	Car_move_x(-quit_x);
	Car_move_y(quit_y);
	Un_Claw;
	Un_Sink;
	Car_r(-r);
	for (int8_t i = 0; i < 3; i++)
	{
		Car_put_material(Test[i]);
	}
}

void Car_stack_material(int i)
{
// 丝杆复位，夹爪高位进入此函数
	switch (i)
	{
	case 1: // 红
		Car_t(-car_r_angle + Theta);
		break;
	case 2: // 绿
		break;
	case 3: // 蓝
		Car_t(car_r_angle + Theta);
		break;
	default:
		break;
	}
	choose_plate(i);
	Sink_plate;
	Claw;
	Un_Sink;
	switch (i)
	{
	case 2: // 绿
		Car_r(car_r_distance);
		break;
	case 3: // 蓝
	case 1: // 红
		Car_r(car_r_angle_distance);
	default:
		break;
	}
	Sink_material;
	Un_Claw;
	Un_Sink;
	switch (i) // 回收
	{
	case 2: // 绿
		Car_r(-r);
		break;
	case 3: // 蓝
		Car_t(Theta - car_r_angle);
		Car_r(-r);
		break;
	case 1: // 红
		Car_t(Theta + car_r_angle);
		Car_r(-r);
	default:
		break;
	}
	osDelay(1);
}

void Car_put_material(int i)
{
	// 丝杆复位，夹爪高位进入此函数
	switch (i)
	{
	case 1: // 红
		Car_t(-car_r_angle + Theta);
		break;
	case 2: // 绿
		break;
	case 3: // 蓝
		Car_t(car_r_angle + Theta);
		break;
	default:
		break;
	}
	choose_plate(i);
	Sink_plate;
	Claw;
	Un_Sink;
	switch (i)
	{
	case 2: // 绿
		Car_r(car_r_distance);
		break;
	case 3: // 蓝
	case 1: // 红
		Car_r(car_r_angle_distance);
	default:
		break;
	}
	Sink_ground;
	Un_Claw;
	Un_Sink;
	switch (i) // 回收
	{
	case 2: // 绿
		Car_r(-r);
		break;
	case 3: // 蓝
		Car_t(Theta - car_r_angle);
		Car_r(-r);
		break;
	case 1: // 红
		Car_t(Theta + car_r_angle);
		Car_r(-r);
	default:
		break;
	}
	osDelay(1);
}

void Car_stack_material_task()
{
	t_low = 0;
	Car_r(car_r_distance);
	t_OpenMV = 1;
	osDelay(100);
	while (Get_cc[3] == 0 && Get_cc[2] == 0);
	while (fabsf(1.0f * Get_cc[2]) > 10 || fabsf(1.0f * Get_cc[3]) > 10)
	{ // 物料不在中心或物料非任务目标
		osDelay(1);
		quit_x = 10.0f * Get_cc[2] / Get_cc[5];
		Car_move_x(-quit_x);
		quit_y = 10.0f * Get_cc[3] / Get_cc[5];
		Car_move_y(quit_y);
	}
	Un_Claw;
	Un_Sink;
	Car_r(-r);
	for (int8_t i = 0; i < 3; i++)
	{
		Car_stack_material(Test[i]);
	}
}

void Car_take_first()
{
	t_move = 1;
	t_y = 1;
	//	t_x = 1;
	t_r = 1;
	t_low = 0;
	t_OpenMV = 1;
	osDelay(100);
	quit_x = 10.0f * Get_cc[2] / Get_cc[5];
	quit_y = 10.0f * Get_cc[3] / Get_cc[5] + 2; // 色块中心偏置+2cm
	Car_move_y(quit_x);
	Car_move_x(quit_y);
	Car_take_stack();
}
void choose_plate(int i)
{
	t_plant = i;
}
void Car_take_stack()
{
	// 丝杆复位，夹爪高位进入此函数
	switch (i)
	{
	case 0: // 红
		Car_t(car_r_angle + Theta);
		break;
	case 1: // 绿
		break;
	case 2: // 蓝
		Car_t(-car_r_angle + Theta);
		break;
	default:
		break;
	}
	choose_plate(i);
	Sink_plate;
	Claw;
	Un_Sink;
	switch (i)
	{
	case 1: // 绿
		Car_r(car_r_distance);
		break;
	case 2: // 蓝
	case 0: // 红
		Car_r(car_r_angle_distance);
	default:
		break;
	}
	Sink_ground;
	Un_Claw;
	Un_Sink;
	switch (i) // 回收
	{
	case 1: // 绿
		Car_r(-r);
		break;
	case 2: // 蓝
		Car_t(Theta + car_r_angle);
		Car_r(-r);
		break;
	case 0: // 红
		Car_t(Theta - car_r_angle);
		Car_r(-r);
	default:
		break;
	}
}

/*
 * my_header.h
 *
 *  Created on: 18 sty 2021
 *      Author: KacperSiemianowski
 */

#ifndef INC_MY_HEADER_H_
#define INC_MY_HEADER_H_

#include "gpio.h"
#include <math.h>
#include "usart.h"
#include "lwip.h"
#include"lwip/opt.h"
#include"lwip/arch.h"
#include"lwip/api.h"

#define L1 200.0f
#define L2 160.0f
#define L3 290.0f

struct peryferia
{
	GPIO_TypeDef *port;
	uint16_t pin;
};

struct axis
{
	int count_of_motors;
	int morots[2];
	int min_degrees;
	int max_degrees;
	int steps_counter;	// counterclockwise decreases clockwise increases
	int steps_by_turn;	//number of steps needed for full rotation
	_Bool sensor_activated; //1-in home position
	_Bool direction;	//0-counterclockwise, 1-clockwise
	_Bool error;	//0-no error, 1-error
	_Bool full_step;
};

void activation(char to_activate);
void axis_enable(int i);
void home_position();
int degrees_to_steps(int axis, float degrees);
float steps_to_degrees(int axis,int steps);
_Bool set_axis_position(int axis,int degrees);
void point_by_angles(float fi1,float fi2, float fi3,float *px,float *py,float *pz);
void go_to_point(float px,float py,float pz,float *fi1,float *fi2, float *fi3);
void global_variables_initialization();
void wyslij(char* wiadomosc);
float round_float_precision(float x, unsigned digits);
int pow_i(int base, int exp);
void regulator();
void server_TCP(void);

float current_px;
float current_py;
float current_pz;
float target_px;
float target_py;
float target_pz;
float fi1;
float fi2;
float fi3;
struct peryferia in_out_table[23];
struct axis main_axis[6];
_Bool activation_table[21];//ostatni element tylko do testów
int stop_home;
_Bool first_lap;
char received;
_Bool regulator_ready;




#endif /* INC_MY_HEADER_H_ */

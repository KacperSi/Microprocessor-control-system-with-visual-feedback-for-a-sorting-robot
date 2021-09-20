#include "my_header.h"

void server_TCP(void)
{
    struct netconn *connection, *newconnection;
    err_t err, accept_err;
    struct netbuf* buffer;
    char* data;
    u16_t len;
    err_t recv_err;
    //Macro creating a new structure and initialising it with new values
    connection = netconn_new(NETCONN_TCP);
    if (connection != NULL)
    {
        err = netconn_bind(connection, NULL, 2000);       //Bind connection to port 2000
        if (err == ERR_OK)
        {
            netconn_listen(connection);                 //Connection switched to listening mode
            while (1)
            {
                //Wait for connection with client
                accept_err = netconn_accept(connection, &newconnection);
                //Connectiong handling
                if (accept_err == ERR_OK)
                {
                    while ((recv_err = netconn_recv(newconnection, &buffer)) == ERR_OK)
                    {


                        do
                        {
                           netbuf_data(buffer, &data, &len);
                           char message=*data;
                           if ( message== 'L') {
                           		activation_table[14] = 1;
                           		activation_table[15] = 0;
                           		activation_table[16] = 0;
                           	}
                           	if (message == 'P') {
                           		activation_table[15] = 1;
                           		activation_table[14] = 0;
                           		activation_table[16] = 0;
                           	}
                           	if (message == 'X') {
                           		activation_table[14] = 0;
                           		activation_table[15] = 0;
                           		activation_table[16] = 1;
                           	}
                           	if (message == 'G') {
                           		activation_table[17] = 1;
                           		activation_table[18] = 0;
                           		activation_table[19] = 0;
                           	}
                           	if (message == 'D') {
                           		activation_table[17] = 0;
                           		activation_table[18] = 1;
                           		activation_table[19] = 0;
                           	}
                           	if (message == 'Y') {
                           		activation_table[17] = 0;
                           		activation_table[18] = 0;
                           		activation_table[19] = 1;
                           	}



                        }
                        while (netbuf_next(buffer) >= 0);
                       netbuf_delete(buffer);
                    }
                    //Close the connection
                    netconn_close(newconnection);
                    netconn_delete(newconnection);
                }
            }
        }
        else { netconn_delete(newconnection); }  //the connection cannot be established
    }
}

void regulator()
{
	_Bool ax1=0,ax2=0,ax3=0;
	if(activation_table[16]==0||activation_table[19]==0)
	{
	if(regulator_ready==1)
	{
	fi1=steps_to_degrees(1,main_axis[0].steps_counter);
	fi2=steps_to_degrees(2,main_axis[1].steps_counter);
	fi3=steps_to_degrees(3,main_axis[2].steps_counter);
	if(fl==0)
	{
	point_by_angles(fi1,fi2,fi3,&current_px,&current_py,&current_pz);
	target_px=current_px;
	target_py=current_py;
	target_pz=current_pz;
	fl=1;
	}
	if(activation_table[14]==1)
	{
		target_px=target_px+5;
	}
	if(activation_table[15]==1)
	{
		target_px=target_px-5;
	}
	if(activation_table[17]==1)
	{
		target_py=target_py+5;
	}
	if(activation_table[18]==1)
	{
		target_py=target_py-5;
	}
	go_to_point(target_px,target_py,target_pz,&fi1,&fi2,&fi3);
	}
	}
	ax1=set_axis_position(1, fi1);
	ax2=set_axis_position(2, fi2);
	ax3=set_axis_position(3, fi3);
	if(ax1==1&&ax2==1&&ax3==1)
	{
		regulator_ready=1;
	}
	else
	{
		regulator_ready=0;
	}

}

void go_to_point(float px,float py,float pz,float *fi1,float *fi2, float *fi3)
{
	float R=sqrt(pow(px,2)+pow(py,2));
	float f=sqrt(pow(R,2)+pow((pz-L1),2));
	*fi1 = -atan2(py,px)*180.0f/M_PI;
	float cosb=(pow(L3,2)-pow(L2,2)-pow(f,2))/(-2*L2*f);
	cosb=round_float_precision(cosb, 1);
	float sinb=sqrt(1.0f-pow(cosb,2));
	*fi2=-(90.0f-atan2((pz-L1),sqrt(pow(px,2)+pow(py,2)))*180.0f/M_PI-atan2(sinb,cosb)*180.0f/M_PI);
	float cosa=(pow(f,2)-pow(L2,2)-pow(L3,2))/(2*L2*L3);
	cosa=round_float_precision(cosa, 1);
	float sina=sqrt(1.0f-pow(cosa,2));
	*fi3=-(atan2(sina,cosa)*180.0f/M_PI);
}
float round_float_precision(float x, unsigned digits){
    unsigned precision = pow_i(10, digits);
    return roundf(x*precision)/precision;
}
int pow_i(int base, int exp){
    int result = 1;
    while (exp){
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

void point_by_angles(float fi1,float fi2, float fi3,float *px,float *py,float *pz)
{
	fi1 =( fi1 * M_PI ) / 180.0f;
	fi2 =( fi2 * M_PI ) / 180.0f;
	fi3 =( fi3 * M_PI ) / 180.0f;
	*pz=L1+L2*cos(fi2)+L3*cos(fi3+fi2);
	float z=L2*sin(-fi2)+L3*sin(-fi3-fi2);
	*px=z*cos(fi1);
	*py=z*sin(-fi1);
}

void home_position()
{
	stop_home = 1;
	if (first_lap == 1) {
		for (int i = 1; i <= 6; i++) {
			main_axis[i - 1].steps_counter = 0;
			main_axis[i - 1].direction = 0;
			main_axis[i - 1].sensor_activated = 0;
			main_axis[i - 1].error = 0;
		}
		first_lap = 0;
	}
	for (int i = 1; i <= 4; i++) {
		if (main_axis[i - 1].sensor_activated == 0
				&& main_axis[i - 1].error != 1) {
			activation_table[i] = 1;
			stop_home = 0;
		}
		if (main_axis[i - 1].sensor_activated == 1) {
			activation_table[i] = 0;
		}
		if (i == 1 || i == 4) {
			if ((main_axis[i - 1].steps_counter < -(main_axis[i - 1].steps_by_turn / 2))) {
				main_axis[i - 1].direction = 1;
			}
			if ((main_axis[i - 1].steps_counter > (main_axis[i - 1].steps_by_turn / 2))) {
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
				main_axis[i - 1].error = 1;
				activation_table[i] = 0;
			}
		}
		if (i == 2 || i == 3 || i == 5) {
			int z = 2;
			if (i == 5) {
				z = 1;
			}
			if (main_axis[i - 1].steps_counter >= -(main_axis[i - 1].steps_by_turn / z + 30)) {
				main_axis[i - 1].direction = 0;
			}
			else {
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
				activation_table[i] = 0;
				main_axis[i - 1].error = 1;
			}
		}
		if (activation_table[i] == 1) {
			axis_enable(i);
		}
	}
	if (stop_home == 1) {
		activation_table[7] = 0;
		for (int i = 1; i <= 6; i++) {
			main_axis[i - 1].steps_counter = 0;
		}
		activation_table[13] = 0;
	}

}
int degrees_to_steps(int axis, float degrees)
{
	return main_axis[axis-1].steps_by_turn*degrees/(main_axis[axis-1].max_degrees-main_axis[axis-1].min_degrees);
}
float steps_to_degrees(int axis,int steps)
{
	return steps*(main_axis[axis-1].max_degrees-main_axis[axis-1].min_degrees)/main_axis[axis-1].steps_by_turn;
}

_Bool set_axis_position(int axis,int degrees)
{

	if(degrees>=main_axis[axis-1].min_degrees&& degrees<=main_axis[axis-1].max_degrees)
	{

		if(main_axis[axis-1].steps_counter > degrees_to_steps(axis,degrees))
		{
			main_axis[axis-1].direction=1;
			axis_enable(axis);
			activation_table[axis] = 1;
			return 0;
		}
		if(main_axis[axis-1].steps_counter < degrees_to_steps(axis,degrees))
		{
			main_axis[axis-1].direction=0;
			axis_enable(axis);
			activation_table[axis] = 1;
			return 0;
		}
		if(main_axis[axis-1].steps_counter == degrees_to_steps(axis,degrees))
		{
			activation_table[axis] = 0;
			return 1;
		}
	}
}

void activation(char to_activate)
{
	if (to_activate == 'L') {
		activation_table[14] = 1;
		activation_table[15] = 0;
		activation_table[16] = 0;
	}
	if (to_activate == 'P') {
		activation_table[15] = 1;
		activation_table[14] = 0;
		activation_table[16] = 0;
	}
	if (to_activate == 'X') {
		activation_table[14] = 0;
		activation_table[15] = 0;
		activation_table[16] = 1;
	}
	if (to_activate == 'G') {
		activation_table[17] = 1;
		activation_table[18] = 0;
		activation_table[19] = 0;
	}
	if (to_activate == 'D') {
		activation_table[17] = 0;
		activation_table[18] = 1;
		activation_table[19] = 0;
	}
	if (to_activate == 'Y') {
		activation_table[17] = 0;
		activation_table[18] = 0;
		activation_table[19] = 1;
	}

	if (to_activate == 'k') {
		activation_table[10] = 0;
		activation_table[8]=0;
		activation_table[7]=0;
		for(int i=1;i<=6;i++)
		{
			activation_table[i]=0;
		}
		activation_table[11] = 1;
		fl=0;//test
	}
	////////////////////////////////////////////////// one axis movement
	if (to_activate == 'o') {
		activation_table[10] = 1;
	}
	if (to_activate == 'g') {
		for (int i = 1; i <= 6; i++) {
			activation_table[i] = 0;
		}
		activation_table[12] = 1;
	}
	if (to_activate == 'h') {
			activation_table[13] = 1;
			first_lap=1;
		}
	int command=(int)to_activate-'0';

		if(command==0) // stop the movement, when the right-left movement key is pressed again it takes on the value 1
			{
			activation_table[8]=0;
			activation_table[7]=0;
			activation_table[11]=0;
			activation_table[13]=0;
			}
		if(command>=1 && command<=6)
		{
			for(int i=1;i<=6;i++)
			{
				activation_table[i]=0;
			}
			activation_table[12]=0;
			activation_table[command]=1;
		}
		if(command==7)
		{
			activation_table[8]=0;
			activation_table[7]=1;
		for (int i = 0; i <= 5; i++) {
			main_axis[i].direction=0;
		}
		}
		if(command==8)
		{
			activation_table[7]=0;
			activation_table[8]=1;
			for (int i = 0; i <= 5; i++) {
				main_axis[i].direction=1;
			}
		}
		if(command==9)
		{
			if(activation_table[9]==0)
			{
				activation_table[9]=1; //permission
				activation_table[0]=0; //axes in motion
			}
			if(activation_table[9]==1)
			{
				activation_table[9]=0; // lack of permission
				activation_table[0]=1; //axis stop
			}
		}


}


void axis_enable(int i)
{
	if (main_axis[i-1].direction==0) {
			HAL_GPIO_WritePin(in_out_table[main_axis[i-1].morots[0]].port,in_out_table[main_axis[i-1].morots[0]].pin,GPIO_PIN_SET);

			if(main_axis[i-1].count_of_motors==2){
				if(i==2){
			HAL_GPIO_WritePin(in_out_table[main_axis[i-1].morots[1]].port,in_out_table[main_axis[i-1].morots[1]].pin,GPIO_PIN_SET);
				}

			if(i==4||i==5)
			{
				HAL_GPIO_WritePin(in_out_table[main_axis[i-1].morots[1]].port,in_out_table[main_axis[i-1].morots[1]].pin,GPIO_PIN_RESET);
			}
			}

		}
		if (main_axis[i-1].direction==1) {
			HAL_GPIO_WritePin(in_out_table[main_axis[i-1].morots[0]].port,in_out_table[main_axis[i-1].morots[0]].pin,GPIO_PIN_RESET);

			if (main_axis[i - 1].count_of_motors == 2) {
				if(i==2){
			HAL_GPIO_WritePin(in_out_table[main_axis[i-1].morots[1]].port,in_out_table[main_axis[i-1].morots[1]].pin,GPIO_PIN_RESET);
				}

			if(i==4||i==5)
			{
			HAL_GPIO_WritePin(in_out_table[main_axis[i-1].morots[1]].port,in_out_table[main_axis[i-1].morots[1]].pin,GPIO_PIN_SET);
			}
			}
		}

}


void wyslij(char* wiadomosc)
{
 HAL_UART_Transmit(&huart3, (uint8_t*)wiadomosc, strlen(wiadomosc), 1000);
}

void global_variables_initialization()
{

		in_out_table[0].port = ENABLE_GPIO_Port;
		in_out_table[0].pin = ENABLE_Pin;
		in_out_table[1].port = DIR_M1_GPIO_Port;
		in_out_table[1].pin = DIR_M1_Pin;
		in_out_table[2].port = DIR_M2_GPIO_Port;
		in_out_table[2].pin = DIR_M2_Pin;
		in_out_table[3].port = DIR_M3_GPIO_Port;
		in_out_table[3].pin = DIR_M3_Pin;
		in_out_table[4].port = DIR_M4_GPIO_Port;
		in_out_table[4].pin = DIR_M4_Pin;
		in_out_table[5].port = DIR_M5_GPIO_Port;
		in_out_table[5].pin = DIR_M5_Pin;
		in_out_table[6].port = DIR_M6_GPIO_Port;
		in_out_table[6].pin = DIR_M6_Pin;
		in_out_table[7].port = DIR_M7_GPIO_Port;
		in_out_table[7].pin = DIR_M7_Pin;
		in_out_table[8].port = DIR_M8_GPIO_Port;
		in_out_table[8].pin = DIR_M8_Pin;
		in_out_table[9].port = STEP_M1_GPIO_Port;
		in_out_table[9].pin = STEP_M1_Pin;
		in_out_table[10].port = STEP_M2_GPIO_Port;
		in_out_table[10].pin = STEP_M2_Pin;
		in_out_table[11].port = STEP_M3_GPIO_Port;
		in_out_table[11].pin = STEP_M3_Pin;
		in_out_table[12].port = STEP_M4_GPIO_Port;
		in_out_table[12].pin = STEP_M4_Pin;
		in_out_table[13].port = STEP_M5_GPIO_Port;
		in_out_table[13].pin = STEP_M5_Pin;
		in_out_table[14].port = STEP_M6_GPIO_Port;
		in_out_table[14].pin = STEP_M6_Pin;
		in_out_table[15].port = STEP_M7_GPIO_Port;
		in_out_table[15].pin = STEP_M7_Pin;
		in_out_table[16].port = STEP_M8_GPIO_Port;
		in_out_table[16].pin = STEP_M8_Pin;
		in_out_table[17].port = SENS_1_GPIO_Port;
		in_out_table[17].pin = SENS_1_Pin;
		in_out_table[18].port = SENS_2_GPIO_Port;
		in_out_table[18].pin = SENS_2_Pin;
		in_out_table[19].port = SENS_3_GPIO_Port;
		in_out_table[19].pin = SENS_3_Pin;
		in_out_table[20].port = SENS_4_GPIO_Port;
		in_out_table[20].pin = SENS_4_Pin;
		in_out_table[21].port = SENS_5_GPIO_Port;
		in_out_table[21].pin = SENS_5_Pin;
		main_axis[0].count_of_motors = 1;
		main_axis[0].morots[0]=1;
		main_axis[0].morots[1]=1;
		main_axis[0].steps_by_turn=998;
		main_axis[0].min_degrees=-180;
		main_axis[0].max_degrees=180;
		main_axis[1].morots[0]=2;
		main_axis[1].morots[1]=3;
		main_axis[1].count_of_motors = 2;
		main_axis[1].steps_by_turn=1456;
		main_axis[1].min_degrees=-40;
		main_axis[1].max_degrees=40;
		main_axis[2].count_of_motors = 1;
		main_axis[2].morots[0]=4;
		main_axis[2].morots[1]=4;
		main_axis[2].steps_by_turn=3200;
		main_axis[2].min_degrees=-95;
		main_axis[2].max_degrees=95;
		main_axis[3].morots[0]=5;
		main_axis[3].morots[1]=6;
		main_axis[3].count_of_motors = 2;
		main_axis[3].steps_by_turn=405;//bylo 810
		main_axis[3].min_degrees=-180;
		main_axis[3].max_degrees=180;
		main_axis[4].morots[0]=7;
		main_axis[4].morots[1]=8;
		main_axis[4].count_of_motors = 2;
		main_axis[4].steps_by_turn=240;
		main_axis[4].min_degrees=0;
		main_axis[4].max_degrees=180;
		main_axis[5].morots[0]=7;
		main_axis[5].morots[1]=8;
		main_axis[5].count_of_motors = 2;
		main_axis[5].min_degrees=-180;
		main_axis[5].max_degrees=180;
		for(int i=0;i<=5;i++)
		{
			main_axis[i].sensor_activated=0;
			main_axis[i].error=0;
			main_axis[i].full_step=0;
		}
		activation_table[13]=0;
		activation_table[14]=0;
		activation_table[15]=0;
		activation_table[17]=0;
		activation_table[18]=0;
		activation_table[16]=1;
		activation_table[19]=1;
		TIM3-> CCR1=0;

		stop_home=0;
		first_lap=1;

		current_px=0;
		current_py=0;
		current_pz=620;
		target_px=0;
		target_py=0;
		target_pz=620;
		fi1=-90;
		fi2=-10;
		fi3=-65;

		regulator_ready=1;
}

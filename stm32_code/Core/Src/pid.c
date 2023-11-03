#include "pid.h"
#include "main.h"
#include "usart.h"


PID pid_location;
PID pid_speed;
char Usart_Str[5];

void PID_param_init()
{
    /* 位置相关初始化参数 */
    pid_location.target_val = 300;              
    pid_location.output_val = 0;
    pid_location.err = 0;
    pid_location.err_last = 0.0;
    pid_location.integral = 0.0;

    pid_location.Kp = 3.93472015927904;                         //6.85796593659062;
    pid_location.Ki = 126.415875522503;                        //7.1007201947644;
    pid_location.Kd = 0.0275058974494752;                        //0.8656;

    /* 速度相关初始化参数 */
    pid_speed.target_val=1.66666;              
    pid_speed.output_val=0.0;
    pid_speed.err=0.0;
    pid_speed.err_last=0.0;
    pid_speed.integral=0.0;

    pid_speed.Kp =   571.54; //530;     
    pid_speed.Ki =   29330.75;  //29330.75;
    pid_speed.Kd = 1.3503;
		pid_speed.direction = 0;
}


float location_pid_realize(PID *pid, float actual_val)
{
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;

    /* 设定闭环死区 */
    if((pid->err >= -LOC_DEAD_ZONE) && (pid->err <= LOC_DEAD_ZONE))
    {
        pid->err = 0;
        pid->integral = 0;
        pid->err_last = 0;
    }

    /*积分项，积分分离，偏差较大时去掉积分作用*/
    if(pid->err > -LOC_INTEGRAL_START_ERR && pid->err < LOC_INTEGRAL_START_ERR)
    {
        pid->integral += pid->err;  
        /*积分范围限定，防止积分饱和*/
        if(pid->integral > LOC_INTEGRAL_MAX_VAL)
        {
            pid->integral = LOC_INTEGRAL_MAX_VAL;
        }
        else if(pid->integral < -LOC_INTEGRAL_MAX_VAL)
        {
            pid->integral = -LOC_INTEGRAL_MAX_VAL;
        }
    }   

    /*PID算法实现*/
    pid->output_val = pid->Kp * pid->err +
                      pid->Ki * pid->integral +
                      pid->Kd * (pid->err - pid->err_last);

    /*误差传递*/
    pid->err_last = pid->err;

    /*返回当前实际值*/
    return pid->output_val;
}


float speed_pid_realize(PID *pid, float actual_val)
{
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;
		
    /* 设定闭环死区 */
    if( (pid->err>-0.016) && (pid->err<0.015 ) )
    {
        pid->err = 0;
        pid->integral = 0;
        pid->err_last = 0;
				
			if(pid->target_val == 1.66666)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
				return  60; 
			}
			else if(pid->target_val <0.4)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);			
			}
			
			
			pid->err_last = pid->err;
			 
    }

    //*积分项，积分分离，偏差较大时去掉积分作用*/
    if(pid->err > -SPE_INTEGRAL_START_ERR && pid->err < SPE_INTEGRAL_START_ERR)
    {
        pid->integral += pid->err;  
        /*积分范围限定，防止积分饱和*/
        if(pid->integral > SPE_INTEGRAL_MAX_VAL)
        {
            pid->integral = SPE_INTEGRAL_MAX_VAL;
        }
        else if(pid->integral < -SPE_INTEGRAL_MAX_VAL)
        {
            pid->integral = -SPE_INTEGRAL_MAX_VAL;
        }
    }   

    /*PID算法实现*/
    pid->output_val = pid->Kp * pid->err +
                      pid->Ki * pid->integral +
                      pid->Kd *(pid->err - pid->err_last);
		if(pid->output_val < 0)
		{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);	
				pid->output_val *= -1;
				pid->output_val = pid->output_val/2;
			if(pid->target_val < 0.4)				
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);	
			}
		}
		else if(pid->output_val > 0)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
			if(pid->output_val > 100)
				pid->output_val = 100;
			pid->direction = 1;
		}
			
    /*误差传递*/
    pid->err_last = pid->err;
		
    /*返回当前实际值*/
    return pid->output_val;
}

void speed_val_protect(PID *pid, float *speed_val)
{
	/*目标速度上限处理*/
	if (*speed_val > 60)
	{
		pid->target_val = TARGET_SPEED_MAX;
	}
	else if (*speed_val < -TARGET_SPEED_MAX)
	{
		pid->target_val = -TARGET_SPEED_MAX;
	}	
	else	//外环高度误差与内环速度误差之间的转化函数
	{
		pid->target_val = *speed_val/80;  
	}
}




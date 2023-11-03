#ifndef _PID_H
#define	_PID_H

#define LOC_DEAD_ZONE  2 /*位置环死区*/
#define LOC_INTEGRAL_START_ERR 0.1 /*积分分离时对应的误差范围*/
#define LOC_INTEGRAL_MAX_VAL 0.001   /*积分范围限定，防止积分饱和*/


#define TARGET_SPEED_MAX 1.66666




#define SPE_DEAD_ZONE 0.0001 /*速度环死区*/
#define SPE_INTEGRAL_START_ERR 0.2 /*积分分离时对应的误差范围*/
#define SPE_INTEGRAL_MAX_VAL 0.001   /*积分范围限定，防止积分饱和*/


typedef struct
{
	float target_val;   //目标值
	float err;          //偏差值
	float err_last;     //上一个偏差值
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float integral;     //积分值
	float output_val;   //输出值
	float direction;    //方向
}PID;

extern PID pid_location;
extern PID pid_speed;

void PID_param_init();
float location_pid_realize(PID *pid, float actual_val);
float speed_pid_realize(PID *pid, float actual_val);
void speed_val_protect(PID *pid, float *speed_val);


#endif

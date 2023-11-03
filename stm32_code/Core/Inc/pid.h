#ifndef _PID_H
#define	_PID_H

#define LOC_DEAD_ZONE  2 /*λ�û�����*/
#define LOC_INTEGRAL_START_ERR 0.1 /*���ַ���ʱ��Ӧ����Χ*/
#define LOC_INTEGRAL_MAX_VAL 0.001   /*���ַ�Χ�޶�����ֹ���ֱ���*/


#define TARGET_SPEED_MAX 1.66666




#define SPE_DEAD_ZONE 0.0001 /*�ٶȻ�����*/
#define SPE_INTEGRAL_START_ERR 0.2 /*���ַ���ʱ��Ӧ����Χ*/
#define SPE_INTEGRAL_MAX_VAL 0.001   /*���ַ�Χ�޶�����ֹ���ֱ���*/


typedef struct
{
	float target_val;   //Ŀ��ֵ
	float err;          //ƫ��ֵ
	float err_last;     //��һ��ƫ��ֵ
	float Kp,Ki,Kd;     //���������֡�΢��ϵ��
	float integral;     //����ֵ
	float output_val;   //���ֵ
	float direction;    //����
}PID;

extern PID pid_location;
extern PID pid_speed;

void PID_param_init();
float location_pid_realize(PID *pid, float actual_val);
float speed_pid_realize(PID *pid, float actual_val);
void speed_val_protect(PID *pid, float *speed_val);


#endif

#ifndef FOC_Algorithm_H
#define FOC_Algorithm_H


#include "stdint.h"

/**
 *  @brief FOC parameter structure 
*/

typedef struct 
{
    float Udc;
    float Tpwm;

    float Vd;
    float Vq;
    float Theta;

    float VAlpha;
    float VBeta;

    float Vu;
    float Vv;
    float Vw;

    float Vu_mode;
    float Vv_mode;
    float Vw_mode;

    int Tcmp_U;
    int Tcmp_V;
    int Tcmp_W;

}FOC_Parameter_TypeDef;

typedef struct
{
   float Acc;              /* 加速度 */
   float Target_Speed;     /* 目标速度 */
   float Dead_Zone;        /* 死区阈值 */
   float Ts;               /* 步长 */

   /* 中间参数 */

   float Speed_Error;      /* 速度误差 */
   float Speed_Error_Dead; /* 带死区的速度误差 */
   float Acc_Dir;          /* 加速度方向 */
   float Speed_Integrator; /* 速度积分器 */
   float Theta_Integrator; /* 角度积分器 */

   /* 输出参数 */
   float Speed;            /* 实际速度 */
   float Theta;            /* 实际角度 */

}Vf_Speed_Control_TypeDef;

void Rev_Park_Transform(FOC_Parameter_TypeDef *FOC_Parameter);
void Rev_Clarke_Transform(FOC_Parameter_TypeDef *FOC_Parameter);
void Zero_Input(FOC_Parameter_TypeDef *FOC_Parameter);
void Motor_Udc_TPWM(FOC_Parameter_TypeDef *FOC_Parameter);
void Motor_Voltage_Update(FOC_Parameter_TypeDef *FOC_Parameter);
void SpeedControl_Param_Init(Vf_Speed_Control_TypeDef *Vf_Speed_Control,uint16_t Acc,uint16_t Speed,float Dead_Zone,float Ts);
void Vf_SpeedControl_Update(Vf_Speed_Control_TypeDef *Vf_Speed_Control,float *Target_Theta);
void SVPWM_Select_PWM1(FOC_Parameter_TypeDef *FOC_Parameter);
void SVPWM_Select_PWM2(FOC_Parameter_TypeDef *FOC_Parameter);

#endif

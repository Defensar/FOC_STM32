#ifndef _PID_H
#define _PID_H

#include "FOC_Algorithm.h"
typedef struct 
{
    float Current_Kp_Id;
    float Current_Ki_Id;

    float Current_Kp_Iq;
    float Current_Ki_Iq;

    float Current_Actual_Id;
    float Current_Actual_Iq;
    float Current_Target_Id;
    float Current_Target_Iq;
    float Current_Error_Id;
    float Current_Error_Iq;
    float Current_Integral_Id;
    float Current_Integral_Iq;

    
}Pid_Parameter_Typedef;

void Current_PID_Init(Pid_Parameter_Typedef *Pid_Para);
void Current_PI_Update(Pid_Parameter_Typedef *Pid_Para,FOC_Parameter_TypeDef *FOC);
void Current_PID_RUN(Pid_Parameter_Typedef *Pid_Para,FOC_Parameter_TypeDef *FOC);


#endif

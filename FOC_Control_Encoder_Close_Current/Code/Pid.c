#include "Pid.h"
#include "FOC_Algorithm.h"
#include "Motor_Configuration.h"

void Current_PID_Init(Pid_Parameter_Typedef *Pid_Para)
{
    Pid_Para->Current_Kp_Id = 0;
    Pid_Para->Current_Ki_Id = 0;
    Pid_Para->Current_Kp_Iq = 0;
    Pid_Para->Current_Ki_Iq = 0;
    

}

void Current_PI_Update(Pid_Parameter_Typedef *Pid_Para,FOC_Parameter_TypeDef *FOC)
{
    Pid_Para->Current_Actual_Id = FOC->Id;
    Pid_Para->Current_Actual_Iq = FOC->Iq;
}

void Current_PID_RUN(Pid_Parameter_Typedef *Pid_Para,FOC_Parameter_TypeDef *FOC)
{

    Pid_Para->Current_Error_Id = Pid_Para->Current_Target_Id - Pid_Para->Current_Actual_Id;//比例 A
    Pid_Para->Current_Integral_Id += Pid_Para->Current_Ki_Id * Pid_Para->Current_Error_Id * Timer_Ts;//积分 V
    FOC->Vd = Pid_Para->Current_Kp_Id * Pid_Para->Current_Error_Id + Pid_Para->Current_Integral_Id;

    Pid_Para->Current_Error_Iq = Pid_Para->Current_Target_Iq - Pid_Para->Current_Actual_Iq;//比例 A
    Pid_Para->Current_Integral_Iq += Pid_Para->Current_Ki_Iq * Pid_Para->Current_Error_Iq * Timer_Ts;//积分 V
    FOC->Vq = Pid_Para->Current_Kp_Iq * Pid_Para->Current_Error_Iq + Pid_Para->Current_Integral_Iq;



}

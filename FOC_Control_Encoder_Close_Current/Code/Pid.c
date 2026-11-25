#include "Pid.h"
#include "FOC_Algorithm.h"
#include "Motor_Configuration.h"
#include "math.h"

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

    /* 记住 PI 请求的电压，再计算母线允许的最大幅值。 */
    const float vd_request = FOC->Vd;
    const float vq_request = FOC->Vq;
    const float voltage_limit = 0.95f * FOC->Udc / sqrtf(3.0f);
    const float magnitude_sq = vd_request * vd_request + vq_request * vq_request;

    if (magnitude_sq > voltage_limit * voltage_limit)//电压输出限幅
    {
        const float scale = voltage_limit / sqrtf(magnitude_sq);
        const float tracking_time = 0.001f;  /* 示例反算时间常数，单位 s */
        const float correction = Timer_Ts / tracking_time;

        FOC->Vd = vd_request * scale;
        FOC->Vq = vq_request * scale;

        if (Pid_Para->Current_Ki_Id > 0.0f)
        {
            Pid_Para->Current_Integral_Id += correction * (FOC->Vd - vd_request);
        }
        if (Pid_Para->Current_Ki_Iq > 0.0f)
        {
            Pid_Para->Current_Integral_Iq += correction * (FOC->Vq - vq_request);
        }
    }



}

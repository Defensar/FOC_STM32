#include "TdTracker.h"
/**
 * @brief 微分跟踪器参数初始化
 * 
 * @param TdTracker_para ：跟踪器结构体指针参数
 * @param Input     ：初始化输入
 * @param Omega_n    ：初始化收敛因子
 * @param Ts    ：步长 
 */
void TdTracker_Init(TdTracker_Typedef *TdTracker_para,float Input,float Omega_n,float Ts)
{
    TdTracker_para->Input = Input;
    TdTracker_para->Omega_n = Omega_n;
    TdTracker_para->Ts = Ts;


}

/**
 * @brief 微分跟踪器周期函数
 * 
 * @param TdTracker_para ：跟踪器结构体指针参数
 * @param u ：阶跃输入u
 * @return 过渡信号y
 */
float TdTracker_UpDate(TdTracker_Typedef *TdTracker_para,float u)
{
    float Factor_1 = u * TdTracker_para->Omega_n * TdTracker_para->Omega_n;
    float Factor_2 = -2 * TdTracker_para->Omega_n * TdTracker_para->y_dot;
    float Factor_3 = -1 * TdTracker_para->Omega_n * TdTracker_para->Omega_n * TdTracker_para->y;

    TdTracker_para->y_ddot = Factor_1 + Factor_2 + Factor_3;

    TdTracker_para->y_dot += TdTracker_para->y_ddot * TdTracker_para->Ts;
    TdTracker_para->y += TdTracker_para->y_dot * TdTracker_para->Ts;

    return TdTracker_para->y;
}

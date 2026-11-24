#include "FOC_Algorithm.h"
#include "Motor_Configuration.h"
#include "math.h"
/**
 * @brief 反帕克变换
 * 
 * @param FOC_Parameter 
 */
void Rev_Park_Transform(FOC_Parameter_TypeDef *FOC_Parameter)
{
   FOC_Parameter->VAlpha = FOC_Parameter->Vd * cosf(FOC_Parameter->Theta) - FOC_Parameter->Vq * sinf(FOC_Parameter->Theta);
   FOC_Parameter->VBeta = FOC_Parameter->Vd * sinf(FOC_Parameter->Theta) + FOC_Parameter->Vq * cosf(FOC_Parameter->Theta);
}

/**
 * @brief 反克拉克变换
 * 
 * @param FOC_Parameter 
 */
void Rev_Clarke_Transform(FOC_Parameter_TypeDef *FOC_Parameter)
{
    FOC_Parameter->Vu = FOC_Parameter->VAlpha;
    FOC_Parameter->Vv = -0.5f * FOC_Parameter->VAlpha + 0.866f * FOC_Parameter->VBeta;
    FOC_Parameter->Vw = -0.5f * FOC_Parameter->VAlpha - 0.866f * FOC_Parameter->VBeta;
}

/**
 * @brief SVPWM扇区矢量作用时间
 * 
 * @param FOC_Parameter 
 */

void SVPWM_Select_PWM1(FOC_Parameter_TypeDef *FOC_Parameter) //PWM模式1
{
    uint8_t Sector_Select = 0;
    //float U4,U6 = 2/3.0f * UDC;
    float X,Y,Z,Q;//各扇区作用时间
    float Tx = 0.0f,Ty = 0.0f,T_sum;
    float Ta,Tb,Tc;//矢量切换时间
    if(FOC_Parameter->VBeta > 0)
    {
        Sector_Select += 1;
    }
    if((float)(_sqrt_3 * FOC_Parameter->VAlpha - FOC_Parameter->VBeta) / 2.0f > 0)
    {
        Sector_Select += 2;
    }
    if((float)(_sqrt_3 * FOC_Parameter->VAlpha * (-1) - FOC_Parameter->VBeta) / 2.0f > 0)
    {
        Sector_Select += 4;
    }
    Q = _sqrt_3 * Timer_Ts / FOC_Parameter->Udc;
    X = Q * FOC_Parameter->VBeta;
    Y = Q * (_sqrt_3 * FOC_Parameter->VAlpha + FOC_Parameter->VBeta) / 2.0f;
    Z = Q * (_sqrt_3 * FOC_Parameter->VAlpha * (-1) + FOC_Parameter->VBeta) / 2.0f;
    if(Sector_Select == 1)
    {
        Tx = Z;
        Ty = Y;
    }
    else if(Sector_Select == 2)
    {
        Tx = Y;
        Ty = -X;
    }
    else if(Sector_Select == 3)
    {
        Tx = -Z;
        Ty = X;
    }
    else if(Sector_Select == 4)
    {
        Tx = -X;
        Ty = Z;
    }
    else if(Sector_Select == 5)
    {
        Tx = X;
        Ty = -Y;
    }
    else if(Sector_Select == 6)
    {
        Tx = -Y;
        Ty = -Z;
    }
    T_sum = Tx + Ty;
    if(T_sum > Timer_Ts)
    {
        Tx = Tx / T_sum * Timer_Ts;
        Ty = Ty / T_sum * Timer_Ts;
    }

    Ta = (Timer_Ts - Tx -Ty) / 4.0f;//T0/4
    Tb = Ta + Ty / 2.0f;
    Tc = Tb + Tx / 2.0f;

     switch(Sector_Select)
    {
    case 1: /* II */
        FOC_Parameter->Tcmp_U = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;

    case 2: /* VI */
        FOC_Parameter->Tcmp_U = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;

    case 3: /* I */
        FOC_Parameter->Tcmp_U = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;
    
    case 4: /* IV */
        FOC_Parameter->Tcmp_U = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;

    case 5: /* III */
        FOC_Parameter->Tcmp_U = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;


    case 6: /* V */
        FOC_Parameter->Tcmp_U = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;



    default:
        /* 零输入：三相均为 50%。 */
    FOC_Parameter->Tcmp_U = (int)(0.5f * FOC_Parameter->Tpwm);
    FOC_Parameter->Tcmp_V = FOC_Parameter->Tcmp_U;
    FOC_Parameter->Tcmp_W = FOC_Parameter->Tcmp_U;
        break;
    }
    
}



void SVPWM_Select_PWM2(FOC_Parameter_TypeDef *FOC_Parameter)
{
    uint8_t Sector_Select = 0;
    float X,Y,Z,Q;//各扇区作用时间
    float Tx = 0.0f,Ty = 0.0f,T_sum;
    float Ta,Tb,Tc;//矢量切换时间

    if(FOC_Parameter->VBeta > 0)
    {
        Sector_Select += 1;
    }
    if((float)(_sqrt_3 * FOC_Parameter->VAlpha - FOC_Parameter->VBeta) / 2.0f > 0)
    {
        Sector_Select += 2;
    }
    if((float)(_sqrt_3 * FOC_Parameter->VAlpha * (-1) - FOC_Parameter->VBeta) / 2.0f > 0)
    {
        Sector_Select += 4;
    }

    Q = _sqrt_3 * Timer_Ts / FOC_Parameter->Udc;
    X = Q * FOC_Parameter->VBeta;
    Y = Q * (_sqrt_3 * FOC_Parameter->VAlpha + FOC_Parameter->VBeta) / 2.0f;
    Z = Q * (_sqrt_3 * FOC_Parameter->VAlpha * (-1) + FOC_Parameter->VBeta) / 2.0f;

    if(Sector_Select == 1)
    {
        Tx = Z;
        Ty = Y;
    }
    else if(Sector_Select == 2)
    {
        Tx = Y;
        Ty = -X;
    }
    else if(Sector_Select == 3)
    {
        Tx = -Z;
        Ty = X;
    }
    else if(Sector_Select == 4)
    {
        Tx = -X;
        Ty = Z;
    }
    else if(Sector_Select == 5)
    {
        Tx = X;
        Ty = -Y;
    }
    else if(Sector_Select == 6)
    {
        Tx = -Y;
        Ty = -Z;
    }

    T_sum = Tx + Ty;
    if(T_sum > Timer_Ts)
    {
        Tx = Tx / T_sum * Timer_Ts;
        Ty = Ty / T_sum * Timer_Ts;
    }

    Ta = (Timer_Ts - Tx - Ty) / 4.0f;//T0/4
    Tb = Ta + Tx / 2.0f;
    Tc = Tb + Ty / 2.0f;

    /* 按表格中的 N 分配比较时间，Tcm1/Tcm2/Tcm3 对应 U/V/W。 */
    switch(Sector_Select)
    {
    case 1: /* II */
        FOC_Parameter->Tcmp_U = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;

    case 2: /* VI */
        FOC_Parameter->Tcmp_U = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;

    case 3: /* I */
        FOC_Parameter->Tcmp_U = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;

    case 4: /* IV */
        FOC_Parameter->Tcmp_U = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;

    case 5: /* III */
        FOC_Parameter->Tcmp_U = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;

    case 6: /* V */
        FOC_Parameter->Tcmp_U = Tb / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_V = Tc / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        FOC_Parameter->Tcmp_W = Ta / (Timer_Ts / 2.0f) * FOC_Parameter->Tpwm;
        break;

    default:
        /* 零输入：三相均为 50%。 */
        FOC_Parameter->Tcmp_U = (int)(0.5f * FOC_Parameter->Tpwm);
        FOC_Parameter->Tcmp_V = FOC_Parameter->Tcmp_U;
        FOC_Parameter->Tcmp_W = FOC_Parameter->Tcmp_U;
        break;
    }
}



void Zero_Input(FOC_Parameter_TypeDef *FOC_Parameter)
{   
    float max,min,V0;
    float voltage[3];
    voltage[0] = FOC_Parameter->Vu;
    voltage[1] = FOC_Parameter->Vv;
    voltage[2] = FOC_Parameter->Vw;
    max = voltage[0];
    min = voltage[0];
    for(int i = 0;i < 3;i++)
    {
        if(max < voltage[i])
        {
            max = voltage[i];
        }
        if(min > voltage[i])
        {
            min = voltage[i];
        }
    }
    V0 = -0.5f * (max + min);
    FOC_Parameter->Vu_mode = FOC_Parameter->Vu + V0;
    FOC_Parameter->Vv_mode = FOC_Parameter->Vv + V0;
    FOC_Parameter->Vw_mode = FOC_Parameter->Vw + V0;

    FOC_Parameter->Tcmp_U = ((FOC_Parameter->Vu_mode) / FOC_Parameter->Udc + 0.5f) * FOC_Parameter->Tpwm;
    FOC_Parameter->Tcmp_V = ((FOC_Parameter->Vv_mode) / FOC_Parameter->Udc + 0.5f) * FOC_Parameter->Tpwm;
    FOC_Parameter->Tcmp_W = ((FOC_Parameter->Vw_mode) / FOC_Parameter->Udc + 0.5f) * FOC_Parameter->Tpwm;

}

void Motor_Udc_TPWM(FOC_Parameter_TypeDef *FOC_Parameter)
{
  FOC_Parameter->Udc = UDC;
  FOC_Parameter->Tpwm = TPWM;
}

void Motor_Voltage_Update(FOC_Parameter_TypeDef *FOC_Parameter)
{
    Rev_Park_Transform(FOC_Parameter);
    Rev_Clarke_Transform(FOC_Parameter);
    //Zero_Input(FOC_Parameter);
    SVPWM_Select_PWM2(FOC_Parameter);
}

void SpeedControl_Param_Init(Vf_Speed_Control_TypeDef *Vf_Speed_Control,uint16_t Acc,uint16_t Speed,float Dead_Zone,float Ts)
{
    Vf_Speed_Control->Acc = Acc;
    Vf_Speed_Control->Speed = Speed;
    Vf_Speed_Control->Dead_Zone = Dead_Zone;
    Vf_Speed_Control->Ts = Ts;

}

void Vf_SpeedControl_Update(Vf_Speed_Control_TypeDef *Vf_Speed_Control,float *Target_Theta)
{
    Vf_Speed_Control->Speed_Error = Vf_Speed_Control->Target_Speed - Vf_Speed_Control->Speed_Integrator;
    if(fabs(Vf_Speed_Control->Speed_Error) < Vf_Speed_Control->Dead_Zone)
    {
        Vf_Speed_Control->Speed_Error_Dead = 0;
    }
    else 
    {
        Vf_Speed_Control->Speed_Error_Dead = Vf_Speed_Control->Speed_Error;
    }

    if(Vf_Speed_Control->Speed_Error_Dead > 0)
    {
        Vf_Speed_Control->Acc_Dir = 1;
    }
    else if(Vf_Speed_Control->Speed_Error_Dead < 0)
    {
        Vf_Speed_Control->Acc_Dir = -1;
    }
    else if(Vf_Speed_Control->Speed_Error_Dead == 0)
    {
        Vf_Speed_Control->Acc_Dir = 0;
    }
    Vf_Speed_Control->Speed_Integrator += Vf_Speed_Control->Acc * Vf_Speed_Control->Acc_Dir * Vf_Speed_Control->Ts;
    Vf_Speed_Control->Theta_Integrator += Vf_Speed_Control->Speed_Integrator * Vf_Speed_Control->Ts;
    Vf_Speed_Control->Theta = fmod(Vf_Speed_Control->Theta_Integrator,_2PI);
		
		if(Vf_Speed_Control->Theta < 0)
		{
			Vf_Speed_Control->Theta += _2PI;
		}
		
		Vf_Speed_Control->Speed = Vf_Speed_Control->Speed_Integrator;

    *Target_Theta = Vf_Speed_Control->Theta;
}

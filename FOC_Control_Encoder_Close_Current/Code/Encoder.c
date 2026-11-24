#include "Encoder.h"
#include "Motor_Configuration.h"
#include "math.h"
void Encoder_Para_Init(Encoder_Typedef *Encoder_Para,uint32_t line,uint8_t dir,uint8_t motorpn,uint32_t phase)
{
    Encoder_Para->Line = line;
    Encoder_Para->Dir = dir;
    Encoder_Para->Motorpn = motorpn;
    Encoder_Para->Phase = phase;
}

void Encoder_Para_Run(Encoder_Typedef *Encoder_Para,uint32_t Pulse_Data)
{
    //uint16_t Encoder_Data = 0;
	//Encoder_Data = MT6701_ReadData();
    if(Encoder_Para->Dir == 0)
    {
        //Encoder_Para->Theta = Encoder_Para->Pulse_Data/((float)Encoder_Para->Line) * _2PI;
        Encoder_Para->Pulse_Data = Pulse_Data;
    }
    else if(Encoder_Para->Dir == 1)
    {
        Encoder_Para->Pulse_Data = Encoder_Para->Line - Pulse_Data;
        //Encoder_Para->Theta = _2PI - Encoder_Para->Pulse_Data/((float)Encoder_Para->Line) * _2PI;
    }
    uint32_t Temp = (Encoder_Para->Pulse_Data - Encoder_Para->Phase + Encoder_Para->Line) % Encoder_Para->Line;

    //机械角度
    Encoder_Para->Theta = (float)Temp / (float) Encoder_Para->Line * _2PI;
    //电角度
    Encoder_Para->Theta_e = fmod(Encoder_Para->Theta * Motor_pn,_2PI);



}

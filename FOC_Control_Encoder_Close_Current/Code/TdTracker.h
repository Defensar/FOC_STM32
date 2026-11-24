#ifndef TdTracker_H
#define TdTracker_H

typedef struct TdTracker
{
    float Input;
    float Omega_n;
    float Ts;

    float y;
    float y_dot;
    float y_ddot;


}TdTracker_Typedef;

void TdTracker_Init(TdTracker_Typedef *TdTracker_para,float Input,float Omega_n,float Ts);
float TdTracker_UpDate(TdTracker_Typedef *TdTracker_para,float u);

#endif

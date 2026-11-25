#include "state_machine.h"
#include "HardWare_Driver.h"
#include "Motor_Configuration.h"


Motor_Typedef MotorSystem;


void Motor_StateMachine_Init(Motor_Typedef *MotorState)
{
  ADC_Init();
  Motor_Udc_TPWM(&MotorState->FOC);
  SpeedControl_Param_Init(&MotorState->Vf_Speed_Control,50,0,0.001f,Timer_Ts);
  Encoder_Para_Init(&MotorState->Encoder_Control,Encoder_Line,0,Motor_pn,3263);
  TdTracker_Init(&MotorState->TdTracker_Uq,0,10,Timer_Ts);
  Current_PID_Init(&MotorState->Pid_control);
  Motor_HardWare_Init();
  //Motor_Udc_TPWM(MotorState->FOC);
}

void Motor_StateMachine_Run(Motor_Typedef *MotorState)
{
  uint32_t Pulse_Data;
  Pulse_Data = MT6701_ReadData();
	Encoder_Para_Run(&MotorState->Encoder_Control,Pulse_Data);
  //角度模式选择
  switch (MotorState->Encoder_Mode)
  {
  case Theta_Mode_Zero:
    MotorState->FOC.Theta = 0;
    break;
  case Theta_Mode_Vf:
    Vf_SpeedControl_Update(&MotorState->Vf_Speed_Control,&MotorState->FOC.Theta);
    break;
  case Theta_Mode_Encoder:
    MotorState->FOC.Theta = MotorState->Encoder_Control.Theta_e;
    break;
  default:
    break;
  }
  ADC_Transform_Value(&MotorState->myadc_control, &MotorState->FOC);
  Clarke_Transform(&MotorState->FOC);
  Park_Transform(&MotorState->FOC);
  Current_PI_Update(&MotorState->Pid_control,&MotorState->FOC);
  Current_PID_RUN(&MotorState->Pid_control,&MotorState->FOC);

  //Vf_SpeedControl_Update(&MotorState->Vf_Speed_Control,&MotorState->FOC.Theta);
//  MotorState->FOC.Vd = 0.0f;
//  MotorState->FOC.Vq = 0.0f;
  //MotorState->FOC.Theta = MotorState->Encoder_Control.Theta_e;
  //MotorState->FOC.Vq = TdTracker_UpDate(&MotorState->TdTracker_Uq,MotorState->TdTracker_Uq.Input);
	Motor_Voltage_Update(&MotorState->FOC);
  Motor_SetPWM(MotorState->FOC.Tcmp_U,MotorState->FOC.Tcmp_V,MotorState->FOC.Tcmp_W);
}

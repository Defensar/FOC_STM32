#ifndef state_machine_H
#define state_machine_H


#include "Foc_Algorithm.h"
#include "Encoder.h"
#include "TdTracker.h"
#include "myadc.h"
#include "Pid.h"


typedef struct 
{
  Encoder_Mode_Typedef Encoder_Mode;
  FOC_Parameter_TypeDef FOC;
  Vf_Speed_Control_TypeDef Vf_Speed_Control;
  Encoder_Typedef Encoder_Control;
  TdTracker_Typedef TdTracker_Uq;
  myadc_Typedef myadc_control;
  Pid_Parameter_Typedef Pid_control;
}Motor_Typedef;

void Motor_StateMachine_Init(Motor_Typedef *MotorState);
void Motor_StateMachine_Run(Motor_Typedef *MotorState);


extern Motor_Typedef MotorSystem;


#endif

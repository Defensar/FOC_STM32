/*==============================================================================
 * Vofa+配置文件
 *============================================================================*/
#include "headfile.h"

/* Vofa发送结构体实例 */
VofaSend_TypeDef VofaSend;


/**
 * @brief Vofa+发送函数
 * @param  无
 * @return 无
 */
void Vofa_Send_Task(void)
{
    /* 填入需要发送的数据 */
//    VofaSend.Send_Data_Array[0] = MotorSystem.FOC.Theta;
//	  VofaSend.Send_Data_Array[1] = MotorSystem.Encoder_Control.Theta;
	   VofaSend.Send_Data_Array[0] = MotorSystem.Pid_control.Current_Target_Id;
	  VofaSend.Send_Data_Array[1] = MotorSystem.Pid_control.Current_Actual_Iq;
		VofaSend.Send_Data_Array[3] = MotorSystem.Pid_control.Current_Actual_Id;
	  VofaSend.Send_Data_Array[4] = MotorSystem.FOC.Vq;
    //VofaSend.Send_Data_Array[1] = MotorSystem.Encoder_Control.Pulse_Data;
//	  VofaSend.Send_Data_Array[0] = MotorSystem.FOC.Vq;
//    VofaSend.Send_Data_Array[1] = MotorSystem.TdTracker_Uq.Input;
//    VofaSend.Send_Data_Array[2] = MotorSystem.Vf_Speed_Control.Speed;
//  	VofaSend.Send_Data_Array[3] = MotorSystem.Vf_Speed_Control.Theta;
	  
	  
//    VofaSend.Send_Data_Array[3] = MotorSystem.FOC.Vu;
//    VofaSend.Send_Data_Array[4] = MotorSystem.FOC.Vv;
//    VofaSend.Send_Data_Array[5] = MotorSystem.FOC.Vw;
//    VofaSend.Send_Data_Array[6] = MotorSystem.FOC.Vu_mode;
//    VofaSend.Send_Data_Array[7] = MotorSystem.FOC.Vv_mode;
//    VofaSend.Send_Data_Array[8] = MotorSystem.FOC.Vw_mode;

//    VofaSend.Send_Data_Array[0] = MotorSystem.myadc_control.Ia_Raw;
//	  VofaSend.Send_Data_Array[1] = MotorSystem.myadc_control.Ib_Raw;
//    VofaSend.Send_Data_Array[2] = MotorSystem.myadc_control.Ic_Raw;
//  	VofaSend.Send_Data_Array[3] = MotorSystem.myadc_control.Va_Raw;
//    VofaSend.Send_Data_Array[4] = MotorSystem.myadc_control.Vb_Raw;
//    VofaSend.Send_Data_Array[5] = MotorSystem.myadc_control.Vc_Raw;
//	VofaSend.Send_Data_Array[3] = MotorSystem.myadc_control.Ia;
//	VofaSend.Send_Data_Array[4] = MotorSystem.myadc_control.Ib;
	VofaSend.Send_Data_Array[5] = MotorSystem.myadc_control.Ic;
//    VofaSend.Send_Data_Array[2] =MotorSystem.myadc_control.Ic;
//		VofaSend.Send_Data_Array[3] = MotorSystem.FOC.IBeta;
//  	VofaSend.Send_Data_Array[3] = MotorSystem.myadc_control.Va;
//    VofaSend.Send_Data_Array[4] = MotorSystem.myadc_control.Vb;
//    VofaSend.Send_Data_Array[5] = MotorSystem.myadc_control.Vc;
	VofaSend.Send_Data_Array[7] = MotorSystem.myadc_control.Va_Raw - MotorSystem.myadc_control.Vb_Raw;
	VofaSend.Send_Data_Array[8] = MotorSystem.myadc_control.VBUS;
    VofaSend.Send_Data_Array[9] = MotorSystem.myadc_control.Ia + MotorSystem.myadc_control.Ib + MotorSystem.myadc_control.Ic;
		



    Vofa_Data_Process();
}


/**
 * @brief Vofa+数据处理函数
 * @param  无
 * @return 无
 */
void Vofa_Data_Process(void)
{
    /* 通过拷贝将浮点数据转换成单字节数据 */
    memcpy(VofaSend.Byte_Data_Array,VofaSend.Send_Data_Array,sizeof(VofaSend.Send_Data_Array));
    /* 填补通信协议的帧尾 */
    VofaSend.Byte_Data_Array[data_len - 4] = 0x00;
    VofaSend.Byte_Data_Array[data_len - 3] = 0x00;
    VofaSend.Byte_Data_Array[data_len - 2] = 0x80;
    VofaSend.Byte_Data_Array[data_len - 1] = 0x7f;

    /* 发送串口数据 */
    CDC_Transmit_FS((uint8_t *)VofaSend.Byte_Data_Array,data_len);
}

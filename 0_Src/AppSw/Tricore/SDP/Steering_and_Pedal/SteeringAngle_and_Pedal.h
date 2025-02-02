/*
 * SteeringAngle_and_Pedal.h
 * Created on: 2025.01.06
 * Author: Kimsiho
 */

#ifndef APPSW_TRICORE_SDP_STEERINGANDPEDAL_STEERINGANGLEANDPEDAL.H
#define APPSW_TRICORE_SDP_STEERINGANDPEDAL_STEERINGANGLEANDPEDAL.H
/*********************************** Includes **********************************/
#include "CanCommunication.h"

/***************************** Macro *********************************/


/************************* Data Structures ***************************/
typedef union SteeringAndPedal
{
    uint32 tx_data[2];
    struct
    {
        uint16 steering_angle     :16;    //[0-15]      //angel -> angle 수정함. 
        uint8 apps                :8;     //[16-23]
        uint8 bpps                :8;     //[24-31]
        uint16 brake_pressure     :16;    //[32-47]
        uint16 reserved_0         :16;    //[48-63]
    } s;
}steering_and_pedal_t;


/************************* Externs ***************************/
IFX_EXTERN steering_and_pedal_t steering_and_pedal_msg;     //CAN통신으로 보낼 정보를 저장하는 애들
IFX_EXTERN CanCommunication_Message steering_and_pedal_msgObj;  //CAN통신으로 직접 정보를 보내는 애들

IFX_EXTERN void SDP_SteeringAngleandPedal_initMessage(void);
IFX_EXTERN void SDP_SteeringAngleandPedal_run_10ms(void);
#endif


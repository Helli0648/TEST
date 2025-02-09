/*
 * SteeringAngle_and_Pedal.c
 * Created on: 2025.01.06
 * Author: Kimsiho
 */

/***************************** Includes ******************************/
#include <IfxCpu.h>

#include "SteeringAngle_and_Pedal.h"
#include "SteeringAngle.h"
#include "PedalBox.h"
#include "RVC_privateDataStructure.h"
/*********************** Global Variables ****************************/
IFX_EXTERN SDP_SteeringAngle_t  SDP_SteeringAngle;
IFX_EXTERN SDP_PedalBox_t		SDP_PedalBox;
IFX_EXTERN RVC_t 				RVC;

steering_and_pedal_t steeringangle_and_pedal;
CanCommunication_Message steering_and_pedal_msgObj;

const uint32 steering_and_pedal_MsgId = 0x40300;

/******************* Function Prototypes *********************/
void SDP_SteeringAngle_and_Pedal_initMessage(void);
void SDP_SteeringAngleandPedal_run_10ms(void);

/********************* Function Implementation ***********************/
IFX_STATIC void SDP_SteeringangleandPedal_transmitMessaage(steering_and_pedal_t *_steering_and_pedal);
IFX_STATIC void SDP_SteeringangleandPedal_setMessage(steering_and_pedal_t * _steering_and_pedal, SDP_SteeringAngle_t * angle, SDP_PedalBox_t * pps, RVC_t *_RVC);
/********************* Function Implementation ***********************/
void SDP_SteeringAngle_and_Pedal_initMessage(void)
{
    {
        CanCommunication_Message_Config config;
		config.messageId = steering_and_pedal_MsgId;
		config.frameType = IfxMultican_Frame_receive;
		config.dataLen = IfxMultican_DataLengthCode_8;
		config.node = &CanCommunication_canNode0;
		config.isStandardId = FALSE;
		CanCommunication_initMessage(&steering_and_pedal_msgObj, &config);
    }
}

void SDP_SteeringAngleandPedal_run_10ms(void)
{
	SDP_SteeringangleandPedal_setMessage(&steeringangle_and_pedal, &SDP_SteeringAngle, &SDP_PedalBox, &RVC);
	SDP_SteeringangleandPedal_transmitMessaage(&steeringangle_and_pedal);
}


/****************** Private Function Implementation ******************/
IFX_STATIC void SDP_SteeringangleandPedal_setMessage(steering_and_pedal_t * _steering_and_pedal, SDP_SteeringAngle_t * angle, SDP_PedalBox_t * pps, RVC_t * _RVC)
{	
	int steeringdegree;

	_steering_and_pedal -> s.steering_angle 	= (sint32)((angle -> degSteeringAngle)*100);
	_steering_and_pedal -> s.apps 				= (uint8)(pps 	-> apps.pps);
	_steering_and_pedal -> s.bpps				= (uint8)(pps 	-> bpps.pps);
	_steering_and_pedal -> s.brake_pressure		= (uint16)(((_RVC	-> BrakePressure1.value)+(_RVC	-> BrakePressure2.value))/2);
}

IFX_STATIC void SDP_SteeringangleandPedal_transmitMessaage(steering_and_pedal_t *_steering_and_pedal)
{
    CanCommunication_setMessageData(_steering_and_pedal->tx_data[0], _steering_and_pedal->tx_data[1], _steering_and_pedal);
    CanCommunication_transmitMessage(_steering_and_pedal);
}

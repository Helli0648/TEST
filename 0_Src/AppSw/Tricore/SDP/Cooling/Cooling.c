/*
 * Cooling.c
 * Created on: 
 * Author: 
 */

/***************************** Includes ******************************/
#include "Cooling.h"

/**************************** Macro **********************************/

/*********************** Global Variables ****************************/
cooling_on_t Cooling_On_msg;
CanCommunication_Message Cooling_On_msgObj;
const uint32 Cooling_On_MsgId = 0x20000;

/******************* Function Prototypes *********************/
void SDP_Cooling_initMessage(void);
void SDP_Cooling_run_10ms(void);

/******************* Private Function Prototypes *********************/
IFX_STATIC void SDP_Cooling_transmitMessage(cooling_on_t * _Cooling_On_msg);

/********************* Function Implementation ***********************/
void SDP_Cooling_initMessage(void){
    /* CAN message init */
	{
        CanCommunication_Message_Config config;
        config.messageId		=	Cooling_On_MsgId;
        config.frameType		=	IfxMultican_Frame_receive;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&Cooling_On_msgObj, &config);
	}
}

void SDP_Cooling_run_10ms(void){
    /* manual_mode */
    if (Cooling_On_msg.s.manual_mode_on == 1)
    {
        SDP_Cooling_transmitMessage(&Cooling_On_msg);
    }
}


/****************** Private Function Implementation ******************/
IFX_STATIC void SDP_Cooling_transmitMessage(cooling_on_t * _Cooling_On_msg)
{
        CanCommunication_setMessageData(_Cooling_On_msg->tx_data[0],_Cooling_On_msg->tx_data[1],_Cooling_On_msg);
        CanCommunication_transmitMessage(_Cooling_On_msg);
}

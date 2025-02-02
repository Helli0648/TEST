/*
 * Created: Aug 11th 2023
 * Author: Terry
 * yoonsb@hanyang.ac.kr
 *
 * This is for the accumulator cooling control circuit, STM32 embedded.
 * */

/***************** INCLUDES ********************/
#include "Accumulator.h"

/*********************** Global Variables ****************************/
battery_diagnose_t battery_diagnose;
fan_status_data_t fan_status_data;
fan_target_duty_t fan_target_duty;

CanCommunication_Message battery_diagnose_msgObj;
CanCommunication_Message fan_status_data_msgObj;
CanCommunication_Message fan_target_duty_msgObj;

const uint32 battery_diagnose_MsgID = 0x10000;
const uint32 fan_status_data_MsgID = 0x20001;
const uint32 fan_target_duty_MsgID = 0x20002;
/******************* Function Prototypes *********************/
void SDP_Accumulator_init(void);
void SDP_Accumulator_run_10ms(void);

/******************* Private Function Prototypes *********************/
IFX_STATIC void SDP_Accumulator_receiveMessage(void);

/********************* Function Implementation ***********************/
void SDP_Accumulator_init(void) {
	{
        CanCommunication_Message_Config config;
        config.messageId		=	battery_diagnose_MsgID;
        config.frameType		=	IfxMultican_Frame_receive;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&battery_diagnose_msgObj, &config);
	}
	{
        CanCommunication_Message_Config config;
        config.messageId		=	fan_status_data_MsgID;
        config.frameType		=	IfxMultican_Frame_receive;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&fan_status_data_msgObj, &config);
	}
	{
        CanCommunication_Message_Config config;
        config.messageId		=	fan_target_duty_MsgID;
        config.frameType		=	IfxMultican_Frame_transmit;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&fan_target_duty_msgObj, &config);
	}
}


void SDP_Accumulator_run_10ms(void)
{
	void SDP_Accumulator_receiveMessage(void);
}
/*
void SDP_Accumulator_run_10ms(void)
{
    CanCommunication_transmitMessage(&Accumulator.msgTC_order);
}



void SDP_Accumulator_setFanDuty(uint8 sideIntakePercentage, uint8 segmentIntakePercentage, uint8 segmentExhaust60Percentage, uint8 segmentExhaust80Percentage) {
	sideIntakePercentage = normalize(sideIntakePercentage);
	segmentIntakePercentage = normalize(segmentIntakePercentage);
	segmentExhaust60Percentage = normalize(segmentExhaust60Percentage);
	segmentExhaust80Percentage = normalize(segmentExhaust80Percentage);
	//ZERI
	Accumulator.TC_order.S.TCFanDutyOrder_SideIntake = sideIntakePercentage;
	Accumulator.TC_order.S.TCFanDutyOrder_SegmentIntake70 = segmentIntakePercentage;
	Accumulator.TC_order.S.TCFanDutyOrder_SegmentExhaust60 = segmentExhaust60Percentage;
	Accumulator.TC_order.S.TCFanDutyOrder_SegmentExhaust80 = segmentExhaust80Percentage;

	CanCommunication_setMessageData(Accumulator.TC_order.TransmitData[0], Accumulator.TC_order.TransmitData[1], &Accumulator.msgTC_order);
}


void SDP_Accumulator_setVCUmode(void) {
	Accumulator.TC_order.S.TCControlMode = 1;

	CanCommunication_setMessageData(Accumulator.TC_order.TransmitData[0], Accumulator.TC_order.TransmitData[1], &Accumulator.msgTC_order);
}

void SDP_Accumulator_resetVCUmode(void) {
	Accumulator.TC_order.S.TCControlMode = 0;

	CanCommunication_setMessageData(Accumulator.TC_order.TransmitData[0], Accumulator.TC_order.TransmitData[1], &Accumulator.msgTC_order);
}
*/


/****************** Private Function Implementation ******************/
IFX_STATIC void SDP_Accumulator_receiveMessage(void)
{
    if(CanCommunication_receiveMessage(&battery_diagnose_msgObj))
    {
    	battery_diagnose.rx_data[0] = battery_diagnose_msgObj.msg.data[0];
    	battery_diagnose.rx_data[1] = battery_diagnose_msgObj.msg.data[1];
    }
    if(CanCommunication_receiveMessage(&fan_status_data_msgObj))
    {
    	fan_status_data.rx_data[0] = fan_status_data_msgObj.msg.data[0];
    	fan_status_data.rx_data[1] = fan_status_data_msgObj.msg.data[1];
    }
    if(CanCommunication_receiveMessage(&fan_target_duty_msgObj))
    {
    	fan_target_duty.rx_data[0] = fan_target_duty_msgObj.msg.data[0];
    	fan_target_duty.rx_data[0] = fan_target_duty_msgObj.msg.data[0];
    }
}

/*
IFX_STATIC int normalize(uint8 percentage) {
	if(percentage < 0) return 0;
	if(percentage > 100) return 100;
	return percentage;
}
*/



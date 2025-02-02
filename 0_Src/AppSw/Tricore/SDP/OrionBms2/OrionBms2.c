/*//////////
 * OrionBms2.c
 * Created on: 2020.08.04
 * Author: Dua
 */

/***************************** Includes ******************************/
#include <IfxCpu.h>

#include "OrionBms2.h"
#include "RVC.h"
/**************************** Macro **********************************/

/*********************** Global Variables ****************************/
OrionBms_t OrionBms;

const uint32 Bms_power_limit_MsgId = 0x300UL;   //CAN�넻�떊 �븘�씠�뵒.
const uint32 Bms_temp_and_soc_MsgId = 0x30000UL;
const uint32 Bms_voltage_MsgId= 0x40000UL;

/******************* Function Prototypes *********************/
void OrionBms2_init(void);
void OrionBms2_run_1ms_c2(void);

/******************* Private Function Prototypes *********************/
IFX_STATIC void OrionBms2_receiveMessage(void);

/********************* Function Implementation ***********************/
void OrionBms2_init(void)
{
	{
		CanCommunication_Message_Config config;
		config.messageId = Bms_power_limit_MsgId;
		config.frameType = IfxMultican_Frame_receive;
		config.dataLen = IfxMultican_DataLengthCode_6;
		config.node = &CanCommunication_canNode0;
		config.isStandardId = FALSE;
		CanCommunication_initMessage(&OrionBms.Bms_power_limit_msgObj, &config);
	}
	{
		CanCommunication_Message_Config config;
		config.messageId = Bms_temp_and_soc_MsgId;
		config.frameType = IfxMultican_Frame_receive;
		config.dataLen = IfxMultican_DataLengthCode_6;
		config.node = &CanCommunication_canNode0;
		config.isStandardId = FALSE;
		CanCommunication_initMessage(&OrionBms.Bms_temp_and_soc_msgObj, &config);
	}
	{
		CanCommunication_Message_Config config;
		config.messageId = Bms_voltage_MsgId;
		config.frameType = IfxMultican_Frame_receive;
		config.dataLen = IfxMultican_DataLengthCode_6;
		config.node = &CanCommunication_canNode0;
		config.isStandardId = FALSE;
		CanCommunication_initMessage(&OrionBms.Bms_voltage_msgObj, &config);
	}
}

void OrionBms2_run_1ms_c2(void)
{
	OrionBms2_receiveMessage();
}


/****************** Private Function Implementation ******************/
IFX_STATIC void OrionBms2_receiveMessage(void)
{
	if(CanCommunication_receiveMessage(&Bms_power_limit_MsgId))
	{
		OrionBms.Bms_power_limit_msg.rx_data[0] = OrionBms.Bms_power_limit_msgObj.msg.data[0];
		OrionBms.Bms_power_limit_msg.rx_data[1] = OrionBms.Bms_power_limit_msgObj.msg.data[1];

		while(IfxCpu_acquireMutex(&RVC_public.bms.shared.mutex));
		// Wait for mutex
		{
			RVC_public.bms.shared.data.current = (float32)OrionBms.Bms_power_limit_msg.s.pack_current / 10;
			RVC_public.bms.shared.data.voltage = (float32)OrionBms.Bms_power_limit_msg.s.pack_voltage / 10;
			RVC_public.bms.shared.data.chargeLimit = OrionBms.Bms_power_limit_msg.s.pack_charge_current_limit;
			RVC_public.bms.shared.data.dischargeLimit = OrionBms.Bms_power_limit_msg.s.pack_discharge_current_limit;
			IfxCpu_releaseMutex(&RVC_public.bms.shared.mutex);
		}
	}

	if(CanCommunication_receiveMessage(&Bms_temp_and_soc_MsgId))
	{
		OrionBms.Bms_temp_and_soc_msg.rx_data[0] = OrionBms.Bms_temp_and_soc_msgObj.msg.data[0];
		OrionBms.Bms_temp_and_soc_msg.rx_data[1] = OrionBms.Bms_temp_and_soc_msgObj.msg.data[1];

		while(IfxCpu_acquireMutex(&RVC_public.bms.shared.mutex));
		// Wait for mutex
		{
			RVC_public.bms.shared.data.soc = (float32)OrionBms.Bms_temp_and_soc_msg.s.pack_soc / 2;
			RVC_public.bms.shared.data.highestTemp = (sint8)OrionBms.Bms_temp_and_soc_msg.s.high_temp;
			RVC_public.bms.shared.data.averageTemp = (sint8)OrionBms.Bms_temp_and_soc_msg.s.avg_temp;
			RVC_public.bms.shared.data.bmsTemp = (sint8)OrionBms.Bms_temp_and_soc_msg.s.bms_temp;
			IfxCpu_releaseMutex(&RVC_public.bms.shared.mutex);
		}
	}

	if(CanCommunication_receiveMessage(&Bms_voltage_MsgId))
	{
		OrionBms.Bms_voltage_msg.rx_data[0] = OrionBms.Bms_voltage_msgObj.msg.data[0];
		OrionBms.Bms_voltage_msg.rx_data[1] = OrionBms.Bms_voltage_msgObj.msg.data[1];

		while(IfxCpu_acquireMutex(&RVC_public.bms.shared.mutex));
		// Wait for mutex
		{
			RVC_public.bms.shared.data.lowestVoltage = (float32)OrionBms.Bms_voltage_msg.s.low_cell_voltage/10000;
			IfxCpu_releaseMutex(&RVC_public.bms.shared.mutex);
		}
	}
}



/* TODO:
 * CRC check
 * Update flag
 */


/*
//////�닔�젙 �쟾 �썑 蹂��닔紐낆씠 �뼱�뼸寃� 諛붾�뚯뿀�뒗吏� �솗�씤�븘�슂

IFX_STATIC void OrionBms2_receiveMessage(void)
{
	uint32 updateFlag;
	if(CanCommunication_receiveMessage(&OrionBms2.msgObj1))
	{
		OrionBms2.msg1.packCurrent = ((OrionBms2.msgObj1.msg.data[0] & 0x0000FFFF) >> 0);
		OrionBms2.msg1.packVoltage = ((OrionBms2.msgObj1.msg.data[0] & 0xFFFF0000) >> 16);
		OrionBms2.msg1.packSoc = ((OrionBms2.msgObj1.msg.data[1] & 0x000000FF) >> 0);
		while(IfxCpu_acquireMutex(&RVC_public.bms.shared.mutex))
			; // Wait for mutex
		{
			1RVC_public.bms.shared.data.current = (float32)OrionBms2.msg1.packCurrent / 10;
			1RVC_public.bms.shared.data.voltage = (float32)OrionBms2.msg1.packVoltage / 10;
			1RVC_public.bms.shared.data.soc = (float32)OrionBms2.msg1.packSoc / 2;
			// RVC_public.bms.shared.isUpdated = TRUE;
			IfxCpu_releaseMutex(&RVC_public.bms.shared.mutex);
		}
	}
	if(CanCommunication_receiveMessage(&OrionBms2.msgObj2))
	{
		OrionBms2.msg2.packChargeLimit = ((OrionBms2.msgObj2.msg.data[0] & 0x0000FFFF) >> 0);
		OrionBms2.msg2.packDischargeLimit = ((OrionBms2.msgObj2.msg.data[0] & 0xFFFF0000) >> 16);
		while(IfxCpu_acquireMutex(&RVC_public.bms.shared.mutex))
			; // Wait for mutex
		{
			1RVC_public.bms.shared.data.chargeLimit = OrionBms2.msg2.packChargeLimit;
			1RVC_public.bms.shared.data.dischargeLimit = OrionBms2.msg2.packDischargeLimit;
			// RVC_public.bms.shared.isUpdated = TRUE;
			IfxCpu_releaseMutex(&RVC_public.bms.shared.mutex);
		}
	}
	if(CanCommunication_receiveMessage(&OrionBms2.msgObj3))
	{
		OrionBms2.msg3.highTemp = ((OrionBms2.msgObj3.msg.data[0] & 0x000000FF) >> 0);
		OrionBms2.msg3.highCell = ((OrionBms2.msgObj3.msg.data[0] & 0x0000FF00) >> 8);
		OrionBms2.msg3.avgTemp = ((OrionBms2.msgObj3.msg.data[0] & 0x00FF0000) >> 16);
		OrionBms2.msg3.bmsTemp = ((OrionBms2.msgObj3.msg.data[0] & 0xFF000000) >> 24);
		OrionBms2.msg3.lowVoltage = ((OrionBms2.msgObj3.msg.data[1] & 0x0000FFFF) >> 0);
		while(IfxCpu_acquireMutex(&RVC_public.bms.shared.mutex))
			; // Wait for mutex
		{
			1RVC_public.bms.shared.data.highestTemp = (sint8)OrionBms2.msg3.highTemp;
			1RVC_public.bms.shared.data.averageTemp = (sint8)OrionBms2.msg3.avgTemp;
			1RVC_public.bms.shared.data.bmsTemp = (sint8)OrionBms2.msg3.bmsTemp;
			RVC_public.bms.shared.data.lowestVoltage = (float32)OrionBms2.msg3.lowVoltage/10000;
			// RVC_public.bms.shared.isUpdated = TRUE;
			IfxCpu_releaseMutex(&RVC_public.bms.shared.mutex);
		}
	}
}
*/


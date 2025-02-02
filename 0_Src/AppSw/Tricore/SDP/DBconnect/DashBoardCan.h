/*
 * 
 * Created on: 
 * Author: 
 */
#ifndef DASHBOARDCAN_H_
#define DASHBOARDCAN_H_

/**************************** Includes *******************************/
#include "HLD.h"
#include <Port/Io/IfxPort_Io.h>
#include "Configuration.h"
#include "CanCommunication.h"
#include "Gpio_Debounce.h"
#include "AmkInverter_can.h"
#include "Platform_Types.h"

/***************************** Macro *********************************/


/************************* Data Structures ***************************/
typedef union{
	uint32 RxData[2];
	struct{
		uint8 StartBtnPushed : 1;
        uint8 OFFvehicle : 1;
		uint32 Remain1 : 30;
        uint32 Remain2;
	}B;

}StartBtnPushed_t;

typedef union 
{
	uint32 data[2];				//[0-31][32-63]
	struct 
	{
		uint8 AmkState;				//[0-7]
		boolean SdcAmsOk		:1;	//[8]
		boolean SdcImdOk		:1;	//[9]
		boolean SdcBspdOk		:1; //[10]
		boolean SdcSen			:1;	//[11]
		boolean tsalOn			:1;	//[12]
		boolean reserved0		:3;	//[13-15]
		uint16 startCnt			:16;//[16-31]
		uint32 reserved2		:32;//[32-63]
	}B;
} DashBoardMsg0_t;

typedef union 
{
	uint32 data[2];				//[0-31][32-63]
	struct 
	{
		boolean StartBtn		:1;	//[0]
		uint16 reserved0		:15;//[1-15]
		uint16 startCntMirror	:16;//[16-31]
		uint32 reserved2		:32;//[32-63]
	}B;
} DashBoardMsg1_t;

typedef struct 
{
	boolean bmsOk;
	boolean imdOk;
	boolean bspdOk;
	boolean sdcSenFinal;
	boolean brakeOn;
	boolean tsalOn;
}DashBoard_info_t;

typedef struct
{
	DashBoard_info_t data;
	struct 
	{
		DashBoard_info_t data;
		IfxCpu_mutexLock mutex;
	}shared;
}DashBoard_public_t;




// id 0x130
// error_list
// VCU to Dash

typedef union ErrorList
{
    // infineon tc (VCU)
    // -> uint32_t tx_data[2];
    // stm32 (Dashboard)
    // -> uint8_t rx_data[8];
    // board spec�뿉 �뵲�씪 �떎由� �쐞 二쇱꽍 李몄“
    uint32 tx_data[2];
    struct
    {
        boolean     bms_ok             :1;  // [0]              --> �썝�옒 ams
        boolean     imd_ok             :1;  // [1]              -->
        boolean     bspd_ok            :1;  // [2]
        boolean     apps_ok            :1;  // [3]form	SDP_PedalBox.apps.isValueOk	
        boolean     bpps_ok            :1;  // [4]SDP_PedalBox.bpps.isValueOk	
        boolean     vcu_ok             :1;  // [5]              -> �뾾�쓬�쓬
        boolean     sdc_ok             :1;  // [6]              -> sdcsen
        boolean     rtd_on             :1;  // [7]              
        uint8       amk_state          :8;  // [8-15]           ->amkstate
        uint16      start_cnt          :16; // [16-31]          ->startcnt
		uint32      reserved_0         :32; // [32-63]
    } s;
} error_list_t;

IFX_EXTERN DashBoard_public_t DashBoard_public;
#endif

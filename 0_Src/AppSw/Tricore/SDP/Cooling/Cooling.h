/*
 * Cooling.h
 * Created on: 2021. 9. 25.
 * Author: Suprhimp
 */

#ifndef SRC_APPSW_TRICORE_SDP_COOLING_H
#define SRC_APPSW_TRICORE_SDP_COOLING_H
/**************************** Includes *******************************/
#include "SDP.h"
#include "CanCommunication.h"
#include "HLD.h"

/***************************** Macro *********************************/


/************************* Data Structures ***************************/
typedef union CoolingOn
{
    uint32 tx_data[2];
    struct
    {
        boolean manual_mode_on         :1;     //[0]
				boolean water_pump0_on         :1;     //[1]
				boolean water_pump1_on         :1;     //[2]
				boolean radiator0_on           :1;     //[3]
				boolean radiator1_on           :1;     //[4]
				boolean external_fan           :1;     //[5]
				boolean on_all                 :1;     //[6]
				boolean rst                    :1;     //[7]
				uint8 reserved_0          :8;     //[8-15]
				uint16 reserved_1         :16;    //[16-31]
				uint32 reserved_2         :32;    //[32-63]
    } s;
} cooling_on_t;

/***************************** Externs *********************************/
IFX_EXTERN cooling_on_t Cooling_On_msg;
IFX_EXTERN CanCommunication_Message Cooling_On_msgObj;

IFX_EXTERN void SDP_Cooling_initMessage(void);
IFX_EXTERN void SDP_Cooling_run_10ms(void);
#endif 

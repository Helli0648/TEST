/*
 * OrionBms2.h
 * Created on: 2020.08.04
 * Author: Dua
 */

#ifndef ORIONBMS2_H
#define ORIONBMS2_H
/**************************** Includes *******************************/
#include "CanCommunication.h"

/***************************** Macro *********************************/


/************************* Data Structures ***************************/
typedef union PowerLimit
{
    uint32 rx_data[2];
    struct
    {
        uint16 pack_current                   :16;    //[0-15]
        uint16 pack_voltage                   :16;    //[16-31]
        uint16 pack_charge_current_limit      :16;    //[32-47]
        uint16 pack_discharge_current_limit   :16;    //[48-63]
    } s;
}power_limit_t;

typedef union BmsTempAndSoc
{
    uint32 rx_data[2];
    struct
    {
        uint8   high_temp     :8;     // [0-7]
        uint8   avg_temp      :8;     // [8-15]
        uint8   bms_temp      :8;     // [16-23]
        uint8   pack_soc      :8;     // [24-31]
        uint16  dtc_status_1  :16;    // [32-47]
        uint8   dtc_status_2  :8;     // [48-55]
        uint8   reserved_0    :8;     // [56-63]
    } s;
}bms_temp_and_soc_t;

typedef union BmsVoltage
{
    uint32 rx_data[2];
    struct
    {
        uint16 low_cell_voltage           :16;    //[0-15]        //차량구동저항에 의한 전압 강하를 보정한 전압
        uint16 high_cell_voltage          :16;    //[16-31]
        uint16 low_open_cell_voltage      :16;    //[32-47]       //차량구동저항에 의한 전압 강하를 보정하지 않은 전압압
        uint16 high_open_cell_voltage     :16;    //[48-63]
    } s;
}bms_voltage_t;

typedef struct 
{
    power_limit_t Bms_power_limit_msg;
	bms_temp_and_soc_t Bms_temp_and_soc_msg;  //CAN통신을 통해 받아온 애들을 내부에 저장 및 사용하는 애들.
	bms_voltage_t Bms_voltage_msg;
   
    CanCommunication_Message Bms_power_limit_msgObj;
	CanCommunication_Message Bms_temp_and_soc_msgObj;  //CAN통신으로 받아오는 애들.
	CanCommunication_Message Bms_voltage_msgObj;

	uint32 canErrorCount;
	boolean canError;
}OrionBms_t;


/************************ Global Variables ***************************/
IFX_EXTERN OrionBms_t OrionBms;

/*********************** Function Prototypes *************************/
IFX_EXTERN void OrionBms2_init(void);
IFX_EXTERN void OrionBms2_run_1ms_c2(void);

#endif
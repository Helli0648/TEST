/*
 * Created: Aug 11th 2023
 * Author: Terry
 * yoonsb@hanyang.ac.kr
 *
 * This is for the accumulator cooling control circuit, STM32 embedded.
 * */

#ifndef __SDP_ACCUMULATOR_H__
#define __SDP_ACCUMULATOR_H__

#include "SDP.h"
#include "CanCommunication.h"
#include "HLD.h"



// id 0x10000
// battery_diagnose
// BATTERY to VCU
typedef union BatteryDiagnose
{
    uint32 rx_data[2];
    struct
    {
        uint8 precharge_state_signal1     :8; //[0-7]
		uint8 precharge_state_signal2     :8; //[8-15]
		uint8 relay_contact_signal1       :8; //[16-23]
		uint8 relay_contact_signal2	      :8; //[24-31]
		uint8 relay_contact_signal3	      :8; //[32-39]
		uint8 tsal_signal			      :8; //[40-47]
		uint8 IMD_status_frequency	      :8; //[48-55]
		uint8 reserved			          :8; //[56-63]
    } s;
} battery_diagnose_t;




/*
 * RX Data
 * Receive From: VCU
 * RX ID: 0x20001
 * Data: Fan Flag, TIM 15, 16, 17 Dutycycle & Frequency
 *
 ***Notation
 * Ex) if fanflag == 1, send frequency&duty cycle of no.7,4,1 fans.
 * Fanflag = 1: tim15: 7
 * 			 tim16: 4
 * 			 tim17: 1
 * Fanflag = 2: tim15: 8
 * 			 tim16: 5
 * 			 tim17: 2
 * Fanflag = 3: tim15: 9
 * 			 tim16: 6
 * 			 tim17: 3
 */
typedef union FanStatusData
{
    uint32 rx_data[2];
    struct
    {
        uint8 FanFlag			    :8; //[0-7]
		uint8 TIM15_Dutycycle     :8; //[8-15]
		uint8 TIM15_Frequency     :8; //[16-23]
		uint8 TIM16_Dutycycle     :8; //[24-31]
		uint8 TIM16_Frequency     :8; //[32-39]
		uint8 TIM17_Dutycycle     :8; //[40-47]
		uint8 TIM17_Frequency     :8; //[48-55]
		uint8 desiredDuty			:8; //[56-63]
    } s;
} fan_status_data_t;



// id 0x20002
// fan_target_duty
// BATTERY to VCU
typedef union FanTargetDuty
{
    uint32 rx_data[2];
    struct
    {
        uint8 TargetDuty_SideIntake           :8;     //[0-7]
		uint8 TargetDuty_SegmentIntake70      :8;     //[8-15]
		uint8 TargetDuty_SegmentExhaust60     :8;     //[16-23]
		uint8 TargetDuty_SegmentExhaust80     :8;     //[24-31]
        uint32 reserved_0                     :32;    //[32-63]
    } s;
} fan_target_duty_t;


/******************************** Externs *******************************/

IFX_EXTERN battery_diagnose_t;
IFX_EXTERN fan_status_data_t;
IFX_EXTERN fan_target_duty_t;

IFX_EXTERN void SDP_Accumulator_init(void);
IFX_EXTERN void SDP_Accumulator_run_10ms(void);





#endif






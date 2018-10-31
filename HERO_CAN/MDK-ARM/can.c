/*
IMPLEMENTATION:
Four files are needed to implement CAN
1. can.c
2. can.h
File 1 and 2 contain HAL CAN drivers that is placed into two files so that all
the functions are same when used on different systems. Moreover, file 1 and 2 also
contain user functions etc.
*/

/* ========== includes ========== */
#include "can.h"
#include <stdio.h>
#include <stdlib.h>
/* ============================== */

/* ========== can bus ========== */
CAN_HandleTypeDef hcan1;
/* ============================= */

/* ========== headers ========== */
CAN_TxHeaderTypeDef can1TxHeader0;
CAN_TxHeaderTypeDef can1TxHeader1;
CAN_RxHeaderTypeDef can1RxHeader;
/* ============================= */

/* ========== filters ========== */
CAN_FilterTypeDef can1Filter;
/* ============================= */

/* ========== buffers ========== */
uint8_t canTxMsg0[8] = {0};
uint8_t canTxMsg1[8] = {0};
uint32_t can_count=0;
/* ============================= */

/* ========== CAN initialize functions ========== */
void CAN_Initialize(void)
{
	hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	can1TxHeader0.IDE = CAN_ID_STD;
	can1TxHeader0.StdId = 0x200;
	can1TxHeader0.DLC = 8;
	
	can1TxHeader1.IDE = CAN_ID_STD;
	can1TxHeader1.StdId = 0x1FF;
	can1TxHeader1.RTR = CAN_RTR_DATA;
	can1TxHeader1.DLC = 8;
	
	
	can1Filter.FilterActivation = ENABLE;
	can1Filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can1Filter.FilterScale = CAN_FILTERSCALE_32BIT;
	can1Filter.FilterFIFOAssignment = CAN_FilterFIFO0;
	can1Filter.FilterIdHigh = 0x0000;
	can1Filter.FilterIdLow = 0x0000;
	can1Filter.FilterBank = 0;
	HAL_CAN_ConfigFilter(&hcan1,&can1Filter);
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_FULL);
	HAL_CAN_Start(&hcan1);
}
/* ============================================== */

/* ========== CAN user/transmit functions ========== */
void CAN_SendMsg(CAN_HandleTypeDef* hcan,CAN_TxHeaderTypeDef *canTxHeader,uint8_t* canMsg)
{
	HAL_CAN_AddTxMessage(hcan,canTxHeader,canMsg,(void*)CAN_TX_MAILBOX0);
}
// use set_CM_speed for chassis test. However, this function will soon be degraded >:)
void set_CM_speed(int16_t cm1_iq,int16_t cm2_iq,int16_t cm3_iq,int16_t cm4_iq)
{
    canTxMsg0[0] = (uint8_t)(cm1_iq >> 8);
    canTxMsg0[1] = (uint8_t)cm1_iq;
    canTxMsg0[2] = (uint8_t)(cm2_iq >> 8);
    canTxMsg0[3] = (uint8_t)cm2_iq;
    canTxMsg0[4] = (uint8_t)(cm3_iq >> 8);
    canTxMsg0[5] = (uint8_t)cm3_iq;
    canTxMsg0[6] = (uint8_t)(cm4_iq >> 8);
    canTxMsg0[7] = (uint8_t)cm4_iq;
    CAN_SendMsg(&hcan1,&can1TxHeader0,canTxMsg0);
}
/* ================================================= */

/* ========== CAN receive functions ========== */
void CanReceiveMsgProcess(CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg)
{      

	can_count++;
		switch(rxHeader->StdId)
		{
				case CAN_BUS2_MOTOR1_FEEDBACK_MSG_ID:
				{                    
				}break;
				case CAN_BUS2_MOTOR2_FEEDBACK_MSG_ID:
				{
				}break;
				case CAN_BUS2_MOTOR3_FEEDBACK_MSG_ID:
				{
				}break;
				case CAN_BUS2_MOTOR4_FEEDBACK_MSG_ID:
				{
				}break;
				case CAN_BUS2_MOTOR5_FEEDBACK_MSG_ID:
				{
				}break;
				
				case CAN_BUS2_MOTOR6_FEEDBACK_MSG_ID:
				{	
				}break;		
				case CAN_BUS2_MOTOR7_FEEDBACK_MSG_ID:
				{
				}break;
				case CAN_BUS2_MOTOR8_FEEDBACK_MSG_ID:
				{
				}break;
				
		}

}
/* =========================================== */





/* Stuff below this line is the new experimental way to use CAN */





/* ========== smart CAN ========== */
Can** all_rx_devices;
unsigned int total_rx_device = 0;
/* =============================== */

/* ========== smart CAN initialize functions ========== */
void Can_Start(Can device, CAN_HandleTypeDef* hcan)
{
	device.canTxHeader.IDE = CAN_ID_STD;
	device.canTxHeader.StdId = device.StdId;
	device.canTxHeader.DLC = device.DLC;
	device.canTxHeader.RTR = device.RTR;
	
	device.canFilter.FilterActivation = ENABLE;
	device.canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
	device.canFilter.FilterScale = CAN_FILTERSCALE_32BIT;
	device.canFilter.FilterFIFOAssignment = CAN_FilterFIFO0;
	device.canFilter.FilterIdHigh = 0x0000;
	device.canFilter.FilterIdLow = 0x0000;
	if (hcan == &hcan1)
		device.canFilter.FilterBank = 0;
	else
		device.canFilter.FilterBank = 14;
	HAL_CAN_ConfigFilter(hcan,&device.canFilter);
	HAL_CAN_ActivateNotification(hcan,CAN_IT_RX_FIFO0_FULL);
	HAL_CAN_Start(hcan);
	
	Device_Activate_Rx(&device);
}
Can create_Device(const uint32_t StdId, const uint32_t IDE, const uint32_t RTR, const uint32_t DLC, const uint32_t rx_StdId)
{
	Can device;
	device.StdId = StdId;
	device.IDE = IDE;
	device.RTR = RTR;
	device.DLC = DLC;
	device.rx_StdId = rx_StdId;
	return device;
}
/* ==================================================== */

/* ========== smart CAN transmit functions ========== */
void Can_Transmit(Can device,CAN_HandleTypeDef* hcan,uint8_t* canMsg)
{
	HAL_CAN_AddTxMessage(hcan,&device.canTxHeader,canMsg,(void*)CAN_TX_MAILBOX0);
}
/* ================================================== */

/* ========== smart CAN receive functions ========== */
void Device_Activate_Rx(Can* device)
{
	Can** new_rx_device = realloc(all_rx_devices, ++total_rx_device);
	if (new_rx_device == NULL)
	{
		// error check
	}
	else
	{
		all_rx_devices = new_rx_device;
		all_rx_devices[total_rx_device-1] = device;
	}
}
void Device_Receive(CAN_RxHeaderTypeDef* canRxHeader,uint8_t* canRxMsg)
{
	for (int i = 0; i < total_rx_device; ++i)
	{
		if (canRxHeader->StdId == all_rx_devices[i]->rx_StdId)
		{
			all_rx_devices[i]->data = canRxMsg;
		}
	}
}
void Can_Receive(CAN_HandleTypeDef* hcan, uint8_t* canRxMsg)	// place this in can1/2_rx_isr
{
	CAN_RxHeaderTypeDef canRxHeader;
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&canRxHeader,canRxMsg);
	Device_Receive(&canRxHeader, canRxMsg);
}
/* ================================================= */


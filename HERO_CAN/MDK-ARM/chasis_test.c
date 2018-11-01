#include "can.h"
/*
Before you test the chassis, there is one thing you must configure.
Here is a model of the chassis. [] are wheels, - is the body.

	 Front
[1]----[2]
	 ----
	 ----
	 ----
[3]----[4]
	 Back
	 
The ESCs have numbers 0x201 to 0x204. According to the positioning of 1234
on the model, edit the below function (sendMsg_2_Chassis).
Follow this scheme:
If position 1 is ESC 0x203, then change "esc1" to "esc3" etc.
If position 2 is ESC 0x202, don't change anything.
If position 3 is ESC 0x201, then change "esc3" to "esc1" etc.
Do this for all four ESC. Change ONLY THE PARAMETERS, don't change what's inside the function.
*/
void sendMsg_2_Chassis(int16_t esc1,int16_t esc2,int16_t esc3,int16_t esc4,Can* device, CAN_HandleTypeDef* hcan)
{
		uint8_t canTxMsg0[8];
    canTxMsg0[0] = (uint8_t)(esc1 >> 8);
    canTxMsg0[1] = (uint8_t)esc1;
    canTxMsg0[2] = (uint8_t)(esc2 >> 8);
    canTxMsg0[3] = (uint8_t)esc2;
    canTxMsg0[4] = (uint8_t)(esc3 >> 8);
    canTxMsg0[5] = (uint8_t)esc3;
    canTxMsg0[6] = (uint8_t)(esc4 >> 8);
    canTxMsg0[7] = (uint8_t)esc4;
    Can_Transmit(device, hcan, canTxMsg0);
}
/* That's all you need to configure yourself. */

/* Functions to aid testing: */
void forward(void);
void backward(void);
void stop(void);
void spin_clockwise(void);
void spin_counter_clockwise(void);
/* ==========================*/

/* Settings */
float power = 0.5; 
// This is not actually power, but the amount of torque
// You can change the power.
// 0.5 is low power, 1 is medium power, 2 is high power
/*==========*/

/* Enter commands below */
Can chassis;
void chassis_Commands(void)	// place in while-loop within main
{
	// Enter your command(s) below this line:
	forward();
	
}


/* Everything that you need is above this line! Ignore the stuff below ;) */

// positive is clockwise.
void start_chasis_test(void)	// place above while in main
{
	Device_Initialize(&chassis, 0x200, CAN_ID_STD, CAN_RTR_DATA, 8, 0x201, 8);
}
void forward(void)
{
	sendMsg_2_Chassis(5000*power, -5000*power, 5000*power, -5000*power, &chassis, &hcan1);
}
void backward(void)
{
	sendMsg_2_Chassis(-5000*power, 5000*power, -5000*power, 5000*power, &chassis, &hcan1);
}
void stop(void)
{
	sendMsg_2_Chassis(0, 0, 0, 0, &chassis, &hcan1);
}
void spin_clockwise(void)
{
	sendMsg_2_Chassis(5000*power, 5000*power, 5000*power, 5000*power, &chassis, &hcan1);
}
void spin_counter_clockwise(void)
{
	sendMsg_2_Chassis(-5000*power, -5000*power, -5000*power, -5000*power, &chassis, &hcan1);
}

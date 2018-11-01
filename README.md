# What is smart CAN

Suitable for the stm32f4xx series, smart CAN is an easy to use, robust, CAN implementation. You can easily add devices to use the CAN protocol and the code takes care of all the devices itself.

# How to use

Smart CAN is made to be easy to use, so naturally, there aren't that many steps to take before we get CAN running on your stm32. But, depending on your electronic device, the inputs to the functions will change so take care.

### Step 1 - Configure CAN and the stm32 internal clock with cubemx (skippable if you just copy this repository):
*To be written soon*

### Step 2 - Code:
Smart CAN take cares of communicating and receiving for you. The variable "Can" is a high level abstraction of the CAN protocol, the variable can be any electronic device that communicates with CAN. Once CAN bus is initialized, messages received by that Can variable will be stored inside an array of that Can variable.

1. Include "can.h" in your file.
```c
// in your_file.c
#include "can.h"
```

2. Create a Can variable.
```c
Can device;
```

3. Initialize Can variable by calling Device_initialize(parameters). The parameters are as follows:
```c
Device_Initialize(Can* device,const uint32_t StdId, const uint32_t IDE, const uint32_t RTR, const uint32_t DLC, const uint32_t rx_StdId, const int rx_buffer_size)

device: the Can variable passed as a pointer

StdId: the identifier (id) of the electronic device when using CAN

IDE: specifies whether you're using the standard or extended CAN protocol. Use the macro "CAN_ID_STD" to specify the standard CAN protocol and "CAN_ID_EXT" for the extended CAN protocol.

RTR: sets the type of frame to be transmitted. Use the macro "CAN_RTR_DATA" to transmit data.

DLC: the size of the data, in bytes (maximum 8 bytes), to be transmitted.

rx_StdId: the identifier (id) of the electronic device to receive CAN messages from

rx_buffer_size: size of the data, in bytes, to be received

// example initialization
Can device;
Device_Initialize(device,0x200, CAN_ID_STD, CAN_RTR_DATA, 8, 0x201, 8);
```

4. Place the function Can_Receive(parameters) into void CAN1_RX0_IRQHandler(void) which is located in the file "stm32f4xx_it.c".
```c
uint8_t canRxMsg[8];
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */
	Can_Receive(&hcan1, canRxMsg);
  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}
```
Note that canRxMsg is used to store all messages received by the stm32 through CAN. So, the size for canRxMsg[x] should be the largest amount of bytes to be received by the stm32 from an electronic device. In this example case, it is 8 bytes.

5. In main, call CAN_Initialize somewhere below MX_CAN1_Init.
```c
// in main.c
MX_CAN1_Init;
CAN_Initialize;
```

6. Lastly, send messages to your Can device by calling Can_Transmit(parameters). The parameters are as follows:
```c
Can_Transmit(Can* device,CAN_HandleTypeDef* hcan,uint8_t* canMsg);

device: the Can variable passed as a pointer
hcan: the CAN bus to be used for sending and receiving messages. In most cases, use CAN1, which by default can be referred to with "&hcan1".
canMsg: the message to be transmitted to the electronic device. This is a pointer to an array containing the data to be sent.
```

# Example

# More info

# Trouble-shooting

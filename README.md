# What is smart CAN

Suitable for the stm32f4xx series, smart CAN is an easy to use, robust, CAN implementation. You can easily add devices to use the CAN protocol and the code takes care of all the devices itself.

# How to use

Smart CAN is made to be easy to use, so naturally, there aren't that many steps to take before we get CAN running on your stm32. But, depending on your electronic device, the inputs to the functions will change so take care.

Step 1 - Configure CAN and the stm32 internal clock with cubemx (skippable if you just copy this repository):
*To be written soon*

Step 2 - Code:
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

3. Initialize Can variable by calling Device_initialize(parameters).
Note that you would've to fill in the paramters yourself according to
your needs. For details refer to complete guide.
4. Place the function Can_Receive(parameters) into void CAN1_RX0_IRQHandler(void)
, which is located in the file "stm32f4xx_it.c".
5. In main, call CAN_Initialize somewhere below MX_CAN1_Init.
6. Lastly, send messages to your Can device by calling Can_Transmit(parameters).


# Example

# More info

# Trouble-shooting

# Elevator based on tring-level PID algorithm
&emsp;&emsp; In this project, I construct a simluation environment in Proteus to simluate the machanism of elevator. I use stm32 to controll the motor and apply  serial PID as control algorithm.  
&emsp;&emsp; In this project, we expect to practice the learned PID control algorithm, intuitively understand the application of PID algorithm in daily production from the way of accomplishing the actual goal, deeply understand the principle of PID through practice, and better master the method of debugging for PID.  
&emsp;&emsp;  
![Static Badge](https://img.shields.io/badge/Keil5-make?style=for-the-badge&logo=STM32cubemx&logoColor=white&labelColor=blue&color=darkgreen)
![Static Badge](https://img.shields.io/badge/CubeMX-make?style=for-the-badge&logo=STMicroelectronics&labelColor=rgb(234%2C178%2C0)&color=blue)
![Static Badge](https://img.shields.io/badge/STM32F103R6-make?style=for-the-badge&logo=STMicroelectronics&labelColor=rgb(234%2C178%2C0)&color=gray)
![Static Badge](https://img.shields.io/badge/proteus-make?style=for-the-badge&logo=Proteus&logoColor=yellow&labelColor=yellow&color=yellow)
![Static Badge](https://img.shields.io/badge/matlab-make?style=for-the-badge&logo=matlab&logoColor=yellow&labelColor=yellow&color=orange)
![Static Badge](https://img.shields.io/badge/Python-make?style=for-the-badge&logo=python&labelColor=white&color=blue)
![Static Badge](https://img.shields.io/badge/Pandas-make?style=for-the-badge&logo=pandas&labelColor=purple&color=purple)


## 2. Structure of this project
&emsp;&emsp; This project can roughly be devied into three parts: **constructing the simulation in Proteus, coding for the STM32, debug the String-PID algorithm.**

### 2.1 Constructing the simulation in Proteus
&emsp;&emsp; I select stm32F103R6 as the main controlling board because of its convenience and easy-controlling. Besides, in order to make the simulation closer to reality, I also add **LCD, Key, LED, Motor Drive, and Motor into the simulation**.  
&emsp;&emsp; I have set four keys as floor keys corresponding to floors 1, 2, 3 and 4, and the assigned GPIO ports are IO ports 12, 13, 14 and 15 in GPIOA. we have set three keys as speed setting keys corresponding to the setting speeds of 1 rd/s, 1.6666 rad/s and 2 rad/s, and the assigned IO ports are IO ports 0, 1 and 2 in GPIOB. The assigned IO ports are IO ports 0, 1 and 2 in GPIOB. Before we run the elevator, we first have to select the desired speed size, the LED of the corresponding speed level will be lit, then select the floor, the corresponding LED will be lit after selecting the corresponding floor, which finally triggers the operation of the elevator. The following figure shows the circuit diagram of the keypad module built under Proteus.  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; ![image](https://github.com/unswimmingduck/Elevator/assets/111033998/1d08ea34-c1e0-4db5-91ca-4befd6247db6)

&emsp;&emsp; I use the LM016L module to display the relevant physical quantities in the system, enabling us to display the system operation status more intuitively. At the same time, I built the driver file for the LCD. the specific construction of the LCD module is shown in the following figure. The following figure shows the circuit diagram of the LCD module block built under Proteus.  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; ![image](https://github.com/unswimmingduck/Elevator/assets/111033998/06b05eb2-fa4f-46c3-84ad-ec478aeb2939)

&emsp;&emsp; In the motor drive control, I choose the L289N driver module to control it. By controlling the high and low levels of its logic channel to control the forward and reverse rotation of the motor, and then by controlling the size of the input PWM duty cycle to control the speed of the motor. The following figure shows the motor and motor driver module circuit I built in Proteus.  
&emsp;&emsp;&emsp;![image](https://github.com/unswimmingduck/Elevator/assets/111033998/4b65f080-cd07-47d6-8251-da38bf097e5a)

  
### 2.2 Configuring and Coding for STM32F103R6
&emsp;&emsp; **After building the relevant module circuits, I configured the STM32F103R6 using the STM32CubeMx, and at the same time wrote the relevant code to control the corresponding GPIO ports of the STM32.**  
&emsp;&emsp;  
&emsp;&emsp; Firstly, I assigned the GPIO pins on the stm32 for the module circuit built in 1.1. The following image shows the assignment of GPIO Pin on STM32F103R6 in Proteus.  
&emsp;&emsp;&emsp;&emsp;![image](https://github.com/unswimmingduck/Elevator/assets/111033998/93f52615-bc08-4921-88d3-d0e00460b1c0)

 &emsp;&emsp; Secondly, I used STM32CubeMx to set the initilization code for the GPIO Pin we used. The following images shows the GPIO Pins we want to enable.  
 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![image](https://github.com/unswimmingduck/Elevator/assets/111033998/82da182f-642c-42ea-b630-2d99be22f245)

  &emsp;&emsp; Thirdly, I coded for the functions in this elevator. I used USART, TIM, NVIC Interrupt and other peripherals to realize the function of this elevator. If you want to get more details, you could open [elavotr/stm32_code/MDK-ARM/project1.uvprojx](https://github.com/unswimmingduck/Elevator/blob/main/stm32_code/MDK-ARM/project1.uvprojx) in Keil5 and find the main.c file to see more detail. What's more, I put the realization of some functions on [elevator/stm32_code/Core/Src](https://github.com/unswimmingduck/Elevator/tree/main/stm32_code/Core/Src) file. 

 ### 2.3 Debug the String-PID
 

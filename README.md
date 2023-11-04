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
![Static Badge](https://img.shields.io/badge/matplotlib-make?style=for-the-badge&logo=python&logoColor=white&labelColor=blue&color=gray)
![Static Badge](https://img.shields.io/badge/Pandas-make?style=for-the-badge&logo=pandas&labelColor=purple&color=purple)


## 2. Structure of this project
&emsp;&emsp; This project can roughly be devied into three parts: **constructing the simulation in Proteus, coding for the STM32, debug the String-PID algorithm.**

### 2.1 Constructing the simulation in Proteus
&emsp;&emsp; I select stm32F103R6 as the main controlling board because of its convenience and easy-controlling. Besides, in order to make the simulation closer to reality, I also add **LCD, Key, LED, Motor Drive, and Motor into the simulation**. **You can see the whole simulation circuit by opening [proteus_world.pdsprj](https://github.com/unswimmingduck/Elevator/blob/main/proteus_world.pdsprj) on Proteus**  
&emsp;&emsp; I have set four keys as floor keys corresponding to floors 1, 2, 3 and 4, and the assigned GPIO ports are IO ports 12, 13, 14 and 15 in GPIOA. we have set three keys as speed setting keys corresponding to the setting speeds of 1 rd/s, 1.6666 rad/s and 2 rad/s, and the assigned IO ports are IO ports 0, 1 and 2 in GPIOB. The assigned IO ports are IO ports 0, 1 and 2 in GPIOB. Before we run the elevator, we first have to select the desired speed size, the LED of the corresponding speed level will be lit, then select the floor, the corresponding LED will be lit after selecting the corresponding floor, which finally triggers the operation of the elevator. The following figure shows the circuit diagram of the keypad module built under Proteus.  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; ![image](https://github.com/unswimmingduck/Elevator/assets/111033998/1d08ea34-c1e0-4db5-91ca-4befd6247db6)

&emsp;&emsp; I use the LM016L module to display the relevant physical quantities in the system, enabling us to display the system operation status more intuitively. At the same time, I built the driver file for the LCD. the specific construction of the LCD module is shown in the following figure. The following figure shows the circuit diagram of the LCD module block built under Proteus.  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; ![image](https://github.com/unswimmingduck/Elevator/assets/111033998/06b05eb2-fa4f-46c3-84ad-ec478aeb2939)

&emsp;&emsp; In the motor drive control, I choose the L289N driver module to control it. By controlling the high and low levels of its logic channel to control the forward and reverse rotation of the motor, and then by controlling the size of the input PWM duty cycle to control the speed of the motor. The following figure shows the motor and motor driver module circuit I built in Proteus.  
&emsp;&emsp;&emsp;![image](https://github.com/unswimmingduck/Elevator/assets/111033998/4b65f080-cd07-47d6-8251-da38bf097e5a)  
&emsp;&emsp;&emsp;

  
### 2.2 Configuring and Coding for STM32F103R6
&emsp;&emsp; **After building the relevant module circuits, I configured the STM32F103R6 using the STM32CubeMx, and at the same time wrote the relevant code to control the corresponding GPIO ports of the STM32.**  
&emsp;&emsp;  
&emsp;&emsp; Firstly, I assigned the GPIO pins on the stm32 for the module circuit built in 1.1. The following image shows the assignment of GPIO Pin on STM32F103R6 in Proteus.  
&emsp;&emsp;&emsp;&emsp;![image](https://github.com/unswimmingduck/Elevator/assets/111033998/93f52615-bc08-4921-88d3-d0e00460b1c0)

 &emsp;&emsp; Secondly, I used STM32CubeMx to set the initilization code for the GPIO Pin we used. The following images shows the GPIO Pins we want to enable.  
 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![image](https://github.com/unswimmingduck/Elevator/assets/111033998/82da182f-642c-42ea-b630-2d99be22f245)

  &emsp;&emsp; Thirdly, I coded for the functions in this elevator. I used USART, TIM, NVIC Interrupt and other peripherals to realize the function of this elevator. If you want to get more details, you could open [elavotr/stm32_code/MDK-ARM/project1.uvprojx](https://github.com/unswimmingduck/Elevator/blob/main/stm32_code/MDK-ARM/project1.uvprojx) in Keil5 and find the main.c file to see more detail. What's more, I put the realization of some functions on [elevator/stm32_code/Core/Src](https://github.com/unswimmingduck/Elevator/tree/main/stm32_code/Core/Src) file. 

 ### 2.3 Realizing the String-PID
&emsp;&emsp; In the process of realizing serial PID, I use the serial port to output the relevant data of the motor, and then use python to organize the output data, and then use MatLab to further process the data to generate the transfer function, and at the same time, use MatLab for the PID auto-tuning parameter to generate the PID parameter of the corresponding meter. There are four parts in realizing the Serial PID: **Collecting motor speed data, Processing the data, Generating transfer functions on Matlab, Generating PID parameter on Matlab.**  
&emsp;&emsp; Firstly, I utilized the USART serial port on the stm32F103R6 to output the relevant information captured. At the same time, we utilize the TIM2 timer (with a period of 0.01s), to determine the sampling frequency, and both output the relevant sampling information to the serial port every 0.01S. The port we captured was the port on the right end of the motor-encoder, and we captured the time difference between the two rising edges in its output PWM, which measured the time it took for the code disk to rotate by one resolution. I copied the data from the terminal in Proteus to a .txt file in local computer.  
&emsp;&emsp; Secondly,I used python to process the sampled data and calculate its rotational speed so that we can import the collected data into matlab for transfer function solving and PID modulation. Since we set the resolution of the disk to 60, and the data we collected is the time taken to rotate between the two disk resolutions, and the frequency of TIM3 is 1KHz, the rotational **speed = ((6/(cnt/1000))/360) rad/s**. After the rotational speed is processed, we use pandas to store the data in .csv format so that it can be easily imported into matlab. You can see way I proccess data on [data_process/rotation_speed.py](https://github.com/unswimmingduck/Elevator/blob/main/data_process/rotation_speed.py). What's more, I put the data processed by [data_process/rotation_speed.py](https://github.com/unswimmingduck/Elevator/blob/main/data_process/rotation_speed.py) in .csv formation no [data_process/rotaion-speed_data.csv](https://github.com/unswimmingduck/Elevator/blob/main/data_process/rotaion-speed_data.csv).   
&emsp;&emsp; Thirdly, I used Matlab's System Identification toolbox to calculate the transfer function. The following image shows the transfer function by matlab.  
 &emsp;&emsp;&emsp;&emsp;![image](https://github.com/unswimmingduck/Elevator/assets/111033998/3d62afcd-1bd0-42ee-a54a-3e1477576c5a)
 &emsp;&emsp;&emsp;&emsp;![image](https://github.com/unswimmingduck/Elevator/assets/111033998/8b3677b1-d73d-48bb-962c-9528419616db)  
&emsp;&emsp; At last, we build Serial PID on matlab simulink and use relavent toolbox to adjusting PID parameters according our need.  
&emsp;&emsp; **In the process of building and debugging the serial PID, the speed PID should be adjusted first, so that the speed PID can respond quickly and make the elevator run at the speed we expect when it is working. Then adjust the position PID, so that the elevator can accurately run to the desired position, to ensure that the elevator runs accurately.**  
&emsp;&emsp; In the process of building and debugging the speed PID, we firstly build speed pid in simulink and then input the transfer function. The following image shows the simulation built in simulink.  
![image](https://github.com/unswimmingduck/Elevator/assets/111033998/da7b2295-c61f-4919-8554-a5d55ccb74a6)
&emsp;&emsp; Then we use the PID Regulation in matlab to adjust PID parameter so that the speed PID can suit our goal. The following image shows the capability of the speed PID and parameter of speed PID.  
&emsp;&emsp; ![image](https://github.com/unswimmingduck/Elevator/assets/111033998/63e8ccc0-2173-482b-9731-f108f2bf1ed3)  
&emsp;&emsp; The following image shows the speed control curves displayed by scope in simulink.  
&emsp;&emsp; ![image](https://github.com/unswimmingduck/Elevator/assets/111033998/fcd7d9e1-7a6d-4d3b-918c-42b6f39c8b3c)  
&emsp;&emsp; To ensuring that this speed PID can perform well in Proteus, I also used python and matplotlib to write a function named [PID_test/V_Proc.py](https://github.com/unswimmingduck/Elevator/blob/main/PID_test/V_Proc.py) to test whether the speed PID works well. The speed of the motor in Proteus will be output through the Usart serial port and the speed curve in Proteus will be drawn through [PID_test/V_Proc.py](https://github.com/unswimmingduck/Elevator/blob/main/PID_test/V_Proc.py), which is more intuitive to observe whether the tuning curve is in line with our expectation. The following image shows that the speed PID works well in Proteus.  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; ![image](https://github.com/unswimmingduck/Elevator/assets/111033998/60a19a1b-1887-4579-aa7f-ddb3f382145b)









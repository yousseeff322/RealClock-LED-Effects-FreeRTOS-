# RealClock-LED-Effects-FreeRTOS 🎉  
This project integrates **Real-Time Clock (RTC)** functionality with dynamic **LED effects** using the **STM32F401Ux (Black Pill)** microcontroller and **FreeRTOS**. It provides a robust way to manage timekeeping, user interaction, and LED displays efficiently.

---

## 🌟 Project Highlights
- **Customizable LED Effects:** Choose from 4 captivating LED patterns.
- **Real-Time Clock Management:** Easily configure and display time and date.
- **Live Time Reporting:** Continuous time updates every second via Serial Wire Viewer (SWV).
- **FreeRTOS-based Task Scheduling:** Ensures responsive and reliable task management.

---

## 🛠️ Peripherals Used (via CubeMX HAL Drivers)
- **UART:** Handles user input and serial communication.
- **RTC:** Manages precise timekeeping and date settings.
- **GPIO:** Controls LED outputs.
- **SysTick Timer:** Acts as the FreeRTOS tick source for task scheduling.

---

## 📝 Task Design Overview
The project follows a modular **RTOS-based task** design:

1. **Command Task:**  
   Manages UART-based user input and forwards it to other tasks.

2. **Display Task:**  
   Displays menus and feedback messages to the user via UART.

3. **Main Menu Task:**  
   Allows users to switch between LED control and RTC configuration.

4. **LED Effect Task:**  
   Runs various LED patterns selected by the user through FreeRTOS **software timers**.

5. **RTC Task:**  
   Configures and displays the current time and date, with live reporting every second.

---

## 📡 Queue Management
Two **FreeRTOS queues** are used to facilitate communication:  
- **Q_Print:** Transmits messages to UART for printing.  
- **Q_Data:** Receives user input from UART.

---

## 🚀 Planned Future Enhancements
- **Alarms:** Add alarm functionality for scheduled alerts.
- **SEGGER Debugging Tool:** Utilize SEGGER tools to enhance performance and debugging.

---

## ⚙️ Setup Instructions

1. **Hardware Required:**
   - STM32F401Ux (Black Pill) Microcontroller  
   - LEDs connected to GPIO pins  
   - UART interface for serial communication  

2. **Software Required:**
   - STM32CubeIDE / Keil uVision / Any IDE with FreeRTOS support
   - FreeRTOS library and STM32 HAL drivers
   - Serial Terminal (e.g., PuTTY, TeraTerm)

3. **How to Build and Run:**
   - Clone the repository:
     ```bash
     git clone https://github.com/your-username/RealClock-LED-Effects-FreeRTOS.git
     cd RealClock-LED-Effects-FreeRTOS
     ```
   - Open the project in **STM32CubeIDE** (or other IDEs).
   - Build and flash the code to the **STM32F401 microcontroller**.
   - Open a **serial terminal** (115200 baud rate) to interact with the menu.

---

## 📋 User Menu Overview

1. **Main Menu:**
2. **LED Effect Menu:**
3. **RTC Configuration Menu:**


---

## 📢 LinkedIn Post
Check out my LinkedIn post about this project [here]([https://www.linkedin.com/in/your-linkedin-profile](https://www.linkedin.com/feed/update/urn:li:activity:7251200021465812993/)).

## 💻 Code Structure
```plaintext
📂 RealClock-LED-Effects-FreeRTOS
│
├── 📁 Core/ (STM32Cube generated core files)
├── 📁 Drivers/ (HAL drivers)
├── 📁 FreeRTOS/ (FreeRTOS configuration files)
├── 📁 Src/ (Source code)
│    ├── main.c
│    ├── RTC.c   
│    ├── TaskHandlers.c   // Core tasks implementation
│    └── led_effect.c   
├── 📁 Inc/ (Header files)
│    ├── main.h
│    ├── FreeRTOS_config.h
├── .gitignore
└── README.md


  ---



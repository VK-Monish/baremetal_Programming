# Baremetal_Programming
STM32F401CCU6
# Baremetal Programming

This repository contains low-level programming examples written for bare-metal environments, with a focus on microcontrollers and SoC platforms. The goal is to understand and control hardware directly without relying on operating systems or high-level abstractions.

## 🚀 Overview

This project is aimed at students, hobbyists, and engineers who want to:

- Learn how embedded systems work at the register and memory level.
- Interface with hardware peripherals without using any RTOS or OS.
- Write clean and efficient C code for microcontrollers such as STM32, ESP8266, and others.

## 🧠 Topics Covered

- GPIO control
- UART communication
- Timers and delay functions
- Interrupt handling
- Bare-metal blinking LED programs
- Direct register access and memory-mapped I/O

## 🧰 Tools & Requirements

- **Toolchain**: `arm-none-eabi-gcc`, `make`
- **Platform**: Ubuntu/Linux (for building and flashing)
- **Debug/Flash Tools**: ST-Link, OpenOCD, or equivalent
- **Board Examples**: STM32F401, ESP8266, or other Cortex-M microcontrollers

## 🛠️ How to Build

Install the required toolchain:

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi make

To build a project:

cd stm32/blinky/
make

To flash (example using OpenOCD):

openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program main.elf verify reset exit"

    Adjust flashing commands based on your hardware.
📚 References

    ARM Cortex-M Programming Manual

    STM32 Reference Manuals

    Interrupts and low-level programming docs

    Manufacturer datasheets

🤝 Contributions

Pull requests and suggestions are welcome! If you'd like to add support for more boards or peripherals, feel free to fork and contribute.

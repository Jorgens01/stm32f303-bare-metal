# STM32F303 Bare-Metal Implementations

This repository serves as a personal collection of minimalistic, bare-metal programming examples and essential files for the STM32F303 series microcontrollers. The core focus is on showcasing direct hardware register manipulation and low-level control, designed for developers who prefer hands-on, register-level programming without relying on extensive abstraction layers or complex external libraries.

---

### Project Purpose

* To demonstrate direct interaction with STM32F303 microcontroller peripherals at a register level.
* To provide clear, concise code implementations illustrating fundamental embedded functionalities.
* To serve as a personal reference and showcase for bare-metal STM32 development techniques.

---

### Hardware Target

All code within this repository is specifically designed and intended for **STM32F303 microcontrollers**. Examples are generally compatible with standard development boards featuring this MCU (e.g., STM32 Nucleo-F3 or Discovery-F3 boards).

---

### Repository Structure

* `include/`: This directory contains essential, shared header files that define the STM32F303 microcontroller's registers, memory maps, and core definitions. These files are fundamental for bare-metal programming.
* `projects/`: This directory houses individual bare-metal application examples. Each sub-directory within `projects/` represents a self-contained code implementation for a specific functionality, typically including its source files and specific build configuration.
* `README.md`: This file, providing an overview of the entire repository.
* `LICENSE`: Defines the terms under which this code can be used.
* `.gitignore`: Specifies files and directories that Git should ignore (e.g., build artifacts, IDE configuration files).

---

### How to Use This Code

This repository primarily focuses on providing **source code implementations** for the STM32F303. It is intended for individuals who:

* Are familiar with STM32 bare-metal development principles.
* Have access to and proficiency with an ARM cross-compilation toolchain (e.g., GNU ARM Embedded Toolchain).
* Utilize their own preferred build system (e.g., Makefiles, or CMake projects configured with their specific toolchain and linker scripts).
* Possess the necessary hardware (an STM32F303 development board) and a debug probe (e.g., ST-Link) for flashing and debugging.

**Please note:** This repository does **NOT** include a pre-configured build environment, detailed toolchain setup instructions, or ready-to-use flashing utilities. It is designed for those who are already equipped with the necessary development environment and wish to review or adapt the direct code implementations.

---

### License

This project is licensed under the [MIT License](LICENSE).

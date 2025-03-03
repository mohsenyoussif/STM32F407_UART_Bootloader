# Custom Bootloader for STM32F407 Discovery Board

## Overview
This project implements a **custom bootloader** for the STM32F407 Discovery Board. The bootloader allows firmware updates via a communication interface and provides various commands for device control.

## Features
- Supports multiple bootloader commands
- CRC verification for data integrity
- Flash memory read, write, and erase operations
- Secure boot mechanism with Read/Write protection



## Requirements
- **Microcontroller**: STM32F407 (or similar)
- **Communication Interface**: UART (can be extended to other protocols)

## Bootloader Commands
| Command Name         | Command Code | Description                         |
|----------------------|-------------|-------------------------------------|
| GET_VERSION         | `0x51`       | Get bootloader version             |
| GET_HELP            | `0x52`       | Get list of supported commands     |
| GET_CID             | `0x53`       | Get chip ID                        |
| GET_RDP_STATUS      | `0x54`       | Read protection level status       |
| GO_TO_ADDR          | `0x55`       | Jump to user application           |
| FLASH_ERASE         | `0x56`       | Erase flash memory                 |
| MEM_WRITE           | `0x57`       | Write to flash memory              |
| EN_RW_PROTECT       | `0x58`       | Enable read/write protection       |
| MEM_READ            | `0x5A`       | Read from flash memory             |
| READ_SECTOR_STATUS  | `0x5B`       | Get flash sector protection status |
| OTP_READ            | `0x5C`       | Read one-time programmable memory  |
| DIS_WR_PROTECT      | `0x5D`       | Disable write protection           |

## Host Setup
The **PC** is used as the host to communicate with the bootloader via a selected tool. The tool sends commands and receives responses through the **UART interface**.

### Required Tool
- **STM32_Bootloader_Tool**: acts as the host, communicating with the bootloader via a selected tool. This tool sends commands and receives responses over the **UART Interface**

### Sending Commands from PC  

I followed these steps to communicate with the bootloader:  

1. **I ensured the bootloader was running on the STM32.**  
2. **I connected my PC to the STM32 via UART** (using a USB-TTL module).  
3. **I used a specific tool on my PC to send commands to the bootloader.**  

The output of my bootloader communication looks like this:  

![Bootloader Output](https://i.postimg.cc/28SsdZ4q/output.png)  








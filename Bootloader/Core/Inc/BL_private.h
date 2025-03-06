#ifndef INC_BL_PRIVATE_H_
#define INC_BL_PRIVATE_H_

/*
 * CRC Status Codes
 * ----------------
 * These macros define the possible results of CRC (Cyclic Redundancy Check) verification.
 * CRC is used to ensure data integrity during communication.
 */
#define CRC_SUCCESS    1u  /* CRC verification passed */
#define CRC_FAIL       0u  /* CRC verification failed */

/*
 * Bootloader Version
 * ------------------
 * Defines the current version of the Bootloader.
 */
#define BL_VERSION     1u  /* Bootloader version 1 */


/*
 * Address Validation Status
 * -------------------------
 * These macros are used to indicate whether a given memory address is valid.
 *
 * - `VALID_ADDRESS`: Indicates that the address falls within a permissible memory region.
 * - `NOT_VALID_ADDRESS`: Indicates that the address is outside the allowed memory regions.
 */
#define VALID_ADDRESS                1u  /* Address is valid */
#define NOT_VALID_ADDRESS            0u  /* Address is not valid */


/*
 * DBGMCU_IDCODE_REGISTER
 * -----------------------
 * Defines the memory-mapped register that holds the Device ID (Chip ID)
 * of the microcontroller. It allows reading the unique identifier stored in the
 * Debug MCU (DBGMCU) module.
 *
 * Usage:
 * ------
 * - The lower 12 bits of this register contain the **Device ID (Chip ID)**.
 * - It helps in identifying the specific STM32 microcontroller model.
 *
 */
#define DBGMCU_IDCODE_REGISTER         *((volatile uint32_t*)0xE0042000)

/*
 * RDP_USER_OPTION_WORD
 * ---------------------
 * This macro provides access to the Read Protection (RDP) and User Option Bytes stored
 * in the system memory at address 0x1FFFC000. The value at this address determines
 * the read protection level of the microcontroller.
 * Usage:
 * ------
 * - This macro can be used to read the current protection level or user configuration settings.
 * - It is often referenced in security-related operations, such as checking if the firmware is protected.
 *
 * Example:
 * --------
 * ```c
 * uint32_t rdpStatus = RDP_USER_OPTION_WORD;
 * if ((rdpStatus & 0xFF) == 0xAA) {
 *     // RDP Level 0 (No protection)
 * } else if ((rdpStatus & 0xFF) == 0xBB) {
 *     // RDP Level 1 (Read protection enabled)
 * } else {
 *     // RDP Level 2 (Permanent protection)
 * }
 */
#define  RDP_USER_OPTION_WORD          *((volatile uint32_t*) 0x1FFFC000)

/*==========================================================================================================*/
/*
 * Function Prototypes
 * -------------------
 * These static functions are used internally within the bootloader.
 */


/*
 * uint8VerifyCRC
 * --------------
 * Verifies the integrity of received data using CRC.
 *
 * @param copy_puint8dataArr   : Pointer to the data array to be verified.
 * @param copy_uint8Length     : Length of the data array in bytes.
 * @param copy_uint32HostCRC   : Expected CRC value received from the Host.
 *
 * @return uint8_t: CRC verification result:
 *         - CRC_SUCCESS (if computed CRC matches expected CRC)
 *         - CRC_FAIL    (if computed CRC does not match)
 */
static uint8_t uint8VerifyCRC(uint8_t* copy_puint8dataArr, uint8_t copy_uint8Length, uint32_t copy_uint32HostCRC);


/*
 * voidSendACK
 * -----------
 * Sends an Acknowledgment (ACK) response to the Host.
 *
 * @param copy_uint8ReplyLength : The length of the response data following the ACK.
 */
static void voidSendACK(uint8_t copy_uint8ReplyeLngth);


/*
 * voidSendNACK
 * ------------
 * Sends a Negative Acknowledgment (NACK) response to the Host,
 * indicating an error or invalid request.
 */
static void voidSendNACK(void);


/*
 * uint8_ValidateAddress
 * ----------------------
 * Validates whether a given memory address falls within the permissible memory regions
 * (Flash or SRAM).
 */
static uint8_t uint8_ValidateAddress(uint32_t Copy_uint32Address);

#endif /* INC_BL_PRIVATE_H_ */

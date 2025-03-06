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




#endif /* INC_BL_PRIVATE_H_ */

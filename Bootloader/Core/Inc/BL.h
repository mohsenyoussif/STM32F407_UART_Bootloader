
#ifndef INC_BL_H_
#define INC_BL_H_

/*
 * Bootloader Acknowledgment Codes
 */
#define BL_ACK                       0xA5     /* Acknowledgment response from Bootloader */
#define BL_NACK                      0x7F    /* Negative acknowledgment (Invalid command or failure) */


/*
 * Bootloader Command Codes
 * -------------------------
 * These commands are sent from the Host to the Bootloader over UART
 * to request specific operations.
 */
#define BL_GET_VESRION               0x51  /* Get Bootloader version */
#define BL_GET_HELP                  0x52  /* Get supported Bootloader commands */
#define BL_GET_CID                   0x53  /* Get chip identification number */
#define BL_GET_RDP_STATUS            0x54  /* Get Read Protection (RDP) status */
#define BL_GO_TO_ADDR                0x55  /* Jump to a specific memory address */
#define BL_FLASH_ERASE               0x56  /* Erase specific sectors/pages in flash memory */
#define BL_MEM_WRITE                 0x57  /* Write data to flash memory */
#define BL_EN_RW_PROTECT             0x58  /* Enable read/write protection for memory sectors */
#define BL_MEM_READ                  0x59  /* Read data from memory */
#define BL_READ_SECTOR_STATUS        0x5A  /* Get the protection status of memory sectors */
#define BL_OTP_READ                  0x5B  /* Read One-Time Programmable (OTP) memory */
#define BL_DIS_WR_PROTECT            0x5C  /* Disable write protection for memory sectors */


/*
 * Bootloader Command Handler Functions
 * ------------------------------------
 * Each function processes a specific Bootloader command received from the Host.
 */


void BL_voidHandleGetVERCmd(uint8_t* copy_puint8CmdPacket);        /* Handles BL_GET_VERSION command */

void BL_voidHandleGetHelpCmd(uint8_t* copy_puint8CmdPacket);       /* Handles BL_GET_HELP command */

void BL_voidHandleGetCIDcmd(uint8_t* copy_puint8CmdPacket);        /* Handles BL_GET_CID command */

void BL_voidHandleGetRDPStatusCmd(uint8_t* copy_puint8CmdPacket);  /* Handles BL_GET_RDP_STATUS command */

void BL_voidHandleGoToAddressCmd(uint8_t* copy_puint8CmdPacket);   /* Handles BL_GO_TO_ADDR command */

void BL_voidHandleFlashEraseCmd(uint8_t* copy_puint8CmdPacket);    /* Handles BL_FLASH_ERASE command */

void BL_voidHandleMemWriteCmd(uint8_t* copy_puint8CmdPacket);      /* Handles BL_MEM_WRITE command */

void BL_voidHandleEnRWProtectCmd(uint8_t* copy_puint8CmdPacket);   /* Handles BL_EN_RW_PROTECT command */

void BL_voidHandleMemReadCmd(uint8_t* copy_puint8CmdPacket);       /* Handles BL_MEM_READ command */

void BL_voidHandleReadSectorStatusCmd(uint8_t* copy_puint8CmdPacket); /* Handles BL_READ_SECTOR_STATUS command */

void BL_voidHandleOTPReadCmd(uint8_t* copy_puint8CmdPacket);       /* Handles BL_OTP_READ command */

void BL_voidHandleDisWRProtectCmd(uint8_t* copy_puint8CmdPacket);  /* Handles BL_DIS_WR_PROTECT command */




#endif /* INC_BL_H_ */

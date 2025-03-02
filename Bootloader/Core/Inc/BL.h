
#ifndef INC_BL_H_
#define INC_BL_H_


#define BL_ACK                       0xA5
#define BL_NACK                      0x7F

/*Define the Bootloader commands*/
#define BL_GET_VESRION               0X51
#define BL_GET_HELP                  0X52
#define BL_GET_CID                   0X53
#define BL_GET_RDP_STATUS            0X54
#define BL_GO_TO_ADDR                0X55
#define BL_FLASH_ERASE                0X56
#define BL_MEM_WRITE                 0X57
#define BL_EN_RW_PROTECT             0X58
#define BL_MEM_READ                  0X5A
#define BL_READ_SECTOR_STATUS        0X5B
#define BL_OTP_READ                  0X5C
#define BL_DIS_WR_PROTECT            0X5D


void BL_voidHandleGetVERCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleGetHelpCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleGetCIDcmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleGetRDPStatusCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleGoToAddressCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleFlashEraseCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleMemWriteCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleEnRWProtectCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleMemReadCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleReadSectorStatusCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleOTPReadCmd(uint8_t* copy_puint8CmdPacket);

void BL_voidHandleDisWRProtectCmd(uint8_t* copy_puint8CmdPacket);


#endif /* INC_BL_H_ */

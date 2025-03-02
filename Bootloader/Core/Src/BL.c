
#include <stdio.h>
#include "BL.h"
#include "main.h"
#include "BL_private.h"


extern CRC_HandleTypeDef hcrc;
extern UART_HandleTypeDef huart2;
static uint8_t uint8VerifyCRC(uint8_t* copy_puint8dataArr,uint8_t copy_uint8Length,uint32_t copy_uint32HostCRC)
{
	uint8_t Local_uint8Iterator , Local_uint8CRCStatus ;
	uint32_t Local_uint8AccCRC   , Local_uint8Temp;
	for(Local_uint8Iterator = 0 ; Local_uint8Iterator < copy_uint8Length; Local_uint8Iterator++)
	{
		Local_uint8Temp = copy_puint8dataArr[Local_uint8Iterator];
		Local_uint8AccCRC = HAL_CRC_Accumulate(&hcrc, &Local_uint8Temp, 1);
	}
	/*Reset the CRC calculation unit because every time i call this function , it start calculate from the beggening*/
	__HAL_CRC_RESET_HANDLE_STATE(&hcrc);

	if(Local_uint8AccCRC == copy_uint32HostCRC)
	{
		Local_uint8CRCStatus = CRC_SUCCESS ;
	}else
	{
		Local_uint8CRCStatus = CRC_FAIL ;
	}
	return Local_uint8CRCStatus  ;
}

/*this function send ACK + length to follow */
static void voidSendACK(uint8_t copy_uint8ReplyeLngth)
{
     uint8_t Local_uint8AckBuffer[2] = {BL_ACK ,copy_uint8ReplyeLngth };

     HAL_UART_Transmit(&huart2, Local_uint8AckBuffer, 2, HAL_MAX_DELAY);
}
static void voidSendNACK(void)
{
	uint8_t Local_uint8NAck = BL_NACK ;

	 HAL_UART_Transmit(&huart2, &Local_uint8NAck, 1, HAL_MAX_DELAY);
}


void BL_voidHandleGetVERCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8BLVersion , Local_uint8CRCStatus , Local_uint8CmdLen ;
	uint32_t Local_uint32HostCRC ;

	Local_uint8CmdLen = copy_puint8CmdPacket[0]+1; /*the first byte already inclues the length to follow*/

	 Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen -4 ));
     /*Verify CRC*/

	 Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen-4), Local_uint32HostCRC);

	 if(Local_uint8CRCStatus == CRC_SUCCESS)
	 {
		 voidSendACK(1u); /*Bootloader version has size of 1 byte*/

		 Local_uint8BLVersion = BL_VERSION ;
		 HAL_UART_Transmit(&huart2, &Local_uint8BLVersion, 1, HAL_MAX_DELAY);
	 }else
	 {
		 voidSendNACK();
	 }

}

void BL_voidHandleGetHelpCmd(uint8_t* copy_puint8CmdPacket)
{

}

void BL_voidHandleGetCIDcmd(uint8_t* copy_puint8CmdPacket)
{

}


void BL_voidHandleGetRDPStatusCmd(uint8_t* copy_puint8CmdPacket)
{

}

void BL_voidHandleGoToAddressCmd(uint8_t* copy_puint8CmdPacket)
{

}

void BL_voidHandleFlashEraseCmd(uint8_t* copy_puint8CmdPacket)
{

}

void BL_voidHandleMemWriteCmd(uint8_t* copy_puint8CmdPacket)
{

}

void BL_voidHandleEnRWProtectCmd(uint8_t* copy_puint8CmdPacket)
{

}

void BL_voidHandleMemReadCmd(uint8_t* copy_puint8CmdPacket)
{

}

void BL_voidHandleReadSectorStatusCmd(uint8_t* copy_puint8CmdPacket)
{

}

void BL_voidHandleOTPReadCmd(uint8_t* copy_puint8CmdPacket)
{

}

void BL_voidHandleDisWRProtectCmd(uint8_t* copy_puint8CmdPacket)
{

}

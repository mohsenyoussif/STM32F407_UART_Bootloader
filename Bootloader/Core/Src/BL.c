
#include <stdio.h>
#include "BL.h"
#include "main.h"
#include "BL_private.h"


extern CRC_HandleTypeDef hcrc;
extern UART_HandleTypeDef huart2;


/*
 * uint8VerifyCRC
 * --------------
 * **Behavior:**
 * This function is responsible for verifying the integrity of received data by calculating its
 * CRC (Cyclic Redundancy Check) and comparing it with the expected CRC received from the host.
 * If both values match, the data is considered valid; otherwise, it is deemed corrupted.
 *
 * **Execution Steps:**
 * 1. Iterate through each byte of the received data array.
 * 2. Accumulate the CRC value using the HAL_CRC_Accumulate() function.
 * 3. Reset the CRC calculation unit to prepare for future computations.
 * 4. Compare the computed CRC with the one received from the host.
 * 5. Return `CRC_SUCCESS` if they match, otherwise return `CRC_FAIL`.
 *
 * **Use Case:**
 * This function is typically used in bootloaders or communication protocols where data integrity
 * verification is required before processing a received command or data block.
 *
 * Parameters:
 * -----------
 * @param copy_puint8dataArr   : Pointer to the data array that needs CRC verification.
 * @param copy_uint8Length     : Length of the data array in bytes.
 * @param copy_uint32HostCRC   : Expected CRC value received from the Host for comparison.
 *
 * Return:
 * -------
 * @return uint8_t: CRC verification result:
 *         - CRC_SUCCESS (if calculated CRC matches the expected CRC)
 *         - CRC_FAIL    (if calculated CRC does not match)
 */


static uint8_t uint8VerifyCRC(uint8_t* copy_puint8dataArr,uint8_t copy_uint8Length,uint32_t copy_uint32HostCRC)
{
	uint8_t Local_uint8Iterator , Local_uint8CRCStatus ;
	uint32_t Local_uint8AccCRC   , Local_uint32Temp;

	/*
	 * Step 1: Compute the CRC for the given data.
	 * The function iterates through each byte of the data array, accumulating the CRC value.
	 */
	for(Local_uint8Iterator = 0 ; Local_uint8Iterator < copy_uint8Length; Local_uint8Iterator++)
	{
		/* Load the current byte from the data array into a temporary variable */
		Local_uint32Temp = copy_puint8dataArr[Local_uint8Iterator];

		/* Accumulate the CRC for the current byte */
		Local_uint8AccCRC = HAL_CRC_Accumulate(&hcrc, &Local_uint32Temp, 1);
	}

	/*
	 * Step 2: Reset the CRC calculation unit.
	 * This ensures that every time this function is called, it starts calculation from the beginning.
	 */
	__HAL_CRC_RESET_HANDLE_STATE(&hcrc);


	/*
	 * Step 3: Compare the computed CRC with the expected CRC from the Host.
	 * If they match, the CRC check is successful; otherwise, it fails.
	 */
	if(Local_uint8AccCRC == copy_uint32HostCRC)
	{
		Local_uint8CRCStatus = CRC_SUCCESS ;   /* CRC verification passed */
	}else
	{
		Local_uint8CRCStatus = CRC_FAIL ;     /* CRC verification failed */
	}

	/* Step 4: Return the verification result (CRC_SUCCESS or CRC_FAIL) */
	return Local_uint8CRCStatus  ;
}



/*
 * voidSendACK
 * -----------
 * This function sends an Acknowledgment (ACK) response to the Host,
 * along with the length of the response data that follows.
 *
 * @param copy_uint8ReplyLength : Number of bytes that the Bootloader will send after ACK.
 *
 * Behavior:
 * ----------
 * - The function creates a buffer containing:
 *     [0] -> BL_ACK (indicating a successful command reception)
 *     [1] -> Length of the response data that follows
 * - It then transmits this buffer over UART to notify the Host.
 */

static void voidSendACK(uint8_t copy_uint8ReplyeLngth)
{
	/* Buffer to hold the ACK response and the length of the following response */
	uint8_t Local_uint8AckBuffer[2] = {BL_ACK, copy_uint8ReplyeLngth};

	/* Send ACK response via UART */
	HAL_UART_Transmit(&huart2, Local_uint8AckBuffer, 2, HAL_MAX_DELAY);
}

/*
 * voidSendNACK
 * ------------
 * This function sends a Negative Acknowledgment (NACK) response to the Host,
 * indicating that the received command or data is invalid or has failed verification.
 *
 * Behavior:
 * ----------
 * - The function prepares a single-byte buffer containing BL_NACK.
 * - It then transmits this NACK response over UART to notify the Host of the failure.
 */

static void voidSendNACK(void)
{
	/* Buffer holding the NACK response */
	uint8_t Local_uint8NAck = BL_NACK;

	/* Send NACK response via UART */
	HAL_UART_Transmit(&huart2, &Local_uint8NAck, 1, HAL_MAX_DELAY);
}

/*
 * uint8_ValidateAddress
 * ---------------------
 * This function checks whether a given memory address is within valid memory regions.
 * The address is considered valid if it falls within the Flash memory or SRAM1.
 *
 * Parameters:
 * -----------
 * @param Copy_uint32Address : The memory address to be validated.
 *
 * Behavior:
 * ---------
 * 1. Initializes the address status as `NOT_VALID_ADDRESS`.
 * 2. Checks if the address falls within the Flash memory range (`FLASH_BASE` to `FLASH_END`).
 * 3. If not in Flash, checks if the address is within the SRAM1 range.
 * 4. If the address matches either region, it is marked as `VALID_ADDRESS`, otherwise remains `NOT_VALID_ADDRESS`.
 * 5. Returns the validation result.
 *
 * Return:
 * -------
 * @return uint8_t : Address validation status:
 *         - `VALID_ADDRESS` if the address is within Flash or SRAM.
 *         - `NOT_VALID_ADDRESS` otherwise.
 */
static uint8_t uint8_ValidateAddress(uint32_t Copy_uint32Address)
{

	     /* Initialize status as NOT valid */
		uint8_t Local_uint8AddressStatus = NOT_VALID_ADDRESS;

		/* Check if the address falls within Flash memory */
	     if((Copy_uint32Address >= FLASH_BASE) && (Copy_uint32Address<= FLASH_END))
	     {
	    	 Local_uint8AddressStatus = VALID_ADDRESS ;
	     }
	     /* Check if the address falls within SRAM memory */
	     else if((Copy_uint32Address >= SRAM1_BASE) && (Copy_uint32Address <= (SRAM1_BASE + (128*1024))))
	     {
	    	 Local_uint8AddressStatus = VALID_ADDRESS ;
	     }
	     else
	     {
	    	 Local_uint8AddressStatus = NOT_VALID_ADDRESS ;
	     }

return Local_uint8AddressStatus ;
}

/**
 * BL_voidHandleGetVERCmd
 * @brief  Handles the "Get Version" command sent by the host.
 *         It verifies the CRC and, if valid, responds with the bootloader version.
 *
 * Behavior:
 * ----------
 * 1. Extracts the command length from the first byte of the packet.
 * 2. Retrieves the CRC value sent by the host (last 4 bytes of the packet).
 * 3. Verifies the CRC of the received data.
 * 4. If CRC is valid:
 *      - Sends an ACK response.
 *      - Sends the bootloader version to the host.
 * 5. If CRC is invalid:
 *      - Sends a NACK response.
 *
 * Parameters:
 * -----------
 * @param  copy_puint8CmdPacket: Pointer to the received command packet from the host.
 *
 * Return:
 * -------
 * @return None.
 */
void BL_voidHandleGetVERCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8BLVersion, Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{
		/* Send ACK with the length of the response payload (1 byte for version) */
		voidSendACK(1u);

		/* Send the bootloader version */
		Local_uint8BLVersion = BL_VERSION;
		HAL_UART_Transmit(&huart2, &Local_uint8BLVersion, 1, HAL_MAX_DELAY);
	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}


/*
 * BL_voidHandleGetHelpCmd
 * -----------------------
 * This function handles the "Get Help" command in the bootloader.
 * It verifies the received command packet using CRC and, if valid,
 * sends back a list of supported bootloader commands to the host.
 *
 * Parameters:
 * -----------
 * @param copy_puint8CmdPacket : Pointer to the received command packet.
 *
 * Behavior:
 * ---------
 * 1. Extracts the command length from the first byte of the packet.
 * 2. Retrieves the CRC value from the last 4 bytes of the packet.
 * 3. Verifies the integrity of the received packet using `uint8VerifyCRC()`.
 * 4. If CRC verification passes:
 *      - Prepares an array of supported bootloader commands.
 *      - Sends an ACK with the size of the supported commands list.
 *      - Transmits the list of supported commands via UART.
 * 5. If CRC verification fails, sends a NACK to the host.
 */
void BL_voidHandleGetHelpCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{

		/* Define an array containing all supported bootloader commands */
		uint8_t Local_uint8BLSupportedCommands []=
		{
			                	BL_GET_VESRION            ,
			                 	BL_GET_HELP               ,
								BL_GET_CID                ,
								BL_GET_RDP_STATUS         ,
								BL_GO_TO_ADDR             ,
								BL_FLASH_ERASE            ,
								BL_MEM_WRITE              ,
								BL_EN_RW_PROTECT          ,
								BL_MEM_READ               ,
								BL_READ_SECTOR_STATUS     ,
								BL_OTP_READ               ,
								BL_DIS_WR_PROTECT
		};

		/* Send an ACK with the size of the supported commands list */
		voidSendACK(sizeof(Local_uint8BLSupportedCommands));

		/* Transmit the list of supported commands to the host over UART */
		HAL_UART_Transmit(&huart2, Local_uint8BLSupportedCommands, sizeof(Local_uint8BLSupportedCommands), HAL_MAX_DELAY);

	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}


/*
 * BL_voidHandleGetCIDcmd
 * ----------------------
 * This function handles the "Get Chip ID" command from the host.
 * It extracts the Chip ID (CID) from the MCU's IDCODE register and sends it back.
 *
 * Parameters:
 * -----------
 * @param copy_puint8CmdPacket : Pointer to the received command packet.
 *
 * Behavior:
 * ---------
 * 1. Extracts the length of the command from the first byte.
 * 2. Retrieves the expected CRC from the last 4 bytes of the packet.
 * 3. Verifies the CRC of the received data.
 * 4. If CRC is valid:
 *    - Reads the Chip ID from the `DBGMCU_IDCODE_REGISTER`.
 *    - Sends an ACK response to indicate success.
 *    - Transmits the Chip ID (2 bytes) to the host.
 * 5. If CRC verification fails, it sends a NACK response.
 */
void BL_voidHandleGetCIDcmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint16_t Local_uint16CID ; // Variable to hold the extracted Chip ID
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Step 1: Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Step 2: Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Step 3: Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{
		/* Step 4: Retrieve Chip ID (12-bit value from the DBGMCU_IDCODE register) */
		Local_uint16CID = DBGMCU_IDCODE_REGISTER & 0x0fff;

		/* Step 5: Send ACK with the response length (2 bytes for Chip ID) */
		voidSendACK(2u);

		/* Step 6: Transmit the Chip ID to the host */
		HAL_UART_Transmit(&huart2, (uint8_t*)&Local_uint16CID , 2 , HAL_MAX_DELAY);
	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}



/*
 * BL_voidHandleGetRDPStatusCmd
 * ----------------------------
 * This function handles the "Get Read Protection (RDP) Status" command from the host.
 * It verifies the CRC, extracts the RDP status from the option bytes, and sends it back.
 *
 * Parameters:
 * -----------
 * @param copy_puint8CmdPacket   : Pointer to the received command packet from the host.
 *
 * Behavior:
 * ---------
 * 1. Extracts the command length from the first byte of the packet.
 * 2. Retrieves the CRC sent by the host from the last 4 bytes of the packet.
 * 3. Calls `uint8VerifyCRC()` to check if the received data is valid.
 * 4. If the CRC check passes:
 *      - Reads the RDP status from the option bytes at `RDP_USER_OPTION_WORD`.
 *      - Sends an ACK followed by the 1-byte RDP status.
 * 5. If the CRC check fails, sends a NACK.
 */
void BL_voidHandleGetRDPStatusCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{

		/* Extract RDP status from option bytes (stored in the upper byte) */
		uint8_t Local_uint8RDPStatus = (uint8_t)((RDP_USER_OPTION_WORD >> 8) & 0xff);

		/* Send ACK with the length of the response payload (1 byte for version) */
		voidSendACK(1u);

		/* Transmit the RDP status back to the host */
		HAL_UART_Transmit(&huart2, (uint8_t*)&Local_uint8RDPStatus , 1 , HAL_MAX_DELAY);

	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}

/*
 * BL_voidHandleGoToAddressCmd
 * ----------------------------
 * Handles the "Go To Address" command from the Host, which requests jumping to a specific
 * memory address and executing the code present there.
 *
 * **Parameters:**
 * ---------------
 * @param copy_puint8CmdPacket : Pointer to the received command packet containing:
 *        - Command length
 *        - Target address
 *        - CRC for verification
 *
 * **Behavior:**
 * ------------
 * 1. Extracts the command length from the packet.
 * 2. Extracts and verifies the CRC to ensure data integrity.
 * 3. If the CRC is valid:
 *    - Sends an ACK response to the Host.
 *    - Extracts the target memory address from the command packet.
 *    - Validates if the address falls within the permissible memory regions.
 *    - If valid:
 *        - Sends confirmation to the Host.
 *        - Jumps to the specified address by updating the Program Counter (PC).
 *    - If invalid:
 *        - Sends a NACK response to indicate failure.
 * 4. If the CRC check fails, a NACK is sent immediately.
 */
void BL_voidHandleGoToAddressCmd(uint8_t* copy_puint8CmdPacket)
{
    uint8_t Local_uint8CRCStatus;
    uint8_t Local_uint8CmdLen;       /* Variable to store command length */
    uint32_t Local_uint32HostCRC;    /* Variable to store CRC received from Host */

    /* Extract command length (first byte includes "Length to follow") */
    Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

    /* Extract CRC from the last 4 bytes of the received packet */
    Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

    /* Verify CRC of the received command */
    Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

    if (Local_uint8CRCStatus == CRC_SUCCESS)
    {
        uint32_t Local_uint32Address;
        uint8_t Local_uint8AddressValidStatus;

        /* Send ACK with the length of the response payload (1 byte for address validation result) */
        voidSendACK(1u);

        /* Extract the target address from the command packet */
        Local_uint32Address = *((uint32_t*)&copy_puint8CmdPacket[2]);

        /* Validate if the extracted address falls within Flash or SRAM */
        Local_uint8AddressValidStatus = uint8_ValidateAddress(Local_uint32Address);

        if (Local_uint8AddressValidStatus == VALID_ADDRESS)
        {
            /* Notify the Host that the address is valid */
            HAL_UART_Transmit(&huart2, &Local_uint8AddressValidStatus, 1, HAL_MAX_DELAY);

            /*
             * Jump to the specified address:
             * - Define a pointer to function.
             * - Increment address by 1 to ensure Thumb mode (T-bit = 1).
             * - Cast address to function pointer and execute.
             */
            void (*Local_pvFuncPtr)(void) = NULL;
            Local_uint32Address++;  /* Increment to set T-bit for ARM Cortex-M Thumb mode */
            Local_pvFuncPtr = (void*)Local_uint32Address;
            Local_pvFuncPtr();  /* Jump to the specified address */

            /* Same to __asm volatile("MSR PC ,%0"::"r"(Local_uint32Address+1));
             * */
        }
        else
        {
        	/* Notify the Host that the address is Invalid */
          HAL_UART_Transmit(&huart2, &Local_uint8AddressValidStatus, 1, HAL_MAX_DELAY);
        }
    }
    else
    {
        /* Send NACK if CRC verification fails */
        voidSendNACK();
    }
}



void BL_voidHandleFlashEraseCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{
		/* Send ACK with the length of the response payload (1 byte for version) */
		voidSendACK(1u);

	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}

void BL_voidHandleMemWriteCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{
		/* Send ACK with the length of the response payload (1 byte for version) */
		voidSendACK(1u);

	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}

void BL_voidHandleEnRWProtectCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{
		/* Send ACK with the length of the response payload (1 byte for version) */
		voidSendACK(1u);

	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}

void BL_voidHandleMemReadCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{
		/* Send ACK with the length of the response payload (1 byte for version) */
		voidSendACK(1u);

	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}

void BL_voidHandleReadSectorStatusCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{
		/* Send ACK with the length of the response payload (1 byte for version) */
		voidSendACK(1u);

	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}

void BL_voidHandleOTPReadCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{
		/* Send ACK with the length of the response payload (1 byte for version) */
		voidSendACK(1u);

	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}

void BL_voidHandleDisWRProtectCmd(uint8_t* copy_puint8CmdPacket)
{
	uint8_t Local_uint8CRCStatus;
	uint8_t Local_uint8CmdLen; // this variable to extract command length
	uint32_t Local_uint32HostCRC; // this variable to extract host CRC

	/* Extract command length (first byte includes "Length to follow") */
	Local_uint8CmdLen = copy_puint8CmdPacket[0] + 1;

	/* Extract CRC from the last 4 bytes of the received packet */
	Local_uint32HostCRC = *((uint32_t*)(copy_puint8CmdPacket + Local_uint8CmdLen - 4));

	/* Verify CRC of the received command */
	Local_uint8CRCStatus = uint8VerifyCRC(copy_puint8CmdPacket, (Local_uint8CmdLen - 4), Local_uint32HostCRC);

	if (Local_uint8CRCStatus == CRC_SUCCESS)
	{
		/* Send ACK with the length of the response payload (1 byte for version) */
		voidSendACK(1u);

	}
	else
	{
		/* Send NACK if CRC verification fails */
		voidSendNACK();
	}
}

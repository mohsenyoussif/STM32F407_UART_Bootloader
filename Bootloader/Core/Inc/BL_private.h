

#ifndef INC_BL_PRIVATE_H_
#define INC_BL_PRIVATE_H_

#define CRC_SUCCESS    1u
#define CRC_FAIL       0u

#define BL_VERSION     1u

static uint8_t uint8VerifyCRC(uint8_t* copy_puint8dataArr,uint8_t copy_uint8Length,uint32_t copy_uint32HostCRC);

static void voidSendACK(uint8_t copy_uint8ReplyeLngth);

static void voidSendNACK(void);


#endif /* INC_BL_PRIVATE_H_ */

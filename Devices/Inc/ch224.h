#ifndef __CH224_H__
#define __CH224_H__

#include "main.h"

// 可能有两种地址
#define CH224_I2C_7ADDR1                    0x22
#define CH224_I2C_7ADDR2                    0x23

#define CH224_REG_PROTOCOL                  0x09
#define CH224_REG_VOLTAGE_SET               0x0A
#define CH224_REG_PD_MAX_CURRENT_GET        0x50
#define CH224_REG_AVS_H                     0x51
#define CH224_REG_AVS_L                     0x52
#define CH224_REG_PPS_VOLTAGE_SET           0x53
#define CH224_REG_PD_SRCCAP_GET             0x60 // 0x60 ~ 0x8F

typedef enum {
    CH224_ERROR_OKAY = 0,
    CH224_ERROR_NO_DEVICE,
    CH224_ERROR_COMMUNICATION_FAILED,
} ch224_error_e;

// 协议类型
typedef enum {
    CH224_PROTOCOL_BC = 0,
    CH224_PROTOCOL_QC2,
    CH224_PROTOCOL_QC3,
    CH224_PROTOCOL_PD,
    CH224_PROTOCOL_EPR,

    CH224_PROTOCOL_MAX_NUM,
} ch224_protocol_e;



struct ch224_stu {
    uint8_t i2c_addr;
    I2C_HandleTypeDef *i2c;

    uint8_t (*read_reg)(struct ch224_stu *ch, uint8_t reg_addr);
    void (*write_reg)(struct ch224_stu *ch, uint8_t reg_addr, uint8_t data);
};

ch224_error_e ch224_init(struct ch224_stu * ch);
ch224_protocol_e ch224_get_protocol(struct ch224_stu *ch);


uint8_t my_ch224_read_reg(struct ch224_stu *ch, uint8_t reg_addr);
void my_ch224_write_reg(struct ch224_stu *ch, uint8_t reg_addr, uint8_t data);

#endif // __CH224_H__

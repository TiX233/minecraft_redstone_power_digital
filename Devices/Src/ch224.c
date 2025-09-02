#include "ch224.h"


ch224_error_e ch224_init(struct ch224_stu * ch){
    uint8_t reg = 0;

    HAL_I2C_Master_Transmit(ch->i2c, CH224_I2C_7ADDR1<<1, &reg, 1, 500);

    if(ch->i2c->ErrorCode == HAL_I2C_ERROR_NONE){
        LOG_FMT(PRINT_DEBUG"Find dev in 0x%02x\n", CH224_I2C_7ADDR1);
        ch->i2c_addr = CH224_I2C_7ADDR1;
    }else {
        HAL_I2C_Master_Transmit(ch->i2c, CH224_I2C_7ADDR2<<1, &reg, 1, 500);
        if(ch->i2c->ErrorCode == HAL_I2C_ERROR_NONE){
            LOG_FMT(PRINT_DEBUG"Find dev in 0x%02x\n", CH224_I2C_7ADDR2);
            ch->i2c_addr = CH224_I2C_7ADDR2;
        }else {
            return CH224_ERROR_NO_DEVICE;
        }
    }

    return CH224_ERROR_OKAY;
}

ch224_protocol_e ch224_get_protocol(struct ch224_stu *ch){
    uint8_t data = ch->read_reg(ch, CH224_REG_PROTOCOL);
    uint8_t i;

    for(i = 0; i < CH224_PROTOCOL_MAX_NUM; i ++){
        if(data & (1 << i)){
            break;
        }
    }

    return i;
}

uint8_t my_ch224_read_reg(struct ch224_stu *ch, uint8_t reg_addr){
    uint8_t readBuffer[2] = {0, 0};

    if(HAL_I2C_Mem_Read(ch->i2c, ch->i2c_addr << 1, reg_addr, 1, readBuffer, 1, 500) != HAL_OK){
        LOG_STR(PRINT_ERROR"I2C read reg ERROR!\n");
        LOG_FMT(PRINT_ERROR"Errcode: %d\n", ch->i2c->ErrorCode);
    }

    return readBuffer[0];
}

void my_ch224_write_reg(struct ch224_stu *ch, uint8_t reg_addr, uint8_t data){
    uint8_t buffer[2] = {reg_addr, data};
    if(HAL_I2C_Master_Transmit(ch->i2c, ch->i2c_addr, buffer, 2, 500) != HAL_OK){
        LOG_STR(PRINT_ERROR"I2C tramsmit ERROR!\n");
    }
}

#include "scan_LED.h"


void scanLED_write_xy(struct scanLED_stu *led, uint8_t x, uint8_t y, uint8_t state){
    led->buffer[x] = state ? (led->buffer[x] | (1 << y)) : (led->buffer[x] & ~(1 << y));
}

void scanLED_scan(struct scanLED_stu *led){
    led->write_pin(led, led->scan_com_map[led->now_com], 0);
    led->now_com ++;
    if(led->now_com == led->num_com){
        led->now_com = 0;
    }
    led->write_pin(led, led->scan_com_map[led->now_com], 1);

    for(uint8_t i = 0; i < led->num_one; i++){
        if(led->buffer[led->now_com] & (1 << i)){
            led->write_pin(led, led->scan_one_map[i], 0);
        }else {
            led->write_pin(led, led->scan_one_map[i], 1);
        }
    }

}




void myScanLED_write_pin(struct scanLED_stu *led, scanLED_pin_map_stu_t *pin, uint8_t pin_state){
    HAL_GPIO_WritePin(pin->port, pin->pin, pin_state);
}

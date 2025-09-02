#ifndef __SCAN_LED_H__
#define __SCAN_LED_H__

#include "main.h"

typedef enum {
    SCANLED_COM1 = 0,
    SCANLED_COM2,
    SCANLED_COM3,
} scanLED_COM_index_e;

typedef enum {
    SCANLED_ONE1 = 0,
    SCANLED_ONE2,
    SCANLED_ONE3,
    SCANLED_ONE4,
} scanLED_ONE_index_e;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} scanLED_pin_map_stu_t;

struct scanLED_stu {
    const uint8_t num_com;
    const uint8_t num_one;

    uint8_t now_com;

    uint8_t *buffer;

    const scanLED_pin_map_stu_t *scan_com_map;
    const scanLED_pin_map_stu_t *scan_one_map;
    
    void (*write_pin)(struct scanLED_stu *led, scanLED_pin_map_stu_t pin, uint8_t pin_state);
};

void scanLED_write_xy(struct scanLED_stu *led, uint8_t x, uint8_t y, uint8_t state);
void scanLED_scan(struct scanLED_stu *led);

void myScanLED_write_pin(struct scanLED_stu *led, scanLED_pin_map_stu_t *pin, uint8_t pin_state);

#endif // __SCAN_LED_H__

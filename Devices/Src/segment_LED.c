#include "segment_LED.h"

const uint8_t seg_LED_num[16] = {
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_F, // 0
    SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_C, // 1
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_G, // 2
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_G, // 3
    SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_F | SEG_LED_SEGMENT_BITMASK_G, // 4
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_F | SEG_LED_SEGMENT_BITMASK_G, // 5
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_F | SEG_LED_SEGMENT_BITMASK_G, // 6
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_C, // 7
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_F | SEG_LED_SEGMENT_BITMASK_G, // 8
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_F | SEG_LED_SEGMENT_BITMASK_G, // 9
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_F | SEG_LED_SEGMENT_BITMASK_G, // A
    SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_F | SEG_LED_SEGMENT_BITMASK_G, // B
    SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_G, // C
    SEG_LED_SEGMENT_BITMASK_B | SEG_LED_SEGMENT_BITMASK_C | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_G, // D
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_D | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_F | SEG_LED_SEGMENT_BITMASK_G, // E
    SEG_LED_SEGMENT_BITMASK_A | SEG_LED_SEGMENT_BITMASK_E | SEG_LED_SEGMENT_BITMASK_F | SEG_LED_SEGMENT_BITMASK_G, // F
};

const SegmentLED_pin_map_stu_t mySegLED_pin_map[8] = {
    [SEG_LED_SEGMENT_A] = {
        .port = GPIOB,
        .pin = GPIO_PIN_0,
    },
    [SEG_LED_SEGMENT_B] = {
        .port = GPIOA,
        .pin = GPIO_PIN_0,
    },
    [SEG_LED_SEGMENT_C] = {
        .port = GPIOA,
        .pin = GPIO_PIN_1,
    },
    [SEG_LED_SEGMENT_D] = {
        .port = GPIOB,
        .pin = GPIO_PIN_3,
    },
    [SEG_LED_SEGMENT_E] = {
        .port = GPIOB,
        .pin = GPIO_PIN_4,
    },
    [SEG_LED_SEGMENT_F] = {
        .port = GPIOB,
        .pin = GPIO_PIN_1,
    },
    [SEG_LED_SEGMENT_G] = {
        .port = GPIOB,
        .pin = GPIO_PIN_2,
    },
    [SEG_LED_SEGMENT_DP] = {
        .port = GPIOA,
        .pin = GPIO_PIN_3,
    },
};


int SEG_LED_init(struct SegmentLED_stu *led){
    if(led->pin_init == NULL){
        return -1;
    }

    if(led->segment_pin_map == NULL){
        return -2;
    }

    led->pin_init(led);
    SEG_LED_clean_num(led);
    SEG_LED_show_point(led, 0);
	
	return 0;
}



void SEG_LED_show_num(struct SegmentLED_stu *led, SEG_LED_NUM_INDEX_t numx){
    if(numx < 0x0 || numx > 0xF){
        return; // 无效数字
    }

    for(uint8_t i = 0; i < 7; i++){
        if(seg_LED_num[numx] & (1 << i)){
            led->write_seg_pin(led, i, 0);
        } else {
            led->write_seg_pin(led, i, 1);
        }
    }
    
}

void SEG_LED_clean_num(struct SegmentLED_stu *led){

    for(uint8_t i = 0; i < 7; i++){
        led->write_seg_pin(led, i, 1);
    }
}

void SEG_LED_show_point(struct SegmentLED_stu *led, uint8_t point){

    // 点亮小数点
    led->write_seg_pin(led, SEG_LED_SEGMENT_DP, point ? 0 : 1);
}



void mySegLED_pin_init(struct SegmentLED_stu *led){

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 初始化引脚
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 设置引脚模式和输出类型
    
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3, GPIO_PIN_SET);
    
}

void mySegLED_write_seg_pin(struct SegmentLED_stu *led, SEG_LED_SEGMENT_INDEX_t seg, uint8_t pin_state){
    if(seg >= 8){
        return; // 无效段
    }

    const SegmentLED_pin_map_stu_t *map = (led->segment_pin_map + seg);
    
    if(pin_state){
        HAL_GPIO_WritePin(map->port, map->pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(map->port, map->pin, GPIO_PIN_RESET);
    }
}

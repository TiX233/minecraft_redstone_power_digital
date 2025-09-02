#ifndef __SEGMENT_LED_H__
#define __SEGMENT_LED_H__

#include "main.h"

typedef enum {
    SEG_LED_SEGMENT_BITMASK_A = 0x01,
    SEG_LED_SEGMENT_BITMASK_B = 0x02,
    SEG_LED_SEGMENT_BITMASK_C = 0x04,
    SEG_LED_SEGMENT_BITMASK_D = 0x08,
    SEG_LED_SEGMENT_BITMASK_E = 0x10,
    SEG_LED_SEGMENT_BITMASK_F = 0x20,
    SEG_LED_SEGMENT_BITMASK_G = 0x40,
    SEG_LED_SEGMENT_BITMASK_DP = 0x80,

} SEG_LED_SEGMENT_BITMASK_t;

typedef enum {
    SEG_LED_SEGMENT_A = 0,
    SEG_LED_SEGMENT_B,
    SEG_LED_SEGMENT_C,
    SEG_LED_SEGMENT_D,
    SEG_LED_SEGMENT_E,
    SEG_LED_SEGMENT_F,
    SEG_LED_SEGMENT_G,
    SEG_LED_SEGMENT_DP,

} SEG_LED_SEGMENT_INDEX_t;

typedef enum {
    SEG_LED_NUM_0 = 0x00,   // 0
    SEG_LED_NUM_1,          // 1
    SEG_LED_NUM_2,          // 2
    SEG_LED_NUM_3,          // 3
    SEG_LED_NUM_4,          // 4
    SEG_LED_NUM_5,          // 5
    SEG_LED_NUM_6,          // 6
    SEG_LED_NUM_7,          // 7
    SEG_LED_NUM_8,          // 8
    SEG_LED_NUM_9,          // 9
    SEG_LED_NUM_A,          // A
    SEG_LED_NUM_B,          // B
    SEG_LED_NUM_C,          // C
    SEG_LED_NUM_D,          // D
    SEG_LED_NUM_E,          // E
    SEG_LED_NUM_F,          // F
} SEG_LED_NUM_INDEX_t;


typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} SegmentLED_pin_map_stu_t;

struct SegmentLED_stu {

    const SegmentLED_pin_map_stu_t *segment_pin_map;

    void (*pin_init)(struct SegmentLED_stu *led);
    void (*write_seg_pin)(struct SegmentLED_stu *led, SEG_LED_SEGMENT_INDEX_t seg, uint8_t pin_state);
};

int SEG_LED_init(struct SegmentLED_stu *led);
void SEG_LED_show_num(struct SegmentLED_stu *led, SEG_LED_NUM_INDEX_t numx);
void SEG_LED_clean_num(struct SegmentLED_stu *led);
void SEG_LED_show_point(struct SegmentLED_stu *led, uint8_t point);

extern const SegmentLED_pin_map_stu_t mySegLED_pin_map[8];
void mySegLED_pin_init(struct SegmentLED_stu *led);
void mySegLED_write_seg_pin(struct SegmentLED_stu *led, SEG_LED_SEGMENT_INDEX_t seg, uint8_t pin_state);

#endif // __SEGMENT_LED_H__

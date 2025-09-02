/*
md 和 keil 的 rtx 重名了，不管了
*/
/**
 * @file rtx.h
 * @author realTiX
 * @brief 简易的前后台调度器
 * @version 0.2
 * @date 2025-08-15 (0.1)
 *       2025-08-18 (0.2, 修复在 remove 或 unsubscribe 时没有成员的话会访问到空指针的 bug)
 *       2025-09-02 (0.3, 修复 alarm 会多延时一个 tick 的 bug，移除记录闹钟超时时间的功能)
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __RTX_H__
#define __RTX_H__

#include "main.h"

// 好像不能直接用 linux 里的，从 rtthread 里偷一个
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


typedef uint32_t TickType_t;

struct rtx_Topic_subscriber_stu {
    // uint8_t flag;

    void (*callback_func)(void *param);

    struct rtx_Topic_subscriber_stu *next;
};

struct rtx_Topic_stu {
    uint8_t flag;
    struct rtx_Topic_subscriber_stu *subscriber;

    struct rtx_Topic_stu *next;
};

struct rtx_Timer_stu {
    
    TickType_t tick_counts; // tick counts 会在每个 tick 减一，当减为 0 时，会赋值为 tick reload，并且发布 Topic
    TickType_t tick_reload; // 重载值/周期，不可赋值为 0，只有数组的最后一个元素为 0，表示数组的结尾

    struct rtx_Topic_stu *topic; // 定时器触发后会给所有订阅者发送通知

    struct rtx_Timer_stu *next;
};

struct rtx_Alarm_stu {
    uint8_t flag; // 响铃标志位
    TickType_t tick_count_down; // 闹钟倒计时，变为零时 flag ++，以通知前台调用回调函数

    void (*callback_alarm)(void *param); // 闹钟到时间后会调用相应回调函数
    
    struct rtx_Alarm_stu *next;
};

void rtx_Timer_add(struct rtx_Timer_stu *timer);
void rtx_Timer_remove(struct rtx_Timer_stu *timer);

void rtx_Alarm_add(struct rtx_Alarm_stu *alarm);
void rtx_Alarm_remove(struct rtx_Alarm_stu *alarm);
void rtx_Alarm_set_count(struct rtx_Alarm_stu *alarm, TickType_t tick_count_down);

void rtx_Topic_add(struct rtx_Topic_stu *topic);
void rtx_Topic_remove(struct rtx_Topic_stu *topic);
void rtx_Topic_subscribe(struct rtx_Topic_stu *topic, struct rtx_Topic_subscriber_stu *subscriber);
void rtx_Topic_unsubscribe(struct rtx_Topic_stu *topic, struct rtx_Topic_subscriber_stu *subscriber);
void rtx_Topic_publish(struct rtx_Topic_stu *topic);

void rtx_Sys_tick_tack(void);
TickType_t rtx_Sys_get_tick(void);

void rtx_Sys_scheduler(void);
void rtx_Sys_schedule_start(void);
void rtx_Sys_schedule_stop(void);

#endif // __RTX_H__

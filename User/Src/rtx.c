#include "rtx.h"

volatile TickType_t realTicks; // 系统时间，溢出处理 todo
uint8_t flag_schedule = 0; // 调度开关标志位

struct rtx_Topic_stu rtx_sys_topic_list = {
    .subscriber = NULL,

    .next = NULL,
};

struct rtx_Timer_stu rtx_sys_timer_list = {
    .next = NULL,
};

struct rtx_Alarm_stu rtx_sys_alarm_list = {
    .next = NULL,
};

// 定时器
void rtx_Timer_add(struct rtx_Timer_stu *timer){
    struct rtx_Timer_stu *pTimer = rtx_sys_timer_list.next;

    if(timer->next != NULL){ // 已经存在，不重复加入
        return ;
    }

    if(rtx_sys_timer_list.next == NULL){
        rtx_sys_timer_list.next = timer;
        timer->next = NULL;

        return ;
    }

    while(pTimer->next != NULL){
        pTimer = pTimer->next;
    }

    pTimer->next = timer;
    timer->next = NULL;
}

void rtx_Timer_remove(struct rtx_Timer_stu *timer){
    struct rtx_Timer_stu *pTimer = rtx_sys_timer_list.next;

    if(pTimer == NULL){ // 定时器不在活跃列表中，不做操作
        return ;
    }

    if(pTimer == timer){
        rtx_sys_timer_list.next = timer->next;
        timer->next = NULL;

        return ;
    }

    while(timer->next != timer && pTimer->next != NULL){
        pTimer = pTimer->next;
    }

    if(pTimer->next == timer){
        pTimer->next = timer->next;
        timer->next = NULL;
    }

}

// 闹钟
void rtx_Alarm_add(struct rtx_Alarm_stu *alarm){
    struct rtx_Alarm_stu *pAlarm = rtx_sys_alarm_list.next;

    if(alarm->next != NULL){ // 已经存在，不重复加入
        return ;
    }
    
    if(rtx_sys_alarm_list.next == NULL){
        rtx_sys_alarm_list.next = alarm;
        alarm->next = NULL;

        return ;
    }

    while(pAlarm->next != NULL){
        pAlarm = pAlarm->next;
    }

    pAlarm->next = alarm;
    alarm->next = NULL;
}

void rtx_Alarm_remove(struct rtx_Alarm_stu *alarm){
    struct rtx_Alarm_stu *pAlarm = rtx_sys_alarm_list.next;

    if(pAlarm == NULL){ // 闹钟不在活跃列表中，不做操作
        return ;
    }

    if(pAlarm == alarm){
        rtx_sys_alarm_list.next = alarm->next;
        alarm->next = NULL;
        alarm->flag = 0;

        return ;
    }

    while(pAlarm->next != alarm && pAlarm->next != NULL){
        pAlarm = pAlarm->next;
    }

    if(pAlarm->next == alarm){
        pAlarm->next = alarm->next;
        alarm->next = NULL;
        alarm->flag = 0;
    }

}

void rtx_Alarm_set_count(struct rtx_Alarm_stu *alarm, TickType_t tick_count_down){
    alarm->tick_count_down = tick_count_down;
}

// 话题，感觉可以用字符串去匹配，就不用全局变量满天飞了
void rtx_Topic_add(struct rtx_Topic_stu *topic){
    struct rtx_Topic_stu *pTopic = rtx_sys_topic_list.next;

    if(topic->next != NULL){ // 已经存在，不重复加入
        return ;
    }

    if(rtx_sys_topic_list.next == NULL){
        rtx_sys_topic_list.next = topic;
        topic->next = NULL;

        return ;
    }

    while(pTopic->next != NULL){
        pTopic = pTopic->next;
    }

    pTopic->next = topic;
    topic->next = NULL;
}

void rtx_Topic_remove(struct rtx_Topic_stu *topic){
    struct rtx_Topic_stu *pTopic = rtx_sys_topic_list.next;

    if(pTopic == NULL){ // 话题没有在活动列表中，不做操作
        return ;
    }

    if(pTopic == topic){
        rtx_sys_topic_list.next = topic->next;
        topic->next = NULL;

        return ;
    }

    while(pTopic->next != topic && pTopic->next != NULL){
        pTopic = pTopic->next;
    }

    if(pTopic->next == topic){
        pTopic->next = topic->next;
        topic->next = NULL;
    }

}

void rtx_Topic_subscribe(struct rtx_Topic_stu *topic, struct rtx_Topic_subscriber_stu *subscriber){
    struct rtx_Topic_subscriber_stu *pSub = topic->subscriber;

    if(subscriber->next != NULL){ // 已经存在，不重复添加
        // 但是不添加额外的成员变量的话，只能遍历所有话题才能避免一个订阅者订阅多个话题，先不管
        return ;
    }
    
    if(pSub == NULL){
        topic->subscriber = subscriber;
        subscriber->next = NULL;

        return ;
    }

    while(pSub->next != NULL){
        pSub = pSub->next;
    }

    pSub->next = subscriber;
    subscriber->next = NULL;
}

void rtx_Topic_unsubscribe(struct rtx_Topic_stu *topic, struct rtx_Topic_subscriber_stu *subscriber){
    struct rtx_Topic_subscriber_stu *pSub = topic->subscriber;
    
    if(pSub == NULL){ // 话题没有订阅者，不做操作
        return ;
    }

    if(pSub == subscriber){
        topic->subscriber = subscriber->next;
        subscriber->next = NULL;
        
        return ;
    }
    
    while(pSub->next != subscriber && pSub->next != NULL){
        pSub = pSub->next;
    }
    
    if(pSub->next == subscriber){
        pSub->next = subscriber->next;
        subscriber->next = NULL;
    }
}

void rtx_Topic_publish(struct rtx_Topic_stu *topic){
    topic->flag ++;

    /*
    struct rtx_Topic_subscriber_stu *pSub = topic->subscriber;
    while(pSub != NULL){
        pSub->flag ++;
        pSub = pSub->next;
    }
    */
}

// 系统调度相关
void rtx_Sys_tick_tack(void){
    realTicks ++;
    if(!flag_schedule){
        return ;
    }

    struct rtx_Alarm_stu *pAlarm = rtx_sys_alarm_list.next;
    while(pAlarm != NULL){
        if(0 == --pAlarm->tick_count_down){
            pAlarm->flag ++;
        }
        pAlarm = pAlarm->next;
    }

    struct rtx_Timer_stu *pTimer = rtx_sys_timer_list.next;
    while(pTimer != NULL){
        if(0 == -- pTimer->tick_counts){
            pTimer->tick_counts = pTimer->tick_reload;
            rtx_Topic_publish(pTimer->topic);
        }
        pTimer = pTimer->next;
    }
}

TickType_t rtx_Sys_get_tick(void){
    return realTicks;
}

void rtx_Sys_scheduler(void){
    struct rtx_Topic_stu *pTopic;
    struct rtx_Topic_subscriber_stu *pSubscriber;
    struct rtx_Alarm_stu *pAlarm;

    while(1){
        // 处理订阅
        pTopic = rtx_sys_topic_list.next;
        while(pTopic != NULL){
            /*
            pSubscriber = pTopic->subscriber;
            while(pSubscriber != NULL){
                if(pSubscriber->flag){
                    pSubscriber->callback_func(NULL);
                    pSubscriber->flag = 0;
                }

                pSubscriber = pSubscriber->next;
            }
            */
            if(pTopic->flag){
                pSubscriber = pTopic->subscriber;
                while(pSubscriber != NULL){
                    pSubscriber->callback_func(pSubscriber);

                    pSubscriber = pSubscriber->next;
                }

                pTopic->flag = 0;
            }

            pTopic = pTopic->next;
        }

        // 处理闹钟
        pAlarm = rtx_sys_alarm_list.next;
        while(pAlarm != NULL){
            if(pAlarm->flag){
                rtx_Alarm_remove(pAlarm); // 必须先移除闹钟再调用回调，不然无法在回调中创建调用自己的闹钟
                pAlarm->callback_alarm(pAlarm);
            }

            pAlarm = pAlarm->next;
        }
    }
}

void rtx_Sys_schedule_start(void){
    flag_schedule = 1;
}

void rtx_Sys_schedule_stop(void){
    flag_schedule = 0;
}

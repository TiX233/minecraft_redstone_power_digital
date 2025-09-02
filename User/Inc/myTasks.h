#ifndef __MY_TASKS_H__
#define __MY_TASKS_H__

#include "main.h"
#include "rtx.h"

// 周期任务结构体
struct mt_Task_stu {
    struct rtx_Topic_subscriber_stu subscriber;
    struct rtx_Topic_stu topic;
    struct rtx_Timer_stu timer;

    void *pData;
};


extern struct mt_Task_stu task_heart_beat;
extern struct mt_Task_stu task_redstone_clock;


int mt_Task_add(struct mt_Task_stu *task);

int myTask_init(void);


#endif // __MY_TASKS_H__

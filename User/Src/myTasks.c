#include "myTasks.h"
#include "rtx.h"
#include "cmdProcess.h"

void task_func_heart_beat(void *param);
void task_func_cmd(void *param);
void task_func_redstone_clock(void *param);


// 心拍周期任务对象
struct mt_Task_stu task_heart_beat = {
    .topic = {
        .flag = 0,
        .subscriber = NULL,

        .next = NULL,
    },

    .timer = {
        .tick_counts = 1000,
        .tick_reload = 1000,

        .topic = &(task_heart_beat.topic),
        .next = NULL,
    },

    .subscriber = {
        .callback_func = task_func_heart_beat,

        .next = NULL,
    },
};


// 命令处理周期任务对象
struct mt_Task_stu task_cmd = {
    .topic = {
        .flag = 0,
        .subscriber = NULL,

        .next = NULL,
    },

    .timer = {
        .tick_counts = 200,
        .tick_reload = 200,

        .topic = &(task_cmd.topic),
        .next = NULL,
    },

    .subscriber = {
        .callback_func = task_func_cmd,

        .next = NULL,
    },
};


// 红石时钟周期任务对象
struct mt_Task_stu task_redstone_clock = {
    .topic = {
        .flag = 0,
        .subscriber = NULL,

        .next = NULL,
    },

    .timer = {
        .tick_counts = 50,
        .tick_reload = 50,

        .topic = &(task_redstone_clock.topic),
        .next = NULL,
    },

    .subscriber = {
        .callback_func = task_func_redstone_clock,

        .next = NULL,
    },
};


// 添加周期任务 API
int mt_Task_add(struct mt_Task_stu *task){
    rtx_Topic_add(&(task->topic));
    rtx_Topic_subscribe(&(task->topic), &(task->subscriber));
    rtx_Timer_add(&(task->timer));

    return 0;
}

// -----------------初始化自定义任务----------------
int myTask_init(void){
    // 创建周期心拍任务
    mt_Task_add(&task_heart_beat);

    // 创建命令处理周期任务
    mt_Task_add(&task_cmd);

    // 创建红石时钟周期任务
    mt_Task_add(&task_redstone_clock);


    return 0;
}


// 心拍任务
uint32_t heart_beat_count = 0;
void task_func_heart_beat(void *param){

    heart_beat_count ++;
    // LOG_FMT("Heartbeat: %d\n", heart_beat_count);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}


uint8_t cmd_buffer[CMD_BUF_SIZE];
// 处理命令任务
void task_func_cmd(void *param){

    // 读取命令
    if(SEGGER_RTT_HasData(0)){
        int len = SEGGER_RTT_Read(0, cmd_buffer, CMD_BUF_SIZE - 1);
        if (len > 0) {
            cmd_buffer[len] = '\0'; // 添加字符串终止符

            // LOG_FMT(PRINT_DEBUG"cmd len: %d\n", len);
            cmd_process((char *)cmd_buffer); // 处理命令
        }
    }
}


uint8_t rs_power_clk_get(void){
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
}

void rs_power_clk_pullup(void *param){
    if(0){ // 如果期间没有产生下降沿中断，说明有其他供电设备
        // 不拉高 clk，等待其松开clk，而后同步
        // todo

        return ;
    }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
}

void rs_power_clk_pulldown(void *param){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
}

struct rtx_Alarm_stu alarm_clock_release = {
    .flag = 0,
    .tick_count_down = 1,

    .callback_alarm = rs_power_clk_pullup,

    .next = NULL,
};

// 红石时钟任务
TickType_t rs_clk_pluse = 1;
void task_func_redstone_clock(void *param){

    // 断开供电
    rs_power_clk_pulldown(NULL);

    // 脉宽时间后恢复供电
    rtx_Alarm_set_count(&alarm_clock_release, rs_clk_pluse);
    rtx_Alarm_add(&alarm_clock_release);
}

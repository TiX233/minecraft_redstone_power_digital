#include "paramProcess.h"
#include "myTasks.h"

// 读写心拍相关内容
uint16_t heart_beat_Hz = 1;
void param_read_heart_beat(struct param_stu *param){
    LOG_FMT(PRINT_LOG"Heart beat: %d Hz\n", heart_beat_Hz);
}
void param_write_heart_beat(struct param_stu *param, const char *new_val){
    uint16_t new_Hz;

    sscanf(new_val, "%d", &new_Hz);

    if(new_Hz > 1000 || new_Hz < 1){
        LOG_FMT(PRINT_WARNNING"New value(%d) not in range(1~1000)!\n", new_Hz);
        return ;
    }

    heart_beat_Hz = 1000/new_Hz;

    task_heart_beat.timer.tick_reload = heart_beat_Hz;
    task_heart_beat.timer.tick_counts = heart_beat_Hz;

    LOG_FMT(PRINT_LOG"Set heart beat to %d Hz\n", new_Hz);
}

// 读写红石时钟信号周期相关内容
void param_read_rs_clk_period(struct param_stu *param){
    LOG_FMT(PRINT_LOG"Clock period: %d ms\n", task_redstone_clock.timer.tick_reload);
}
void param_write_rs_clk_period(struct param_stu *param, const char *new_val){
    TickType_t new_period;

    sscanf(new_val, "%d", &new_period);

    if(new_period < 1){
        LOG_STR(PRINT_WARNNING"Period must >= 1ms\n");

        return ;
    }

    task_redstone_clock.timer.tick_reload = new_period;
    task_redstone_clock.timer.tick_counts = new_period;

    LOG_FMT(PRINT_LOG"Set redstond clock period to %d ms\n", new_period);
}

// 读写红石时钟信号拉低时间相关内容
extern TickType_t rs_clk_pluse;
void param_read_rs_clk_pluse(struct param_stu *param){
    LOG_FMT(PRINT_LOG"Clock period: %d ms\n", rs_clk_pluse);
}
void param_write_rs_clk_pluse(struct param_stu *param, const char *new_val){
    TickType_t new_tick;

    sscanf(new_val, "%d", &new_tick);

    if(new_tick < 1){
        LOG_STR(PRINT_WARNNING"Pulldown tick must >= 1ms\n");

        return ;
    }

    LOG_FMT(PRINT_LOG"Set redstond clock pulldown time to %d ms\n", new_tick);
    rs_clk_pluse = new_tick;
}


struct param_stu param_list[] = {
    {
        .param_name = "heart_beat_Hz",
        .param_read = param_read_heart_beat,
        .param_write = param_write_heart_beat,
    },

    {
        .param_name = "rs_clk_period",
        .param_read = param_read_rs_clk_period,
        .param_write = param_write_rs_clk_period,
    },

    {
        .param_name = "rs_clk_pluse",
        .param_read = param_read_rs_clk_pluse,
        .param_write = param_write_rs_clk_pluse,
    },

    // 末尾项
    {
        .param_name = " ",
    },
};

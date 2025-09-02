#include "cmdProcess.h"
#include <stdio.h>
#include "paramProcess.h"
#include "main.h"
#include "rtx.h"
#include "myTasks.h"
#include "ch224.h"

typedef struct{
    const char *cmdName;
    const char *brief;
    void (*pCmdCallback)(uint8_t argc, char *argv[]);
}cmdItem;

void cmd_cb_echo(uint8_t argc, char *argv[]);
void cmd_cb_help(uint8_t argc, char *argv[]);
void cmd_cb_hello(uint8_t argc, char *argv[]);
void cmd_cb_print(uint8_t argc, char *argv[]);
void cmd_cb_alarm(uint8_t argc, char *argv[]);
void cmd_cb_param(uint8_t argc, char *argv[]);
void cmd_cb_protocol(uint8_t argc, char *argv[]);

cmdItem cmdList[] = {
    {
        .cmdName = "echo",
        .brief = "return 2nd param to test uart",
        .pCmdCallback = cmd_cb_echo,
    },
    {
        .cmdName = "hello",
        .brief = "hello",
        .pCmdCallback = cmd_cb_hello,
    },
    {
        .cmdName = "help",
        .brief = "print brief of commands",
        .pCmdCallback = cmd_cb_help,
    },

    {
        .cmdName = "print",
        .brief = "track a data item and print it whenever it is updated",
        .pCmdCallback = cmd_cb_print,
    },

    {
        .cmdName = "param",
        .brief = "read or write some param",
        .pCmdCallback = cmd_cb_param,
    },

    {
        .cmdName = "alarm",
        .brief = "set an alarm for test",
        .pCmdCallback = cmd_cb_alarm,
    },

    {
        .cmdName = "protocol",
        .brief = "get ch224 charge protocol now",
        .pCmdCallback = cmd_cb_protocol,
    },


    // end of list:
    {
        .cmdName = " ",
        .pCmdCallback = NULL,
    },
};

int my_str_cmp(const char *str1, char *str2){
    uint32_t i;
    for(i = 0; i < CMD_BUF_SIZE - 1; i++){
        if(str1[i] == '\0' || str1[i] == ' '){
            if(str2[i] == '\0' || str2[i] == ' ' || str2[i] == '\n'){
                str2[i] = '\0';
                return 0;
            }else {
                return -1;
            }
        }
        if(str1[i] != str2[i]){
            return -1;
        }
    }

    return 0;
}


void cmd_cb_echo(uint8_t argc, char *argv[]){
    if(argc > 1){
        if(my_str_cmp("-h", argv[1]) == 0){
            goto Useage_echo;
        }
        LOG_FMT(PRINT_LOG"%s\n", argv[1]);
    }else {
        LOG_FMT(PRINT_WARNNING"Need param to echo!\n");
    }
    return ;
Useage_echo:
    LOG_FMT(PRINT_LOG"Useage: This function will return the first param to test uart\n");
}

void cmd_cb_hello(uint8_t argc, char *argv[]){
    LOG_FMT(PRINT_LOG"Ciallo World~ (∠・ω< )⌒☆\n");
}

void cmd_cb_help(uint8_t argc, char *argv[]){
    uint8_t i;
    
    if(argc > 1){
        for(i = 0; cmdList[i].cmdName[0] != ' '; i ++){
            if(my_str_cmp(cmdList[i].cmdName, argv[1]) == 0){
                LOG_FMT(PRINT_LOG"%s   -   %s\n", cmdList[i].cmdName, cmdList[i].brief);
                return;
            }
        }
    }else {
        for(i = 0; cmdList[i].cmdName[0] != ' '; i ++){
            LOG_FMT(PRINT_LOG"%s:\n\t%s\n", cmdList[i].cmdName, cmdList[i].brief);
        }

        return ;
    }

    LOG_FMT(PRINT_WARNNING"Unknown cmd: %s, Type /help to list all commands\n", argv[1]);
}


void alarm_cb_test_alarm(void *param){
    LOG_FMT(PRINT_LOG"Alarm ring: %d\n", rtx_Sys_get_tick());
}
struct rtx_Alarm_stu alarm_test = {
    .flag = 0,
    
    .tick_count_down = 0,
    .callback_alarm = alarm_cb_test_alarm,

    .next = NULL,
};

void cmd_cb_alarm(uint8_t argc, char *argv[]){
    if(argc != 2){
        goto Useage_alarm;
    }
    uint32_t ticks_alarm;
    sscanf(argv[1], "%d", &ticks_alarm);

    if(ticks_alarm < 1){
        alarm_cb_test_alarm(NULL);
        return ;
    }

    rtx_Alarm_set_count(&alarm_test, ticks_alarm);
    rtx_Alarm_add(&alarm_test);

    LOG_FMT(PRINT_LOG"Tick now: %d\n", rtx_Sys_get_tick());
    LOG_FMT(PRINT_LOG"Alarm will ring after %d ticks(%d)\n", ticks_alarm, ticks_alarm + rtx_Sys_get_tick());

    return ;
Useage_alarm:
    LOG_FMT(PRINT_LOG"Useage: %s <ticks>\n", argv[0]);
}


// 数据更新追踪打印相关内容
// 心跳计数数据更新打印回调
void print_cb_heart_beat(void *param){
    extern uint32_t heart_beat_count;
    LOG_FMT("Heartbeat: %d\n", heart_beat_count);
}

// 可供打印的数据对象
struct {
    const char *item_name;
    struct rtx_Topic_stu *topic;
    struct rtx_Topic_subscriber_stu subscriber;
} print_data_item_list[] = {
    {
        .item_name = "heart_beat",
        .topic = &(task_heart_beat.topic),
        .subscriber = {
            .callback_func = print_cb_heart_beat,

            .next = NULL,
        },
    },


    // 列表结尾项
    {
        .item_name = " ",
    },
};

// 数据更新打印订阅设置命令
void cmd_cb_print(uint8_t argc, char *argv[]){
    if(argc < 2){
        goto Useage_print;
    }

enum print_option_e{
    PO_START = 0,
    PO_STOP = 1,
    PO_LIST = 2,
};

    const char *print_option_list[] = {
        [PO_START] = "-start",
        [PO_STOP] = "-stop",
        [PO_LIST] = "-list",

        " ",
    };

    uint8_t i;
    for(i = 0; print_option_list[i][0] != ' ' && my_str_cmp(print_option_list[i], argv[1]) != 0; i++);

    switch(i){
        case PO_START:
            if(argc != 3){
                LOG_STR(PRINT_WARNNING"Please enter the data name correctly!\n");

                goto Useage_print;
            }

            // LOG_STR(PRINT_DEBUG"going into PO_START\n");

            for(i = 0; print_data_item_list[i].item_name[0] != ' '; i ++){
                if(my_str_cmp(print_data_item_list[i].item_name, argv[2]) == 0){
                    LOG_FMT(PRINT_LOG"Start track data \"%s\"...\n", argv[2]);

                    rtx_Topic_subscribe(print_data_item_list[i].topic, &(print_data_item_list[i].subscriber));
                    return ;
                }
            }

            LOG_FMT(PRINT_WARNNING"Data name \"%s\" not matched! Use <-list> option to list all data name.\n", argv[2]);

            break;

        case PO_STOP:
            // LOG_STR(PRINT_DEBUG"going into PO_STOP\n");

            for(i = 0; print_data_item_list[i].item_name[0] != ' '; i ++){
                rtx_Topic_unsubscribe(print_data_item_list[i].topic, &(print_data_item_list[i].subscriber));
            }
            LOG_STR(PRINT_LOG"All data tracking has been stopped.\n");

            break;

        case PO_LIST:
            // LOG_STR(PRINT_DEBUG"going into PO_LIST\n");

            LOG_STR(PRINT_LOG"All data item name:\n");
            for(i = 0; print_data_item_list[i].item_name[0] != ' '; i ++){
                LOG_FMT("\t%s\n", print_data_item_list[i].item_name);
            }

            break;

        default:
            LOG_FMT(PRINT_WARNNING"Unknown option: %s\n", argv[1]);

            goto Useage_print;
            break;
    }

    return ;

Useage_print:
    LOG_FMT(PRINT_LOG"Useage: %s <-start/-stop/-list> [data_name]\n", argv[0]);
}


// 读写某些参数的命令
void cmd_cb_param(uint8_t argc, char *argv[]){
    if(argc < 2){
        goto Useage_param;
    }

    switch(argv[1][1]){
        case 'r':
            if(argc < 3){
                LOG_STR(PRINT_WARNNING"Please enter param name! You can use -l option to list all param.\n");
                return ;
            }

            for(uint8_t i = 0; param_list[i].param_name[0] != ' '; i ++){
                if(my_str_cmp(param_list[i].param_name, argv[2]) == 0){
                    param_list[i].param_read(&param_list[i]);

                    return ;
                }
            }

            LOG_FMT(PRINT_WARNNING"Param '%s' was not found, use -l to list all param\n", argv[2]);

            break;

        case 'w':
            if(argc < 4){
                LOG_STR(PRINT_WARNNING"Please enter param name and new value! You can use -l option to list all param.\n");
                return ;
            }

            for(uint8_t i = 0; param_list[i].param_name[0] != ' '; i ++){
                if(my_str_cmp(param_list[i].param_name, argv[2]) == 0){
                    param_list[i].param_write(&param_list[i], argv[3]);

                    return ;
                }
            }

            LOG_FMT(PRINT_WARNNING"Param '%s' was not found, use -l to list all param\n", argv[2]);

            break;

        case 'l':
            LOG_STR(PRINT_LOG"All rw-able parameter:\n");
            for(uint8_t i = 0; param_list[i].param_name[0] != ' '; i ++){
                LOG_FMT(PRINT_LOG"\t%s\n", param_list[i].param_name);
            }

            break;

        default:
            LOG_FMT(PRINT_WARNNING"Unknown option: %s\n", argv[1]);
            goto Useage_param;
            break;
    }

    return ;
    
Useage_param:
    LOG_FMT(PRINT_LOG"Useage: %s <-r/-s/-l> [<param_name> [new_value]]\n", argv[0]);
    LOG_STR(PRINT_LOG"\t-r: read, -w: write, -l: list\n");
}


const char *protocol_list[] = {
    
    [CH224_PROTOCOL_BC] = "BC",
    [CH224_PROTOCOL_QC2] = "QC2",
    [CH224_PROTOCOL_QC3] = "QC3",
    [CH224_PROTOCOL_PD] = "PD",
    [CH224_PROTOCOL_EPR] = "EPR",

    [CH224_PROTOCOL_MAX_NUM] = "NONE",
};

// 获取当前快充协议的命令
void cmd_cb_protocol(uint8_t argc, char *argv[]){
    if(my_ch224.i2c_addr == 0){
        LOG_STR(PRINT_WARNNING"CH224k do not support i2c!\n");
        
        return ;
    }

    ch224_protocol_e protocol = ch224_get_protocol(&my_ch224);

    LOG_FMT(PRINT_LOG"CH224 charge protocol: %s\n", protocol_list[protocol]);
}

// 处理输入，执行命令
void cmd_process(char *cmd){
    uint32_t i = 0;
    uint8_t index = 1;
    char *argv[CMD_MAX_ARG_COUNTS] = {
        [0] = cmd + 1,
    };

    if(!(cmd[0] == '/' || cmd[0] == '#')){ // 非指令
        LOG_FMT(PRINT_WARNNING"Unknow format!\n");
        // LOG_FMT(PRINT_DEBUG"%s\n", cmd);
        return;
    }

    for(i = 1; (cmd[i] != '\0') && (i < CMD_BUF_SIZE - 1) && (index < CMD_MAX_ARG_COUNTS - 1); i ++){
        if((cmd[i-1] == ' ') && (cmd[i] != ' ')){
            argv[index] = &cmd[i];
            index ++;
            cmd[i-1] = '\0';
        }
    }

    for(i = 0; cmdList[i].cmdName[0] != ' '; i ++){
        if(my_str_cmp(cmdList[i].cmdName, argv[0]) == 0){
            if(cmdList[i].pCmdCallback != NULL){
                argv[0] = cmd;
                cmdList[i].pCmdCallback(index, argv);
            }else {
                LOG_FMT(PRINT_ERROR"Callback function of \"%s\" is not define!\n", argv[0]);
            }
            return;
        }
    }

    LOG_FMT(PRINT_WARNNING"Unknown cmd: %s\n", argv[0]);
    LOG_FMT(PRINT_LOG"Type /help to list all commands\n");
}

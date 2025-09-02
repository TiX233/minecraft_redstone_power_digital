/**
 * @file paramProcess.h
 * @author realTiX
 * @brief 用于读写一些预设的系统参数，由 cmdProcess.c 中的 param 命令调用
 * @version 0.1
 * @date 2025-09-1
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __PARAM_PROCESS_H__
#define __PARAM_PROCESS_H__

struct param_stu {
    const char *param_name;
    
    void (*param_read)(struct param_stu *param);
    void (*param_write)(struct param_stu *param, const char *new_val);
};

extern struct param_stu param_list[];

#endif // __PARAM_PROCESS_H__

/**
 * @file cmdProcess.h
 * @author realTiX
 * @brief 对上位机发来的命令进行处理用
 * @version 0.2
 * @date 2025-08-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __CMD_PROCESS_H_
#define __CMD_PROCESS_H_

#define CMD_MAX_ARG_COUNTS      10
#define CMD_BUF_SIZE            128

void cmd_process(char *cmd);

#endif

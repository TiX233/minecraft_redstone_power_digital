/**
  ******************************************************************************
  * @file    main.h
  * @author  MCU Application Team
  * @brief   Header for main.c file.
  *          This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by Puya under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
#include "SEGGER_RTT.h"
#include "rtx.h"

/* Private defines -----------------------------------------------------------*/

#define PRINT_LOG           "# "
#define PRINT_WARNNING      "W "
#define PRINT_ERROR         "E "
#define PRINT_DEBUG         "D "

#define LOG_STR(str)        SEGGER_RTT_WriteString(0, str)
#define LOG_FMT(fmt, ...)   SEGGER_RTT_printf(0, fmt, ##__VA_ARGS__)

/* Exported variables prototypes ---------------------------------------------*/

extern I2C_HandleTypeDef               peripheral_handler_I2C;

extern struct ch224_stu my_ch224;

/* Exported functions prototypes ---------------------------------------------*/
void APP_ErrorHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/

/*
 * Author       : chenzutao
 * Date         : 2015-08-18
 * Function     : record logs
 */

#pragma once

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define _LOG_DEBUG_LEVEL         0
#define _LOG_INFO_LEVEL          1
#define _LOG_NOTICE_LEVEL        2
#define _LOG_WARNING_LEVEL       3
#define _LOG_ERROR_LEVEL         4
#define _LOG_CRIT_LEVEL          5
#define _LOG_ALERT_LEVEL         6
#define _LOG_EMERG_LEVEL         7

#define  __S      _LOG_EMERG_LEVEL
#define  __E      _LOG_ERROR_LEVEL
#define  __W      _LOG_WARNING_LEVEL
#define  __N      _LOG_NOTICE_LEVEL
#define  __I      _LOG_INFO_LEVEL
#define  __D      _LOG_DEBUG_LEVEL

#define _LOG_S_VALUE     "[SYS    ]"
#define _LOG_E_VALUE     "[ERROR  ]"
#define _LOG_W_VALUE     "[WARNING]"
#define _LOG_N_VALUE     "[NOTICE ]"
#define _LOG_I_VALUE     "[INFO   ]"
#define _LOG_D_VALUE     "[DEBUG  ]"

#define _MAX_FILE_PATH_SIZE      64
#define _MAX_HEAD_BUF_SIZE       128
#define _MAX_MSG_BUG_SIZE        2048

#define _LN __LINE__
#define _FN __FUNCTION__
#define _FL __FILE__

#define _LINE __LINE__
#define _FUNC __FUNCTION__
#define _FILE __FILE__

#define PATH _FL, _FN, _LN

#define _COLOR_NONE            "\x1B[m"
#define _COLOR_GRAY            "\x1B[0;30m"
#define _COLOR_LIGHT_GRAY      "\x1B[1;30m"
#define _COLOR_RED             "\x1B[0;31m"
#define _COLOR_LIGHT_RED       "\x1B[1;31m"
#define _COLOR_GREEN           "\x1B[0;32m"
#define _COLOR_LIGHT_GREEN     "\x1B[1;32m"
#define _COLOR_YELLOW          "\x1B[0;33m"
#define _COLOR_LIGHT_YELLOW    "\x1B[1;33m"
#define _COLOR_BLUE            "\x1B[0;34m"
#define _COLOR_LIGHT_BLUE      "\x1B[1;34m"
#define _COLOR_PURPLE          "\x1B[0;35m"
#define _COLOR_LIGHT_PURPLE    "\x1B[1;35m"
#define _COLOR_CYAN            "\x1B[0;36m"
#define _COLOR_LIGHT_CYAN      "\x1B[1;36m"
#define _COLOR_WHITE           "\x1B[0;37m"
#define _COLOR_LIGHT_WHITE     "\x1B[1;37m"

#define _LOG_D_COLOR		_COLOR_LIGHT_GREEN
#define _LOG_I_COLOR		_COLOR_LIGHT_GREEN
#define _LOG_N_COLOR		_COLOR_LIGHT_YELLOW
#define _LOG_W_COLOR		_COLOR_LIGHT_YELLOW
#define _LOG_E_COLOR		_COLOR_LIGHT_RED
#define _LOG_S_COLOR		_COLOR_LIGHT_RED


#ifdef OPEN_DEBUG
#define logger(_lv_, _path, _fmt_, ...) fprintf(stdout, _LOG_##_lv_##_COLOR _LOG_##_lv_##_VALUE \
                _COLOR_LIGHT_CYAN "%16s" _COLOR_LIGHT_PURPLE "(%20s)" _COLOR_LIGHT_BLUE "|%4d|-->" \
                _LOG_##_lv_##_COLOR _fmt_ _COLOR_NONE, _path, ##__VA_ARGS__)
#else //not open debug
#define full_log(_lv_,_FL_, _FN_, _LN_, _fmt_, _args...) log_info(__##_lv_, _LOG_##_lv_##_VALUE, _FL_, _FN_, _LN_, _fmt_, ##_args)
#define logger(_lv_, _path, _fmt_, _args...) log_info(__##_lv_, _LOG_##_lv_##_VALUE, _path, _fmt_, ##_args)
#endif //end OPEN_DEBUG define

typedef struct log_file {
        int level;
        int old_fd;
        int new_fd;
        char path[_MAX_FILE_PATH_SIZE];
        char file[_MAX_FILE_PATH_SIZE];
}LOG_FILE;

void init_log(char *path, char *file, int level);

void open_log(void);

void close_old_log(void);

int log_info(int level, char *value, const char *__FILE, const char *__FUNC, int __LINE, const char *fmt, ...);

#endif //end __LOG_H

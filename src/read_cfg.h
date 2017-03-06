/**
 * Author       : chenzutao
 * Date         : 2016-05-16
 * Function     : read configuration from file
 **/

#ifndef _READ_CFG_H_
#define _READ_CFG_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "json.h"
#include "log.h"

typedef struct cfg_file {
    char *redis_host;
    short redis_port;
    char *hdfs_host;
    short hdfs_port;
    char *hdfs_file;
    char *redis_list_key;

    char *log_path;
    char *log_file;
    short log_level;
}cfg_file_t;

void read_cfg(cfg_file_t **cfg_file, char *file_name);

void free_cfg(cfg_file_t *cfg_file);
#endif

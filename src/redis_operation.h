/**
 * Author       : chenzutao
 * Date         : 2016-05-16
 * Function     : redis operation in C language
 **/

#ifndef _REDIS_OPERATION_H_
#define _REDIS_OPERATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hiredis.h"
#include "log.h"

redisContext *redis_connect(const char *host, short port);

int redis_ping(redisContext *conn);

redisReply *redis_lrange(redisContext *conn, char *key, int min_idx, int max_idx);

char *redis_lpop(redisContext *conn, const char *key);

redisReply *redis_hgetall(redisContext *conn, char *key);

int redis_delete(redisContext *conn, const char *key);

#endif

#include "redis_operation.h"

redisContext *redis_connect(const char *host, short port)
{
    struct timeval _tv = {3, 0};
    redisContext *conn = redisConnectWithTimeout(host, port, _tv);
    if(NULL != conn && conn->err)
    {
        logger(E, PATH, "[** FAILED **] Failed to connect to redis server(%s:%d) !\n", host, port);
        redisFree(conn);
        return NULL;
    }

    return conn;
}

int redis_delete(redisContext *conn, const char *key)
{
    if(conn == NULL || key == NULL)
    {
        logger(E, PATH, "redis connection or delete key is NULL !\n");
        return -1;
    }

    redisReply *reply = (redisReply *)redisCommand(conn, "DEL %s", key);
    if(!(NULL != reply && reply->type == REDIS_REPLY_INTEGER))
    {
        logger(E, PATH, "[** FAILED ] Failed to execute command [DEL %s].\n", key);
        freeReplyObject(reply);
        return -1;
    }

    freeReplyObject(reply);
    return 0;
}

int redis_ping(redisContext *conn)
{
    if(NULL == conn)
    {
        logger(E, PATH, "redis ping connection error !\n");
        return 0;
    }
    redisReply *reply = (redisReply *)redisCommand(conn, "PING");

    if(reply && strcasecmp(reply->str, "PONG") == 0)
    {
        freeReplyObject(reply);
        return 1;
    }

    freeReplyObject(reply);
    return 0;
}

redisReply *redis_lrange(redisContext *conn, char *key, int min_idx, int max_idx)
{
    if(NULL == conn || NULL == key)
    {
        logger(E, PATH, "redis lrange parameters error .\n");
        return NULL;
    }

    char *cmd_str = (char *)calloc(strlen(key) + 32, sizeof(char));
    if(!cmd_str)
    {
        logger(S, PATH, "malloc space error.\n");
        return NULL;
    }

    sprintf(cmd_str, "LRANGE %s %d %d", key, min_idx, max_idx);
    redisReply *reply = (redisReply *)redisCommand(conn, cmd_str);
    free(cmd_str);

    return reply;
}

redisReply *redis_hgetall(redisContext *conn, char *key)
{
    if(NULL == conn || NULL == key)
    {
        logger(E, PATH, "hgetall parameter error.\n");
        return NULL;
    }
    redisReply *reply = (redisReply *)redisCommand(conn, "HGETALL %s", key);
    return reply;
}

char *redis_lpop(redisContext *conn, const char *key)
{
	if(NULL == conn || NULL == key)
	{
		logger(E, PATH, "lpop parameter error.\n");
		return NULL;
	}
	redisReply *reply = (redisReply *)redisCommand(conn, "LPOP %s", key);
	if(NULL != reply && reply->type == REDIS_REPLY_STRING && reply->str != NULL)
	{
		int len = strlen(reply->str);
		char *value = (char *)calloc(len + 1, sizeof(char));
		if(NULL == value)
		{
			logger(S, PATH, "malloc space for lpop value failed .\n");
			freeReplyObject(reply);
			return NULL;
		}

		strncpy(value, reply->str, len);
		freeReplyObject(reply);
		return value;
	}
	else
	{
		logger(E, PATH, "[** FAILED **] Failed to execute command [LPOP %s].\n", key);
		freeReplyObject(reply);
		return NULL;
	}
}

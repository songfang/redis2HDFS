#include "read_cfg.h"

char *x_strdup(const char *src)
{
    if(NULL == src)
    {
        return NULL;
    }
    char *out = (char *)calloc(strlen(src) + 1, sizeof(char));
    strcpy(out, src);
    return out;
}

void read_cfg(cfg_file_t **p_cfg, char *file_name)
{
    const char *str_val = NULL;
    struct json_object *obj = NULL;
    struct json_object *cfg = json_object_from_file(file_name);

    if(NULL == cfg)
    {
        logger(E, PATH, "failed to read config file [%s].\n", file_name);
        return ;
    }

    *p_cfg = (cfg_file_t *)calloc(sizeof(cfg_file_t), 1);
    if(*p_cfg == NULL)
    {
        logger(S, PATH, "malloc for config file failed .\n");
        goto fail;
    }

    if(json_object_object_get_ex(cfg, "redis_host", &obj))
    {
        str_val = json_object_get_string(obj);
        (*p_cfg)->redis_host = x_strdup(str_val);
    }
    else goto fail;

    if(json_object_object_get_ex(cfg, "redis_port", &obj))
    {
        (*p_cfg)->redis_port = (short)json_object_get_int(obj);
    }
    else goto fail;

    if(json_object_object_get_ex(cfg, "hdfs_host", &obj))
    {
        str_val = json_object_get_string(obj);
        (*p_cfg)->hdfs_host = x_strdup(str_val);
    }
    else goto fail;

    if(json_object_object_get_ex(cfg, "hdfs_port", &obj))
    {
         (*p_cfg)->hdfs_port = (short)json_object_get_int(obj);
    }
    else goto fail;

    if(json_object_object_get_ex(cfg, "hdfs_file", &obj))
    {
        str_val = json_object_get_string(obj);
        (*p_cfg)->hdfs_file = x_strdup(str_val);
    }
    else goto fail;

    if(json_object_object_get_ex(cfg, "redis_list_key", &obj))
    {
        str_val = json_object_get_string(obj);
        (*p_cfg)->redis_list_key = x_strdup(str_val);
    }
    else goto fail;

    if(json_object_object_get_ex(cfg, "log_path", &obj))
    {
        str_val = json_object_get_string(obj);
        (*p_cfg)->log_path = x_strdup(str_val);
    }
    else goto fail;

    if(json_object_object_get_ex(cfg, "log_file", &obj))
    {
        str_val = json_object_get_string(obj);
        (*p_cfg)->log_file = x_strdup(str_val);
    }
    else goto fail;

    if(json_object_object_get_ex(cfg, "log_level", &obj))
    {
        (*p_cfg)->log_level = (short)json_object_get_int(obj);
    }
    else goto fail;

    json_object_put(cfg);
    return ;

fail:
    json_object_put(cfg);
    logger(E, PATH, "Invalid config parameter !\n");
    exit(EXIT_FAILURE);
}

void free_cfg(cfg_file_t *p_cfg)
{
    free(p_cfg->redis_host);
    free(p_cfg->hdfs_host);
    free(p_cfg->hdfs_file);
    free(p_cfg->redis_list_key);
    free(p_cfg->log_path);
    free(p_cfg->log_file);
    free(p_cfg);
}

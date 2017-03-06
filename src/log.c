/*
 * Author       : chenzutao
 * Date         : 2015-08-18
 * Function     : record logs
 */

#include "log.h"

static LOG_FILE g_log_file = {};

void init_log(char *path, char *file, int level)
{
        if(path == NULL)
        {
                if((path = (char *)calloc(sizeof(char), sizeof("./_logs") + 1)))
                        strcpy(path, "./_logs");
        }
        if(file == NULL)
        {
                if((file = (char *)calloc(sizeof(char), sizeof("access.log") + 1)))
                        strcpy(file, "access.log");
        }

        if(access(path, 0) == -1)
        {//if path not exist, then create file
                if(mkdir(path, 0777))
                {
                        return ;
                }
        }
        g_log_file.level = level;
        g_log_file.old_fd = 0;
        g_log_file.new_fd = 0;
        strncat(g_log_file.path, path, _MAX_FILE_PATH_SIZE - 1);
        strncat(g_log_file.file, file, _MAX_FILE_PATH_SIZE - 1);
}

void open_log(void)
{
        g_log_file.old_fd  = g_log_file.new_fd;
        time_t now = time(NULL);
        struct tm *_tm = localtime(&now);
        char file[_MAX_FILE_PATH_SIZE] = { 0 };
        snprintf(file, (sizeof(file) - 1), "%s/%s_%04d%02d.log", g_log_file.path, g_log_file.file, 1900 + _tm->tm_year, 1 + _tm->tm_mon);
        g_log_file.new_fd = open(file, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
        //printf("log file new fd:%d\n", g_log_file.new_fd);
}

void close_old_log(void)
{
        close(g_log_file.old_fd);
}

int log_info(int level, char *value, const char *__FILE, const char *__FUNC, int __LINE, const char *fmt, ...)
{
        if(level < g_log_file.level)
        {
                return -1;
        }

        time_t now = time(NULL);
        char time_buf[32] = {0};
        strftime(time_buf, sizeof(time_buf), "%y%m%d_%H%M%S", localtime(&now));

        char prefix[_MAX_HEAD_BUF_SIZE] = { 0 }; 
        snprintf(prefix, sizeof(prefix), "%s %s%16s[%20s()]|%4d|-->", time_buf, value, __FILE, __FUNC, __LINE);
        
//        printf("sizeof(prefix):%d, prefix len:%d, prefix:%s\n",sizeof(prefix), strlen(prefix), prefix);

        char msg[_MAX_MSG_BUG_SIZE] = { 0 }; //FIXME: here we use dynamic array to ensure all message have been saved, so we should use 'gcc -std=C99' to complie
        va_list ap;
        va_start(ap, fmt);
        vsprintf(msg, fmt, ap);
        va_end(ap);
       
//        printf("sizeof(msg):%d, message len:%d, message:%s\n", sizeof(msg), strlen(msg), msg);
        dprintf(g_log_file.new_fd, "%s%s", prefix, msg);
        return 0;
}

/************************************************************/
/****************** TEST CODE *******************************/
/***** include single thread test, mutiple thread test and mutiple process test ******/
#if 0
#define SINGLE_THREAD 1
#if SINGLE_THREAD

int main()
{
        char *path = "./log_test/";
        char *file = "testLog";
        int level = 0;

        struct timeval st_tv, ed_tv;
        gettimeofday(&st_tv, NULL);

        init_log(path, file, level);
        open_log();
        
        int i = 0;
        for(i=0; i<1000000; i++)
        {
                //logger(E, _FILE, _FUNC, _LINE, "use system log to record round:%d, PID:%d\n", getpid());
                //logger(I, _FILE, _FUNC, _LINE, "send message to console if can not be logged.\n");
                //logger(D, _FILE, _FUNC, _LINE, "test debug level.\n");
                logger(D, PATH, "test debug level.\n");
                logger(E, PATH, "usr dprintf to record log info at round:%d, PID:%d\n", i, getpid());
                logger(I, PATH, "send message to console if information can not be logged at round:%d\n", i);
        }
        gettimeofday(&ed_tv, NULL);
        printf("total cost:%ld\n", (ed_tv.tv_sec*1000000+ed_tv.tv_usec ) - (st_tv.tv_sec*1000000 + st_tv.tv_usec));
 
        return 0;
}
#endif

//mutiple thread test
#define MUTIPLE_THREAD 0
#if MUTIPLE_THREAD

#include <pthread.h>

void thread(int *pid)
{
        int i;
        for(i=0; i<100; i++)
        {
                logger(I, PATH, "sub thread[%d] running at :%d round\n", *pid, i);
        }
}

int main()
{
        pthread_t id;
        int i, ret;
        
        char *path = "./_log_test";
        char *file = "testLog";
        int level = 0;
        init_log(path, file, level);
        open_log();

        for(i=0; i<4; i++)
        {
                if((ret = pthread_create(&id, NULL, (void *)thread, &i)) !=0)
                {
                        logger(E, PATH, "[** FAILED **] Failed to create the %d thread.\n", i);
                        exit(0);
                }
        }

        for(i=0; i<100; i++)
        {
                logger(D, PATH, "parent thread running at :%d round\n", i+1);
        }
        pthread_join(id, NULL);
        return 0;
}
#endif

//mutiple process test
#define MUTIPLE_PROCESS 0
#if MUTIPLE_PROCESS

int main()
{

        struct timeval st_tv, ed_tv;
        gettimeofday(&st_tv, NULL);

        char *path = "./_log_test";
        char *file = "testLog";
        int level = 0;
        init_log(path, file, level);
        open_log();

        int i, j, ret;

        for(i=0; i<4; i++)
        {
                if(fork() == 0)
                {
                        for(j=0; j<1000; j++)
                        {
                                logger(E, PATH, "process[%d] running at round:%d, PPID:%d, PID:%d\n", i, j, getppid(), getpid());
                        }
                }
                else
                {
                        for(j=0; j<1000; j++)
                                logger(I, PATH, "parent process running at round:%d, PPID:%d, PID:%d\n", j, getppid(), getpid());
                }
        }

        gettimeofday(&ed_tv, NULL);
        printf("total cost:%ld\n", (ed_tv.tv_sec*1000000+ed_tv.tv_usec ) - (st_tv.tv_sec*1000000 + st_tv.tv_usec));
        return 0;
}

#endif
#endif

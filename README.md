redisHDFS :redis数据写入HDFS
===============================

redisHDFS是一个独立的C程序，它用来将写入redis中的数据格式化成json格式并写入到HDFS中。

### 1.编译lib库
因为该程序依赖cJSON库和hiredis库，首先进入到redisHDFS/lib目录下，进行编译:
```
make all
```

### 2. 安装hadoop
该程序依赖libjvm.so和libhdfs.so
其中libhdfs.so在hadoop安装路径的lib/native目录下，libjvm.so在java的jre/lib/amd64/server/目录下

### 3.修改Makefile
#### 1. INC_DIR变量中修改hdfs.h所在路径
#### 2. LIB_DIR变量中修改libhdfs.so和libjvm.so到对应的路径

### 4.编译
```
make
```
### 5.修改配置文件config.json并启动程序
```
{
    "redis_host" : "127.0.0.1",  //读取redis的IP
    "redis_port" : 6379,        // redis 端口
    "hdfs_host" : "127.0.0.1",  //HDFS IP
    "hdfs_port" : 8010,         //HDFS 端口
    "hdfs_file" : "/tmp/httpRequest/header.txt", //存储HDFS的路径
    "redis_list_key" : "header:members", 
    "zset_min_index" : 0,
    "zset_max_index" : -1,
    "log_path" : "./logs",   //日志文件路径
    "log_file" : "redis_to_hdfs.log",  //日志文件名
    "log_level" : 0  //日志等级
}

```

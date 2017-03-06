
CFLAGS = -g -O0 -Wall -Werror \
	 -lpthread -lrt -lm \
	 -DOPEN_DEBUG


LIBA =  -ljvm -lhdfs -lhiredis -lcjson -ljson-c -fPIC

INC_DIR = -I/usr/hdp/2.4.0.0-169/usr/include \
	-I./lib/cJSON \
	-I./lib/hiredis \
	-I./lib/json-c

LIB_DIR = -L/usr/hdp/2.4.0.0-169/hadoop/lib/native \
	-L/usr/hdp/2.4.0.0-169/usr/lib \
	-L./lib/ \
	-L/usr/java/jdk1.8.0_73/jre/lib/amd64/server/

SRC_DIR = ./src

SRC = $(addprefix $(SRC_DIR)/, \
      log.c \
      redis_to_hdfs.c \
      read_cfg.c \
      redis_operation.c)

redis_to_hdfs:
	gcc $(CFLAGS) $(SRC) $(INC_DIR) $(LIB_DIR) $(LIBA) -o $@

clean:
	rm -f redis_to_hdfs

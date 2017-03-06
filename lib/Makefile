LIB_DIR=$(shell pwd)

OBJ = libjson-c.a \
      libhiredis.a \
      libcjson.a

all:$(OBJ)
	-@echo "Done LIB !"

libhiredis.a:
	cd ./hiredis
	$(MAKE) -C $(LIB_DIR)/hiredis
	mv $(LIB_DIR)/hiredis/$@ $(LIB_DIR)

libjson-c.a:
	cd ./json-c && ./autogen.sh && ./configure
	$(MAKE) -C $(LIB_DIR)/json-c
	mv $(LIB_DIR)/json-c/.libs/$@ $(LIB_DIR)

libcjson.a:
	cd ./cJSON
	$(MAKE) -C $(LIB_DIR)/cJSON
	mv $(LIB_DIR)/cJSON/$@ $(LIB_DIR)

clean:
	-if [ -f json-c/Makefile ];then cd ./json-c && make distclean; fi
	-if [ -f hiredis/Makefile ];then cd ./hiredis && make clean; fi
	-if [ -f cJSON/Makefile ];then cd ./cJSON && make clean; fi
	rm -rf $(OBJ)

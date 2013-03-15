APP=sbs_ads

CFG_FILE=sbs_ads.cfg

all: $(APP)

.PHONY:all clean install

CXX = arm-linux-gcc

OBJS= sbs_device.o sbs_cmd.o ftdi.o net_tcp_srv.o xml_cfg.o $(APP).o

EXT_LIB= libtinyxml.a

LIBS= -lboost_thread  -lusb-1.0 -lstdc++ -lpthread   -lboost_system


arm_linux_lib=/opt/FriendlyARM/toolschain/4.5.1/arm-none-linux-gnueabi
arm_nfs_root =/home/ybb/arm_6410/nfsroot/tiny6410


boost_include_dir =/home/ybb/arm_6410/libs/boost_1_53_0
boost_lib_dir=/home/ybb/arm_6410/libs/boost_1_53_0/stage/lib

%.o:%.c
	arm-linux-gcc -I./ -I$(boost_include_dir)  -I./usb/include -c $<

%.o:%.cc
	arm-linux-gcc -I$(boost_include_dir)  -I./  -I./usb/include -c $<

$(APP):$(OBJS)
	$(CXX)  -I./ -L$(arm_linux_lib) -L. -L$(boost_lib_dir) -L./usb/lib  $(LIBS) -o $@  $(OBJS)  $(EXT_LIB)




clean:
	-rm *.o $(APP)

install:
	-rm $(arm_nfs_root)/bin/$(APP)
	-rm $(arm_nfs_root)/etc/$(CFG_FILE)
	cp ./$(APP) $(arm_nfs_root)/bin
	cp ./$(CFG_FILE)  $(arm_nfs_root)/etc
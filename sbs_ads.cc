#include <stdio.h>
#include <stdlib.h>
#include <boost/thread.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "xml_cfg.hpp"
#include "sbs_device.hpp"
#include "net_tcp_srv.hpp"


//app config file path
#define APP_CONFIG_FILE_NAME "/etc/sbs_ads.cfg"


//check sbs-c config result
#define CHECK_RET(n)\
    if(!isOK) \
    exit(-1) ;


void print_dbg_read_fn(std::string data ) 
{
	printf("[Size=%d]\r\n" , data.size() );
	for(int i=0 ; i <data.size() ; i++)
		printf("%02X " , data[i]);
	printf("\r\n");
}



//demon
void init_daemon(void)
{
  int pid;
  int i;
  if(pid=fork())
    exit(0);
  else if(pid<0)
    exit(1);
  //是第一子进程
    setsid();//第一子进程成为新的会话组长和进程组长, 并与控制终端分离
    if(pid=fork())
      exit(0);//结束第一子进程
      else if(pid< 0)
  exit(1);
      //第二子进程不再是会话组长
  for(i=0;i< NOFILE;++i)//关闭打开的文件描述符
close(i);
chdir("/");//改变工作目录到/
umask(0);//重设文件创建掩模
return;
}

int main()
{

init_daemon() ; 


sleep(1) ; 

ConfigManager xml_config ;          //XML config file reader 
boost::asio::io_service io_service;  // io service
sbs_device device ;  //sbs-3 device 
bool isOK = false ; 

//sbs-3  device parameter 
	int vid = 0x0403;
    int pid = 0x6010;
    int baudrate = 3000000;
    ftdi_interface interface = INTERFACE_B;

isOK = device.init() ; 
CHECK_RET(isOK) ; 

isOK = device.open(vid , pid ,interface ) ; 
CHECK_RET(isOK);

isOK = device.set_latency_timer(0x10) ; 
CHECK_RET(isOK) ;

isOK = device.set_baudrate(baudrate ) ; 
CHECK_RET(isOK) ;



//read app config 
bool load_cfg_success = xml_config.LoadConfigFile(APP_CONFIG_FILE_NAME) ;
if(!load_cfg_success)
{
#ifdef _DEBUG
  printf("load app sbs_ads config file error!\r\n");
#endif

return -1 ; 
}

// net tcp listen port 
std::string net_tcp_listen_port_str = get_node_text(xml_config , "net_config" , "listen_port") ;
if(net_tcp_listen_port_str.empty())
{
  #ifdef _DEBUG
  printf("read app config net_config->listen_port  error!\r\n") ;
  #endif
  return -1 ; 
}
int net_tcp_listen_port = atoi(net_tcp_listen_port_str.c_str()) ;

 #ifdef _DEBUG
 printf("net tcp listen port = %d\r\n" , net_tcp_listen_port) ;
 #endif


      tcp::endpoint endpoint(tcp::v4(), net_tcp_listen_port);
	    ais_net_server_ptr server(new ais_net_server(io_service, endpoint));
      server->start_accept() ;  // tcp port start listen ... 

   
      boost::thread t(boost::bind(&boost::asio::io_service::run , &io_service)) ;

 

//isOK =  device.start_read(print_dbg_read_fn) ;  
isOK =  device.start_read(boost::bind(&ais_net_server::send_msg_2_all_client , server , _1 ) ) ;  
CHECK_RET(isOK);

return 0 ; 

}
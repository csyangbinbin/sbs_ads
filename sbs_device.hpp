/*
sbs_device.hpp
*/
#ifndef __SBS_DEVICE_INCLUDE__
#define __SBS_DEVICE_INCLUDE__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <ftdi.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <string>

typedef boost::function<void (std::string) > read_data_callback ; 


#define READ_SLEEP (100)

class sbs_device
{
public:
	sbs_device() ; 
public:
	bool init() ;  // init 
	bool open(int vid ,int pid , ftdi_interface interface) ;  //open device 
	bool set_latency_timer(int t) ; 
	bool set_baudrate(int rate ) ; 

	bool start_read(read_data_callback fn ) ;  
	void stop_read() ;

	void close() ; 
private:
	void default_read_data_callback(std::string msg) ;  
private:
	 struct ftdi_context *ftdi;
     bool exitRequested ; 
	 read_data_callback fn_ ; 
	 enum {max_recv_buf_szie = 128} ; 
	 unsigned char buf[max_recv_buf_szie];
}; 


#endif
/*
sbs_device.cc
*/
#include "sbs_device.hpp"
#include "sbs_cmd.h"
#include "config.hpp"

sbs_device::sbs_device()
	:ftdi(0) , fn_(0) , 
    exitRequested(false)
{

}


bool sbs_device::init() 
{
	int f = 0 ; 
    if ((ftdi = ftdi_new()) == 0)
    {
    	#ifdef _DEBUG
        fprintf(stderr, "ftdi_new failed\n");
        #endif 
ftdi = 0 ; 
        return false  ;
    }

f  = ftdi_init(  ftdi   ); 
if(f<0)
	{
	#ifdef _DEBUG
  fprintf(stderr, "ftdi_init eeror\r\n\n");
	#endif 
  ftdi = 0 ;
	return false  ; 
	}
     return true ; 
}

bool sbs_device::open(int vid ,int pid , ftdi_interface interface) 
{
	int f = 0 ; 

	if(!ftdi)
	{
	#ifdef _DEBUG
		fprintf(stderr , "ftdi is not init !\r\n") ;
	#endif 
	return false ; 
	} 

        // Select interface
        ftdi_set_interface(ftdi, interface);
        // Open device
        f = ftdi_usb_open(ftdi, vid, pid);
        if (f < 0)
	 {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(ftdi));
       return false ; 
    }

    return true ;
}

bool sbs_device::set_latency_timer(int t)
{
int f = 0 ; 

	if(!ftdi)
	{
	#ifdef _DEBUG
		fprintf(stderr , "ftdi is not init !\r\n") ;
	#endif 
	return false ; 
	} 
  f =  ftdi_set_latency_timer(ftdi, t);
     if (f < 0)
    {
        fprintf(stderr, "unable to ftdi_set_latency_timer: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        return false  ;
    }
    return true ; 
}

bool sbs_device::set_baudrate(int rate ) 
{
int f = 0 ; 

	if(!ftdi)
	{
	#ifdef _DEBUG
		fprintf(stderr , "ftdi is not init !\r\n") ;
	#endif 
	return false ; 
	} 
 f = ftdi_set_baudrate(ftdi, rate);
    if (f < 0)
    {
        fprintf(stderr, "unable to set baudrate: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        return false ;
    }
    return true ;
}
void sbs_device::default_read_data_callback(std::string msg ) 
{

//nothing to do 
}

bool sbs_device::start_read(read_data_callback fn ) 
{
exitRequested = false ;
    int f=0 ; 

    if(!ftdi)
    {
    #ifdef _DEBUG
        fprintf(stderr , "ftdi is not init !\r\n") ;
    #endif 
    return false ; 
    } 

       f = ftdi_set_line_property(ftdi, BITS_8, STOP_BIT_1, NONE);
    if (f < 0)
    {
        fprintf(stderr, "unable to set line parameters: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        return false ; 
    }
    


    f =  ftdi_setflowctrl    (    ftdi ,  SIO_DISABLE_FLOW_CTRL  ) ; 
      if (f < 0)
    {
        fprintf(stderr, "unable to ftdi_setflowctrl: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        return false ;
    }   
   ftdi_usb_purge_rx_buffer(ftdi);
   ftdi_usb_purge_tx_buffer(ftdi);

   if(fn == 0 )
        fn_ = boost::bind(&sbs_device::default_read_data_callback , this ,_1 ) ;
    else
        fn_  = fn ; 

u8 config_buffer[128] ; 
int csize = SetDigitalOutput(false , config_buffer) ;
 //int ftdi_write_data(struct ftdi_context *ftdi, unsigned char *buf, int size);

 f = ftdi_write_data(ftdi , config_buffer , csize ) ; 
  f = ftdi_write_data(ftdi , config_buffer , csize ) ; 
   f = ftdi_write_data(ftdi , config_buffer , csize ) ; 
   if(f<0)
   {

    fprintf(stderr, "unable to write config buffer : %d (%s)\n", f, ftdi_get_error_string(ftdi));
        return false ;
   }



//start loop to read  
    while(!exitRequested)
    {
         f = ftdi_read_data(ftdi, buf, sizeof(buf));
         if (f<=0)
            usleep(1 * READ_SLEEP);
        else 
        {
            if(fn_)
            {
                    std::string msg((const char*)buf , f) ;
                   fn_(msg ) ;
            }
             
        }

    } //end for read 

   return true ; 
}

void sbs_device::close()
{
    if(ftdi)
    {        
    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);
    }
    ftdi = 0 ; 
}

void sbs_device::stop_read()
{
exitRequested = true ; 
}
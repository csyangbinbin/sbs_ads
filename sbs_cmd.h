/********************************************************************
	created:	2012/06/05
	created:	5:6:2012   10:37
	filename: 	i:\AB3.0\SBS-TCP\sbs_tcp\sbs_tcp\sbs_cmd.h
	file path:	i:\AB3.0\SBS-TCP\sbs_tcp\sbs_tcp
	file base:	sbs_cmd
	file ext:	h
	author:		ybb
	
	purpose:	SBS参数设置
*********************************************************************/
#ifndef __SBS_CMD_SET_INCLUDE__
#define __SBS_CMD_SET_INCLUDE__

#include "KineticAPI.h"




typedef unsigned char u8 ;
typedef unsigned short u16 ; 
typedef unsigned int   u32 ; 



#define DLE 0x10 
#define STX 0x02
#define ETX 0x03


#define TUNER_TAG 0x66
#define SDR_TAG   0x77
#define AUDIO_TAG 0x88



int calcrc(const unsigned char *ptr,int count)   ; 

#pragma pack(1)

//寄存器访问命令 
 struct RegAccessReadCmd
{
u8 pack_type ; 
u8 tag  ; 
u8 op ; 
u8 reserved_1 ; 
u8 reg_lo ; 
u8 reg_hi ;
u8 subgup_lo ;
u8 subgup_hi ; 
u8 gup_lo ; 
u8 gup_hi ;
u8 read_size ; 
u8 reserved_zero  ; 
};


//Tuner设置命令
struct KAL_Tuner_Data
{
u32 min_freq ;  //0x0000
u32 max_freq ;  //0x0004
u16 tuner_id ;  //0x0008
u8 reserve_1[6] ;//保留
u32 centre_freq ; //0x0010
u32 band_width ; //0x0014
u8 tuner_powerup ;//0x0018
// u8 reserve_2[7];
// u8 specific ;  //0x0020
} ;


struct KAL_Tuner_DataSetCmd
{
RegAccessReadCmd head ; 
KAL_Tuner_Data data ; 
} ;


struct KAL_SDR_Data
{
u32 centre_freq ; //0x0000
u8  sdr_source ;  //0x0004
u8  sdr_demod  ;  //0x0005
u8  sdr_squelch;  //0x0006
u8  sdr_mute ;    //0x0007
};

//读取SDR配置参数
struct KAL_SDR_READ_Data
{
RegAccessReadCmd head ; 
KAL_SDR_Data data ; 
u32 sdr_rssi  ;   
};


//写入的SDR配置参数
struct KAL_SDR_WRITE_Data
{
	RegAccessReadCmd head ; 
	KAL_SDR_Data data ;   
};



struct KAL_Tuner_DataSetCmd2
{
	RegAccessReadCmd head ; 
	u32 data ; 
} ;


struct sdr_status_data{u8  signalStrength ; u8 Flags ; } ; 
struct tuner_status_data{u8 AGC ; u8 reserved[3] ; } ; 
//收音机状态报告
struct KAL_Radio_report
{
u8 pack_type ; 
u8 tag ; //0x00
u8 reserved_1 ; //0x00
u8 sbrs_numbers ; 
u8 bytesPersbr ; 
u8 tuners_numbers ; 
u8 bytesPertuner; 
sdr_status_data sdr_datas[4] ; 
tuner_status_data tuner_datas[2] ; 
} ;


//收音机音量调节
struct KAL_Audio_Data
{
u8 num_inputs ; //0x0000
u8 max_inputs ; //0x0001
u8 audio_mode ; //0x0002
u8 audio_rate ; //0x0003
//-----------//
u8 master_gain_L ; //0x0004
u8 muster_mute_L ; //0x0005
u8 master_gain_R ; //0x0006
u8 master_mute_R ; //0x0007
u8 reserved_1[8] ; //0x00008
//------------//
u32 input0_gain ; //0x0010
u8  input0_gain_L; //0x0014
u8 input0_mute_L ; //0x0015
u8 input0_gain_R ;//0x0016
u8 input0_mute_R ;//0x0017
//-------------//
u32 input1_gain ; //0x0018
u8  input1_gain_L; //0x001c
u8 input1_mute_L ; //0x001d
u8 input1_gain_R ;//0x001e
u8 input1_mute_R ;//0x001f
//-------------//
u32 input2_gain ; //0x0020
u8  input2_gain_L; //0x0024
u8 input2_mute_L ; //0x0025
u8 input2_gain_R ;//0x0026
u8 input2_mute_R ;//0x0027
//-------------//
u32 input3_gain ; //0x0028
u8  input3_gain_L; //0x002c
u8 input3_mute_L ; //0x002d
u8 input3_gain_R ;//0x002e
u8 input3_mute_R ;//0x002f
//-------------//
u32 input4_gain ; //0x0030
u8  input4_gain_L; //0x0034
u8 input4_mute_L ; //0x0035
u8 input4_gain_R ;//0x0036
u8 input4_mute_R ;//0x0037
//-------------//
u32 input5_gain ; //0x0038
u8  input5_gain_L; //0x003c
u8 input5_mute_L ; //0x003d
u8 input5_gain_R ;//0x003e
u8 input5_mute_R ;//0x003f
//-------------//
} ;


struct KAL_AUDIO_WRITE_DATA
{
RegAccessReadCmd head ; 
KAL_Audio_Data data ; 
} ; 



struct KAL_AUDIO_READ_DATA
{
	RegAccessReadCmd head ; 
    KAL_Audio_Data data ; 
} ;


struct KAL_AUDIO_REG_WRITE
{
RegAccessReadCmd head ; 
u8 data ; 
};

#pragma pack()


int cmdtest(u8* outdata)  ;
int cmdtest2(u8* outdata)  ; 


enum tuner_no { tuner_a = 0 , tuner_b=1} ; 
enum sdr_channel{sdr_1=0 , sdr_2=1 , sdr_3=2 , sdr_4=3} ; 
enum sdr_demod {FM=0 ,AM=1 ,WFM=2} ;
enum audio_mode{ audio_analog_mode=0 , audio_digital_mode=1} ; 



//----------------------------------------------------------------------------//
//生成设置Tuner的配置数据
int SetTuner( tuner_no no , double centre_freq ,  u8* outdata )  ; 
//生成查询Tuner的配置参数
int QueryTuner(tuner_no no , u8* outdata)  ; 

//----------------------------------------------------------------------------//
//生成查询SDR配置参数
int QuerySDR(sdr_channel channel ,u8* outdata) ;
//生成配置SDR配置参数
int SetSDR(sdr_channel channel , double freq ,tuner_no source , sdr_demod demod , u8 squelch , bool mute ,  u8* outdata) ; 


//----------------------------------------------------------------------------//
//生成查询AUDIO配置参数
int QueryAudio(audio_mode mode,u8* outdata) ; 

int SetAudio(u8* outdata) ; 


int SetDigitalOutput(bool enable , u8* outdata) ; 

int SetDigitalReg(u16 reg , u8 data , u8* outdata) ; 


int Set_AC_Mode(bool bopen ,  u8* outdata) ;


#endif
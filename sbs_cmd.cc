#include "sbs_cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int calcrc(const unsigned char *ptr,int count)   
{ 
	int crc, i; 

	crc = 0; 

	while (--count >= 0) { 
		crc = crc ^ (int)*ptr++ << 8; 

		for (i = 0; i < 8; ++i) 
			if (crc & 0x8000) 
				crc = crc << 1 ^ 0x1021; 
			else 
				crc = crc << 1; 
	} 
	return (crc & 0xFFFF); 
}  

int QueryTuner(tuner_no no, u8* outdata) 
{
	RegAccessReadCmd cmd ; 
	cmd.pack_type = KAL_PKT_REG_ACCESS ; 
	cmd.tag  = TUNER_TAG ; 
	cmd.op	= KAL_REG_OP_READ ; 
	cmd.reserved_1 = 0 ; 
	cmd.reg_lo = 0 ; 
	cmd.reg_hi = 0 ; 
no==tuner_a? (cmd.subgup_lo  = 0 ): (cmd.subgup_lo  = 1);
	cmd.subgup_hi  = 0; 
	cmd.gup_lo = 0x03 ; 
	cmd.gup_hi =0 ; 
	cmd.read_size = 0x19 ; 
	cmd.reserved_zero =0 ; 


	outdata[0] = DLE ; 
	outdata[1] = STX  ;

	memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

	outdata[2+sizeof(cmd)] = DLE ; 

	outdata[2+sizeof(cmd)+1] = ETX ; 

	int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

	outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
	outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

	return (6+sizeof(cmd)) ; 

}

int cmdtest(u8* outdata) 
{
RegAccessReadCmd cmd ; 
cmd.pack_type = KAL_PKT_REG_ACCESS ; 
cmd.tag  = 0x66 ; 
cmd.op	= KAL_REG_OP_READ ; 
cmd.reserved_1 = 0 ; 
cmd.reg_lo = 0 ; 
cmd.reg_hi = 0 ; 
cmd.subgup_lo  = 0;
cmd.subgup_hi  = 0; 
cmd.gup_lo = 0x03 ; 
cmd.gup_hi =0 ; 
cmd.read_size = 0x19 ; 
cmd.reserved_zero =0 ; 


outdata[0] = DLE ; 
outdata[1] = STX  ;

memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

outdata[2+sizeof(cmd)] = DLE ; 

outdata[2+sizeof(cmd)+1] = ETX ; 

int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

return (6+sizeof(cmd)) ; 
}





int cmdtest2(u8* outdata) 
{
KAL_Tuner_DataSetCmd cmd ; 
memset(&cmd , 0 , sizeof(KAL_Tuner_DataSetCmd));
cmd.head.pack_type = KAL_PKT_REG_ACCESS ; 
cmd.head.tag  = 0x66 ; 
cmd.head.op	= KAL_REG_OP_WRITE| KAL_REG_OP_SAVE; 
cmd.head.reserved_1 = 0 ; 
cmd.head.reg_lo = 0 ; 
cmd.head.reg_hi = 0 ; 
cmd.head.subgup_lo  = 0;
cmd.head.subgup_hi  = 0; 
cmd.head.gup_lo = 0x03 ; 
cmd.head.gup_hi =0 ; 
cmd.head.read_size = sizeof(KAL_Tuner_Data) ; 
cmd.head.reserved_zero =0 ;

cmd.data.max_freq = cmd.data.min_freq =0 ; 
cmd.data.tuner_id = 0 ; 
cmd.data.centre_freq = static_cast<u32>(120.123 *1000000 ); 
cmd.data.band_width = 0x007A1200  ;
cmd.data.tuner_powerup = 0xFF ; 



outdata[0] = DLE ; 
outdata[1] = STX  ;

memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

outdata[2+sizeof(cmd)] = DLE ; 

outdata[2+sizeof(cmd)+1] = ETX ; 

int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

return (6+sizeof(cmd)) ; 



}




int SetTuner(
			 tuner_no no , 
			 double centre_freq ,   /*中心频点(单位:MHZ)*/
			 u8* outdata	
			 )
{



	KAL_Tuner_DataSetCmd cmd ; 
	memset(&cmd , 0 , sizeof(KAL_Tuner_DataSetCmd));
	cmd.head.pack_type = KAL_PKT_REG_ACCESS ; 
	cmd.head.tag  = TUNER_TAG ; 
	cmd.head.op	= KAL_REG_OP_WRITE| KAL_REG_OP_SAVE; 
	cmd.head.reserved_1 = 0 ; 
	cmd.head.reg_lo = 0 ; 
	cmd.head.reg_hi = 0 ; 
	no==tuner_a? (cmd.head.subgup_lo  = 0 ): (cmd.head.subgup_lo  = 1);
	cmd.head.subgup_hi  = 0; 
	cmd.head.gup_lo = 0x03 ; 
	cmd.head.gup_hi =0 ; 
	cmd.head.read_size = sizeof(KAL_Tuner_Data) ; 
	cmd.head.reserved_zero =0 ;

	cmd.data.max_freq = cmd.data.min_freq =0 ; 
	cmd.data.tuner_id = 0 ; 
	cmd.data.centre_freq = static_cast<u32>(centre_freq *1000000 ); 
	cmd.data.band_width = static_cast<u32>(4 *1000000 ); //4M
	cmd.data.tuner_powerup = 0xFF ; 



	outdata[0] = DLE ; 
	outdata[1] = STX  ;

	memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

	outdata[2+sizeof(cmd)] = DLE ; 

	outdata[2+sizeof(cmd)+1] = ETX ; 

	int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

	outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
	outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

	return (6+sizeof(cmd)) ; 
}


//////////////////////////////////////////////////////////////////////////朴素的分割线
int QuerySDR(sdr_channel channel ,u8* outdata) 
{
/*
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
*/
	RegAccessReadCmd cmd ; 
	cmd.pack_type = KAL_PKT_REG_ACCESS ; 
	cmd.tag  = SDR_TAG ; 
	cmd.op	= KAL_REG_OP_READ ; 
	cmd.reserved_1 = 0 ; 
	cmd.reg_lo = 0; 
	cmd.reg_hi = 0 ; 
	cmd.subgup_lo  =channel ;
	cmd.subgup_hi  = 0; 
	cmd.gup_lo = 0x04 ; 
	cmd.gup_hi =0 ; 
	cmd.read_size = 12 ;  
	cmd.reserved_zero =0 ; 


	outdata[0] = DLE ; 
	outdata[1] = STX  ;

	memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

	outdata[2+sizeof(cmd)] = DLE ; 

	outdata[2+sizeof(cmd)+1] = ETX ; 

	int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

	outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
	outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

	return (6+sizeof(cmd)) ; 

}


//////////////////////////////////////////////////////////////////////////朴素的分割线
int SetSDR(sdr_channel channel , double freq ,tuner_no source , sdr_demod demod , u8 squelch , bool mute,u8* outdata ) 
{
	/*
	struct KAL_SDR_WRITE_Data
	{
		RegAccessReadCmd head ; 
		KAL_SDR_Data data ;   
	};
	*/

KAL_SDR_WRITE_Data cmd ; 


	cmd.head.pack_type = KAL_PKT_REG_ACCESS ; 
	cmd.head.tag  = SDR_TAG ; 
	cmd.head.op	= KAL_REG_OP_WRITE| KAL_REG_OP_SAVE ; 
	cmd.head.reserved_1 = 0 ; 
	cmd.head.reg_lo = 0; 
	cmd.head.reg_hi = 0 ; 
	cmd.head.subgup_lo  =channel ;
	cmd.head.subgup_hi  = 0; 
	cmd.head.gup_lo = 0x04 ; 
	cmd.head.gup_hi =0 ; 
	cmd.head.read_size = 8 ;  
	cmd.head.reserved_zero =0 ; 


	cmd.data.centre_freq =static_cast<u32>(freq* 1000000) ; 
    cmd.data.sdr_source = source ; 
    cmd.data.sdr_demod = demod ; 
	cmd.data.sdr_squelch = squelch ; 
	cmd.data.sdr_mute = mute?0xFF:0x00 ; 

	outdata[0] = DLE ; 
	outdata[1] = STX  ;

	memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

	outdata[2+sizeof(cmd)] = DLE ; 

	outdata[2+sizeof(cmd)+1] = ETX ; 

	int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

	outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
	outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

	return (6+sizeof(cmd)) ; 
}
//////////////////////////////////////////////////////////////////////////朴素的分割线
int QueryAudio(audio_mode mode,u8* outdata ) 
{
	RegAccessReadCmd cmd ; 
	cmd.pack_type = KAL_PKT_REG_ACCESS ; 
	cmd.tag  = AUDIO_TAG ; 
	cmd.op	= KAL_REG_OP_READ ; 
	cmd.reserved_1 = 0 ; 
	cmd.reg_lo = 0; 
	cmd.reg_hi = 0 ; 
	cmd.subgup_lo  =mode ;
	cmd.subgup_hi  = 0; 
	cmd.gup_lo = 0x05 ; 
	cmd.gup_hi =0 ; 
	cmd.read_size = sizeof(KAL_Audio_Data) ;  
	cmd.reserved_zero =0 ; 

	outdata[0] = DLE ; 
	outdata[1] = STX  ;

	memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

	outdata[2+sizeof(cmd)] = DLE ; 

	outdata[2+sizeof(cmd)+1] = ETX ; 

	int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

	outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
	outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

	return (6+sizeof(cmd)) ; 

}

int SetAudio(u8* outdata)
{


	KAL_AUDIO_WRITE_DATA cmd ; 

	/*
	cmd.head.pack_type = KAL_PKT_REG_ACCESS ; 
	cmd.head.tag  = AUDIO_TAG ; 
	cmd.head.op	= KAL_REG_OP_WRITE| KAL_REG_OP_SAVE  ; 
	cmd.head.reserved_1 = 0 ; 
	cmd.head.reg_lo = 0; 
	cmd.head.reg_hi = 0 ; 
	cmd.head.subgup_lo  =1 ;
	cmd.head.subgup_hi  = 0; 
	cmd.head.gup_lo = 0x05 ; 
	cmd.head.gup_hi =0 ; 
	cmd.head.read_size = sizeof(KAL_Audio_Data) ;  
	cmd.head.reserved_zero =0 ; 

	*/

	cmd.head.pack_type = KAL_PKT_REG_ACCESS ; 
	cmd.head.tag  = AUDIO_TAG ; 
	cmd.head.op	= KAL_REG_OP_WRITE| KAL_REG_OP_SAVE ; 
	cmd.head.reserved_1 = 0 ; 
	cmd.head.reg_lo = 0; 
	cmd.head.reg_hi = 0 ; 
	cmd.head.subgup_lo  =1 ;
	cmd.head.subgup_hi  = 0; 
	cmd.head.gup_lo = 0x05; 
	cmd.head.gup_hi =0 ; 
	cmd.head.read_size = sizeof(KAL_Audio_Data) ;  
	cmd.head.reserved_zero =0 ; 


	memset(&(cmd.data) , 0,sizeof(KAL_Audio_Data) ) ;
/*
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

*/
cmd.data.num_inputs =4 ; 
cmd.data.max_inputs =4 ; 
cmd.data.audio_mode = 0x83 ; 
cmd.data.audio_rate = 3 ; 
cmd.data.master_gain_L = 100 ; 
cmd.data.muster_mute_L=0 ;
cmd.data.master_gain_R =100 ; 
cmd.data.master_mute_R = 0 ; 
cmd.data.input0_gain =0 ;
cmd.data.input0_gain_L =100 ;
cmd.data.input0_mute_L =0 ;
cmd.data.input0_gain_R =100;
cmd.data.input0_mute_R = 0 ;

outdata[0] = DLE ; 
outdata[1] = STX  ;

memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

outdata[2+sizeof(cmd)] = DLE ; 

outdata[2+sizeof(cmd)+1] = ETX ; 

int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

 return (6+sizeof(cmd)) ; 
}

//////////////////////////////////////////////////////////////////////////朴素的分割线
int SetDigitalOutput(bool enable , u8* outdata) 
{
KAL_AUDIO_REG_WRITE cmd ; 

cmd.head.pack_type = KAL_PKT_REG_ACCESS ; 
cmd.head.tag  = AUDIO_TAG ; 
cmd.head.op	= KAL_REG_OP_WRITE| KAL_REG_OP_SAVE ; 
cmd.head.reserved_1 = 0 ; 
cmd.head.reg_lo = 0x02; 
cmd.head.reg_hi = 0 ; 
cmd.head.subgup_lo  =0x01 ;
cmd.head.subgup_hi  = 0; 
cmd.head.gup_lo = 0x05; 
cmd.head.gup_hi =0 ; 
cmd.head.read_size = 1 ;  
cmd.head.reserved_zero =0 ; 

if(enable)
cmd.data = 0x80 ; 
else
cmd.data = 0x00;


outdata[0] = DLE ; 
outdata[1] = STX  ;

memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

outdata[2+sizeof(cmd)] = DLE ; 

outdata[2+sizeof(cmd)+1] = ETX ; 

int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

return (6+sizeof(cmd)) ; 


}

int SetDigitalReg(u16 reg , u8 data , u8* outdata) 
{
	KAL_AUDIO_REG_WRITE cmd ; 

	cmd.head.pack_type = KAL_PKT_REG_ACCESS ; 
	cmd.head.tag  = AUDIO_TAG ; 
	cmd.head.op	= KAL_REG_OP_WRITE| KAL_REG_OP_SAVE ; 
	cmd.head.reserved_1 = 0 ; 
	cmd.head.reg_lo = u8((reg)&0x00FF); 
	cmd.head.reg_hi = u8((reg>>8)&0x00FF) ; 
	cmd.head.subgup_lo  =0x01 ;
	cmd.head.subgup_hi  = 0; 
	cmd.head.gup_lo = 0x05; 
	cmd.head.gup_hi =0 ; 
	cmd.head.read_size = 1 ;  
	cmd.head.reserved_zero =0 ; 


		cmd.data = data ; 
	


	outdata[0] = DLE ; 
	outdata[1] = STX  ;

	memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

	outdata[2+sizeof(cmd)] = DLE ; 

	outdata[2+sizeof(cmd)+1] = ETX ; 

	int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

	outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
	outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

	return (6+sizeof(cmd)) ; 
}


// 
// struct KAL_Tuner_DataSetCmd2
// {
// 	RegAccessReadCmd head ; 
// 	u32 data ; 
// } ;
// 


int Set_AC_Mode(bool bopen ,  u8* outdata)
{
	KAL_Tuner_DataSetCmd2 cmd ; 
	
	memset(&cmd , 0 , sizeof(KAL_Tuner_DataSetCmd2));
	cmd.head.pack_type = KAL_PKT_REG_ACCESS ; 
	cmd.head.tag  = 0x99 ; 
	cmd.head.op	= KAL_REG_OP_WRITE| KAL_REG_OP_SAVE; 
	cmd.head.reserved_1 = 0 ; 
	cmd.head.reg_lo = 0x80 ; 
	cmd.head.reg_hi = 0 ; 
	cmd.head.subgup_lo  = 0;
	cmd.head.subgup_hi  = 0; 
	cmd.head.gup_lo = 0x01 ; 
	cmd.head.gup_hi =0 ; 
	cmd.head.read_size = 4 ; 
	cmd.head.reserved_zero =0 ;

	if(bopen)
	cmd.data = 2; 
	else
		cmd.data = 0 ; 


	outdata[0] = DLE ; 
	outdata[1] = STX  ;

	memcpy(&outdata[2] , &cmd , sizeof(cmd)) ;

	outdata[2+sizeof(cmd)] = DLE ; 

	outdata[2+sizeof(cmd)+1] = ETX ; 

	int crc = calcrc( (const unsigned char*)&cmd ,sizeof(cmd) ) ; 

	outdata[2+sizeof(cmd)+2] = u8((crc>>8)&0x000000FF) ; 
	outdata[2+sizeof(cmd)+3] =u8(crc&0x000000FF) ; 

	return (6+sizeof(cmd)) ; 

}
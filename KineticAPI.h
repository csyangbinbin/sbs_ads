/**
\par Filename :  KineticAPI.h

Kinetic Avionics Limited Protocol Packet Types and Field Types

Date		Version
24-02-2012	1.01
23-05-2012	1.02	Added packet types for AIS and ACARS data

*\par Copyright 2008-2012 Kinetic Avionics Limited. All rights reserved.

*/
#define KAL_HEADER_FILE_MAJOR   0x01
#define KAL_HEADER_FILE_MINOR   0x02

// packet types
#define KAL_PKT_MODES_ADSB      0x01
#define KAL_PKT_MODES_LONG      0x05
#define KAL_PKT_MODES_SHORT     0x07
#define KAL_PKT_SERIAL_ENQ      0x11
#define KAL_PKT_LOGIN_REQ       0x17
#define KAL_PKT_RS232_BAUD_SET  0x19
#define KAL_PKT_RS232_OUT       0x1a
#define KAL_PKT_OPTION_REQ      0x1c
#define KAL_PKT_MODE_SET        0x1d
#define KAL_PKT_LED_SET         0x1e
#define KAL_PKT_STATUS          0x20
#define KAL_PKT_SERIAL_REPLY    0x21
#define KAL_PKT_LOGIN_INFO      0x26
#define KAL_PKT_RS232_OUT_ECHO  0x2a
#define KAL_PKT_RS232_IN        0x2b
#define KAL_PKT_RADIO_STATUS    0x38
#define KAL_PKT_AUDIO_DATA      0x3b
#define KAL_PKT_REG_DATA        0x45
#define KAL_PKT_REG_ACCESS      0x54
#define KAL_PKT_AIS_DATA        0x57
#define KAL_PKT_ACARS_DATA      0x58

// register groups
#define KAL_REG_GROUP_GENERAL   0x0001
#define KAL_REG_GROUP_NETWORK   0x0002
#define KAL_REG_GROUP_TUNER     0x0003
#define KAL_REG_GROUP_SDR       0x0004
#define KAL_REG_GROUP_AUDIO     0x0005


// register operation flags
#define KAL_REG_OP_WRITE        0x01
#define KAL_REG_OP_RESTORE      0x02
#define KAL_REG_OP_DEFAULT      0x03
#define KAL_REG_OP_READ         0x04
#define KAL_REG_OP_SAVE         0x08
#define KAL_REG_OP_NV_ONLY      0x10

// general group register numbers
#define KAL_REG_VERSION_STR     0x0040
#define KAL_REG_FIRMWARE_VERS   0x0050
#define KAL_REG_FPGA_VERS       0x0052
#define KAL_REG_UI_VERS         0x0056
#define KAL_REG_DEVICE_ID       0x0058
#define KAL_REG_NUM_TUNERS      0x0060
#define KAL_REG_NUM_SDR         0x0061
#define KAL_REG_MODE			0x0080

// tuner group register numbers
#define KAL_REG_TUNER_MIN_FREQ      0x0000
#define KAL_REG_TUNER_MAX_FREQ      0x0004
#define KAL_REG_TUNER_ID            0x0008
#define KAL_REG_TUNER_CENTRE_FREQ   0x0010
#define KAL_REG_TUNER_BANDWIDTH     0x0014
#define KAL_REG_TUNER_POWERUP       0x0018
#define KAL_REG_TUNER_SPECIFIC      0x0020

// SDR group register numbers
#define KAL_REG_SDR_CENTRE_FREQ     0x0000
#define KAL_REG_SDR_SOURCE          0x0004
#define KAL_REG_SDR_DEMOD           0x0005
#define KAL_REG_SDR_SQUELCH         0x0006
#define KAL_REG_SDR_MUTE            0x0007
#define KAL_REG_SDR_RSSI            0x0008

// Network group register numbers
#define KAL_REG_NET_IP_ADDR         0x0000
#define KAL_REG_NET_SUBNET_MASK     0x0004
#define KAL_REG_NET_GATEWAY         0x0008
#define KAL_REG_NET_ETH_SPEED       0x000c
#define KAL_REG_NET_ETH_DUPLEX      0x000d
#define KAL_REG_NET_ETH_ANE         0x000e
#define KAL_REG_NET_RESET           0x000f
#define KAL_REG_NET_MAIN_PORT       0x0010
#define KAL_REG_NET_NMEA_PORT       0x0012

// Audio group register numbers
#define KAL_REG_AUDIO_NUM_INPUTS    0x0000
#define KAL_REG_AUDIO_MAX_INPUTS    0x0001
#define KAL_REG_AUDIO_MODE          0x0002
#define KAL_REG_AUDIO_RATE          0x0003
#define KAL_REG_AUDIO_MASTER_GAIN_L 0x0004
#define KAL_REG_AUDIO_MASTER_MUTE_L 0x0005
#define KAL_REG_AUDIO_MASTER_GAIN_R 0x0006
#define KAL_REG_AUDIO_MASTER_MUTE_R 0x0007
#define KAL_REG_AUDIO_INPUT0_CHAN   0x0010
#define KAL_REG_AUDIO_INPUT0_GAIN_L 0x0014
#define KAL_REG_AUDIO_INPUT0_MUTE_L 0x0015
#define KAL_REG_AUDIO_INPUT0_GAIN_R 0x0016
#define KAL_REG_AUDIO_INPUT0_MUTE_R 0x0017

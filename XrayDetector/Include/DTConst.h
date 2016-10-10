
#pragma once
#include <windef.h>

#define _CRT_SECURE_NO_DEPRECATE


#define EID_SUCCESS						0

#define EID_UNKNOWN_CMD_CHANNEL			1
#define EID_UNKNOWN_IMG_CHANNEL			2
#define EID_NO_IMG_CHANEL				3
#define EID_NO_CMD_CHANEL				4


#define EID_USB_CHANEL_OPEN_FAILED		5
#define EID_USB_CMD_SEND_FAIL			6
#define EID_USB_CMD_REV_FAIL			7
#define EID_USB_IMG_CHANEL_OPEN_FAILED	8
#define EID_USB_IMG_REV_FAIL			9



#define EID_NET_CMD_CONNECT_FAILED		10
#define EID_NET_IMG_CONNECT_FAILED		11
#define EID_NET_CMD_SEND_FAILED			12
#define EID_NET_CMD_REV_FAILED			13
#define EID_NET_IMG_REV_FAILED			14

#define EID_SERIAL_CMD_OPEN_FAILED		15
#define EID_SERIAL_CMD_REV_FAILED		16

#define EID_CMDCHANEL_RT_ERROR			17

#define EID_FRAMEBUF_ALLOC_FAIL			18
#define EID_CANNOT_OPEN_IMG_OBJ			19
#define EID_PACKET_LEN_ERROR			20
#define EID_DTP_PACKET_NONE				21
#define EID_MEM_ALLOC_FAILED			22
#define EID_FRAMEBUF_OPEN_FAIL			23
#define EID_DUAL_ENERGY_ERROR           24
#define EID_CONNECTION_LOST				25

#define EID_WRONG_LUGAS_COMMAND         26
#define EID_LUGAS_02                    27
#define EID_LUGAS_03                    28
#define EID_LUGAS_04                    29
#define EID_LUGAS_05                    30
#define EID_LUGAS_06                    31
#define EID_LUGAS_07                    32
#define EID_LUGAS_08                    33
#define EID_LUGAS_09                    34

#define EID_GIG_CMD_CONNECT_FAILED		35
#define EID_GIG_IMG_CONNECT_FAILED		36
#define EID_GIG_CMD_SEND_FAILED			37
#define EID_GIG_CMD_REV_FAILED			38
#define EID_GIG_IMG_REV_FAILED			39

#define DT_EVENT_DATA_LOST				1
#define DT_EVENT_FRAME_BUF_FULL			2
//Channel Type
#define DT_PCDIG		0
#define DT_USB			1
#define DT_NET			2	
#define DT_UDP			6	
#define DT_GIGA			7

#define DT_SERIAL		0
#define DT_FILE			3
#define DT_IMGOBJ		4


//Return Msg
#define STX		'['
#define ETX		']'
#define	REV_BUF_SIZE 200

#define USB_PACKET_BUF_SIZE		0x10000//0x10000   //128*512//BufSize must be multiple of the 512
#define NET_PACKET_BUF_SIZE		0x8000 
#define GIGA_PACKET_BUF_SIZE	0x40000			   //Max size is 8192*2*16
#define PACKET_NUM				256 
#define GIGA_PACKET_NUM			128
#define GIGA_BLOCK_HEIGHT		16				   //The block height of gigbit image
#define DT_USB_COMMIT_NUM		4
#define SINK_SIZE				10
#define MAX_ADC					0xFFFFF
#define FRAME_BUF_NUM			32				   //The buffer number used in DTFrameBufFactory

#define DTP_HEAD_LEN			6
#define DTP_HEAD_LEN_UDP		8
#define DTP_HEAD_LENSEG_LEN		3
#define DT_USB_PXIEL_NUMBER		2048
#define DT_USB_PACKET_LEN		2048	

#define DTP_MAX_LEN				(8192*2+6)

#define DT_DISPPLAY_REFRESH_INTERVAL 30 //ms
//Calibration Type
#define DT_GAIN					0
#define DT_OFFSET				1
#define DT_BASELINE				2
#define DT_MIN					0
#define DT_MEAN					1
#define DT_MAX					2
#define DT_FRAME_UNIT			32

#define DT_MAX_USB_PACKET_SIZE 0x6C00

#define FILE_BUFFER_SIZE	0x4000
#define DT_MAX_CANIS_MODULE_NUMBER	64
#define WM_DISPLAY_REFRESH		0x10000		

#define  NM_SLIDER_MOVE WM_USER+1000
#define  NM_EXTEND_CHANGE WM_USER+1001


typedef unsigned * PBEGINTHREADEX_THREADID;
typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC) (LPVOID lpThreadParameter);


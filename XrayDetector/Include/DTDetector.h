// DTDetector.h : Declaration of the CDTDetector
#pragma once

#include "DTConst.h"

class   CCyUSBDevice;
class	CCyControlEndPoint;
class	CSocket;
class	CSerial;
//class	GigabitDevice;

class _declspec(dllexport) CDTDetector 
{
public:
	CDTDetector();
	~CDTDetector();
private:
	BYTE m_CardType;
	char m_RemoteIP[20];
	ULONG m_SerialPortNum;
	BOOL m_bIsOpened;	
	ULONG m_CmdPort;
	ULONG m_LastErrID;
	ULONG m_BaudRate;
	ULONG m_TimeOut;
	ULONG m_UsbPort;
	BOOL m_bIsLUGAS;
	WORD WValue[100];
	BYTE m_Length ;
	BOOL m_bLugasCheck;
	BYTE CheckBuf[64];
	LONG m_DCULineTrigger;
	BOOL m_bPing;
	BOOL m_bEnableLog;

	CRITICAL_SECTION m_CmdSection;

	CCyUSBDevice		*m_pDTUSBDevice;
	CCyControlEndPoint  *m_pCtlEndPt;
	CSocket				*m_pSocket;
	CSerial				*m_pSerial;
	//GigabitDevice		*m_pGigaDevice;
	HANDLE				m_hPingThread;
	

	ULONG				m_GigInterfaceNum;
	ULONG				m_GigDeviceNum;

private:
	//Define copy constructor and operator as private
	CDTDetector(const CDTDetector&);
	CDTDetector& operator = (const CDTDetector&);
	
	BOOL SendUSBData(CHAR* pStrCmd);
	BOOL RevUSBData(CHAR* pStrRT);
	int SendNETData(CHAR* pStrCmd);
	int RevNETData(CHAR* pRT,WORD BuffSize = REV_BUF_SIZE);
	int	SendSerialData(CHAR* pStrCmd);
	int RevSerialData(CHAR* pRT,WORD BuffSize = REV_BUF_SIZE);
	BOOL TestConnection();
	int RevFirstNETData(CHAR* pRT,WORD BuffSize = REV_BUF_SIZE);	//For X-DCU
	//Thread function
	static DWORD WINAPI ThreadFunc(LPVOID param);
	DWORD ThreadMemberFunc();

	ULONG GetNumber(const CHAR* pSrc, LONG Index);
	void (*fOnDTError)(LONG, CHAR*);				//The callback function for net connectionlost

	//int SendGigaData(CHAR* pStrCmd);
	//int RevGigaData(CHAR* pRT, WORD BuffSize = REV_BUF_SIZE);
public:
	const char* GetIPAddress() const;
	void PutIPAddress(const char* newVal);
	ULONG GetCmdPort() const;
	void PutCmdPort(ULONG newVal);
	ULONG GetSerialPort() const;
	void PutSerialPort(ULONG newVal);
	BOOL GetIsOpened() const;
	ULONG GetLastErrID() const;
	ULONG GetChannelType() const;
	void PutChannelType(ULONG newVal);
	char* GetErrMsg(LONG ErrID) const;
	BOOL GetIsLUGAS() const;
	ULONG GetBaudRate() const;
	void PutBaudRate(ULONG newVal);
	ULONG GetCmdTimeOut() const;
	void PutCmdTimeOut(ULONG newVal);
	ULONG GetUsbPort() const;
	void PutUsbPort(ULONG newVal);	
	LONG GetDCULineTrigger() const;
	
	BOOL Open();
	void Close();	
	BOOL SendCommandA(CHAR* Cmd, CHAR* pRT);	
	BOOL StartPing(ULONG nInterval);
	void StopPing();
	void SetCallback(void (*OnDTError)(LONG, CHAR*)); //Set the callback funciton

	//For GigE device
	/*GigabitDevice* GetGigaDevice();
	ULONG GetGigaInterfaceNum();
	void  PutGigaInterfaceNum(ULONG newVal);
	ULONG GetGigaDeviceNum();
	void  PutGigaDeviceNum(ULONG newVal);*/

	BOOL GetEnableLog();
	void PutEnableLog(BOOL newVal,const char* pFileName = "DTLog.dat");

};


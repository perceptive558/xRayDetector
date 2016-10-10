// DTImage.h : Declaration of the CDTImage
#pragma once
#include "DTDetector.h"
#include "EventmanagerIF.h"


class CDTFrameBufFactory;
class CDTPPacketFactory;
class CDTPQueueFactory;
class CDTDataSrcFactory;
class CImageObjectFactory;
//class CDTEventManagerIF;
class CDTDataSrcIF;
class CDTDetector;
class CDTFrameBufIF;
class CDTPQueueIF;
class CImageObject;
//class GigabitDevice;


class _declspec(dllexport) CDTImage : public CDTEventManagerIF
{

public:
	CDTImage();
	virtual ~CDTImage();
public:
	ULONG GetImgHeight() const;
	void PutImgHeight(ULONG newVal);
	ULONG GetImgWidth() const;
	void PutImgWidth(ULONG newVal);
	ULONG GetBytesPerPixel() const;
	void PutBytesPerPixel(ULONG newVal);
	ULONG GetChannelType() const;
	void PutChannelType(ULONG newVal);
	ULONG GetImagePort() const;
	void PutImagePort(ULONG newVal);
	CDTDetector* GetDetectorObject() const;
	void PutDetectorObject(CDTDetector* pDetector);
	BOOL GetIsOpened() const;
	ULONG GetLastErrID() const;	
	ULONG GetSubFrameHeight() const;
	void PutSubFrameHeight(ULONG newVal);
	BOOL GetIsGrabing() const;
	CImageObject* GetImageObject() const;
	CDTImage* GetObjectHandle() const;
	ULONG GetTimeOut() const;
	void PutTimeOut(ULONG newVal);
	ULONG GetExFrameTrigger();
	void PutExFrameTrigger(ULONG newVal);
	
	BOOL Save(const CHAR* FilePath);	
	BOOL StartGrabEngine();
	void TrigerOneGrab(void);
	
	ULONG GetLostStartID() const;
	ULONG GetStreamPixelPerBytes() const;
	void PutStreamPixelPerBytes(ULONG newVal);
	ULONG GetDualEnergyMode() const;
	void PutDualEnergyMode(ULONG newVal);
	/*ULONG GetGigaHOffset();
	void PutGigaHOffset(ULONG newVal);*/

	void OnDTEvent(BYTE EventID,BYTE EventContent);
	void OnDTFrameReady();
	void OnDTSubFrameReady(LONG  RowID, LONG NumLines,BOOL bLastFrame);
	void OnDTError(LONG ErrID, CHAR* ErrInfo);
	
	BOOL Open();
	void Close(void);
	BOOL Grab(ULONG FrameNum);
	BOOL Snap();
	void Stop(void);

	BOOL SetDirectFlag(ULONG Flag);

	void SetCallBack(void(*OnEvent)(BYTE, BYTE), void(*OnError)(LONG, CHAR*), 
		             void(*OnFrameReady)(), void(*OnSubFrameReady)(LONG, LONG, LONG, BOOL)=NULL);
	void EnOffGainOffset(BOOL bGain, BOOL bOffset);
	//For loopback process
	BOOL* GetModeCheck();
	BOOL* GetGainCheck();
	BOOL* GetConnectCheck();
	BOOL* GetModeChangeCheck();
	BOOL   CreateLoopbackParameters(LONG ModuleNum);
	void   DeleteLoopbackParameters(void);
	void   SetLoopbackParameters(LONG ArrayIndex, LONG ModuleAddr, LONG TargetGainLE, LONG TargetGainHE);
	BOOL ConfLoopback(LONG ModuleNum, LONG LoopTimes);

	float GetDualRegShift();
	void SetDualRegShift(float shift);
private:
	CDTImage(const CDTImage&);
	CDTImage& operator =(const CDTImage&);
	
	BOOL AllocateFactory();
	void FinalRelease();
	void ResetAllDTPPacket();
	void SetFrameTrigger();
	void SendFrameTrigger();
	BOOL CanEnterOperation();	

	friend class CNETDataSrc;
	friend class CUSBDataSrc;
private:
	BOOL	m_bGrabSnapCalling;
	WORD	m_ImagePort;
	ULONG	m_Timeout;
	ULONG	m_ImageHeight;
	ULONG	m_ImageWidth;
	BOOL	m_bIsOpened;
	ULONG	m_CardType;
	ULONG	m_BytesPerPixel;	
	ULONG	m_LastErrID;
	ULONG	m_SubFrameHeight;
	BOOL	m_bExFrameTrigger;	
	BOOL	m_bDebugMode ;
	ULONG   m_bDualEnergy;
	ULONG    m_StreamBytes;
	ULONG	m_FrameNum;
	ULONG	m_CurFrameCount;
	HANDLE	m_hAllFrameSended;
	BOOL	m_bFactoryAllocateResult;
	//ULONG	m_GigaHOffset;
	

	CDTFrameBufFactory*		m_pFrameBufFactory;
	CDTPPacketFactory*		m_pDTPPacketFactory;
	CDTPQueueFactory*		m_pDTPQueueFactory;
	CDTDataSrcFactory*		m_pDTDataSrcFactory;
	CImageObjectFactory*	m_pImageObjectFactory;
	//CDTEventManagerIF*		m_pEventManager;
	CRITICAL_SECTION		m_GrabEnterSection;

	CDTDataSrcIF*	m_pDataSrc;
	CDTDetector*	m_pDetector;
	//void*			m_pDisplay;
	CDTFrameBufIF*	m_pFrameBuf;
	CDTPQueueIF*	m_pDTPQueue;
	CImageObject*	m_pImageObject;
	//GigabitDevice*  m_pGigaDevice;

	BOOL* m_pModeCheck;
	BOOL* m_pGainCheck;
	BOOL* m_pConnectCheck;
	BOOL* m_pModeChangeCheck;
	ULONG* m_pDMAddr;
	ULONG* m_pTargetGainLE;
	ULONG* m_pTargetGainHE;

	//Callback function
	void (*fOnDTEvent)(BYTE EventID,BYTE EventContent);
	void (*fOnDTError)(LONG ErrID, CHAR* ErrInfo); 
	void (*fOnDTFrameReady)();
	void (*fOnDTSubFrameReady)(LONG LeftBlocks, LONG RowID, LONG NumLines,BOOL bLastFrame);  

};

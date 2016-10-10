// ImageObject.h : Declaration of the CImageObject

#pragma once
#include "DTImage.h"
#include "DTCommander.h"
class CDTFrameBuf;
class CDTFrameBufOpt;
class _declspec(dllexport) CImageObject 
{
public:
	CImageObject();
	~CImageObject();

public:	
	
	const ULONG_PTR GetImageDataAddress() const;
	
	ULONG GetWidth() const;
	ULONG GetHeight() const;
	ULONG GetBytesPerPixel() const;
	ULONG GetPixel(ULONG X, ULONG Y) const;
	void PutPixel(ULONG X, ULONG Y, ULONG newVal);
	const ULONG_PTR GetImageLineAddress(ULONG LineIndex) const;
	
	void ColAverage(ULONG** pAvg);	
	void ColMax(ULONG** pMax);
	void ColMin(ULONG** pMin);
	void RowMin(ULONG** pMin);
	void RowMax(ULONG** pMax);
	void RowAverage(ULONG** pAvg);
	void ColNoise(FLOAT** pNoise);
	void RowNoise(FLOAT** pNoise);
	void DoStatistical(ULONG CalType);

private:
	CImageObject(const CImageObject& );
	CImageObject& operator=(const CImageObject&);
	
	void FinalRelease();
	void PutImageDataAddress(ULONG_PTR newVal);
	BOOL AllocateBuffer();
	void ReleaseBuffer();
	BOOL Open(ULONG Width, ULONG Height, ULONG BytesPerPixel);
	void PutGain(ULONG i, FLOAT newVal);
	float GetGain(ULONG i);
	void PutOffset(ULONG i, ULONG newVal);
	ULONG GetOffset(ULONG i);
	void Calibrate(void);
	void EnableGainOffset(BOOL enGain, BOOL enOffset);
	
	friend CDTImage;
	friend CDTFrameBuf;
	friend CDTFrameBufOpt;
	friend CDTCommander;
private:
	ULONG m_Width;
	ULONG m_Height;
	ULONG  m_BytesPerPixel;
	ULONG* m_XTable;
	ULONG* m_YTable;
	BYTE* m_DataBase;	
	ULONG* m_ColAvg;
	ULONG* m_ColMax;
	ULONG* m_ColMin;
	float* m_ColNoise;
	ULONG* m_RowAvg;
	ULONG* m_RowMax;
	ULONG* m_RowMin;
	float* m_RowNoise;

	ULONG* m_pOffset;	//The offset value array
	float* m_pGain;		//The gain value array
	BOOL m_bGain;		//The gain enable flag	
	BOOL m_bOffset;		//The offset enable flag
};


#pragma once

class _declspec(dllexport)CDTEventManagerIF
{
public:
	CDTEventManagerIF(void);
	virtual ~CDTEventManagerIF(void);
	virtual void OnDTEvent(BYTE EventID,BYTE EventContent)=0;
	virtual void OnDTError(LONG ErrID, CHAR* ErrInfo) =0; 
	virtual void OnDTFrameReady()=0;
	virtual void OnDTSubFrameReady(LONG  RowID, LONG NumLines,BOOL bLastFrame)=0;
	//virtual void OnNewDataComimg(ULONG Size)=0;
};

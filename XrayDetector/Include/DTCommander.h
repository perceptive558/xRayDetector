// DTCommander.h : Declaration of the CDTCommander
#pragma once

#include "DTdetector.h"
#include "DTImage.h"
//#include "ImageObject.h"
class _declspec(dllexport) CDTCommander 
{
public:

	CDTCommander();
	~CDTCommander();

public:
	CDTDetector* GetDetectorObject() const;
	void PutDetectorObject(CDTDetector* pDetector);
	BOOL OffBoardCalibration(ULONG CalibrationType, CDTImage* pDTImage, ULONG StartPixel, ULONG EndPixel, ULONG TargetValue=1);
	BOOL SaveOffGainOffset(char* pFile,CDTImage* pDTImage);
	BOOL LoadOffGainOffset(char* pFile,CDTImage* pDTImage);
	BOOL EnOffGainOffset(BOOL bGain, BOOL bOffset, CDTImage* pDTImage);
	
	BOOL GetGain(ULONG Index, FLOAT* pVal);
	BOOL PutGain(ULONG Index, FLOAT newVal);
	BOOL GetOffset(ULONG Index, ULONG* pVal);
	BOOL PutOffset(ULONG Index, ULONG newVal);
	BOOL GetCorrectionGain(BOOL* pVal);
	BOOL PutCorrectionGain(BOOL newVal);
	BOOL GetCorrectionOffset(BOOL* pVal);
	BOOL PutCorrectionOffset(BOOL newVal);
	BOOL GetCorrectionBaseline(BOOL* pVal);
	BOOL PutCorrectionBaseline(BOOL newVal);
	BOOL GetBaseline(ULONG* pVal);
	BOOL PutBaseline(ULONG newVal);
	BOOL GetIntegrationTime(ULONG* pVal);
	BOOL PutIntegrationTime(ULONG newVal);
	BOOL GetAverageFilter(ULONG* pVal);
	BOOL PutAverageFilter(ULONG newVal);
	BOOL GetDataPattern(BOOL* pVal);
	BOOL PutDataPattern(BOOL newVal);
	//BOOL GetFrameTrigger(LONG* pVal);
	//BOOL PutFrameTrigger(LONG newVal);
	BOOL GetLineTrigger(BOOL* pVal);
	BOOL PutLineTrigger(BOOL newVal);
	BOOL Initialize(BOOL Mode);
	BOOL SaveOffset(void);
	BOOL SaveGain(ULONG Index);
	BOOL LoadOffset(void);
	BOOL LoadGain(ULONG Index);
	BOOL GetOutputBits(ULONG* pVal);
	BOOL PutOutputBits(ULONG newVal);
	BOOL SendExFrameTrigger(void);
	//BOOL GetFrameOut(LONG* pVal);
	//BOOL PutFrameOut(LONG newVal);
	/*BOOL GetImageOutputType(ULONG* pVal);
	BOOL PutImageOutputType(ULONG newVal);*/
	BOOL GetSensitivityLevel(ULONG* pVal);
	BOOL PutSensitivityLevel(ULONG newVal);
	BOOL GetSensitivityLevelLG(ULONG Channel,ULONG* pVal);
	BOOL PutSensitivityLevelLG(ULONG Channel,ULONG newVal);
	BOOL GetSumLines(ULONG* pVal);
	BOOL PutSumLines(ULONG newVal);
	BOOL GetChannelType(ULONG Channel, ULONG* pVal);
	BOOL PutChannelType(ULONG Channel, ULONG newVal);
	//BOOL WriteDataPattern(LONG Base, LONG Step);
	BOOL GetIsOpened() const;
	BOOL GetModuleTypeName(char** pVal);
	BOOL SaveCurrentStatus(void);
	BOOL GetPixelSize(FLOAT* pVal);
	BOOL GetPixelNumber(ULONG* pVal);
	BOOL GetModuleType(ULONG* pVal);
	BOOL GetMaxIntTime(ULONG* pVal);
	BOOL GetMinIntTime(ULONG* pVal);
	BOOL GetOverallGain(FLOAT* pVal);
	BOOL PutOverallGain(FLOAT newVal);
	BOOL GetStartPixel(ULONG* pVal);
	BOOL PutStartPixel(ULONG newVal);
	BOOL GetEndPixel(ULONG* pVal);
	BOOL PutEndPixel(ULONG newVal);

	BOOL GetLinacSource(ULONG* pVal);
	BOOL PutLinacSource(ULONG newVal);
	BOOL GetFrameSource(ULONG* pVal);
	BOOL PutFrameSource(ULONG newVal);

	BOOL OnBoardOffsetCalibration(void);
	BOOL OnBoardGainCalibration(ULONG TargetValue);
	BOOL ResetGain(void);
	BOOL ResetOffset(void);
	BOOL GetFPGAVersion(ULONG* pVal);
	BOOL GetFPGABuild(ULONG* pVal);
	BOOL GetMCUVersion(ULONG* pVal);
	
	BOOL GetSerialNumber(char** pVal);

	
	BOOL GetTemperature(ULONG* pVal);
	BOOL GetFECardsNum(ULONG* pVal);
	BOOL GetFEMode(ULONG* pVal);
	BOOL GetDataPatternMode(ULONG* pVal);
	BOOL PutDataPatternMode(ULONG newVal);
	BOOL GetExFrameTriggerDelay(ULONG* pVal);
	BOOL PutExFrameTriggerDelay(ULONG newVal);
	BOOL GetSHTriggerDelay(ULONG* pVal);
	BOOL PutSHTriggerDelay(ULONG newVal);

	BOOL GetOperationMode(ULONG* pVal);
	BOOL PutOperationMode(ULONG newVal);

	BOOL GetTestMode(ULONG address,ULONG* pVal);
	BOOL PutTestMode(ULONG address,ULONG newVal);

	BOOL GetNoContinueIntegrationTime(ULONG* pVal);
	BOOL PutNoContinueIntegrationTime(ULONG newVal);

	BOOL GetMovingOffsetFilter(ULONG* pVal);
	BOOL PutMovingOffsetFilter(ULONG newVal);
	
	BOOL GetTestPulseNumber( ULONG* pVal);
	BOOL PutTestPulseNumber(ULONG newVal);

	BOOL GetPixelMovingAvg( ULONG* pVal);
	BOOL PutPixelMovingAvg(ULONG newVal);

	BOOL PutDMAverageFilter(ULONG newVal);
	BOOL LoadDMAverageFilter(void);
	BOOL SaveDMAverageFilter(void);
	BOOL GetDMAverageFilter(ULONG* pVal);
	BOOL PutIFTestMode(BOOL newVal);
	BOOL GetTriggerRawDelay( ULONG* pVal);
	BOOL PutTriggerRawDelay(ULONG newVal);
	BOOL PutDMTestMode(ULONG address,ULONG newVal);
	BOOL PutDetectorType(ULONG newVal);
	BOOL LoadDetectorType(void);
	BOOL SaveDetectorType(void);
	BOOL GetDetectorType(ULONG* pVal);
	BOOL PutASICType(ULONG newVal);
	BOOL LoadASICType(void);
	BOOL SaveASICType(void);
	BOOL GetASICType(ULONG* pVal);
	BOOL PutDelaySweepMode(ULONG newVal);
	BOOL GetDelaySweepMode(ULONG* pVal);	
	BOOL PutModuleGain(ULONG ModuleIndex, ULONG EnergyMode, ULONG SensitivityValue);
	//BOOL GetModuleGain(ULONG* pVal);
	BOOL GetHighLowMode(ULONG* pVal);
	BOOL PutHighLowMode(ULONG newVal);

private:
	CDTDetector*	m_pDetector;
	CHAR			m_Command[100];
	CHAR			m_Return[REV_BUF_SIZE];
	//FI command
	ULONG			m_FPGA_Version;
	ULONG			m_FPGA_Build;
	//RI Command
	ULONG			m_MCU_Version;
	ULONG			m_PixelNumber;
	float			m_PixelSize;
	ULONG			m_ModuleType;	
	ULONG			m_MaxIntTime;
	ULONG			m_MinIntTime;
	BOOL			m_bRetrieved;        //Flag to indicate whether there is RI Called
	ULONG			m_FEBoardNum;
	ULONG			m_FEMode;
	ULONG			m_PatternMode;
	ULONG			m_ExFrameDealy;
	char			m_ModuleName[20];
	char			m_SerialNum[20];
	BYTE			m_TestMode[DT_MAX_CANIS_MODULE_NUMBER];
	CRITICAL_SECTION m_CalibrationCritical;
	friend CDTImage;
private:
	CDTCommander(const CDTCommander&);
	CDTCommander& operator=(const CDTCommander&);

	ULONG Min(ULONG* pData,ULONG Width);
	ULONG Mean(ULONG* pData,ULONG Width);
	ULONG Max(ULONG* pData,ULONG Width);
	static ULONG GetNumber(CHAR* pSrc ,ULONG Index);
	LONG GetString(CHAR* pSrc ,ULONG Index,CHAR* pDest);
	BOOL RetrieveSysinfo(void);
};


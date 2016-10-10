#pragma once


#define MAX_VALUE_8		0xFF
#define MAX_VALUE_10	0x3FF
#define MAX_VALUE_12	0xFFF
#define MAX_VALUE_14	0x3FFF
#define MAX_VALUE_16	0xFFFF	
#define MAX_VALUE_20	0xFFFFF


//Map the pixel value to 8-bits
struct Windowing
{
	float             gamma;
	unsigned int	  start;
	unsigned int      end;
	unsigned int	  max;
};
//DDraw
class CDDrawWin;

class _declspec(dllexport) CDTDisplay
{
public:
	CDTDisplay(void);
	~CDTDisplay(void);

	ULONG	GetImageWidth() const;
	ULONG	GetImageHeight() const;
	ULONG	GetPixelBits() const;
	void	PutImgDataAddress(BYTE* pData);
	ULONG	GetLastErrorID() const;
	void	PutGammaVal(float newVal);
	float	GetGammaVal() const;
	
	
	BOOL	Open(ULONG imageWidth, ULONG imageHeight, ULONG pixelDepth,HWND hWnd, int colorMode=0);
	void	Close();
	BOOL	GetIsDDrawEnable();
	BOOL	GetIsOpened() const;
	void	Display();

private:
	CDTDisplay(const CDTDisplay&);
	CDTDisplay& operator =(const CDTDisplay&); 

	void	GetDisplayFrame();
	ULONG	GetPixValue(const unsigned int x, const unsigned int y);
	void	InvertDisplayImg();
	void	DDrawDisplay();
	void	AutoSetWindowing();
	BOOL	CreateBMI(int colorMode);
	BOOL	CreateLUT();

private:
	ULONG	m_imgWidth;
	ULONG	m_imgHeight;
	ULONG	m_pixelByte;

	BYTE	*m_pImgBuf;			//the raw image data
	BITMAPINFO* m_pBMI;			//image head			
	BYTE	*m_pImgDisplay;		//8 bits image data
	BYTE	*m_pImgDisInvert;	//8 bits inverted image data for display	
	HWND	m_hWnd;				//image show window;
	CDDrawWin*	m_pDDraw;		//ddraw show
	BOOL	m_bOpened;
	BYTE*	m_pLUT;			//for windowing,turn the raw data into 8bit 
	UINT	m_pixelBits;	//the bits per pixel

	Windowing m_windowing;	
	ULONG	m_lastErrID;

};

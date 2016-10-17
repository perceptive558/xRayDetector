#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MaxNo   65535
#define MyConfig "./MyConfig.conf"

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include <QDesktopServices>
#include <QProcess>
#include <connection.h>
#include <QSqlQueryModel>
#include <QTimer>

#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QFileDialog>
#include <QtPrintSupport/QPageSetupDialog>
#include <QtPrintSupport/qprinter.h>
#include <Windows.h>
#include <QSettings>
#include <mymessage.h>

#include <seriallink.h>

#include "Include\DTDetector.h"
#include "Include\DTImage.h"
#include "Include\DTCommander.h"
#include "Include\DTDisplay.h"
#include "Include\ImageObject.h"

#include <Halcon.h>
#include <HalconCpp.h>
using namespace Halcon;


struct printDorcument{
    QString productName;
    QString startTime;
    QString endTime;
    int No;
    int NG;
};

struct selectProduct{
    QString name;
    int productNo;
    int threshold1;
    int threshold2;
    int erodeSize;

    QString date;
    QString startTime;
    QString endTime;
    int No;
    int NG;
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSqlQueryModel *model;

    void initialization();
    void printSelectChange();

    QString printStartTime;
    QString printEndTime;

    QString prepareForALL();
    QString prepareForEach();

    QString printEdit(printDorcument myDorcument);

    int printFlag;

    void initHalconWindows();

    HTuple m_HWindowID1;//show1
    HTuple m_HWindowID2;//show2
    HTuple m_HWindowID3;//show3
    HTuple m_HWindowID4;//show4
    HTuple m_HWindowID5;//show5

    HTuple m_HWindowIDTest;//showTest

    HTuple m_HWindowNEW;//NEW

    selectProduct presentDetectItems;
    printDorcument PresentDetection;

    void saveSettings();
    void loadSettings();

    int presentSelectID;//当前选中的检测编号
    int nextSelectID;//当前选中的检测编号

    bool FlagInspect;

    QDateTime _time1;
    QDateTime _time2;

    QProcess inputProcess;

    void Readregedit();

    SerialLink myLink;
    void initialSerialLink();

    void _processData();

    //***探测器相关定义及函数********************************************
    int				m_DetectorType;
    int				m_PseudoColor;
    int				m_DataLost;
    int				m_FrameCount;
    CDTDetector		m_Detector;
    CDTImage		m_Image;
    CDTCommander	m_Commander;
    CImageObject*	m_pImgObject;

    ULONG			m_ImgWidth;
    ULONG			m_ImgHeight;
    ULONG			m_BytesPerPixel;
    ULONG			m_ImgSize;
    BYTE*			m_pImgBuf;		//For image processing
    FLOAT			m_Gamma;
    BOOL			m_bDataPattern;
    UINT			m_IntTime;
    QString			m_Cmd;
    char			m_RevCmd[REV_BUF_SIZE];


    void OnBnClickedEnableDatapattern();
    void OnBnClickedIntTime();
    void OnClose();
    void OnBnClickedSendCmd();


    //***************************************************************

    int _count;

   // void saveImages(bool ok,)
    HTuple imageID;
    QTimer DT_timer;

private:
    Ui::MainWindow *ui;

    QTimer show_time;
    QTimer start_time;
    QTimer serial_time;
    QTimer global_time;


    bool isBoot;

signals:
    void explains(); //如果要自定义槽和信号, explains信号是必须的
    void revertPhone(unsigned int ret, QString password);

    void OnEventSignal(BYTE ID, BYTE Num);
    void OnErrorSignal(LONG ID, CHAR* Info);
    void OnFrameReadySignal();

private slots:
    void show_present_time();
    void on_NewDetect_clicked();
    void on_SelectDetect_clicked();
    void on_SystemSetting_clicked();
    void on_SystemInspect_clicked();
    void on_PrintingResult_clicked();

    void on_ShowImages_clicked();
    void on_operation_level_clicked();
    void on_shutDownSystem_clicked();
    void on_forceTransport_clicked();
    void on_startInspect_clicked();
    void on_home_page_clicked();
    void on_calendarWidget_startDate_selectionChanged();
    void on_calendarWidget_endDate_selectionChanged();
    void on_PrintResults_clicked();
    void on_saveAsPresent_clicked();
    void on_detectItems_clicked(const QModelIndex &index);
    void on_refreshDetectItem_clicked();
    void on_ON_OFF_clicked();
    void on_lineEdit_selectionChanged();
    void on_confirm_clicked();

    void on_pushButton_open_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_grab_clicked();
    void on_pushButton_stop_clicked();
    void on_pushButton_snap_clicked();
    void on_pushButton_onboardcalibration_clicked();
    void on_pushButton_loadcalibration_clicked();

    void OnRevertPhone(unsigned int ret, QString phone);

    void OnEventMsg(BYTE ID, BYTE Num);
    void OnErrorMsg(LONG ID, CHAR* Info);
    void OnFrameReadyMsg();
    void on_pushButton_onboardcalibration_2_clicked();

    void shutDownSystem();
    void startingSystem();
    void on_DT_setting_clicked();
};

#endif // MAINWINDOW_H

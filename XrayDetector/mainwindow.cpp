#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <Windows.h>
#include <securitybaseapi.h>
#include <WinBase.h>
#include <winnt.h>

#include <QMargins>
#include <QTextCodec>
#include <iostream>  //.h去掉
#include <string.h> //添加这句
#include <stdlib.h>
#include <QColor>

using namespace Halcon;

MainWindow *UI;

//Define the call back funcitons
void OnDTEvent(BYTE ID, BYTE Num)
{
    emit UI->OnEventMsg(ID, Num); //发送自定义信号
}
void OnDTError(LONG ID, CHAR* Info)
{
    emit UI->OnErrorMsg(ID, Info); //发送自定义信号
}
void OnFrameReady()
{
//    UI->OnFrameReadyMsg(); //发送自定义信号

    qDebug() << "Images In!!! \n" << endl;

    emit UI->revertPhone(1,"");


    //QTimer::singleShot(1,UI,UI->OnFrameReadyMsg());

    //UI->DT_timer.singleShot(0,UI->OnFrameReadyMsg());
/*
//    if(UI->m_Image.GetIsOpened())
//        UI->m_Image.Stop();
    if(UI->m_pImgBuf)
    {
        memcpy(UI->m_pImgBuf, (BYTE*)UI->m_pImgObject->GetImageDataAddress(), UI->m_ImgSize);
        //Process with the image buffer

        unsigned char *Pointer = (unsigned char *)UI->m_pImgBuf;
        Hobject ImageInput;
        int width = (int)UI->m_ImgWidth;
        int height = (int)UI->m_ImgHeight;

        gen_image1_extern(&ImageInput, "byte",  width, height,(long)Pointer, NULL);
        mirror_image(ImageInput,&ImageInput,"row");
        disp_image(ImageInput,UI->m_HWindowIDTest);

        write_image(ImageInput, "bmp", 0, UI->imageID +".bmp");
        UI->imageID = UI->imageID + 1;

        qDebug() << "Images Get!!! \n" << endl;
    }
*/
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    FlagInspect(false),
    m_DetectorType(0),
    m_FrameCount(0),
    m_pImgBuf(NULL),
    m_PseudoColor(0),
    m_ImgWidth(1280),
    m_ImgHeight(600),
    m_BytesPerPixel(2),
    m_ImgSize(1280*600*2),
    m_Gamma(1.0f),
    m_bDataPattern(false),
    m_IntTime(0),
    m_Cmd(""),
    imageID(1),
    ui(new Ui::MainWindow)
{
    UI = this;

    ui->setupUi(this);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);

    connect(&show_time, &QTimer::timeout, this, &MainWindow::show_present_time);
    show_time.start();

    //connect(&start_time, &QTimer::timeout, this, &MainWindow::on_startInspect_clicked);
    //start_time.start(10000);

    connect(&serial_time, &QTimer::timeout, this, &MainWindow::_processData);

    isBoot = false;

    ui->SelectDetect->setStyleSheet("border:2px groove green;"
                                    "border-radius:40px;"
                                    "padding:2px 4px;"
                                    "font: 60 14pt '华文仿宋';"
                                    "background:url(://res/bg_win1024x768.png)");
    ui->SystemInspect->setStyleSheet("border:2px groove green;"
                                    "border-radius:40px;"
                                    "padding:2px 4px;"
                                     "font: 60 14pt '华文仿宋';");
    ui->NewDetect->setStyleSheet("border:2px groove green;"
                                    "border-radius:40px;"
                                    "padding:2px 4px;"
                                    "font: 60 14pt '华文仿宋';");
    ui->PrintingResult->setStyleSheet("border:2px groove green;"
                                    "border-radius:40px;"
                                    "padding:2px 4px;"
                                    "font: 60 14pt '华文仿宋';"
                                    "background:url(://res/print.png)");
    ui->ShowImages->setStyleSheet("border:2px groove green;"
                                    "border-radius:40px;"
                                    "padding:2px 4px;"
                                    "font: 60 14pt '华文仿宋';"
                                    "background:url(://res/image.png)");
    ui->SystemSetting->setStyleSheet("border:2px groove green;"
                                    "border-radius:40px;"
                                    "padding:2px 4px;"
                                    "font: 60 14pt '华文仿宋';"
                                     "background:url(://res/setting.png)");

    ui->shutDownSystem->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;");
    ui->forceTransport->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;");
    ui->startInspect->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;");
    ui->operation_level->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;");
    ui->home_page->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;");

    createConnection();
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("memory.db");
//    if (!db.open()) {
//        qDebug() << "cannot connect to database";
//    }
    initialization();

    //Readregedit();

    connect(this, SIGNAL(revertPhone(unsigned int, QString)), this, SLOT(OnRevertPhone(unsigned int, QString)), Qt::QueuedConnection);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::show_present_time()
{
    ui->present_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if (FlagInspect)
    {
        _time2 = QDateTime::currentDateTime();

        int _Second = _time1.msecsTo(_time2)/1000;//秒
        int _hour   = _Second/3600;
        int _mint   = _Second%3600/60;
        int _second = _Second%3600%60;
        QString str;
        if (_hour < 10)
            str += "0" + QString::number(_hour,10) + ":";
        else
            str += QString::number(_hour,10) + ":";

        if (_mint < 10)
            str += "0" + QString::number(_mint,10) + ":";
        else
            str += QString::number(_mint,10) + ":";

        if (_second < 10)
            str += "0" + QString::number(_second,10);
        else
            str += QString::number(_second,10);

        //    QString str = (_hour  <10)?"0":"" + QString::number(_hour,10) + ":" +
        //                  (_mint  <10)?"0":"" + QString::number(_mint,10) + ":" +
        //                  (_Second<10)?"0":"" + QString::number(_second,10);
        ui->label_onTime->setText(str);
    }
}

void MainWindow::on_NewDetect_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_SelectDetect_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    model = new QSqlQueryModel;

    model->setQuery("select * from detectItems");
    model->setHeaderData(0, Qt::Horizontal, QStringLiteral("检测名称"));
    model->setHeaderData(1, Qt::Horizontal, QStringLiteral("产品数量"));
    model->setHeaderData(2, Qt::Horizontal, QStringLiteral("阈值1"));
    model->setHeaderData(3, Qt::Horizontal, QStringLiteral("阈值2"));
    model->setHeaderData(4, Qt::Horizontal, QStringLiteral("腐蚀面积"));

    ui->detectItems->setModel(model);
    ui->detectItems->resizeColumnsToContents();
    ui->detectItems->setSelectionBehavior(QAbstractItemView::SelectRows);

    QAbstractItemModel *myModel = ui->detectItems->model();
    int ROW = myModel->rowCount(myModel->index(MaxNo,0));
    for (int i = 0; i < ROW; i++)
    {
        QString detectName = myModel->data(myModel->index(i,0)).toString();
        if (detectName == presentDetectItems.name)
        {
            presentSelectID = i;
            ui->detectItems->selectRow(presentSelectID);
            ui->detectItems->setFocus();
            break;
        }
    }
}

void MainWindow::on_SystemSetting_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_SystemInspect_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_PrintingResult_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);

}

void MainWindow::on_ShowImages_clicked()
{
    QDesktopServices::openUrl(QUrl("file:///D:/", QUrl::TolerantMode));

}

void MainWindow::on_operation_level_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}

void MainWindow::on_shutDownSystem_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);

    QStringList list;
    list<<"-s"<<"-t"<<"10";
    QProcess::startDetached("shutdown.exe",list);

    exit(0);
    //mymessage.show();
}

void MainWindow::on_forceTransport_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow::on_startInspect_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    //初始化检测界面图像
    initHalconWindows();
    start_time.stop();
}

void MainWindow::on_home_page_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    if (isBoot == false)
    {
        isBoot = true;
        start_time.stop();
    }
}

void MainWindow::on_calendarWidget_startDate_selectionChanged()
{
    QDate date = ui->calendarWidget_startDate->selectedDate();
    ui->label_startDate->setText(QStringLiteral("开始时间:")+date.toString("yyyy-MM-dd"));

    ui->calendarWidget_endDate->setMinimumDate(date);
    ui->calendarWidget_endDate->setMaximumDate(QDate::currentDate());

    printStartTime = date.toString("yyyyMMdd");

    printSelectChange();
}

void MainWindow::on_calendarWidget_endDate_selectionChanged()
{
    QDate date = ui->calendarWidget_endDate->selectedDate();
    ui->label_endDate->setText(QStringLiteral("结束时间:")+date.toString("yyyy-MM-dd"));

    ui->calendarWidget_startDate->setMaximumDate(date);

    printEndTime = date.toString("yyyyMMdd");

    printSelectChange();
}

void MainWindow::initialization()
{
    //打印界面初始化
    ui->calendarWidget_startDate->setSelectedDate(QDate::currentDate());
    ui->calendarWidget_endDate->setSelectedDate(QDate::currentDate());

    ui->label_startDate->setText(QStringLiteral("开始时间:") + QDate::currentDate().toString("yyyy-MM-dd"));
    ui->label_endDate->setText(QStringLiteral("结束时间:") + QDate::currentDate().toString("yyyy-MM-dd"));

    ui->calendarWidget_startDate->setMaximumDate(QDate::currentDate());

    ui->calendarWidget_endDate->setMinimumDate(QDate::currentDate());
    ui->calendarWidget_endDate->setMaximumDate(QDate::currentDate());

    printStartTime = QDate::currentDate().toString("yyyyMMdd");
    printEndTime = QDate::currentDate().toString("yyyyMMdd");

    ui->detectResults->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                                         "stop:0 #616161, stop: 0.5 yellow,stop: 0.6 green, stop:1 cyan);"
                                                         "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}"
                                                         "QHeaderView::section:checked{background-color: red;}");
    printSelectChange();

    //载入设置参数
    loadSettings();

    //初始化串口连接
    initialSerialLink();

}

void MainWindow::printSelectChange()
{
    model = new QSqlQueryModel;


    model->setQuery("select productName, Date, startTime, endTime, ProductNo, "
                    "NGProductNo from detectResults where "
                    "(Date >= " + printStartTime + ") and (Date <=" + printEndTime + ") order by Date ASC ");
    model->setHeaderData(0, Qt::Horizontal, QStringLiteral("产品名称"));
    model->setHeaderData(1, Qt::Horizontal, QStringLiteral("检测日期"));
    model->setHeaderData(2, Qt::Horizontal, QStringLiteral("开始时间"));
    model->setHeaderData(3, Qt::Horizontal, QStringLiteral("结束时间"));
    model->setHeaderData(4, Qt::Horizontal, QStringLiteral("总数量"));
    model->setHeaderData(5, Qt::Horizontal, QStringLiteral("非良品数量"));


    printFlag = model->rowCount(model->index(10,0));

    ui->detectResults->setModel(model);
    ui->detectResults->resizeColumnsToContents();
    ui->detectResults->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->detectResults->setAlternatingRowColors(true);

}

void MainWindow::on_PrintResults_clicked()
{
    if (printFlag > 0)
    {
        // 创建打印机对象
        QPrinter printer;

        QTextDocument doc;

        QFont font = doc.defaultFont();
        font.setBold(true);
        font.setPointSize(font.pointSize()+1);
        doc.setDefaultFont(font);

        QSizeF s = QSizeF(printer.logicalDpiX() * ( 58 / 25.4),
                          printer.logicalDpiY() * (2500 / 25.4));

        doc.setPageSize(s);
        printer.setOutputFormat(QPrinter::NativeFormat);

        QString html;
        html += QStringLiteral("<body align=\"center\"><font size=\"+2\">X射线检测记录</font></body>\n");
        html += "<body align=\"center\">**************************</body>\n";

        html += prepareForALL();

        if (ui->checkBox_print->isChecked())
        {
            html += prepareForEach();
        }

        html += "<body align=\"center\">**************************</body><br>\n";

        doc.setHtml(html);
        doc.print(&printer);
    }
}


QString MainWindow::prepareForALL()
{
    printDorcument myDorcument;

    model = new QSqlQueryModel;
    model->setQuery("select productName, Date, startTime, endTime, ProductNo, "
                    "NGProductNo from detectResults where "
                    "(Date >= " + printStartTime + ") and (Date <=" + printEndTime + ") order by Date ASC ");

    myDorcument.productName =   model->data(model->index(0,0)).toString();
    myDorcument.startTime   =   printStartTime;
    myDorcument.endTime     =   printEndTime;
    int rowCount = model->rowCount(model->index(MaxNo,0));
    for (int i = 0; i < rowCount; i++)
    {
        bool ok;
        myDorcument.No = myDorcument.No + model->data(model->index(i,4)).toString().toInt(&ok,10);
        myDorcument.NG = myDorcument.NG + model->data(model->index(i,5)).toString().toInt(&ok,10);
    }

    return printEdit(myDorcument);
}


QString MainWindow::prepareForEach()
{
    printDorcument myDorcument;
    QString myPrint;

    model = new QSqlQueryModel;
    model->setQuery("select productName, Date, startTime, endTime, ProductNo, "
                    "NGProductNo from detectResults where "
                    "(Date >= " + printStartTime + ") and (Date <=" + printEndTime + ") order by Date ASC ");

    int rowCount = model->rowCount(model->index(MaxNo,0));
    for (int i = 0; i < rowCount; i++)
    {
        bool ok;
        myDorcument.productName = model->data(model->index(i,0)).toString();
        myDorcument.startTime   = model->data(model->index(i,1)).toString() + "&nbsp;" + model->data(model->index(i,2)).toString();
        myDorcument.endTime     = model->data(model->index(i,1)).toString() + "&nbsp;" + model->data(model->index(i,3)).toString();
        myDorcument.No          = model->data(model->index(i,4)).toString().toInt(&ok,10);
        myDorcument.NG          = model->data(model->index(i,5)).toString().toInt(&ok,10);

        myPrint += printEdit(myDorcument);
    }
    return myPrint;
}

QString MainWindow::printEdit(printDorcument myDorcument)
{
    QString html;

    html += "<body align=\"left\"><font><b>"+ QStringLiteral("产品名称:&nbsp;")
            + myDorcument.productName + "\n</b></font></body>";
    html += "<body align=\"left\"><font><b>"+ QStringLiteral("开始时间:&nbsp;")
            + myDorcument.startTime + "\n</b></font></body>";
    html += "<body align=\"left\"><font><b>"+ QStringLiteral("结束时间:&nbsp;")
            + myDorcument.endTime + "\n</b></font></body>";
    html += "<body align=\"left\"><font><b>"+ QStringLiteral("产品总数:&nbsp;")
            + QString::number(myDorcument.No,10) + "\n</b></font></body>";
    html += "<body align=\"left\"><font><b>"+ QStringLiteral("非良品数:&nbsp;")
            + QString::number(myDorcument.NG,10) + "\n</b></font></body>";
    html += "<body align=\"left\"><font><b>"+ QStringLiteral("良品率:&nbsp;&nbsp;")
            + QString::number((double(myDorcument.No)-double(myDorcument.NG))/double(myDorcument.No)*100,'.',2) + "%\n</b></font></body>";
    html += "<body align=\"center\"><font><b>-------------------------\n</b></font></body>";

    return html;
}


//初始化halcon显示窗口
void MainWindow::initHalconWindows()
{
    //设置边框宽度
    int W = 3;


    HTuple HWindowRow;
    HTuple HWindowColumn;
    HTuple HWindowWidth;
    HTuple HWindowHeight;

//****show1初始化*************************************************************************************************

    Hlong HWindowID1 = (Hlong) ui->frame_show1->winId();
    ui->frame_show1->adjustSize();


    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show1->width()-2*W;
    HWindowHeight = ui->frame_show1->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID1,"visible","",&m_HWindowID1);
    set_part(m_HWindowID1, 0, 0, 600, 616);
//****show2初始化*************************************************************************************************
    Hlong HWindowID2 = (Hlong) ui->frame_show2->winId();

    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show2->width()-2*W;
    HWindowHeight = ui->frame_show2->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID2,"visible","",&m_HWindowID2);
    set_part(m_HWindowID2, 0, 0, 963, 1291);

//****show3初始化*************************************************************************************************
    Hlong HWindowID3 = (Hlong) ui->frame_show3->winId();

    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show3->width()-2*W;
    HWindowHeight = ui->frame_show3->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID3,"visible","",&m_HWindowID3);
    set_part(m_HWindowID3, 0, 0, 963, 1291);
//****show4初始化*************************************************************************************************
    Hlong HWindowID4 = (Hlong) ui->frame_show4->winId();

    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show4->width()-2*W;
    HWindowHeight = ui->frame_show4->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID4,"visible","",&m_HWindowID4);
    set_part(m_HWindowID4, 0, 0, 963, 1291);

//****show5初始化*************************************************************************************************
    Hlong HWindowID5 = (Hlong) ui->frame_show5->winId();

    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show5->width()-2*W;
    HWindowHeight = ui->frame_show5->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID5,"visible","",&m_HWindowID5);
    set_part(m_HWindowID5, 0, 0, 963, 1291);

    //****show5初始化*************************************************************************************************
    Hlong HWindowIDTest = (Hlong) ui->frame_show_Test->winId();

    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show_Test->width()-2*W;
    HWindowHeight = ui->frame_show_Test->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowIDTest,"visible","",&m_HWindowIDTest);
    set_part(m_HWindowIDTest, 0, 0, HWindowWidth, HWindowHeight);

}


void MainWindow::saveSettings()
{
    QSettings setting(MyConfig, QSettings::IniFormat);

    setting.beginGroup("selectedProduct");
    setting.setValue("Name",presentDetectItems.name);
    setting.endGroup();
}

void MainWindow::loadSettings()
{
    QSettings setting(MyConfig,QSettings::IniFormat);

    setting.beginGroup("selectedProduct");
    presentDetectItems.name = setting.value("Name","").toString();
    setting.endGroup();
}

void MainWindow::on_saveAsPresent_clicked()
{
    QAbstractItemModel *model = ui->detectItems->model();
    bool *ok = false;

    presentDetectItems.name         = model->data(model->index(nextSelectID,0)).toString();
    presentDetectItems.productNo    = model->data(model->index(nextSelectID,1)).toInt(ok);
    presentDetectItems.threshold1   = model->data(model->index(nextSelectID,2)).toInt(ok);
    presentDetectItems.threshold2   = model->data(model->index(nextSelectID,3)).toInt(ok);
    presentDetectItems.erodeSize    = model->data(model->index(nextSelectID,4)).toInt(ok);

    saveSettings();

    presentSelectID = nextSelectID;
    ui->detectItems->selectRow(presentSelectID);
    ui->detectItems->setFocus();
}

void MainWindow::on_detectItems_clicked(const QModelIndex &index)
{
    nextSelectID = index.row();
}

void MainWindow::on_refreshDetectItem_clicked()
{
    ui->detectItems->selectRow(presentSelectID);
    ui->detectItems->setFocus();
}

void MainWindow::on_ON_OFF_clicked()
{
    if (FlagInspect)
    {
        FlagInspect = !FlagInspect;
        ui->ON_OFF->setText(QStringLiteral("开始检测"));
        ui->label_name->setText("");
        ui->label_startTime->setText("");
        ui->label_onTime->setText("");
        ui->label_productNo->setText("0");
        ui->label_productNG->setText("0");
        ui->label_pesent->setText("0%");
        //保存检测记录 <<名称<< 检测日期<<开始时间<<结束时间<<持续时间<<总数量<<NG数量<<合格率
    } else
    {
        presentDetectItems.date      =  QDateTime::currentDateTime().toString("yyyy-MM-dd");
        presentDetectItems.startTime =  QDateTime::currentDateTime().toString("hh:mm:ss");

        _time1 = QDateTime::currentDateTime();

        ui->ON_OFF->setText(QStringLiteral("停止检测"));
        ui->label_name->setText(presentDetectItems.name);
        ui->label_startTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        FlagInspect = !FlagInspect;
    }
}



void MainWindow::on_lineEdit_selectionChanged()
{
    QStringList list;
    list<<"";

    inputProcess.startDetached("TabTip.exe",list);
}

void MainWindow::on_confirm_clicked()
{
    WinExec("cmd.exe /c taskkill /IM TabTip.exe", SW_HIDE);
}

void MainWindow::Readregedit()
{
    QString path;
    QDir dir;
    path=dir.currentPath();//在QtCreator中点击运行的话地址是工程地址，想要获得应用地址就点击文档中的应用程序（.exe文件）
    QString sApp = path + "/XrayDetector.exe";//我的程序名称
    sApp.replace("/","\\");
    qDebug()<<sApp;
    QSettings *reg=new QSettings("HKEY_LOCAL_MACHINE",QSettings::NativeFormat);
    //开机自动运行
    reg->setValue("SOFTWARE/Microsoft/Windows/CurrentVersion/Run/XrayDetector.exe",QVariant(sApp));

}

void MainWindow::initialSerialLink()
{
    myLink.Port1_Open();
    serial_time.start(10);
}

void MainWindow::_processData()
{
    myLink.Port1_Read();
}

//**探测器相关函数*************************************************************
void MainWindow::on_pushButton_open_clicked()
{
    // TODO: Add your control notification handler code here
    m_Detector.SetCallback(&OnDTError);
    m_Image.SetCallBack(&OnDTEvent, &OnDTError, &OnFrameReady);


    switch(m_DetectorType)
    {
    case 0:
        //USB for F3/C4/X-DAQ
        if(m_Detector.GetIsOpened())
            return;

        m_BytesPerPixel = 2;
        m_Detector.PutChannelType(DT_USB);
        m_Detector.PutCmdTimeOut(3000);
        m_Image.PutChannelType(DT_USB);
        m_Image.PutBytesPerPixel(m_BytesPerPixel);
        m_Image.PutStreamPixelPerBytes(2);
        m_Image.PutImgHeight(m_ImgHeight);
        m_Image.PutTimeOut(3000);

        if(m_Detector.Open())
        {
            m_Image.PutDetectorObject(&m_Detector);
            m_Commander.PutDetectorObject(&m_Detector);

            m_Commander.GetPixelNumber(&m_ImgWidth);
            //Set image with to pixel number
            m_Image.PutImgWidth(m_ImgWidth);
            m_ImgSize = m_ImgWidth*m_ImgHeight*m_BytesPerPixel;
            //Allocat image buffer
            m_pImgBuf = new BYTE[m_ImgSize];
            if(m_Image.Open())
            {

                m_pImgObject = m_Image.GetImageObject();
                qDebug() << "Image opened successfully!!! \n" << endl;
            }
            else
                qDebug() << "Image failed open!!! \n" << endl;

        }
        else
            qDebug() << "Detector failed connecting USB!!!\n" << endl;

        break;
    case 1:
        //NET for X-DAQ/X-DCU
        if(m_Detector.GetIsOpened())
            return;
        m_BytesPerPixel = 2;
        m_Detector.PutChannelType(DT_NET);
        m_Detector.PutIPAddress("192.168.1.2");
        m_Detector.PutCmdPort(3000);
        m_Image.PutChannelType(DT_UDP);
        m_Image.PutImagePort(4001);
        m_Image.PutBytesPerPixel(m_BytesPerPixel);
        m_Image.PutStreamPixelPerBytes(2);
        m_Image.PutImgHeight(m_ImgHeight);
        m_Image.PutTimeOut(3000);

        if(!m_Detector.Open())
//            AfxMessageBox("Detector failed connecting NET!!!\n");
        if(m_Detector.GetIsOpened())
        {
            m_Detector.StartPing(1);
            m_Image.PutDetectorObject(&m_Detector);
            m_Commander.PutDetectorObject(&m_Detector);

            m_Commander.GetPixelNumber(&m_ImgWidth);
            //Set image with to pixel number
            m_Image.PutImgWidth(m_ImgWidth);
            m_ImgSize = m_ImgWidth*m_ImgHeight*m_BytesPerPixel;
            //Allocat image buffer
            m_pImgBuf = new BYTE[m_ImgSize];

            if(m_Image.Open())
            {
                m_pImgObject = m_Image.GetImageObject();
            }
//            else
//                AfxMessageBox("Image failed open!!! \n");
        }
        break;
    case 2:
        if(m_Detector.GetIsOpened())
            return;
        m_BytesPerPixel = 4;
        m_Detector.PutChannelType(DT_NET);
        m_Detector.PutIPAddress("192.168.1.2");
        m_Detector.PutCmdPort(3000);
        m_Image.PutChannelType(DT_UDP);
        m_Image.PutImagePort(4001);
        m_Image.PutBytesPerPixel(m_BytesPerPixel);
        m_Image.PutStreamPixelPerBytes(3);
        m_Image.PutImgHeight(m_ImgHeight);
        m_Image.PutTimeOut(3000);

        if(!m_Detector.Open())
//            AfxMessageBox("Detector failed connecting NET!!!\n");
        if(m_Detector.GetIsOpened())
        {
            m_Detector.StartPing(1);
            m_Image.PutDetectorObject(&m_Detector);
            m_Commander.PutDetectorObject(&m_Detector);

            m_Commander.GetPixelNumber(&m_ImgWidth);
            //Set image with to pixel number
            m_Image.PutImgWidth(m_ImgWidth);
            m_ImgSize = m_ImgWidth*m_ImgHeight*m_BytesPerPixel;

            ////For dual energy mode
            /*m_Image.PutDualEnergyMode(1);
            m_Image.PutImgWidth(2*m_ImgWidth);
            m_Commander.PutOperationMode(3);
            m_Commander.PutLineTrigger(1);
            m_ImgSize = 2*m_ImgWidth*m_ImgHeight*m_BytesPerPixel;*/

            //Allocat image buffer
            m_pImgBuf = new BYTE[m_ImgSize];

            if(m_Image.Open())
            {
                m_pImgObject = m_Image.GetImageObject();
            }
//            else
//                AfxMessageBox("Image failed open!!! \n");
        }
        break;
    case 3:
        if(m_Detector.GetIsOpened())
            return;
        m_BytesPerPixel = 2;
        m_Detector.PutChannelType(DT_NET);
        m_Detector.PutIPAddress("192.168.1.2");
        m_Detector.PutCmdPort(3000);
        m_Image.PutChannelType(DT_UDP);
        m_Image.PutImagePort(4001);
        m_Image.PutBytesPerPixel(m_BytesPerPixel);
        m_Image.PutStreamPixelPerBytes(2);
        m_Image.PutImgHeight(m_ImgHeight);
        m_Image.PutTimeOut(3000);

        if(!m_Detector.Open())
//            AfxMessageBox("Detector failed connecting NET!!!\n");
        if(m_Detector.GetIsOpened())
        {
            m_Detector.StartPing(1);
            m_Image.PutDetectorObject(&m_Detector);
            m_Commander.PutDetectorObject(&m_Detector);

            m_Commander.GetPixelNumber(&m_ImgWidth);
            //Set image with to pixel number
            m_Image.PutImgWidth(m_ImgWidth/2);
            m_ImgSize = m_ImgWidth*m_ImgHeight*m_BytesPerPixel/2;

            ////For dual energy mode
            //m_Image.PutDualEnergyMode(1);
            //m_Image.PutImgWidth(m_ImgWidth);
            //m_Commander.PutOperationMode(3);
            //m_Commander.PutLineTrigger(1);
            //m_ImgSize = m_ImgWidth*m_ImgHeight*m_BytesPerPixel;

            //Allocat image buffer
            m_pImgBuf = new BYTE[m_ImgSize];

            if(m_Image.Open())
            {
                m_Image.SetDirectFlag(0);
                m_pImgObject = m_Image.GetImageObject();
            }
//            else
//                AfxMessageBox("Image failed open!!! \n");
        }
        break;
    case 4:
        //Giga-bit NET for UCU
        if(m_Detector.GetIsOpened())
            return;
        m_BytesPerPixel = 2;
        m_Detector.PutChannelType(DT_UDP);
        m_Detector.PutIPAddress("192.168.1.2");
        m_Detector.PutCmdPort(3000);
        m_Image.PutChannelType(DT_UDP);
        m_Image.PutImagePort(4001);
        m_Image.PutBytesPerPixel(m_BytesPerPixel);
        m_Image.PutStreamPixelPerBytes(2);
        m_Image.PutImgHeight(m_ImgHeight);
        m_Image.PutTimeOut(3000);

        if(!m_Detector.Open())
//            AfxMessageBox("Detector failed connecting NET!!!\n");
        if(m_Detector.GetIsOpened())
        {
            m_Detector.StartPing(1);
            m_Image.PutDetectorObject(&m_Detector);
            m_Commander.PutDetectorObject(&m_Detector);

            m_Commander.GetPixelNumber(&m_ImgWidth);
            //Set image with to pixel number
            m_Image.PutImgWidth(m_ImgWidth);
            m_ImgSize = m_ImgWidth*m_ImgHeight*m_BytesPerPixel;
            //Allocat image buffer
            m_pImgBuf = new BYTE[m_ImgSize];

            if(m_Image.Open())
            {
                m_pImgObject = m_Image.GetImageObject();
            }
//            else
//                AfxMessageBox("Image failed open!!! \n");
        }
        break;
    default:
//        AfxMessageBox("No matched detector!!!");
        break;
    }
    //Get detector info
    if(m_Commander.GetIsOpened())
    {
//        char* pInfo;
//        QString str;
//        if(m_Commander.GetModuleTypeName(&pInfo))
//            str.Format("Detector Info: %s ", pInfo);
//        if(m_Commander.GetSerialNumber(&pInfo))
//            str += pInfo;
//        GetDlgItem(IDC_DETECTOR_INFO)->SetWindowText(str);

//        m_Commander.GetDataPattern(&m_bDataPattern);
//        m_Commander.GetIntegrationTime((ULONG*)&m_IntTime);
//        UpdateData(0);
    }
}

void MainWindow::on_pushButton_close_clicked()
{
    // TODO: Add your control notification handler code here
    if(m_Image.GetIsOpened())
    {
        m_Image.Close();
        qDebug() << "Image closed successfully!!! \n" << endl;
    }

    if(m_Detector.GetIsOpened())
    {
        m_Detector.Close();
        qDebug() << "Detector closed successfully!!! \n" << endl;
    }
    if(m_pImgBuf)
        delete [] m_pImgBuf;
    m_pImgBuf = NULL;


}

void MainWindow::on_pushButton_grab_clicked()
{
    // TODO: Add your control notification handler code here
    if(m_Image.GetIsOpened() && !m_Image.GetIsGrabing())
    {
        m_FrameCount = 0;
        m_DataLost = 0;
        m_Image.Grab(0);
        qDebug() << "start grab!!! \n" << endl;
    }
}

void MainWindow::on_pushButton_stop_clicked()
{
    // TODO: Add your control notification handler code here
    if(m_Image.GetIsOpened())
    {
        m_Image.Stop();
        qDebug() << "stop grab!!! \n" << endl;
    }
}

void MainWindow::on_pushButton_snap_clicked()
{
    // TODO: Add your control notification handler code here
    qDebug() << "start snap button was clicked!!! \n" << endl;
    if(m_Image.GetIsOpened() && !m_Image.GetIsGrabing())
    {
        qDebug() << "Image and Detector were open and present is not grabing!!! \n" << endl;
        m_FrameCount = 0;
        m_DataLost = 0;
        m_Image.Snap();
        qDebug() << "start snap!!! \n" << endl;
    }
}

void MainWindow::on_pushButton_onboardcalibration_clicked()
{
    // TODO: Add your control notification handler code here
    if(m_Commander.GetIsOpened() && m_Image.GetIsOpened())
    {
        qDebug() << "Start on-board offset calibration, please turn off X-Ray.\n" << endl;

        m_Commander.ResetGain();
        m_Commander.ResetOffset();
        m_Commander.PutCorrectionGain(0);
        m_Commander.PutCorrectionOffset(0);
        m_Commander.OnBoardOffsetCalibration();

        qDebug() << "Start on-board gain calibration, please turn on X-Ray.\n"<<endl;

        m_Commander.PutCorrectionOffset(1);
        m_Commander.OnBoardGainCalibration(1);
        m_Commander.PutCorrectionGain(1);
        m_Commander.SaveOffset();
        m_Commander.SaveGain(0);
        qDebug() << "Finish calibration.\n"<<endl;

    }
}

void MainWindow::on_pushButton_loadcalibration_clicked()
{
    // TODO: Add your control notification handler code here
    if(m_Commander.GetIsOpened())
    {
        m_Commander.LoadGain(0);
        m_Commander.LoadOffset();
        m_Commander.PutCorrectionGain(1);
        m_Commander.PutCorrectionOffset(1);
    }
}


void MainWindow::OnBnClickedEnableDatapattern()
{
    // TODO: Add your control notification handler code here

    if(m_Commander.GetIsOpened())
        m_Commander.PutDataPattern(m_bDataPattern);

}

void MainWindow::OnBnClickedIntTime()
{
    // TODO: Add your control notification handler code here

    if(m_Commander.GetIsOpened())
        m_Commander.PutIntegrationTime(m_IntTime);
}

void MainWindow::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    if(m_Image.GetIsOpened())
        m_Image.Close();

    if(m_Detector.GetIsOpened())
        m_Detector.Close();

    if(m_pImgBuf)
        delete [] m_pImgBuf;
}

void MainWindow::OnBnClickedSendCmd()
{
    // TODO: Add your control notification handler code here
//    UpdateData(1);
//    if(m_Detector.GetIsOpened())
//    {
//        m_Detector.SendCommandA(m_Cmd.GetBuffer(), m_RevCmd);
//        m_Cmd += m_RevCmd;
//    }
//    UpdateData(0);
}


LRESULT	MainWindow::OnEventMsg(BYTE ID, BYTE Num)
{
//    CString str;
//    switch(ID)
//    {
//    case 1:
//        m_DataLost += (int)Num;
//        str.Format("Data Lost: %d lines", m_DataLost);
//        GetDlgItem(IDC_DATA_LOST)->SetWindowText(str);
//        break;
//    case 2:
//        AfxMessageBox( "Frame buffer is full, the frame ready callback function takes too much time!" );
//        break;
//    default:
//        break;
//    }
    return 0;
}

LRESULT MainWindow::OnErrorMsg(LONG ID, CHAR* Info)
{
//    CString str;
//    str.Format("Error: %s", lParam);
//    GetDlgItem(IDC_ERROR)->SetWindowText(str);
    return 0;
}

LRESULT MainWindow::OnFrameReadyMsg()
{
//    m_FrameCount++;
//    CString str;
//    str.Format("Frame Count: %d", m_FrameCount);
//    GetDlgItem(IDC_FRAME_COUNT)->SetWindowText(str);

    // Caculate column average, max and min value
//    m_pImgObject->DoStatistical(2);
//    ULONG *pAvg, *pMin, *pMax;
//    m_pImgObject->ColAverage(&pAvg);
//    m_pImgObject->ColMin(&pMin);
//    m_pImgObject->ColMax(&pMax);


    // Process image, copy the image data to the processing buffer
    if(m_pImgBuf)
    {
        memcpy(m_pImgBuf, (BYTE*)m_pImgObject->GetImageDataAddress(), m_ImgSize);
        //Process with the image buffer

        unsigned char *Pointer = (unsigned char *)m_pImgBuf;
        Hobject ImageInput;
        gen_image1_extern(&ImageInput, "byte", (HTuple)(int)m_ImgWidth, (HTuple)(int)m_ImgHeight, (long)Pointer, NULL);
        mirror_image(ImageInput,&ImageInput,"row");
        disp_image(ImageInput,m_HWindowID1);

    }
    return 0;
}

void MainWindow::OnRevertPhone(unsigned int ret, QString phone)
{
    qDebug() << "Process the Image!!! \n" << endl;
    if(m_pImgBuf)
    {
        memcpy(m_pImgBuf, (BYTE*)m_pImgObject->GetImageDataAddress(), m_ImgSize);
        //Process with the image buffer

        unsigned char *Pointer = (unsigned char *)m_pImgBuf;
        Hobject ImageInput;
        gen_image1_extern(&ImageInput, "byte", (HTuple)(int)m_ImgWidth, (HTuple)(int)m_ImgHeight, (long)Pointer, NULL);
        mirror_image(ImageInput,&ImageInput,"row");
        disp_image(ImageInput,m_HWindowID1);

    }
    if(m_pImgBuf)
    {
        memcpy(m_pImgBuf, (BYTE*)m_pImgObject->GetImageDataAddress(), m_ImgSize);
        //Process with the image buffer

        unsigned char *Pointer = (unsigned char *)m_pImgBuf;
        Hobject ImageInput;
        int width = (int)m_ImgWidth;
        int height = (int)m_ImgHeight;

        gen_image1_extern(&ImageInput, "byte",  width, height,(long)Pointer, NULL);
        mirror_image(ImageInput,&ImageInput,"row");
        disp_image(ImageInput,m_HWindowIDTest);

        write_image(ImageInput, "bmp", 0, imageID +".bmp");
        imageID = imageID + 1;

        qDebug() << "Images Processed Successfully!!! \n" << endl;
    }
}

//**探测器相关函数*************************************************************


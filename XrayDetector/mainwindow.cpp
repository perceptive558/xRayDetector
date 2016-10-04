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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    FlagInspect(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&show_time, &QTimer::timeout, this, &MainWindow::show_present_time);
    show_time.start();

    connect(&start_time, &QTimer::timeout, this, &MainWindow::on_startInspect_clicked);
    start_time.start(3000);

    isBoot = false;

    ui->SelectDetect->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;"
                                    "font: 60 14pt '华文仿宋';"
                                    "background:url(://res/bg_win1024x768.png)");
    ui->SystemInspect->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;"
                                     "font: 60 14pt '华文仿宋';");
    ui->NewDetect->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;"
                                    "font: 60 14pt '华文仿宋';");
    ui->PrintingResult->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;"
                                    "font: 60 14pt '华文仿宋';"
                                    "background:url(://res/print.png)");
    ui->ShowImages->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;"
                                    "font: 60 14pt '华文仿宋';"
                                    "background:url(://res/image.png)");
    ui->SystemSetting->setStyleSheet("border:2px groove green;"
                                    "border-radius:10px;"
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
   // ui->stackedWidget->setCurrentIndex(7);

//    QStringList list;
//    list<<"-s"<<"-t"<<"10";
//    QProcess::startDetached("shutdown.exe",list);

//    exit(0);
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
    //initHalconWindows();
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
//****show1初始化*************************************************************************************************
    Hlong HWindowID2 = (Hlong) ui->frame_show2->winId();

    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show2->width()-2*W;
    HWindowHeight = ui->frame_show2->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID2,"visible","",&m_HWindowID2);
    set_part(m_HWindowID2, 0, 0, 963, 1291);

//****show1初始化*************************************************************************************************
    Hlong HWindowID3 = (Hlong) ui->frame_show3->winId();

    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show3->width()-2*W;
    HWindowHeight = ui->frame_show3->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID3,"visible","",&m_HWindowID3);
    set_part(m_HWindowID3, 0, 0, 963, 1291);
//****show1初始化*************************************************************************************************
    Hlong HWindowID4 = (Hlong) ui->frame_show4->winId();

    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show4->width()-2*W;
    HWindowHeight = ui->frame_show4->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID4,"visible","",&m_HWindowID4);
    set_part(m_HWindowID4, 0, 0, 963, 1291);

//****show1初始化*************************************************************************************************
    Hlong HWindowID5 = (Hlong) ui->frame_show5->winId();

    HWindowRow = W;
    HWindowColumn = W;
    HWindowWidth = ui->frame_show5->width()-2*W;
    HWindowHeight = ui->frame_show5->height()-2*W;

    open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID5,"visible","",&m_HWindowID5);
    set_part(m_HWindowID5, 0, 0, 963, 1291);

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



















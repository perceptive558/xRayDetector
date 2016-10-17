#ifndef SERIALLINK_H
#define SERIALLINK_H

#define CMD_START           0x01        //开始强制输送
#define CMD_STOP            0x02        //停止强制输送
#define CMD_COM_TEST        0x03        //通讯自检
#define CMD_SENSORS_TEST    0x04        //传感器自检
#define CMD_TRANS_TEST      0x05        //传动系统自检
#define CMD_XRAY_TEST       0x06        //x射线系统自检
#define CMD_START_INSPECT   0x07        //开始检测
#define CMD_STOP_INSPECT    0x08        //停止检测
#define CMD_SENSOR_DELAY    0x09        //设置传感器延迟
#define CMD_TRANS_SPEED     0x0A        //设置传送带速度
#define CMD_PRESSURE_TEST   0x0B        //气压自检


#define SENSOR1             0x01        //传感器1编号
#define SENSOR2             0x02        //传感器1编号
#define SENSOR3             0x03        //传感器1编号
#define SENSOR4             0x04        //传感器1编号
#define SENSOR5             0x05        //传感器1编号

#define TRANS1              0x01        //传送带1编号
#define TRANS2              0x02        //传送带1编号
#define TRANS3              0x03        //传送带1编号

#define CMD_CLOSE_SYSTEM    0x00        //关闭系统


#include <QtSerialPort/QSerialPort>

class SerialLink
{
public:
    SerialLink();



    void Port1_Open();
    void Port1_Close();
    void Port1_Read();
    void Port1_Write(int CMD, int Num, unsigned char arg1,unsigned char arg2);

    void set_sensor_delay(int SensorID, int Delay);
    void set_trans_speed(int TransID, int Speed);

    void DO_Sensor_Test();
    void DO_Trans_Test();
    void DO_Comm_Test();
    void DO_XRay_Test();
    void DO_Pressure_Test();

    void Start_Transport();
    void Stop_Transport();
    void Start_Inspect();
    void Stop_Inspect();

    void Close_System();

    void process_data(QByteArray _data);

protected:
    QSerialPort* _port1;            //与下位机通讯
    QByteArray byteArray;
    QByteArray _data;
    unsigned char s[2];

};

#endif // SERIALLINK_H

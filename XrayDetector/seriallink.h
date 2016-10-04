#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <QtSerialPort/QSerialPort>

class SerialLink
{
public:
    SerialLink();

    void Port1_Open();
    void Port1_Close();
    void Port1_Read();
    void Port1_Write(char CMD, char Num, char arg1,char arg2, char arg3, char arg4);

    void set_sensor_delay(int SensorID, int Delay);
    void set_trans_speed(int TransID, int Speed);

    void DO_Sensor_Test();
    void DO_Trans_Test();
    void DO_Comm_Test();
    void DO_XRay_Test();

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
    char s[2];

};

#endif // SERIALLINK_H

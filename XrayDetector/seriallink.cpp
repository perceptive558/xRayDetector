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


#define SENSOR1             0x01        //传感器1编号
#define SENSOR2             0x02        //传感器1编号
#define SENSOR3             0x03        //传感器1编号
#define SENSOR4             0x04        //传感器1编号
#define SENSOR5             0x05        //传感器1编号

#define TRANS1              0x01        //传送带1编号
#define TRANS2              0x02        //传送带1编号
#define TRANS3              0x03        //传送带1编号

#define CMD_CLOSE_SYSTEM    0x00        //关闭系统


#include "seriallink.h"

SerialLink::SerialLink()
{
    //s[2] = {0x5A,0xA5};
}

void SerialLink::Port1_Open()
{
    _port1 = new QSerialPort();
    _port1->setPortName("com1");
    _port1->open(QIODevice::ReadWrite);
    _port1->setBaudRate(9600);
    _port1->setParity(QSerialPort::NoParity);
    _port1->setDataBits(QSerialPort::Data8);
    _port1->setStopBits(QSerialPort::OneStop);
    _port1->setFlowControl(QSerialPort::NoFlowControl);
}

void SerialLink::Port1_Close()
{
    _port1->clear();
    _port1->deleteLater();
}

void SerialLink::Port1_Read()
{
    QByteArray temp = _port1->readAll();


    if(!temp.isEmpty())
    {
        byteArray.append(temp);
        while (byteArray.contains(s[0]))
        {
            if (byteArray[byteArray.indexOf(s[0])+1] == s[1])
            {
                //取出右边的数据
                byteArray = byteArray.right(byteArray.length()-byteArray.indexOf(s[0])-2);
                //判断是否包含指定字段
                while (byteArray.contains(s[0]))
                {
                    if (byteArray[byteArray.indexOf(s[0])+1] == s[1])
                    {
                        _data.append(byteArray.split(s[0]).at(0));
                        if (_data != "")
                        {
                            process_data(_data);
                            _data.clear();
                        }

                        byteArray = byteArray.right(byteArray.length()-byteArray.indexOf(s[0])-2);
                    }
                }

            } else
            {
                _data.append(byteArray.split(s[0]).at(0));
                _data.append(s[0]);
                byteArray = byteArray.right(byteArray.length()-byteArray.indexOf(s[0])-1);
            }
        }
    }

}

void SerialLink::Port1_Write(char CMD, char Num, char arg1,char arg2, char arg3, char arg4)
{
    char sPacket[10];
    unsigned int nCrcItt;


    memset(sPacket, 0, 10);

    sPacket[0]  = 0xA5;
    sPacket[1]  = 0x5A;
    sPacket[2]  = CMD;
    sPacket[3]  = Num;
    sPacket[4]  = arg1;
    sPacket[5]  = arg2;
    sPacket[6]  = arg3;
    sPacket[7]  = arg4;

    nCrcItt = sPacket[2] + sPacket[3] + sPacket[4] + sPacket[5] + sPacket[6] + sPacket[7];

    sPacket[8] = nCrcItt/256;
    sPacket[9] = nCrcItt%256;


    _port1->write((char*)sPacket,10);
}


void SerialLink::set_sensor_delay(int SensorID, int Delay)
{
    char Num = SensorID;
    char arg1, arg2, arg3, arg4;
    arg1 = Delay/256;
    arg2 = Delay%256;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_SENSOR_DELAY, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::set_trans_speed(int TransID, int Speed)
{
    char Num = TransID;
    char arg1, arg2, arg3, arg4;
    arg1 = Speed/256;
    arg2 = Speed%256;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_TRANS_SPEED, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::DO_Sensor_Test()
{
    char Num = 0x00;
    char arg1, arg2, arg3, arg4;
    arg1 = 0x00;
    arg2 = 0x00;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_SENSORS_TEST, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::DO_Trans_Test()
{
    char Num = 0x00;
    char arg1, arg2, arg3, arg4;
    arg1 = 0x00;
    arg2 = 0x00;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_TRANS_TEST, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::DO_Comm_Test()
{
    char Num = 0x00;
    char arg1, arg2, arg3, arg4;
    arg1 = 0x00;
    arg2 = 0x00;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_COM_TEST, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::DO_XRay_Test()
{
    char Num = 0x00;
    char arg1, arg2, arg3, arg4;
    arg1 = 0x00;
    arg2 = 0x00;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_XRAY_TEST, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::Start_Transport()
{
    char Num = 0x00;
    char arg1, arg2, arg3, arg4;
    arg1 = 0x00;
    arg2 = 0x00;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_START, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::Stop_Transport()
{
    char Num = 0x00;
    char arg1, arg2, arg3, arg4;
    arg1 = 0x00;
    arg2 = 0x00;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_STOP, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::Start_Inspect()
{
    char Num = 0x00;
    char arg1, arg2, arg3, arg4;
    arg1 = 0x00;
    arg2 = 0x00;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_START_INSPECT, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::Stop_Inspect()
{
    char Num = 0x00;
    char arg1, arg2, arg3, arg4;
    arg1 = 0x00;
    arg2 = 0x00;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_STOP_INSPECT, Num, arg1, arg2, arg3, arg4);
}

void SerialLink::Close_System()
{
    char Num = 0x00;
    char arg1, arg2, arg3, arg4;
    arg1 = 0x00;
    arg2 = 0x00;
    arg3 = 0x00;
    arg4 = 0x00;

    Port1_Write(CMD_CLOSE_SYSTEM, Num, arg1, arg2, arg3, arg4);
}


void SerialLink::process_data(QByteArray _data)
{
    unsigned int nCrcItt;
    nCrcItt = _data[0]+_data[1]+_data[2]+_data[3]+_data[4]+_data[5];

    char sPacket[10];


    memset(sPacket, 0, 10);

    sPacket[0]  = _data[0];
    sPacket[1]  = _data[1];
    sPacket[2]  = _data[2];
    sPacket[3]  = _data[3];
    sPacket[4]  = _data[4];
    sPacket[5]  = _data[5];
    sPacket[6]  = _data[6];
    sPacket[7]  = _data[7];


    _port1->write((char*)sPacket,8);

    unsigned char In[2];
    In[0] = nCrcItt/256;
    In[1] = nCrcItt%256;

    if ((char(_data[6]) == In[0])&&(char(_data[7]) == In[1]))
    {
        switch (char(_data[0])) {
        case CMD_CLOSE_SYSTEM:

            break;
        case CMD_COM_TEST:
            break;

        default:
            break;
        }
    }
}

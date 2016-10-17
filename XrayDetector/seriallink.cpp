#include "seriallink.h"

SerialLink::SerialLink()
{
    s[0] = 0x55;
    s[1] = 0xAA;
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
            if (unsigned char(byteArray[byteArray.indexOf(s[0])+1]) == s[1])
            {
                //取出有效数据
                _data = byteArray.right(byteArray.length()-byteArray.indexOf(s[0])-2);
                _data = _data.left(4);
                if (_data != "")
                {
                    process_data(_data);
                    _data.clear();
                }

                //取出右边的数据
                byteArray = byteArray.right(byteArray.length()-byteArray.indexOf(s[0])-6);
            } else
            {
                _data.append(byteArray.split(s[0]).at(0));
                _data.append(s[0]);
                byteArray = byteArray.right(byteArray.length()-byteArray.indexOf(s[0])-1);
            }
        }
    }

}

void SerialLink::Port1_Write(int CMD, int Num, unsigned char arg1,unsigned char arg2)
{
    unsigned char sPacket[6];
    unsigned int nCrcItt;


    memset(sPacket, 0, 6);

    sPacket[0]  = 0x55;
    sPacket[1]  = 0xAA;
    sPacket[2]  = CMD*16 + Num;
    sPacket[3]  = arg1;
    sPacket[4]  = arg2;

    nCrcItt = sPacket[2] + sPacket[3] + sPacket[4];

    sPacket[5] = nCrcItt;


    _port1->write((char*)sPacket,6);
}


void SerialLink::set_sensor_delay(int SensorID, int Delay)
{
    unsigned char arg1, arg2;
    arg1 = Delay/256;
    arg2 = Delay%256;

    Port1_Write(CMD_SENSOR_DELAY, SensorID, arg1, arg2);
}

void SerialLink::set_trans_speed(int TransID, int Speed)
{
    unsigned char arg1, arg2;
    arg1 = Speed/256;
    arg2 = Speed%256;

    Port1_Write(CMD_TRANS_SPEED, TransID, arg1, arg2);
}

void SerialLink::DO_Sensor_Test()
{
    unsigned char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;

    Port1_Write(CMD_SENSORS_TEST, 0, arg1, arg2);
}

void SerialLink::DO_Trans_Test()
{
    unsigned char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;

    Port1_Write(CMD_TRANS_TEST, 0, arg1, arg2);
}

void SerialLink::DO_Comm_Test()
{
    unsigned char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;

    Port1_Write(CMD_COM_TEST, 0, arg1, arg2);
}

void SerialLink::DO_XRay_Test()
{
    unsigned char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;


    Port1_Write(CMD_XRAY_TEST, 0, arg1, arg2);
}

void SerialLink::DO_Pressure_Test()
{
    unsigned char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;


    Port1_Write(CMD_PRESSURE_TEST, 0, arg1, arg2);
}

void SerialLink::Start_Transport()
{
    unsigned char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;


    Port1_Write(CMD_START, 0, arg1, arg2);
}

void SerialLink::Stop_Transport()
{
    unsigned char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;

    Port1_Write(CMD_STOP, 0, arg1, arg2);
}

void SerialLink::Start_Inspect()
{
    unsigned char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;

    Port1_Write(CMD_START_INSPECT, 0, arg1, arg2);
}

void SerialLink::Stop_Inspect()
{
    char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;

    Port1_Write(CMD_STOP_INSPECT, 0, arg1, arg2);
}

void SerialLink::Close_System()
{
    unsigned char arg1, arg2;
    arg1 = 0x00;
    arg2 = 0x00;


    Port1_Write(CMD_CLOSE_SYSTEM, 0, arg1, arg2);
}


void SerialLink::process_data(QByteArray _data)
{
    unsigned char nCrcItt;
    nCrcItt = _data[0]+_data[1]+_data[2];

    int CMD = unsigned char(_data[0])/16;
    int ID  = unsigned char(_data[0])%16;

    if (unsigned char(_data[3]) == nCrcItt%256)
    {
        switch (CMD) {
        case CMD_START         :    //开始强制输送
        {
            break;
        }
        case CMD_STOP          :    //停止强制输送
        {
            break;
        }
        case CMD_COM_TEST      :    //通讯自检
        {
            break;
        }
        case CMD_SENSORS_TEST  :    //传感器自检
        {
            switch (ID) {
            case 0:

                break;
            default:
                break;
            }
            break;
        }
        case CMD_TRANS_TEST    :    //传动系统自检
        {
            break;
        }
        case CMD_XRAY_TEST     :    //x射线系统自检
        {
            break;
        }
        case CMD_START_INSPECT :    //开始检测
        {
            break;
        }
        case CMD_STOP_INSPECT  :    //停止检测
        {
            break;
        }
        case CMD_SENSOR_DELAY  :    //设置传感器延迟
        {
            break;
        }
        case CMD_TRANS_SPEED   :    //设置传送带速度
        {
            break;
        }
        case CMD_CLOSE_SYSTEM  :    //关闭系统
        {
            break;
        }

        default:
            break;
        }
    }
}

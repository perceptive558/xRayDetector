#ifndef MYQEXTSERIALPORT_H
#define MYQEXTSERIALPORT_H

/*系统操作*/
#define CMD_OPENCLOSE                   0x01            //系统开关机
#define CMD_ONOFF                       0x02            //开始运行/停止运行
#define CMD_SENSOR_TEST                 0x03            //传感器自检
#define CMD_XRAY_TEST                   0x04            //X射线自检
#define CMD_PRESSURE_TEST               0x05            //气压自检
#define CMD_FORCE_ONOFF                 0x06            //强制输送/停止
#define CMD_TRANSPORT_TEST              0x07            //传送系统自检

#define CMD_DELAY_SETTING               0x08            //传感器延迟时间设置
#define CMD_SPEED_SETTING               0x09            //传送带速度设置




class myqextserialport
{
public:
    myqextserialport();

    int comOpen(int nPortno);
    int comClose(int nPortno);

    int com1Write(int nCmd, char sModuleType, char sIntAddr, char sUnitAddr, char arg1, char arg2, char arg3, char arg4, char arg5, int nCount);
    int com1Read(char *sBuf, int nLen);


    int com2Write(int nCmd, char sModuleAddr, char arg, int nCount);
    int com2Read(int nCmd, char sModuleAddr, int nLen);


private:
    // window使用
    Win_QextSerialPort *myPort1, *myPort2;
};

#endif // MYQEXTSERIALPORT_H

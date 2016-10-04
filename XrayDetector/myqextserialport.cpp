#include "myqextserialport.h"

myqextserialport::myqextserialport()
{

}

int myqextserialport::comClose(int portno)
{
    switch(portno) {
    case 0:
        myPort1->close();
        delete myPort1;
        myPort1 = NULL;
        break;
    case 1:
        myPort2->close();
        delete myPort2;
        myPort2 = NULL;
        break;
    }

    return 0;
}


int myqextserialport::comOpen(int nPortno)
{
    char sComName[32];
    int ret = -1;
    struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIMEOUT};

    memset(sComName, 0 , sizeof(sComName));
    sprintf(sComName, "%s", "COM1", nPortno);

    switch(nPortno) {
    case 1:
            myPort1 = new Win_QextSerialPort(sComName,myComSetting,Win_QextSerialPort::Polling);
            ret = myPort1->open(QIODevice::ReadWrite);
            break;
    case 2:
           myPort2 = new Win_QextSerialPort(sComName,myComSetting,Win_QextSerialPort::Polling);
           ret = myPort2->open(QIODevice::ReadWrite);
           break;
    }

    if(ret != 1) {
        qDebug("open port%d is err!", nPortno);
    }
    return 1;
}


int myqextserialport::com1Write(int nCmd, char SModuleType, char sIntAddr, char sUnitAddr,
        char arg1, char arg2, char arg3,char arg4, char arg5, int nCount)
{
    char sPacket[16];
    char sCrcData[10];
    unsigned int nCrcItt=0;
    int i, ret = 0;

    memset(sPacket, 0, 16);

    sPacket[0]  = 0xA5;
    sPacket[1]  = 0x5A;

    sPacket[2]   = SModuleType;
    switch(SModuleType) {
    case INT:
//	if(nCmd == CMD_INT_EJTEST || nCmd == CMD_INT_ONOFF) {
    if(nCmd == CMD_INT_EJTEST) {
            sPacket[3]  = sUnitAddr << 4;
    }
    else if(nCmd == CMD_INT_ONOFF) {
            sPacket[3]  = sIntAddr << 4;
    }
    else {
            sPacket[3]   = (sUnitAddr/2) << 4 | (sUnitAddr%2);
    }
    break;
    case UNIT:
        sPacket[3]   = (sUnitAddr/2) << 4 | (sUnitAddr%2);
    }

    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;

    sPacket[6]   = struGsh.nSerialCount1++;
    sPacket[7]   = arg1;
    sPacket[8]   = arg2;
    sPacket[9]   = arg3;
    sPacket[10]  = arg4;
    sPacket[11]  = arg5;

    for (i = 0; i < 10; i++) {
    sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 10);

    sPacket[12] = nCrcItt/256;
    sPacket[13] = nCrcItt%256;

    sPacket[14] = 0xff;
    sPacket[15] = 0xff;

    if (struGsh.nSerialCount1 > 250) {
       struGsh.nSerialCount1 = 0;
    }
#ifdef Q_OS_UNIX
    if((nCmd == CMD_UNIT_CAMERA_VERSION) || (nCmd == CMD_UNIT_IMAGE) || (nCmd == CMD_INT_VERSION)) {
        myPort1->readAll();     // 清除缓冲区
    }

    for (i = 0; i < nCount; i++) {
        ret = myPort1->write(sPacket, 16);
    }
#endif
#if DEBUG_OUTPUT == 1
    printf("com1 write >> ");
    for (i = 0; i < 16; i++) {
       printf("%02x ",sPacket[i]&0xFF);
    }
    printf("\n");
#endif

    return ret;
}


int myqextserialport::com1Read(char *sBuf, int nLen)
{
    int ret = 0;
    int i;
    int nDispLen = 0;
#ifdef Q_OS_UNIX
    nDispLen = (nLen > 24) ? 24 : nLen;
    memset(sBuf, 0, nLen);
    ret = myPort1->read(sBuf, nLen);
#endif
#if DEBUG_OUTPUT == 2
    printf("com1 read >> %d\n",ret);
    for (i = 0; i < nDispLen; i++) {
        printf("%02x ", sBuf[i]&0xFF);
    }
    printf("\n");
#endif

    return ret;
}

int myqextserialport::com2Write(int nCmd, char sModuleAddr, char arg, int nCount)
{
    char sPacket[16];
    char sCrcData[10];
    unsigned int nCrcItt = 0;
    int i, ret = 0;

    /* pack & add checksum */
    memset(sPacket, 0, 16);

    sPacket[0]  = 0xA5;
    sPacket[1]  = 0x5A;

    sPacket[2]   = 0x00;
    sPacket[3]   = sModuleAddr;
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;

    sPacket[6]   = struGsh.nSerialCount2++;
    switch(nCmd) {
    case CMD_CTRL_FEEDER:
        if (arg == 0) {
            sPacket[7]   = 0;
            sPacket[8]   = 0;
            sPacket[9]   = 0;
            sPacket[10]  = 0;
            sPacket[11]  = (struGsh.bStatFeed == 1) ? 0xAA : 0x55;
        }
        else {
            sPacket[7]   = 0;
            sPacket[8]   = arg;
            sPacket[9]   = (struCnfp.struGroupCtrl[sModuleAddr].nFeeder[arg-1]*10)/256;
            sPacket[10]  = (struCnfp.struGroupCtrl[sModuleAddr].nFeeder[arg-1]*10)%256;
            sPacket[11]  = 0;
        }
        break;
    case CMD_CTRL_LAMP:
        sPacket[7]   = 0;
        sPacket[8]   = 0;
        sPacket[9]   = arg;
        sPacket[10]  = (struCnfp.struGroupCtrl[sModuleAddr].nLamp[arg]*4)/256;
        sPacket[11]  = (struCnfp.struGroupCtrl[sModuleAddr].nLamp[arg]*4)%256;
        break;
    case CMD_CTRL_WIPE:
        sPacket[7]   = 0;
        sPacket[8]   = 0;
        sPacket[9]   = 0;
        sPacket[10]  = struCnfp.struGroupCtrl[sModuleAddr].nWipeDuration;
        sPacket[11]  = arg;
        break;
    case CMD_CTRL_WIPE_WIND:
        sPacket[7]   = 0;
        sPacket[8]   = 0;
        sPacket[9]   = struCnfp.struGroupCtrl[sModuleAddr].nWipeWindInterval;
        sPacket[10]  = struCnfp.struGroupCtrl[sModuleAddr].nWipeWindDuration;
        sPacket[11]  = arg;
        break;
    case CMD_CTRL_BACKGROUND:
        sPacket[7]   = 0;
        sPacket[8]  = ((struCnfp.struGroupCtrl[sModuleAddr].nBackgroundRear-1)*511/359)/256;
        sPacket[9]  = ((struCnfp.struGroupCtrl[sModuleAddr].nBackgroundFront-1)*511/359)/256;
        sPacket[10]  = ((struCnfp.struGroupCtrl[sModuleAddr].nBackgroundRear-1)*511/359)%256;
        sPacket[11]  = ((struCnfp.struGroupCtrl[sModuleAddr].nBackgroundFront-1)*511/359)%256;
        break;
    case CMD_CTRL_ALARM:
        sPacket[7]   = 0;
        sPacket[8]   = 0;
        sPacket[9]   = 0;
        sPacket[10]  = 0;
        sPacket[11]  = 0xAA;
        break;
    case CMD_CTRL_VERSION:
        sPacket[7]   = 0;
        sPacket[8]   = 0;
        sPacket[9]   = 0;
        sPacket[10]  = 0;
        sPacket[11]  = arg;
        break;
    case CMD_CTRL_PERIPHE_ON:
        sPacket[7]   = 0;
        sPacket[8]   = 0;
        sPacket[9]   = 0;
        sPacket[10]  = 0;
        sPacket[11]  = arg;
        break;
    case CMD_CTRL_PERIPHE_OFF:
        sPacket[7]   = 0;
        sPacket[8]   = 0;
        sPacket[9]   = 0;
        sPacket[10]  = 0;
        sPacket[11]  = arg;
        break;
    case CMD_CTRL_MACHINE_OFF:
        sPacket[7]   = 0;
        sPacket[8]   = 0;
        sPacket[9]   = 0;
        sPacket[10]  = 0;
        sPacket[11]  = arg;
        break;
    }

    for (i = 0; i < 10; i++) {
        sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 10);
    sPacket[12] = nCrcItt/256;
    sPacket[13] = nCrcItt%256;
    sPacket[14] = 0xff;
    sPacket[15] = 0xff;

    if (struGsh.nSerialCount2 > 250) {
        struGsh.nSerialCount2 = 0;
    }

#if DEBUG_OUTPUT == 2
    printf("com2 write >> ");
    for (i = 0; i < 16; i++) {
       printf("%02x ",sPacket[i]&0xFF);
    }
    printf("\n");
#endif
#ifdef Q_OS_UNIX
    if((nCmd == CMD_CTRL_ALARM) || (nCmd == CMD_CTRL_VERSION)) {
        myPort2->readAll();     // 清除缓冲区
    }
    for (i = 0; i < nCount; i++) {
        ret = myPort2->write(sPacket, 16);
    }
#endif
    return ret;
}

int myqextserialport::com2Read(int nCmd, char sModuleAddr, int nLen)
{
    int ret = 0;
    char sBuf[24];
    int i;
#ifdef Q_OS_UNIX
    memset(sBuf, 0, 24);
    ret = myPort2->read(sBuf, nLen);
#endif
#if DEBUG_OUTPUT == 2
    printf("com2 read >> %d\n",ret);
    for (i = 0; i < nLen; i++) {
        printf("%02x ", sBuf[i]&0xFF);
    }
    printf("\n");
#endif

    if (sBuf[0] == 0xAA && sBuf[1] == 0xAA && sBuf[2] == 0xAA && sBuf[3] == 0xAA) {
        switch(sBuf[5]) {
        case CMD_CTRL_ALARM:
            struGsh.bStatBelt[0] = sBuf[10] & 1;
            struGsh.bStatBelt[1] = (sBuf[10] & 2)/2;

            struGsh.bAlarmPressure = (sBuf[11] & 2)/2;
            break;
        case CMD_CTRL_VERSION:
        printf("version 0 is %d, version 1 is %d\n", struGsh.struVer.sControl[sModuleAddr][0], struGsh.struVer.sControl[sModuleAddr][1]);
            struGsh.struVer.sControl[sModuleAddr][0] = sBuf[11];
            struGsh.struVer.sControl[sModuleAddr][1] = sBuf[10];
            break;

        }
    }

    return ret;
}


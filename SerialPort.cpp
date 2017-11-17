#include "TypeDefine.h"
#include "QtSerialPort/qserialport.h"
#include "QtSerialPort/qserialportinfo.h"
#include "SerialPort.h"
#include <QtCore>


QSerialPort *serial;
bool SerialOpenState = false;
bool SerialPort_Init(QString serialname,uint8_t boudindex,uint8_t stopbit,uint8_t parity,uint8_t len)
{
    serial = new QSerialPort;

    serial->setPortName(serialname);
    switch (len) {
    case 0:
        serial->setDataBits(QSerialPort::Data8);
        break;
    case 1:
        serial->setDataBits(QSerialPort::Data7);
        break;
    case 2:
        serial->setDataBits(QSerialPort::Data6);
        break;
    case 3:
        serial->setDataBits(QSerialPort::Data5);
        break;
    default:
        break;
    }
    switch (stopbit) {
    case 0:
        serial->setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        serial->setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case 2:
        serial->setStopBits(QSerialPort::TwoStop);
        break;
    default:
        break;
    }
    switch (boudindex) {
    case 0:
        serial->setBaudRate(QSerialPort::Baud2400);
        break;
    case 1:
        serial->setBaudRate(QSerialPort::Baud4800);
        break;
    case 2:
        serial->setBaudRate(QSerialPort::Baud9600);
        break;
    case 3:
        serial->setBaudRate(QSerialPort::Baud19200);
        break;
    case 4:
        serial->setBaudRate(QSerialPort::Baud38400);
        break;
    case 5:
        serial->setBaudRate(QSerialPort::Baud57600);
        break;
    case 6:
        serial->setBaudRate(QSerialPort::Baud115200);
        break;
    default:
        break;
    }
    switch (parity) {
    case 0:
        serial->setParity(QSerialPort::NoParity);
        break;
    case 1:
        serial->setParity(QSerialPort::OddParity);
        break;
    case 2:
        serial->setParity(QSerialPort::EvenParity);
        break;
    default:
        break;
    }
    SerialOpenState = serial->open(QIODevice::ReadWrite);

    return SerialOpenState;
}

QString SerialPort_Process(void)
{
    QByteArray RecBuff;

    if(!SerialOpenState) return RecBuff;

    RecBuff = serial->readAll();
    if(!RecBuff.isEmpty())
    {


    }
    RecBuff.clear();

    return RecBuff;
}

bool SerialPort_Send(QString send)
{
    QByteArray SendBytes = send.toUtf8();

    if(send.isEmpty()) return false;

    if(SerialOpenState)
    {
        serial->write(SendBytes,sizeof(SendBytes));
        return true;
    }
    return false;
}

void SerialPort_Close()
{
    if(SerialOpenState)
    {
        serial->close();
    }
}
bool GetSerialPortState()
{
    return SerialOpenState;
}

#ifndef SERIALPORT_H
#define SERIALPORT_H


#include "QtSerialPort/qserialport.h"

#ifdef __cplusplus
extern "C" {
#endif

    QString SerialPort_Process();
    bool SerialPort_Init(QString serialname,uint8_t boudindex,uint8_t stopbit,uint8_t parity,uint8_t len);
bool GetSerialPortState();
bool SerialPort_Send(QString send);
void SerialPort_Close();


#ifdef __cplusplus
}
#endif




#endif

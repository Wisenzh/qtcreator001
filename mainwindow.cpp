#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SerialPort.h"
#include "TypeDefine.h"
#include <QtCore>
#include "QtSerialPort/qserialport.h"
#include "QtSerialPort/qserialportinfo.h"

QTimer *timer_COM_Rx;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    WindowInit();



}

MainWindow::~MainWindow()
{
    delete ui;
}

QDateTime TimeNow ;
QLabel *StatusTimelabel;
QLabel *StatusRxTxLenLabel;
uint32_t Txlen,Rxlen;
void MainWindow::WindowInit()
{
    QStringList boud ;
    QStringList stopbit;
    QStringList parity;
    QStringList slen;
    boud<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
    stopbit<<"1"<<"1.5"<<"2";
    parity<<"无"<<"奇校验"<<"偶校验";
    slen<<"8"<<"7"<<"6"<<"5";
    ui->comboBox_2->addItems(boud);
    ui->comboBox_2->setCurrentIndex(2);
    ui->comboBox_3->addItems(slen);
    ui->comboBox_4->addItems(stopbit);
    ui->comboBox_5->addItems(parity);

    ui->comboBox_5->addItems(parity);
        //查找可用的串口
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            QSerialPort serial;
            serial.setPort(info);
            if(serial.open(QIODevice::ReadWrite))
            {
                ui->comboBox->addItem(serial.portName());
                serial.close();
            }
        }

    timer_COM_Rx = new QTimer(this);      //新建定时器
    connect(timer_COM_Rx,SIGNAL(timeout()),this,SLOT(timerUpDate()));      //关联定时器计满信号和相应的槽函数
    statusbarinit();
    ui->textEdit->setText("test");
    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(false);


}

void MainWindow::timerUpDate()
{
    QString RecData = "";
    TimeNow = QDateTime::currentDateTime();
    QString strtime = TimeNow.toString("yyyy-MM-dd hh:mm:ss");//("yyyy-MM-dd hh:mm:ss dddd");

    StatusTimelabel->setText(" "+strtime+" ");
    ui->lcdNumber->display(strtime);

    RecData = SerialPort_Process();
    if(!RecData.isEmpty())
    {
        StatusRxTxLenLabel->setText("    Rx："+QString("%1").arg(Rxlen++)+"    Tx："+QString("%1").arg(Txlen)+"    ");
        ui->textEdit_2->moveCursor(QTextCursor::End);
        if(ui->checkBox_2->isChecked())
        {
            if(RecData.endsWith("\r\n"))
            {
                RecData.remove("\r\n");
            }
            ui->textEdit_2->insertPlainText(RecData+" ["+strtime+"]\r\n");
        }
        else
        {
            ui->textEdit_2->insertPlainText(RecData);
        }
    }
    else
    {
        ui->textEdit_2->moveCursor(QTextCursor::End);
        if(ui->checkBox_2->isChecked())
        {
            if(strtime.endsWith("\r\n"))
            {
                strtime.remove("\r\n");
            }
            ui->textEdit_2->insertPlainText(strtime+" ["+strtime+"]\r\n");
        }
        else
        {
            ui->textEdit_2->insertPlainText(strtime);
        }
    }
    ui->textEdit_2->moveCursor(QTextCursor::End);


    //ui->textBrowser->setText(nowtime.toString());
    //ui->lcdNumber->display(nowtime.toString());
}

void MainWindow::on_pushButton_clicked()
{
    QString sername = "";
    uint8_t ser[4] ={0};
    sername = ui->comboBox->currentText();

    timer_COM_Rx->start(1000);
    if(!GetSerialPortState())
    {
        if(sername == "") return;
        ui->pushButton->setText("关闭");
        ser[0] = ui->comboBox_2->currentIndex();
        ser[1] = ui->comboBox_4->currentIndex();
        ser[2] = ui->comboBox_5->currentIndex();
        ser[3] = ui->comboBox_3->currentIndex();
        if(SerialPort_Init(sername,ser[0],ser[1],ser[2],ser[3]) == true)
        {
            ui->lcdNumber->setDigitCount(28);
            timer_COM_Rx->start(1000);
        }
    }
    else
    {
        SerialPort_Close();
        timer_COM_Rx->stop();
        ui->pushButton->setText("打开");
    }
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    (void)arg1;
    ui->comboBox->clear();
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(serial.portName());
            serial.close();
        }
    }
}

void MainWindow::statusbarinit()
{
    Txlen = 0;
    Rxlen = 0;
    StatusTimelabel = new QLabel;
    StatusRxTxLenLabel = new QLabel;
    //TimeNow = QDateTime::currentDateTime();
    //QString strtime = TimeNow.toString("yyyy-MM-dd hh:mm:ss dddd");
    QString strtime ="欢迎使用串口调试助手！";

    //ui->statusBar->showMessage(tr("欢迎使用串口调试助手！"), 1000);
    StatusRxTxLenLabel->setText("    Rx："+QString("%1").arg(Rxlen)+"    Tx："+QString("%1").arg(Txlen)+"    ");
    ui->statusBar->addWidget(StatusTimelabel);
    ui->statusBar->addWidget(StatusRxTxLenLabel);
    StatusTimelabel->setText(" "+strtime+" ");
}

void MainWindow::on_pushButton_2_clicked()
{
    bool state = false;

    if(ui->checkBox->isChecked())
    {
        state = SerialPort_Send(ui->textEdit->toPlainText()+"\r\n");

    }
    else
    {
        state = SerialPort_Send(ui->textEdit->toPlainText());
    }

    if(state == false) return;

    StatusRxTxLenLabel->setText("    Rx："+QString("%1").arg(Rxlen)+"    Tx："+QString("%1").arg(Txlen++)+"    ");
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->textEdit_2->clear();
    Rxlen = 0;
    StatusRxTxLenLabel->setText("    Rx："+QString("%1").arg(Rxlen)+"    Tx："+QString("%1").arg(Txlen)+"    ");
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textEdit->clear();
    Txlen = 0;
    StatusRxTxLenLabel->setText("    Rx："+QString("%1").arg(Rxlen)+"    Tx："+QString("%1").arg(Txlen)+"    ");
}

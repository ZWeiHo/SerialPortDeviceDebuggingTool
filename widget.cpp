#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>

#include <algorithm>

using std::sort;

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("串口设备调试");
    getSerialPort();
    ui->SendData->setEnabled(false);
    ui->ClosePort->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

//获取串口
void Widget::getSerialPort()
{

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        SerialPortNameList << info.portName();
    }
    sort(SerialPortNameList.begin(), SerialPortNameList.end());
    ui->SerialName->addItems(SerialPortNameList);
}

//读取数据
void Widget::readData()
{
    QByteArray buffer;
    buffer = OpenSerialPort.readAll();
    if(!buffer.isEmpty())
    {
        QString str = ui->ReceiveWindow->toPlainText();
        str += tr(buffer);
        ui->ReceiveWindow->clear();
        ui->ReceiveWindow->append(str);
    }
}

QSerialPort::DataBits Widget::getDataBit()
{
    int DataBit = ui->DataBit->currentText().toUInt();
    switch(DataBit)
    {
        case 8:
            return QSerialPort::Data8;
        case 7:
            return QSerialPort::Data7;
        case 6:
            return QSerialPort::Data6;
        case 5:
            return QSerialPort::Data5;
    }
}

QSerialPort::Parity Widget::getParity()
{
    int Parity = ui->Parity->currentText().toUInt();
    switch(Parity)
    {
        case 0:
            return QSerialPort::NoParity;
        case 2:
            return QSerialPort::EvenParity;
        case 3:
            return QSerialPort::OddParity;
        case 4:
            return QSerialPort::SpaceParity;
        case 5:
            return QSerialPort::MarkParity;
    }
}

QSerialPort::StopBits Widget::getStopBit()
{
    int StopBit = ui->StopBit->currentText().toUInt();
    switch(StopBit)
    {
        case 1:
            return QSerialPort::OneStop;
        case 2:
            return QSerialPort::TwoStop;
        case 3:
            return QSerialPort::OneAndHalfStop;
    }
}


//点击事件
//获取串口
void Widget::on_GetSerialPort_clicked()
{
    ui->SerialName->clear();
    SerialPortNameList.clear();
    getSerialPort();
}

//打开串口
void Widget::on_OpenPort_clicked()
{
    update();
    OpenSerialPort.setPortName(ui->SerialName->currentText());
    if(OpenSerialPort.open(QIODevice::ReadWrite))
    {
        OpenSerialPort.setBaudRate(ui->BaudRate->currentText().toUInt());
        OpenSerialPort.setDataBits(getDataBit());
        OpenSerialPort.setParity(getParity());
        OpenSerialPort.setStopBits(getStopBit());
        OpenSerialPort.setFlowControl(QSerialPort::NoFlowControl);
        connect(&OpenSerialPort, &QSerialPort::readyRead, this, &Widget::readData);

        ui->SendData->setEnabled(true);
        ui->OpenPort->setEnabled(false);
        ui->ClosePort->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this, QString("Error"), QString("Open failed"), QMessageBox::Ok);
    }
}

//关闭串口
void Widget::on_ClosePort_clicked()
{
    OpenSerialPort.clear();
    OpenSerialPort.close();

    ui->SendData->setEnabled(false);
    ui->OpenPort->setEnabled(true);
    ui->ClosePort->setEnabled(false);
}

//发送数据
void Widget::on_SendData_clicked()
{
    OpenSerialPort.write(ui->SendWindow->toPlainText().toUtf8());
}

//清空读写窗口
void Widget::on_ReceiveClear_clicked()
{
    ui->ReceiveWindow->clear();
}

void Widget::on_SendClear_clicked()
{
    ui->SendWindow->clear();
}

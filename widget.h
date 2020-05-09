#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QList>
#include <QString>
#include <QTime>



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_GetSerialPort_clicked();
    void on_OpenPort_clicked();
    void on_ClosePort_clicked();

    void on_SendData_clicked();

    void on_ReceiveClear_clicked();
    void on_SendClear_clicked();


private:
    void getSerialPort();
    void readData();

    QSerialPort::DataBits getDataBit();
    QSerialPort::Parity getParity();
    QSerialPort::StopBits getStopBit();

    Ui::Widget *ui;
    QSerialPort OpenSerialPort;
    QList<QString> SerialPortNameList;

};
#endif // WIDGET_H

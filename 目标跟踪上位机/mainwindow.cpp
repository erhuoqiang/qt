#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawwidget.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QPainter>
#include <QPaintDevice>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comBox->addItem(serial.portName());
            serial.close();
        }
     }
     //设置波特率下拉菜单默认显示第三项
     ui->rateBox->setCurrentIndex(2);
     ui->checkbitBox->setCurrentIndex(0);
     ui->databitBox->setCurrentIndex(0);
     ui->stopbitBox->setCurrentIndex(0);
     //关闭发送按钮的使能
     qDebug() << tr("界面设定成功！");

     draw = new DrawWidget();
    // draw->show();
    // draw->Draw(10,10,30,30,1);
   //  setCentralWidget(draw);

}

MainWindow::~MainWindow()
{
    delete ui;
}

int s2i(char * src)
{
    int sum = 0;
    while(*src >= '0' && *src <= '9')
    {
        sum = sum * 10 +  *src - 48;
        src++;
    }
    return sum;
}

void StrtoInt(QString &str, int *x, int *y, int *w, int *h)
{
    QByteArray ba = str.toLatin1();
    char * s = ba.data();
    while(*s)
    {
        switch(*s)
        {
            case 'X': *x = s2i(s+2);break;
            case 'Y': *y = s2i(s+2);break;
            case 'W': *w = s2i(s+2);break;
            case 'H': *h = s2i(s+2);break;
            default :break;
        }
        s++;
    }
  //  qDebug()<<s;
}

void MainWindow::ReadData()
{
    QByteArray buf;
    buf = serial->readAll(); 
    static volatile int count = 0;
    int x = 0, y = 0, w = 0, h= 0;
    if(buf!=NULL)
    {
        QString str;
        str = tr(buf);
        if(str.startsWith("A")&&str.endsWith("AA\n"))
        {
            count++;
            if(count == 2)
            {
                ui->thing2->clear();
                ui->thing2->append("没有发现物体2");
            }
            ui->thing1->clear();
            str.replace("A"," ");
            ui->thing1->append(str);

            StrtoInt(str, &x, &y, &w, &h);
            this->draw->clear();

            this->draw->Draw(x,y,w,h,1);

        }
        else if(str.startsWith("B")&&str.endsWith("BB\n"))
        {
            count = 0;
            ui->thing2->clear();
            str.replace("B"," ");
            ui->thing2->append(str);

            StrtoInt(str, &x, &y, &w, &h);
            this->draw->clear();
            this->draw->Draw(x,y,w,h,0);
        }

    }
    buf.clear();
}

void MainWindow::on_openButton_clicked()
{
        if(ui->openButton->text()==tr("OPEN") && ui->comBox->currentText() != "")
        {
            serial = new QSerialPort;
            //设置串口名
            serial->setPortName(ui->comBox->currentText());
            //打开串口
            serial->open(QIODevice::ReadWrite);
            //设置波特率
            serial->setBaudRate(ui->rateBox->currentText().toInt());
            //设置数据位数
            switch(ui->databitBox->currentIndex())
            {
                case 0: serial->setDataBits(QSerialPort::Data8); break;
                case 1: serial->setDataBits(QSerialPort::Data7); break;
                default: break;
            }
            //设置奇偶校验
            switch(ui->checkbitBox->currentIndex())
            {
                case 0: serial->setParity(QSerialPort::NoParity); break;
                default: break;
            }
            //设置停止位
            switch(ui->stopbitBox->currentIndex())
            {
            case 0: serial->setStopBits(QSerialPort::OneStop); break;
            case 1: serial->setStopBits(QSerialPort::TwoStop); break;
            default: break;
            }
            //设置流控制
            serial->setFlowControl(QSerialPort::NoFlowControl);
            //关闭设置菜单使能
            ui->comBox->setEnabled(false);
            ui->rateBox->setEnabled(false);
            ui->databitBox->setEnabled(false);
            ui->checkbitBox->setEnabled(false);
            ui->stopbitBox->setEnabled(false);
            ui->openButton->setText(tr("CLOSE"));
            //连接信号槽
            QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::ReadData);
        }
        else if(ui->openButton->text()==tr("CLOSE"))
        {
            //关闭串口
            serial->clear();
            serial->close();
            serial->deleteLater();
            //恢复设置使能
            ui->comBox->setEnabled(true);
            ui->rateBox->setEnabled(true);
            ui->databitBox->setEnabled(true);
            ui->checkbitBox->setEnabled(true);
            ui->stopbitBox->setEnabled(true);
            ui->openButton->setText(tr("OPEN"));

        }
        else
        {
            return;
        }

}


void MainWindow::on_displaybutton_clicked()
{
    if(ui->displaybutton->text()==tr("Display"))
    {
        this->draw->show();
        ui->displaybutton->setText("close display");
    }
    else if(ui->displaybutton->text()==tr("close display"))
    {
        this->draw->close();
        ui->displaybutton->setText("Display");
    }

}

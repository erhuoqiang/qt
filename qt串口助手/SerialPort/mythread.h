#ifndef MYTHREAD
#define MYTHREAD
#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread();
    ~MyThread();
    void setName(const QString &name);
    void setBuad(const QString &buad);
    void setStopBits(const QString &num);
    void setParity(const QString &num);
    void setDataBits(const QString &num);
    void closePort();
protected:
    void run();

private:
    QSerialPort *serial;
};

#endif // MYTHREAD


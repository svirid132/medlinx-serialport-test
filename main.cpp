#include <QCoreApplication>
#include <QSerialPort>
#include <QDebug>
#include <QThread>
#include <QRandomGenerator>
#include <string.h>

unsigned short crc16(unsigned char *pcBlock, unsigned short len);
QByteArray uintToQByteArray(const unsigned int& num);
QByteArray ushortToQByteArray(const unsigned short& num);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSerialPort serial;
    serial.setPortName("tnt1");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setStopBits(QSerialPort::OneStop);
    if (!serial.open(QIODevice::ReadWrite)) {
        qWarning() << "dont open";
        return 1;
    }
    QRandomGenerator random;
    while(1) {
        QThread::msleep(100);
        QString text = "message";
        QByteArray data;
        const unsigned address = random.bounded(0, 50);
        const unsigned char type = random.bounded(1, 7);
        QByteArray messageData;
        if (type == 1) { // число
            qint32 num = random.generate();
            messageData = uintToQByteArray((quint32)num);
        } else if (type == 2) { // text
            messageData = text.toUtf8();
        }
        unsigned short size = messageData.size();
        QByteArray sizeBytes = ushortToQByteArray(size);
        data.append(address);
        data.append(type);
        data.append(sizeBytes);
        data.append(messageData);
        unsigned short crc = crc16((unsigned char*)data.data(), data.size());
        QByteArray crcBytes = ushortToQByteArray(crc);
        data.append(crcBytes);
        serial.write(data);
        serial.flush();
    }
    return a.exec();
}

unsigned short crc16(unsigned char *pcBlock, unsigned short len)
{
    unsigned short crc = 0xFFFF;
    unsigned char i;

    while (len--)
    {
        crc ^= *pcBlock++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

QByteArray uintToQByteArray(const quint32& num) {
    QByteArray arr;
    arr.resize(4);
    arr[0] = (num >> 24) & 0xff;
    arr[1] = (num >> 16) & 0xff;
    arr[2] = (num >> 8) & 0xff;
    arr[3] = num & 0xff;

    return arr;
}

QByteArray ushortToQByteArray(const unsigned short& num) {
    QByteArray arr;
    arr.resize(2);
    arr[0] = (num >> 8) & 0xff;
    arr[1] = num & 0xff;

    return arr;
}

#include <QCoreApplication>
#include <QSerialPort>
#include <QDebug>
#include <QThread>
#include <QRandomGenerator>

unsigned short crc16(unsigned char *pcBlock, unsigned short len);
template<typename T> QByteArray translateNumToByteArray(T num);

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
            messageData = translateNumToByteArray<qint32>(num);
        } else if (type == 2) { // text
            messageData = text.toUtf8();
        }
        unsigned short size = messageData.size();
        QByteArray sizeBytes = translateNumToByteArray<unsigned short>(size);
        data.append(address);
        data.append(type);
        data.append(sizeBytes);
        data.append(messageData);
        unsigned short crc = crc16((unsigned char*)data.data(), data.size());
        QByteArray crcBytes = translateNumToByteArray<unsigned short>(crc);
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

template<typename T> QByteArray translateNumToByteArray(T num) {
    QByteArray bytes;
    bytes.resize( sizeof(T) );
    for (int i = bytes.size() - 1; i > -1; --i) {
        bytes[i] = num & 0xFF;
        num = num >> 8;
    }

    return bytes;
}

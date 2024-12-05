#include "arduino.h"
#include "QtSerialPort/qserialportinfo.h"
#include "qdebug.h"

Arduino::Arduino(QObject *parent) : QObject(parent), isConnected(false) {
    serial = new QSerialPort(this);
}

Arduino::~Arduino() {
    closeConnection();
}

bool Arduino::connectToArduino() {
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.hasVendorIdentifier() && info.hasProductIdentifier()) {
            if (info.vendorIdentifier() == arduinoVendorId &&
                info.productIdentifier() == arduinoProductId) {
                arduinoPortName = info.portName();
                serial->setPortName(arduinoPortName);
                serial->open(QSerialPort::ReadOnly);
                serial->setBaudRate(QSerialPort::Baud9600);
                serial->setDataBits(QSerialPort::Data8);
                serial->setParity(QSerialPort::NoParity);
                serial->setStopBits(QSerialPort::OneStop);
                serial->setFlowControl(QSerialPort::NoFlowControl);
                isConnected = true;
                return true;
            }
        }
    }
    return false;
}

QString Arduino::readData() {
    static QString buffer;
    if (serial->isOpen() && serial->isReadable()) {
        QByteArray data = serial->readAll();
        buffer.append(QString::fromUtf8(data));


        if (buffer.contains("\n")) {
            QString message = buffer.section('\n', 0, 0).trimmed();
            buffer.remove(0, buffer.indexOf('\n') + 1);
            qDebug() << "Complete message received:" << message;
            return message;
        }
    }
    return QString();
}

void Arduino::closeConnection() {
    if (serial->isOpen()) {
        serial->close();
    }
}

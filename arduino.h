#ifndef ARDUINO_H
#define ARDUINO_H

#include "QtSerialPort/qserialport.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class Arduino : public QObject {
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool connectToArduino();
    QString readData();
    void closeConnection();
    void handleFireDetected(bool fire);

private:
    QSerialPort *serial;
    static const quint16 arduinoVendorId = 0x2341; // Update with your Arduino's Vendor ID
    static const quint16 arduinoProductId = 0x0043; // Update with your Arduino's Product ID
    QString arduinoPortName;
    bool isConnected;
};

#endif // ARDUINO_H

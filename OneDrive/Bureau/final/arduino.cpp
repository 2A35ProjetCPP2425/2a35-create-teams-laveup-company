#include "arduino.h"

Arduino::Arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()
{
    return serial;
}

int Arduino::connect_arduino()
{
    // Search for the port where the Arduino is connected
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                break;
            }
        }
    }

    qDebug() << "arduino_port_name is: " << arduino_port_name;

    if (arduino_is_available) {
        // Configure serial communication settings
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);        // Baud rate: 9600 bits/s
            serial->setDataBits(QSerialPort::Data8);           // Data length: 8 bits
            serial->setParity(QSerialPort::NoParity);          // No parity bit
            serial->setStopBits(QSerialPort::OneStop);         // One stop bit
            serial->setFlowControl(QSerialPort::NoFlowControl); // No flow control
            qDebug() << "Arduino connected successfully.";
            return 0; // Success
        } else {
            qDebug() << "Failed to open the serial port.";
            return 1; // Error: Failed to open port
        }
    }

    qDebug() << "Arduino is not available.";
    return -1; // Error: Arduino not available
}

int Arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Arduino connection closed successfully.";
        return 0; // Success
    }
    qDebug() << "Failed to close Arduino connection (port not open).";
    return 1; // Error
}

QByteArray Arduino::readFromArduino()
{
    if (serial->isReadable()) {
        data = serial->readAll(); // Retrieve the received data
        qDebug() << "Data read from Arduino: " << data;
        return data;
    } else {
        qDebug() << "Serial port not readable!";
        return QByteArray(); // Return an empty array on failure
    }
}

void Arduino::writeToArduino(QByteArray d)
{
    if (serial->isWritable()) {
        serial->write(d); // Send data to Arduino
        qDebug() << "Data written to Arduino: " << d;
    } else {
        qDebug() << "Couldn't write to the serial port!";
    }
}

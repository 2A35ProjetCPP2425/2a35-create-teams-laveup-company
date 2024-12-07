#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <seconddialog.h>
#include <QSerialPort>
#include <QSerialPortInfo>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QString getCardCode() const;
    QString getEmployeName();
    void getemployeemail(QString &email);
     QString getEmail() const;
     QString getEmpUid(const QString &email);
     QString getArduinoData();
     void sendGreenLEDSignal();
     void sendredLEDSignal();
     void scanrfid();






signals:
    void cardScanned();


private slots:
      void readFromArduino(); // Slot to read data from Arduino
      //void sendGreenLEDSignal();  // Function to send a signal to start the green LED






private:
    Ui::Dialog *ui;
    SecondaryDialog *secondDialog;
    bool loginSuccessful;  // Flag to track login success
    void on_creerUnCompteButton_clicked();
    void on_loginButton_clicked();
    void resetResultLabel();
    void connecttoapp();
    QSerialPort *arduino;  // Serial port for Arduino
    QString arduinoData;   // Data received from Arduino
    QString cardCode;




};

#endif // DIALOG_H

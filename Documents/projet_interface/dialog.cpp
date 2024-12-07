#include "dialog.h"
#include "ui_dialog.h"
#include "seconddialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVariant>
#include "employe.h"
#include <QTimer>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    secondDialog(new SecondaryDialog(this)),
    arduino(new QSerialPort(this))  // Initialize the second dialog
{
    ui->setupUi(this);
    connect(arduino, &QSerialPort::readyRead, this, &Dialog::readFromArduino);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.description().contains("Arduino")) { // Adjust this based on your Arduino description
            arduino->setPort(info);
            break;
        }
    }

    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);

    // Open the serial port
    if (!arduino->isOpen()) {
        if (arduino->open(QIODevice::ReadWrite)) {
            qDebug() << "Port opened again.";
        } else {
            qDebug() << "Failed to reopen port:" << arduino->errorString();
        }
    }


    connect(ui->compte, &QPushButton::clicked, this, &Dialog::connecttoapp);
    connect(ui->connecter, &QPushButton::clicked, this, &Dialog::on_loginButton_clicked);
    connect(ui->mdpo, &QPushButton::clicked, this, &Dialog::on_creerUnCompteButton_clicked);
}

Dialog::~Dialog()
{
    delete ui;
    delete secondDialog;  // Clean up
}



void Dialog::readFromArduino()
{
    QByteArray data = arduino->readAll(); // Read all available data from Arduino
    arduinoData.append(data);             // Append to buffer in case of partial data

    // Check if we have a complete line of data (e.g., if data contains a newline)
    if (arduinoData.contains('\n')) {
        // Assuming that the RFID data is the complete message, trim extra spaces/newlines
        cardCode = arduinoData.trimmed();
        arduinoData.clear(); // Clear the buffer after processing

        // Remove the "Card UID: " prefix if it exists, to get the actual UID
        if (cardCode.startsWith("Card UID: ")) {
            cardCode = cardCode.mid(10); // Remove the first 10 characters ("Card UID: ")
        }

        // Emit signal indicating a card has been scanned
        emit cardScanned();
    }
}

QString Dialog::getArduinoData()
{
    // Return the processed card code from the `cardCode` variable
    return cardCode; // This is the UID that was extracted from the Arduino data
}


void Dialog::on_creerUnCompteButton_clicked()
{
    this->hide();
    SecondaryDialog sd;
    sd.exec();     // Display the secondary dialog modally
}

void Dialog::connecttoapp(){
    this->hide();
    accept();
}

void Dialog::on_loginButton_clicked()
{
    QString identifiant = ui->email->text();  // Get the email input from the user
    QString mdp = ui->password->text();  // Get the password input from the user
    Employe emp;

    if (identifiant.isEmpty()){
        QMessageBox::warning(this, "Input Error", "Identifiant is empty.");
        return;
    }
    if (mdp.isEmpty()){
        QMessageBox::warning(this, "Input Error", "Password is empty.");
        return;
    }

    bool loginSuccess1 = emp.getemployeemailpassword(identifiant, mdp);
    bool loginSuccess2 = emp.getemployeemailpassword(identifiant, mdp);
    if (loginSuccess1) {
        // Get the welcome message with the user's name
        QString welcomeMessage = getEmployeName();

        // Set the welcome message in label_2 after successful login
        if (!welcomeMessage.isEmpty()) {
            ui->label_2->setText(welcomeMessage);  // Display the welcome message
            ui->label_2->setStyleSheet("color: green;");  // Optionally, set green color

            // Set a timer to change the text after a delay (e.g., 3 seconds)
            QTimer::singleShot(3000, this, [this]() {
                ui->label_2->setText("Please scan your RFID card.");
                ui->label_2->setStyleSheet("color: green;");  // Reset text color (optional)
            });
        } else {
            ui->label_2->setText("Failed to retrieve user name.");
            ui->label_2->setStyleSheet("color: red;");
        }

        // Create an event loop to wait for card scanning
        QEventLoop loop;
        scanrfid();
        connect(this, &Dialog::cardScanned, &loop, &QEventLoop::quit);

        // Start the event loop, this will block until cardScanned signal is emitted
        loop.exec();

        // After card is scanned, compare the card's UID with the employee's UID in the database
        QString employeeUID = emp.getEmpUid(identifiant);  // Get the UID for the given email
        QString cardUID = getArduinoData();  // Retrieve the UID from the card (Arduino data)
        // Compare the employee's UID with the scanned card's UID
        if (employeeUID == cardUID) {
            ui->label_2->setText("Card accepted. Unlocking the application...");
            ui->label_2->setStyleSheet("color: green;");

            // Set a timer to delay the execution of the accept function
            QTimer::singleShot(2000, this, &Dialog::accept); // 2000 ms = 2 seconds delay

            // Send command to start the green LED (or whatever is the action for the LED)
            sendGreenLEDSignal(); // Your function to send a signal to start the green LED

        } else {
            sendredLEDSignal();
            ui->label_2->setText("Sorry, your card is declined.");
            ui->label_2->setStyleSheet("color: red;");
            QTimer::singleShot(2000, this, [=]() {
                ui->email->clear();  // Get the email input from the user
                ui->password->clear();
                ui->label_2->setText("");  // Clear the label after the actions
            });
        }

    } else {
        // No matching record found, display an error message
        ui->label_2->setText("Incorrect username or password.");
        ui->label_2->setStyleSheet("color: red;"); // Set text color to red

        // Simulate delay while keeping the UI responsive
        QTimer::singleShot(2000, [this]() {
            ui->label_2->setText("");  // Clear the label text after 2 seconds
        });
    }
}


void Dialog::sendGreenLEDSignal()
{
    if (arduino->isOpen()) {
        QByteArray data = "g";  // Command for green LED
        arduino->write(data);
    } else {
        qDebug() << "Arduino port not open.";
    }
}

void Dialog::sendredLEDSignal()
{
    if (arduino->isOpen()) {
        QByteArray data = "r";  // Command for red LED
        arduino->write(data);
    } else {
        qDebug() << "Arduino port not open.";
    }
}
void Dialog::scanrfid()
{
    if (arduino->isOpen()) {
        QByteArray data = "s";  // Command for red LED
        arduino->write(data);
    } else {
        qDebug() << "Arduino port not open.";
    }
}






QString Dialog::getEmployeName()
{
    QString userName;
    QString login = ui->email->text();  // Get the email input from the user

    // Prepare the query to fetch the user's name based on the email (email field in the table)
    QSqlQuery query;
    query.prepare("SELECT nom FROM empeloye WHERE id_p = :login");
    query.bindValue(":login", login);  // Bind the login value to the query

    // Execute the query and check if it succeeded
    if (query.exec() && query.next()) {  // Ensure the query is successful and move to the first result
        // If the query is successful and we have a result, retrieve the name from the result
        userName = query.value("nom").toString();
    } else {
        // If the query fails or there are no results, print the error to the debug log
        qDebug() << "Failed to retrieve user name:" << query.lastError().text();
        return "";  // Return empty if not found
    }

    // Return the message with the user's name
    QString welcomeMessage = "Bienvenue " + userName + " dans notre application!";
    return welcomeMessage;
}


QString Dialog::getEmail() const {
    return ui->email->text();  // Return the email entered in the QLineEdit
}

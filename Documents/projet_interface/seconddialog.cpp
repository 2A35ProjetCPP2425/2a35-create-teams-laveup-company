#include "seconddialog.h"
#include "employe.h"
#include "ui_seconddialog.h"
#include "dialog.h"
#include <QCloseEvent>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSortFilterProxyModel>
#include <QTextStream>
#include <QDebug>
#include <QSslSocket>
#include <QMessageBox>

SecondaryDialog::SecondaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondaryDialog)
{
    ui->setupUi(this);
    connect(ui->insc, &QPushButton::clicked, this, &SecondaryDialog::sendEmailUsingGmailSmtp);

    if (!QSslSocket::supportsSsl()) {
        qDebug() << "SSL is not supported on this system.";
    }


}

SecondaryDialog::~SecondaryDialog()
{
    delete ui;
}

void SecondaryDialog::on_loginUnCompteButton_clicked()
{
    this->hide();  // Hide the current dialog

    Dialog d(nullptr);  // Create dialog without parent
    d.setWindowFlags(Qt::Window);  // Make it a regular window
    d.exec();  // Execute the Dialog modally

    this->show();  // Show the secondary dialog after the dialog is closed
}

void SecondaryDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();  // Prevent app from quitting
    this->hide();  // Hide the current dialog
    Dialog d;
    d.exec();  // Open main dialog
}

void SecondaryDialog::sendEmailUsingGmailSmtp()
{
    Employe emp;
    int emailAttemptCount = 0;
    QString id = ui->id->text();
    qDebug() << "id =" << id;

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "L'ID est vide.");
        return;
    }

    if (ui->confirmation->isChecked()) {
        QPair<QString, QString> emailPassword = emp.getEmployeePassword(id);

        if (emailPassword.first.isEmpty() || emailPassword.second.isEmpty()) {
            qDebug() << "Error: No email or password found for the given ID.";
            return;
        }

        QString recipientEmail = emailPassword.first;
        QString senderPassword = emailPassword.second;

        qDebug() << "email =" << recipientEmail;
        qDebug() << "password =" << senderPassword;

        QSslSocket socket;

        // Connect to Gmail SMTP server
        socket.connectToHostEncrypted("smtp.gmail.com", 465);
        if (!socket.waitForEncrypted()) {
            qDebug() << "Error: Could not connect to Gmail SMTP server.";
            qDebug() << "Error details:" << socket.errorString();
            return;
        }

        QTextStream stream(&socket);

        // SMTP commands
        stream << "EHLO smtp.gmail.com\r\n";
        stream.flush();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error: No response to EHLO.";
            return;
        }

        // Authenticate
        stream << "AUTH LOGIN\r\n";
        stream.flush();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error: No response to AUTH LOGIN.";
            return;
        }

        stream << QByteArray("tebourbimalek0@gmail.com").toBase64() + "\r\n";
        stream.flush();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error: No response to username.";
            return;
        }

        stream << QByteArray("qtcm sqtr qtlj wrsd").toBase64() + "\r\n";
        stream.flush();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error: No response to password.";
            return;
        }

        stream << "MAIL FROM: <tebourbimalek0@gmail.com>\r\n";
        stream.flush();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error: No response to MAIL FROM.";
            return;
        }

        stream << "RCPT TO: <" + recipientEmail + ">\r\n";
        stream.flush();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error: No response to RCPT TO.";
            return;
        }

        stream << "DATA\r\n";
        stream.flush();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error: No response to DATA.";
            return;
        }

        stream << "Subject: mot de passe oublier :\r\n";
        stream << "From: tebourbimalek0@gmail.com\r\n";
        stream << "To: " + recipientEmail + "\r\n";
        stream << "\r\n";
        stream << "identifiant : " + id + ".\r\n";
        stream << "mot de passe : " + senderPassword + ".\r\n";
        stream << ".\r\n";
        stream.flush();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error: No response to email content.";
            return;
        }

        stream << "QUIT\r\n";
        stream.flush();

        qDebug() << "Email sent successfully.";
        emailAttemptCount++;

        if (emailAttemptCount <= 3) {
            QMessageBox::information(this, "Email Sent", "Email was sent successfully.");
        } else {
            QMessageBox::warning(this, "Too Many Requests", "Too many requests. Please retry later.");
        }

    } else {
        QMessageBox::warning(this, "Input Error", "You must confirm first.");
        return;
    }
}

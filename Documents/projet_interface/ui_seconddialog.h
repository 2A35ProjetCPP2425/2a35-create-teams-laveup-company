/********************************************************************************
** Form generated from reading UI file 'seconddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECONDDIALOG_H
#define UI_SECONDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SecondaryDialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QCheckBox *confirmation;
    QPushButton *insc;
    QLineEdit *id;
    QLabel *label_8;
    QLabel *label_11;
    QLabel *label_3;

    void setupUi(QDialog *SecondaryDialog)
    {
        if (SecondaryDialog->objectName().isEmpty())
            SecondaryDialog->setObjectName(QString::fromUtf8("SecondaryDialog"));
        SecondaryDialog->resize(742, 570);
        SecondaryDialog->setStyleSheet(QString::fromUtf8("/* QWidget styling */\n"
"QWidget {\n"
"	background-color: white;\n"
"  }\n"
"\n"
"/* QLineEdit styling */\n"
"QLineEdit {\n"
"    background-color: #f0f0f0;\n"
"    border: 1px solid #5c5c5c;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"    color: #333333;\n"
"}\n"
"\n"
"QLineEdit:hover {\n"
"    border: 1px solid #0078d7;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 1px solid #3b5998;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"    color: black;\n"
"    border: 1px solid #5c5c5c;\n"
"    padding: 5px 15px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(252, 246, 243);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #C0C0C0; /* Change background color when pressed */\n"
"    border: 1px solid black; /* Change border color when pressed */\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: #C0C0C0;\n"
"    color: #a0a0a0;\n"
"    border: 1px solid #d3d3d3;\n"
"}\n"
"\n"
"\n"
"/* QLabel styling */\n"
"QLabel "
                        "{\n"
"    font-size: 14px;\n"
"    color: #333333;\n"
"    padding: 5px;\n"
"    border: none; /* Remove border if not needed */\n"
"}\n"
"\n"
"\n"
"\n"
""));
        label = new QLabel(SecondaryDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(-40, -10, 781, 231));
        label->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/WhatsApp Image 2024-10-09 \303\240 01.43.56_e93c0859.jpg")));
        label_2 = new QLabel(SecondaryDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(188, 203, 441, 81));
        label_2->setStyleSheet(QString::fromUtf8("background-color: none;"));
        confirmation = new QCheckBox(SecondaryDialog);
        confirmation->setObjectName(QString::fromUtf8("confirmation"));
        confirmation->setGeometry(QRect(11, 458, 251, 51));
        confirmation->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        insc = new QPushButton(SecondaryDialog);
        insc->setObjectName(QString::fromUtf8("insc"));
        insc->setGeometry(QRect(11, 499, 721, 61));
        id = new QLineEdit(SecondaryDialog);
        id->setObjectName(QString::fromUtf8("id"));
        id->setGeometry(QRect(10, 421, 721, 41));
        label_8 = new QLabel(SecondaryDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(4, 376, 181, 51));
        label_8->setStyleSheet(QString::fromUtf8("background-color:white ;"));
        label_11 = new QLabel(SecondaryDialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(360, 590, 371, 61));
        label_3 = new QLabel(SecondaryDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 270, 741, 101));
        label_8->raise();
        label->raise();
        confirmation->raise();
        insc->raise();
        id->raise();
        label_11->raise();
        label_3->raise();
        label_2->raise();

        retranslateUi(SecondaryDialog);

        QMetaObject::connectSlotsByName(SecondaryDialog);
    } // setupUi

    void retranslateUi(QDialog *SecondaryDialog)
    {
        SecondaryDialog->setWindowTitle(QCoreApplication::translate("SecondaryDialog", "Dialog", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("SecondaryDialog", "<html><head/><body><p><span style=\" font-size:24pt;\">mot de passe oublier ?</span></p></body></html>", nullptr));
        confirmation->setText(QCoreApplication::translate("SecondaryDialog", "Confiramtion", nullptr));
        insc->setText(QCoreApplication::translate("SecondaryDialog", "Continuer", nullptr));
        label_8->setText(QCoreApplication::translate("SecondaryDialog", "<html><head/><body><p><span style=\" font-size:11pt;\">Identifiant:</span></p></body></html>", nullptr));
        label_11->setText(QString());
        label_3->setText(QCoreApplication::translate("SecondaryDialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt;\">Renseignez vote identifiant afin de recevoir les </span></p><p align=\"center\"><span style=\" font-size:18pt;\">instruction pour le r\303\251intialiser</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SecondaryDialog: public Ui_SecondaryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECONDDIALOG_H

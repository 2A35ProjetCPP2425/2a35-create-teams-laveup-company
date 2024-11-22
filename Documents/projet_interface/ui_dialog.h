/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *label;
    QLineEdit *email;
    QLabel *label_3;
    QLineEdit *password;
    QPushButton *connecter;
    QPushButton *compte;
    QPushButton *mdpo;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_2;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(742, 769);
        Dialog->setStyleSheet(QString::fromUtf8("/* QWidget styling */\n"
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
""));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(-40, 0, 781, 251));
        label->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/WhatsApp Image 2024-10-09 \303\240 01.43.56_e93c0859.jpg")));
        email = new QLineEdit(Dialog);
        email->setObjectName(QString::fromUtf8("email"));
        email->setGeometry(QRect(200, 340, 391, 51));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(172, 410, 31, 51));
        label_3->setStyleSheet(QString::fromUtf8("border:1px solid black;"));
        label_3->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/padlock.png")));
        password = new QLineEdit(Dialog);
        password->setObjectName(QString::fromUtf8("password"));
        password->setGeometry(QRect(200, 410, 391, 51));
        password->setEchoMode(QLineEdit::Password);
        password->setCursorMoveStyle(Qt::LogicalMoveStyle);
        connecter = new QPushButton(Dialog);
        connecter->setObjectName(QString::fromUtf8("connecter"));
        connecter->setGeometry(QRect(170, 510, 421, 51));
        compte = new QPushButton(Dialog);
        compte->setObjectName(QString::fromUtf8("compte"));
        compte->setGeometry(QRect(170, 690, 421, 51));
        mdpo = new QPushButton(Dialog);
        mdpo->setObjectName(QString::fromUtf8("mdpo"));
        mdpo->setGeometry(QRect(440, 560, 171, 41));
        mdpo->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 168);\n"
"border:none;"));
        mdpo->setFlat(true);
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(170, 640, 291, 41));
        label_5 = new QLabel(Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(170, 340, 31, 51));
        label_5->setContextMenuPolicy(Qt::PreventContextMenu);
        label_5->setLayoutDirection(Qt::LeftToRight);
        label_5->setAutoFillBackground(false);
        label_5->setStyleSheet(QString::fromUtf8("border:1px solid black;"));
        label_5->setTextFormat(Qt::AutoText);
        label_5->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/user (1).png")));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 459, 421, 61));
        label_2->raise();
        label_4->raise();
        mdpo->raise();
        label_5->raise();
        label->raise();
        email->raise();
        label_3->raise();
        password->raise();
        connecter->raise();
        compte->raise();

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QString());
        email->setPlaceholderText(QCoreApplication::translate("Dialog", "identifiant", nullptr));
        label_3->setText(QString());
        password->setPlaceholderText(QCoreApplication::translate("Dialog", "password", nullptr));
        connecter->setText(QCoreApplication::translate("Dialog", "Se Connecter", nullptr));
        compte->setText(QCoreApplication::translate("Dialog", "Connexion facile en tant que Client", nullptr));
        mdpo->setText(QCoreApplication::translate("Dialog", "mot de passe oublier ?", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "vous pouvez connect en tant que client :", nullptr));
        label_5->setText(QString());
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H

/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QWidget *widget_5;
    QLineEdit *lineEdit_121;
    QLabel *label_183;
    QLabel *label_184;
    QLineEdit *lineEdit_122;
    QPushButton *pushButton_57;
    QPushButton *pushButton_58;
    QLabel *label_185;
    QPushButton *pushButton_59;
    QLabel *label_186;
    QLabel *label_187;
    QLabel *label_150;
    QLabel *label_149;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(1485, 728);
        widget_5 = new QWidget(login);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setGeometry(QRect(710, 180, 551, 491));
        lineEdit_121 = new QLineEdit(widget_5);
        lineEdit_121->setObjectName(QString::fromUtf8("lineEdit_121"));
        lineEdit_121->setGeometry(QRect(170, 30, 261, 28));
        label_183 = new QLabel(widget_5);
        label_183->setObjectName(QString::fromUtf8("label_183"));
        label_183->setGeometry(QRect(30, 20, 141, 41));
        label_184 = new QLabel(widget_5);
        label_184->setObjectName(QString::fromUtf8("label_184"));
        label_184->setGeometry(QRect(50, 130, 121, 51));
        lineEdit_122 = new QLineEdit(widget_5);
        lineEdit_122->setObjectName(QString::fromUtf8("lineEdit_122"));
        lineEdit_122->setGeometry(QRect(170, 140, 261, 28));
        pushButton_57 = new QPushButton(widget_5);
        pushButton_57->setObjectName(QString::fromUtf8("pushButton_57"));
        pushButton_57->setGeometry(QRect(170, 200, 261, 41));
        pushButton_58 = new QPushButton(widget_5);
        pushButton_58->setObjectName(QString::fromUtf8("pushButton_58"));
        pushButton_58->setGeometry(QRect(350, 250, 171, 31));
        pushButton_58->setStyleSheet(QString::fromUtf8(""));
        pushButton_58->setFlat(true);
        label_185 = new QLabel(widget_5);
        label_185->setObjectName(QString::fromUtf8("label_185"));
        label_185->setGeometry(QRect(30, 310, 261, 51));
        pushButton_59 = new QPushButton(widget_5);
        pushButton_59->setObjectName(QString::fromUtf8("pushButton_59"));
        pushButton_59->setGeometry(QRect(190, 380, 261, 41));
        pushButton_59->setFlat(true);
        label_186 = new QLabel(widget_5);
        label_186->setObjectName(QString::fromUtf8("label_186"));
        label_186->setGeometry(QRect(-540, 100, 471, 171));
        label_186->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/f3125a47-b4f4-421a-8396-6681f4e90252 (1).jpeg")));
        label_187 = new QLabel(widget_5);
        label_187->setObjectName(QString::fromUtf8("label_187"));
        label_187->setGeometry(QRect(-310, -80, 411, 81));
        QFont font;
        font.setPointSize(36);
        font.setBold(true);
        font.setItalic(true);
        label_187->setFont(font);
        label_150 = new QLabel(login);
        label_150->setObjectName(QString::fromUtf8("label_150"));
        label_150->setGeometry(QRect(460, 10, 501, 121));
        label_150->setFont(font);
        label_149 = new QLabel(login);
        label_149->setObjectName(QString::fromUtf8("label_149"));
        label_149->setGeometry(QRect(100, 320, 471, 171));
        label_149->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/f3125a47-b4f4-421a-8396-6681f4e90252 (1).jpeg")));

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Dialog", nullptr));
        label_183->setText(QCoreApplication::translate("login", "Nom d'utilisateur :", nullptr));
        label_184->setText(QCoreApplication::translate("login", "Mot de passe :", nullptr));
        pushButton_57->setText(QCoreApplication::translate("login", "Se connecter", nullptr));
        pushButton_58->setText(QCoreApplication::translate("login", "mot de passe oublier ?", nullptr));
        label_185->setText(QCoreApplication::translate("login", "vous n'avez pas un compte ?", nullptr));
        pushButton_59->setText(QCoreApplication::translate("login", "cr\303\251er un compte", nullptr));
        label_186->setText(QString());
        label_187->setText(QCoreApplication::translate("login", "<html><head/><body><p align=\"center\">Bienvenue</p></body></html>", nullptr));
        label_150->setText(QCoreApplication::translate("login", "<html><head/><body><p align=\"center\"><span style=\" font-size:48pt;\">Bienvenue</span></p></body></html>", nullptr));
        label_149->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H

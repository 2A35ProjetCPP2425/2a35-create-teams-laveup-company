#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <seconddialog.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QString getEmployeName();
    void getemployeemail(QString &email);
     QString getEmail() const;




private:
    Ui::Dialog *ui;
    SecondaryDialog *secondDialog;
    bool loginSuccessful;  // Flag to track login success
    void on_creerUnCompteButton_clicked();
    void on_loginButton_clicked();
    void resetResultLabel();
    void connecttoapp();





};

#endif // DIALOG_H


#ifndef SECONDDIALOG_H
#define SECONDDIALOG_H

#include <QDialog>

namespace Ui {
class SecondaryDialog;
}

class SecondaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecondaryDialog(QWidget *parent = nullptr);
    ~SecondaryDialog();
    void on_loginUnCompteButton_clicked();
    void closeEvent(QCloseEvent *event);



private:
    Ui::SecondaryDialog *ui;
    QDialog dialog;
};

#endif // SECONDDIALOG_H

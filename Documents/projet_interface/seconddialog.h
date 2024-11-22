#ifndef SECONDDIALOG_H
#define SECONDDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class SecondaryDialog;
}

class SecondaryDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::SecondaryDialog *ui;
    QDialog dialog;


public:

    explicit SecondaryDialog(QWidget *parent = nullptr);
    ~SecondaryDialog();
    void on_loginUnCompteButton_clicked();
    void closeEvent(QCloseEvent *event);
    void on_addclientButton_clicked();
    void on_motDePasseOublieButton_clicked();
    void onSubmit();
    void sendEmail(const QString& recipientEmail, const QString& employeeId, const QString& password);
    void sendEmailUsingGmailSmtp();











};

#endif // SECONDDIALOG_H

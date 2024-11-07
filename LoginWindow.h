#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccessful();  // Signal to notify when login is successful

private slots:
    void on_confirmButton_clicked();  // Slot for handling login confirmation

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H

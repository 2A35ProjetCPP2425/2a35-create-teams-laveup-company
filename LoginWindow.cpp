#include "LoginWindow.h"
#include "ui_LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_confirmButton_clicked()
{
    // Add your login validation logic here
    // For simplicity, let's assume login is successful
    emit loginSuccessful();  // Emit the signal that login is successful
    this->close();  // Close the login window
}

#include "seconddialog.h"
#include "ui_seconddialog.h"
#include "dialog.h"
#include <QCloseEvent>


SecondaryDialog::SecondaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondaryDialog)
{
    ui->setupUi(this);
    connect(ui->alogin, &QPushButton::clicked, this, &::SecondaryDialog::on_loginUnCompteButton_clicked);

}

SecondaryDialog::~SecondaryDialog()
{
    delete ui;
}

void SecondaryDialog::on_loginUnCompteButton_clicked()
{
    this->hide();  // Hide the current dialog

    Dialog d(nullptr);  // Make sure it has no parent
    d.setWindowFlags(Qt::Window);  // Make it a regular window
    d.exec();  // Execute the Dialog modally

    this->show();  // Show the secondary dialog after the dialog is closed
}


void SecondaryDialog::closeEvent(QCloseEvent *event)
{

    // Prevent the default close event behavior, so the app doesn't quit
    event->ignore();  // Keep the application running without quitting
    this->hide();  // Hide the SecondaryDialog
    Dialog d;
    d.exec();
}

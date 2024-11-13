#include "dialog.h"
#include "ui_dialog.h"
#include "seconddialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVariant>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    secondDialog(new SecondaryDialog(this))  // Initialize the second dialog
{
    ui->setupUi(this);
    connect(ui->compte, &QPushButton::clicked, this, &Dialog::on_creerUnCompteButton_clicked);
    connect(ui->connecter, &QPushButton::clicked, this, &Dialog::on_loginButton_clicked);
}

Dialog::~Dialog()
{
    delete ui;
    delete secondDialog;  // Clean up
}

void Dialog::on_creerUnCompteButton_clicked()
{
    this->hide();                 // Hide the first dialog
    secondDialog->exec();         // Show the second dialog modally
    this->show();                 // Show the first dialog again if needed after closing the second
}

void Dialog::on_loginButton_clicked()
{
    QString identifiant = ui->email->text();
    QString mdp = ui->password->text();

    // Prepare SQL query to check the credentials
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM empeloye WHERE MDP = :mdp and id_p = :id;");
    query.bindValue(":mdp", mdp);
    query.bindValue(":id", identifiant);

    int i = 0;

    if (query.exec()) {
        if (query.next() && query.value(0).toInt() > 0) {
            // If a record is found (i.e., count > 0), login is successful
            ui->label_2->setText("Login successful!");  // Display success message
            ui->label_2->setStyleSheet("color: green;"); // Set text color to green

            // Simulate a delay while keeping the UI responsive
            for (i = 0; i < 5000000; i++) {
                QCoreApplication::processEvents();  // Keeps the UI responsive
            }

            // After the loop, close the dialog
            accept();  // Close the dialog and return accepted status
        } else {
            // No matching record found, display an error message
            ui->label_2->setText("Incorrect username or password / u dont have an account.");
            ui->label_2->setStyleSheet("color: red;"); // Set text color to red

            // Simulate delay while keeping the UI responsive
            for (i = 0; i < 1000000; i++) {
                QCoreApplication::processEvents();  // Keeps the UI responsive
            }

            // After the loop, update the label again
            if (i >= 1000000) {
                ui->label_2->setText("");  // Reset the label text
            }
        }
    } else {
        // If the query execution failed, display a database error message
        qDebug() << "Query failed: " << query.lastError().text();
    }
}




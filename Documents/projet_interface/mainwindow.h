#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connexion.h"
#include "employe.h"
#include <QDialog> // For the login dialog


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);  // Explicit keyword added
    ~MainWindow();

private slots:
    void showAceuille();
    void showDemandeConge();
    void showMetierComm1();
    void showMetierComm2();
    void showMetierArtic1();
    void showMetierArtic2();
    void showMetierMatie1();
    void showMetierMatie2();
    void showMetierClient1();
    void showMetierClient2();
    void onSortSelectionChanged(int index);
    void on_pushButton_23_clicked();



private:
    Ui::MainWindow *ui;
    Employe emp;
    QDialog *loginDialog;  // This will be the login dialog window
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;




    // Function to hide all tab widgets
    void hideAllTabWidgets();

    // Functions related to employee management
    void on_addEmployeeButton_clicked();
    void on_supprimerEmploye_clicked();
    void on_modifyEmployeeButton_clicked();
    void on_searchButton_clicked();
    void onclearbutton();

    // Method to set up and populate the combo box
    void setupComboBoxes();

};

#endif // MAINWINDOW_H

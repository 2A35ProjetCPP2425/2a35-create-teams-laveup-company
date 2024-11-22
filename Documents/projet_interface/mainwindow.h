#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"
#include <QDialog> // For the login dialog


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &email, QWidget *parent = nullptr);  // Explicit keyword added
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
    void onDepartmentChanged();
    void onSelectionChanged(const QItemSelection &selected);
    void displayCongeStats();










private:
    Ui::MainWindow *ui;
    QString userEmail;
    Employe emp;





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
    void privilege();
    void employetelechargerPDF();
    void displayGenderStatistics();
    void bienmessage();
    void onTabChanged(int index);
    void onrasionChanged();
    void onIdChanged();
    void populateEmployeeInfo();
    void on_idEmpChanged();
    void onaddconge();
    void onUpdateTypeCongeClicked();
    void on_acceptation_clicked();
    void on_refuser_clicked();
    void refreshTableView();
    void on_searchcongeButton_clicked();
    void onclearcongebutton();
    void afficherCongeEmploye();
    void on_searchcongeButtonhistorique_clicked();
    void onclearcongebuttonhistorique();
    void updateSearchResults(const QString &searchText);
    void filterEmployees(const QString &searchText);





































};

#endif // MAINWINDOW_H

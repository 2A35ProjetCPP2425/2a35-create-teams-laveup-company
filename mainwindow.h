#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commande.h"
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

    void on_ok_2_clicked();
    void on_pushButton_supprimer_2_clicked();
    void on_pushButton_mod_clicked();
    void on_pushButton_17_clicked();

private:
    Ui::MainWindow *ui;
    void hideAllTabWidgets();// Function to hide all tab widgets
    commande cmd;

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "achat.h"
#include "commande.h"
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
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
    //void on_PDF_clicked();
    void on_trieButton_clicked();
    void on_statButton_clicked();
    void ajoutachat();
   void afficherAchats();

   void on_PDF_clicked();

private:
    Ui::MainWindow *ui;
    void hideAllTabWidgets();  // Function to hide all tab widgets
    commande cmd;
    achat ach;
    QSqlQueryModel *model_;  // Utiliser QSqlQueryModel ici


};

#endif // MAINWINDOW_H

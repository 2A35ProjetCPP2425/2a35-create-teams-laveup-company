#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client.h"
#include <QMainWindow>
#include <QTableView>
#include <QSqlError>
#include <QNetworkReply>
#include <QSqlQuery>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QGraphicsScene>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//void displayAvailableDrivers();
    void displayRegionStatistics();
    void displayLoyaltyPointsStatistics();
private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_trier_par_id_clicked();
    void on_searchButton_clicked();
    void onGeneratePdfClicked();
    void calculateAndStoreFidelityPoints();
    void applyLoyaltyDiscount();



    //void onsmsSent(bool success, const QString &message);
    void on_sendSMSButton_clicked();  // Button click slot
    void onsmsSent(bool success, const QString &message);
    QString formatPhoneNumber(const QString &phoneNumber);


private slots:
    //void on_sendSMSButton_clicked();
    // Declaration of the slot
        //void readSerialData(); // Slot to read RFID UID from serial port







private:
    Ui::MainWindow *ui;
    client CLS;  // Utilisez la bonne casse pour Client
    QSqlQueryModel *model;


};

#endif // MAINWINDOW_H

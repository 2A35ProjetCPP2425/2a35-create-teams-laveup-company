#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "arduino.h"
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

    void update_label();   // slot permettant la mise à jour du label état de la lampe 1,
        // ce slot est lancé à chaque réception d'un message de Arduino

        void on_pushButton_clicked();   // bouton ON

        void on_pushButton_2_clicked(); // bouton OFF

        void on_pushButton_3_clicked(); // bouton +

        void on_pushButton_4_clicked(); // bouton -
       // void update_label(); // Update label from Arduino data
            void readSerialData(); // Read RFID UID from Arduino serial port

private slots:
    //void on_sendSMSButton_clicked();
    // Declaration of the slot
        //void readSerialData(); // Slot to read RFID UID from serial port







private:
    Ui::MainWindow *ui;
    client CLS;  // Utilisez la bonne casse pour Client
    QSqlQueryModel *model;
    QByteArray data; // variable contenant les données reçues

        Arduino A; // objet temporaire
        QTimer *uidTimeoutTimer; // Timer for UID timeout
        bool uidProcessedRecently; // Flag for tracking recent UID processing


};

#endif // MAINWINDOW_H

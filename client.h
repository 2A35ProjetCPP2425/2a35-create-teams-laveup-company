#ifndef CLIENT_H
#define CLIENT_H
#include<QString>
#include<QSqlQueryModel>
#include <QSqlQuery>
#include <QRandomGenerator>
//#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QRandomGenerator>
#include <QTextDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QStandardItemModel>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
class client: public QObject
{
    QString NOM_CLIENT, PRENOM_CLIENT,ADRESSE,MAIL;
    int ID_CLIENT , TELEPHONE;
    int generateIdentifiant();
    int nbrCommandes;
    double totalMontant;
    double pointsFidelite;




     Q_OBJECT
public:
    client(){}
    client(int, QString, QString, int TELEPHONE, QString ADRESSE, QString MAIL);
    // Constructor without ID_CLIENT (ID is generated automatically)
    client(QString NOM_CLIENT, QString PRENOM_CLIENT, int TELEPHONE, QString ADRESSE, QString MAIL);
    QString getNom(){return NOM_CLIENT;}
    QString getPrenom(){return PRENOM_CLIENT;}
    QString getADRESSE(){return ADRESSE;}
    QString getMAIL(){return MAIL;}
    int getID(){return ID_CLIENT;}
    int getTelephone(){return TELEPHONE;}


    void setNom(QString n){NOM_CLIENT=n;}
    void setPrenom(QString p){PRENOM_CLIENT=p;}
    void setID(int id){this->ID_CLIENT=id;}
    void setTelephone(int TELEPHONE){this->TELEPHONE = TELEPHONE;}
    void setADRESSE(QString ADRESSE){this->ADRESSE=ADRESSE;}
    void setMAIL(QString MAIL){this->MAIL=MAIL;}


    QSqlQueryModel* trierParID(bool ordreCroissant = true);

    bool ajouter();
    QSqlQueryModel * afficher();
    QStandardItemModel *model;
    bool supprimer(int);
    bool modifier();
    void onSearchButtonClicked();
    void generatePdfReport();
    //bool envoyerSMS(const QString& numero, const QString& message);

    QSqlQueryModel* rechercherParPremiereLettre(const QString& lettre);
    void sendSMS(const QString &phoneNumber, const QString &message); // Fonction pour envoyer un SMS
    //QNetworkAccessManager *networkManager;

signals:
   void smsSent(bool success, const QString &message);
};

#endif // CLIENT_H*/

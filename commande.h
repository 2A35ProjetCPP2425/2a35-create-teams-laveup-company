#ifndef COMMANDE_H
#define COMMANDE_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QString>
#include <QStandardItemModel>
#include <QPrinter>

class commande
{
    QString nom,prenom,adresse;
    int id,telephone,qte_couette,qte_tapis,etat;

public:
    //constructeur
    commande(){};
    commande(int, int, int, int,int, QString , QString , QString);

    //GETTERS
    QString getnom(){return nom;}
    QString getprenom(){return prenom;}
    QString getadresse(){return adresse;}
    int getid(){return id;}
    int gettelephone(){return telephone;}
    int getqte_couette(){return qte_couette;}
    int getqte_tapis(){return qte_tapis;}
    int getetat(){return etat;}
    /*float getprix_tapis(){return prix_tapis;}
    float getprix_couette(){return prix_couette;}*/

    //SETTERS
    void setnom( QString n) { nom = n; }
    void setprenom( QString p) { prenom = p; }
    void setadresse( QString a) { adresse = a; }
    void setid(int id) {this-> id = id; }
    void settelephone(int te) {this->telephone  = te; }
    void setqte_couette(int qte_c) {this->qte_couette  = qte_c; }
    void setqte_tapis(int qte_t) {this->qte_tapis  = qte_t; }
    void setetat( int e) { etat = e; }
     /*void setprix_tapis(float pt ){prix_tapis = pt ;}
     void setprix_couette(float pc ){prix_couette = pc ;}*/

    // Fonctionnalités de Base relatives
    bool ajouter();
    bool supprimer(int);
    QSqlQueryModel *afficher();
    bool recherche(int);

    bool modifier(int  , int  , int , int , QString  , QString , QString );
    void pdf_downloader(int id);
    QString coomandecount();
    QString count();
    QVector<QString> getAndSortPrenomsAlphabetically();
    QSqlQueryModel * trie_tab();

    QSqlQueryModel* trieButton(const QString& column, bool ordreCroissant);

    void pdf_downloader();
    void generatePdfReport();
    void on_PDF_clicked(); // Ajoute cette ligne



   /* float prix_tapis,prix_couette;*/
};

#endif // COMMANDE_H

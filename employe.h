/*#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>



class Employe
{
private:
    int id,salaire;
    QString nom,prenom,email;

public:
    Employe();
    Employe(int ,int , QString , QString , QString);

    QString getnom(){return nom;}
    QString getprenom(){return prenom;}
    int getid(){return id;}

    void setNom(QString n){nom=n;}
    void setprenom(QString p){prenom=p;}
    void setid(int id){this->id=id;}


};

#endif // EMPLOYE_H*/

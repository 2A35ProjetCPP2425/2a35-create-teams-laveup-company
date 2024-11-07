#include "commande.h"

commande::commande(int id , int telephone , int qte_couette, int qte_tapis, QString nom , QString prenom, QString adresse)
{
    this->id=id;
    this->telephone=telephone;
    this->qte_couette=qte_couette;
    this->qte_tapis=qte_tapis;
    this->nom=nom;
    this->prenom=prenom;
    this->adresse=adresse;
   /* this->prix_tapis=prix_tapis;
    this->prix_couette=prix_couette;*/
}
bool commande::ajouter()
{
    QSqlQuery query;
    QString res = QString::number(id);
    QString res1 = QString::number(telephone);
    QString res2 = QString::number(qte_couette);
    QString res3 = QString::number(qte_tapis);

    query.prepare("INSERT INTO COMMANDE (IDCOMMANDE,NOM,PRENOM,ADRESSE,TELEPHONE,PRIXCOUETTE,PRIXTAPIS,QUANTITEOUETTE,QTANTITETAPIS) "
                  "VALUES (:id ,:nom , :prenom,:adresse,:telephone, NULL ,NULL,:qte_couette , :qte_tapis  )");
    query.bindValue(":id",res);
    query.bindValue(":nom",nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":adresse",adresse);

    query.bindValue(":telephone",res1);
    query.bindValue(":qte_couette",res2);
    query.bindValue(":qte_tapis",res3);

    return query.exec();
}

bool commande ::supprimer(int id){
        QSqlQuery query;
        QString res=QString ::number(id);
        query.prepare("DELETE FROM COMMANDE WHERE IDCOMMANDE = :id");
        query.bindValue(":id", res);
        return query.exec();

}

QSqlQueryModel  * commande :: afficher ()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM COMMANDE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDCOMMANDE"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("QUANTITEOUETTE"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("QTANTITETAPIS"));
    return model;
}


bool commande::modifier(int id, int telephone, int qte_couette, int qte_tapis, QString nom, QString prenom, QString adresse) {
    QSqlQuery query;
    QString res = QString::number(id);
    QString res1 = QString::number(telephone);
    QString res2 = QString::number(qte_couette);
    QString res3 = QString::number(qte_tapis);

    query.prepare("UPDATE COMMANDE SET NOM=:nom, PRENOM=:prenom, ADRESSE=:adresse, TELEPHONE=:telephone, "
                  "PRIXCOUETTE=NULL, PRIXTAPIS=NULL, QUANTITEOUETTE=:qte_couette, QTANTITETAPIS=:qte_tapis WHERE IDCOMMANDE=:id");

    query.bindValue(":id", res);
    query.bindValue(":telephone", res1);
    query.bindValue(":qte_couette", res2);
    query.bindValue(":qte_tapis", res3);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":adresse", adresse);

    return query.exec();
}


bool commande ::recherche(int id){
        QSqlQuery query;
        QString res=QString ::number(id);
        query.prepare("select * from COMMANDE where IDCOMMANDE = :id");
        query.bindValue(":id", res);
        return query.exec();

}




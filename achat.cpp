#include "achat.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

// Ajoutez
achat::achat() : ida(0), qt_tapis(0), qt_couette(0), prix_tapis(0), prix_couette(0), nom(""), prenom("")
{
    // Initialisation
}

achat::achat(int ida, int qt_tapis, int qt_couette, int prix_tapis, int prix_couette, const QString& nom, const QString& prenom)
    : ida(ida), qt_tapis(qt_tapis), qt_couette(qt_couette), prix_tapis(prix_tapis), prix_couette(prix_couette), nom(nom), prenom(prenom)
{
    // Initialisation avec des valeurs spécifiques
}

bool achat::ajouter()
{
    QSqlQuery query;
    int prix_couette = 50;
    int prix_tapis = 60;

    // Calculer le prix total
   int prix_total = (qt_couette * prix_couette) + (qt_tapis * prix_tapis);

    // Préparer la requête SQL
    query.prepare("INSERT INTO ACHAT (IDA, NOM, PRENOM, QT_COUETTE, QT_TAPIS, PRIX_COUETTE, PRIX_TAPIS, PRIXTOTAL) "
                  "VALUES (:id, :nom, :prenom, :qt_couette, :qt_tapis, :prix_couette, :prix_tapis, :prix_total)");

    // Lier les valeurs
    query.bindValue(":id", ida);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":qt_couette", qt_couette);
    query.bindValue(":qt_tapis", qt_tapis);
    query.bindValue(":prix_couette", prix_couette);
    query.bindValue(":prix_tapis", prix_tapis);
    query.bindValue(":prix_total", prix_total);

    qDebug() << "IDA:" << ida;
    qDebug() << "Nom:" << nom;
    qDebug() << "Prénom:" << prenom;
    qDebug() << "Quantité Couette:" << qt_couette;
    qDebug() << "Quantité Tapis:" << qt_tapis;
    qDebug() << "Prix Couette:" << prix_couette;
    qDebug() << "Prix Tapis:" << prix_tapis;
    qDebug() << "Prix Total:" << prix_total;

    // Exécuter la requête et vérifier le succès
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de l'achat:" << query.lastError();
        return false;
    }

    return true;
}


QSqlQueryModel  * achat :: afficher ()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM ACHAT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDA"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("QT_COUETTE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("QT_TAPIS"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("PRIX_COUETTE"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("PRIX_TAPIS"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("PRIX_TOTAL"));
    return model;
}

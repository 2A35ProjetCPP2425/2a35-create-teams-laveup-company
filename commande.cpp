#include "commande.h"
#include "qfont.h"
#include "qpdfwriter.h"
#include "qpixmap.h"
#include<QPdfWriter>
#include "qsqlerror.h"
#include"QPointer"
#include <QStandardItemModel>
#include <ui_mainwindow.h>
#include "commande.h"
#include <QPrinter>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include "commande.h"
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QStringList>
#include "commande.h"
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include <QPrinter>
#include <QTextDocument>
#include "commande.h"
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QStringList>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QPixmap>
#include <QColor>
#include <QFont>



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

void commande::pdf_downloader(int id)
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Enregistrer le rapport PDF", "", "*.pdf");
    if (filePath.isEmpty()) {
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);

    QPainter painter(&printer);

    if (!painter.isActive()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'initialiser le dessin PDF.");
        return;
    }

    // Titre avec marge supplémentaire au début
    QFont titleFont("Arial", 20, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(300, 200, "Rapport des Commandes");

    // Police pour l'en-tête et le contenu
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Arial", 10);

    painter.setFont(headerFont);

    // Marges et espacements ajustés
    int xMargin = 200;
    int yMargin = 600;
    int rowHeight = 350;
    int colWidth[] = {500, 1500, 1500, 2000, 1500, 2000, 2000, 1500, 1500};

    // En-têtes des colonnes
    QStringList headers = {"ID COMMANDE", "Nom", "Prénom", "Adresse", "Téléphone", "Prix Couette", "Prix Tapis", "Quantité Couette", "Quantité Tapis"};
    int x = xMargin;
    for (const QString& header : headers) {
        painter.drawText(x, yMargin, header);
        x += colWidth[headers.indexOf(header)];
    }

    yMargin += rowHeight;

    QSqlQuery query;
    query.prepare("SELECT* FROM COMMANDE");
    //query.bindValue(":id", id);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la requête pour récupérer les commandes.");
        return;
    }

    painter.setFont(contentFont);
    yMargin += 200;

    while (query.next()) {
        x = xMargin;

        QString idCommande = query.value("IDCOMMANDE").toString();
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();
        QString adresse = query.value("ADRESSE").toString();
        QString telephone = query.value("TELEPHONE").toString();
        QString prixCouette = query.value("PRIXCOUETTE").toString();
        QString prixTapis = query.value("PRIXTAPIS").toString();
        QString quantiteCouette = query.value("QUANTITEOUETTE").toString();
        QString quantiteTapis = query.value("QTANTITETAPIS").toString();

        painter.drawText(x, yMargin, idCommande);
        x += colWidth[0];
        painter.drawText(x, yMargin, nom);
        x += colWidth[1];
        painter.drawText(x, yMargin, prenom);
        x += colWidth[2];
        painter.drawText(x, yMargin, adresse);
        x += colWidth[3];
        painter.drawText(x, yMargin, telephone);
        x += colWidth[4];
        painter.drawText(x, yMargin, prixCouette);
        x += colWidth[5];
        painter.drawText(x, yMargin, prixTapis);
        x += colWidth[6];
        painter.drawText(x, yMargin, quantiteCouette);
        x += colWidth[7];
        painter.drawText(x, yMargin, quantiteTapis);

        yMargin += rowHeight;
    }

    painter.end();
    QMessageBox::information(nullptr, "Succès", "Rapport PDF des commandes généré avec succès !");
}



QString commande::count() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM commande");

    if (query.exec()) {
        if (query.next()) {
            int countValue = query.value(0).toInt();
            return QString::number(countValue);
        }
    } else {
        qDebug() << "Query failed: " << query.lastError().text();
    }
    return "0";
}



QSqlQueryModel* commande::trieButton(const QString& column, bool ordreCroissant) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = QString("SELECT IDCOMMANDE, NOM, PRENOM, ADRESSE, TELEPHONE, QUANTITEOUETTE, QTANTITETAPIS "
                               "FROM COMMANDE ORDER BY %1 ").arg(column);
    queryStr += ordreCroissant ? "ASC" : "DESC";

    model->setQuery(queryStr);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDCOMMANDE"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("QUANTITEOUETTE"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("QTANTITETAPIS"));
    return model;
}





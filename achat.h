#ifndef ACHAT_H
#define ACHAT_H

#include <QString>
#include <QSqlQueryModel>

class achat
{
public:
    // Constructeurs
    achat();
    achat(int ida, int qt_tapis, int qt_couette, int prix_tapis, int prix_couette, const QString& nom, const QString& prenom);

    // Méthodes
    bool ajouter(); // Ajouter un enregistrement dans la base de données
    QSqlQueryModel* afficher(); // Afficher les enregistrements de la table

private:
    // Attributs
    int ida;              // Identifiant de l'achat
    int qt_tapis;         // Quantité de tapis achetée
    int qt_couette;       // Quantité de couette achetée
    int prix_tapis;       // Prix unitaire du tapis
    int prix_couette;     // Prix unitaire de la couette
    QString nom;          // Nom du client
    QString prenom;       // Prénom du client
};

#endif // ACHAT_H

#ifndef ACHAT_H
#define ACHAT_H

#include <QSqlQueryModel>
#include <QString>

class achat
{
public:
    achat();  // Ajoutez cette ligne pour déclarer le constructeur par défaut
    achat(int ida, int qt_tapis, int qt_couette, int prix_tapis, int prix_couette, const QString& nom, const QString& prenom);

    bool ajouter();
    QSqlQueryModel* afficher();

private:
    int ida;
    int qt_tapis;
    int qt_couette;
    int prix_tapis;
    int prix_couette;
    QString nom;
    QString prenom;
};

#endif // ACHAT_H

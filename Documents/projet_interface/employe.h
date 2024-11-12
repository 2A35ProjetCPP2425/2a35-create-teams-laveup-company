#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QComboBox>
#include <QFileDialog>
#include "connexion.h"
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QRandomGenerator>


class Employe {
private:
    Connection conn; // Use your existing Connection class for database connection
    QString nom, prenom, sexe, email, mdp;
    int salaire, id_dep;
    QString id_poste; // Change id_poste from int to QString
    QString generateNumericPassword();
    QString generateIdentifiant(const QString& nom, const QString& prenom);


public:
    Employe();
    Employe(QString, QString, QString, QString, float, QString, int, QString, QDate);
    QDate date_emboche;
    void fillComboBox(QComboBox *comboBox, const QString &queryStr);

    bool idExists(const QString& id);

    // Updated declaration

    // Getters
    QString getnom() { return nom; }
    QString getprenom() { return prenom; }
    QString getsexe() { return sexe; }
    QString getemail() { return email; }
    QString getmdp() { return mdp; }
    int getid_dep() { return id_dep; }
    QString getide_poste() { return id_poste; } // Updated to return QString

    float getsalaire() { return salaire; }

    // Setters
    void setnom(QString n) { nom = n; }
    void setprenom(QString p) { prenom = p; }
    void setsexe(QString s) { sexe = s; }
    void setemail(QString e) { email = e; }
    void setsalaire(float s) { salaire = s; }
    void setmdp(QString m) { mdp = m; }
    void setdep(int dep) { id_dep = dep; }
    void setposte(QString poste) { id_poste = poste; } // Updated to accept QString

    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(QString);
    bool modifier(QString nom, QString prenom, QString sexe, QString email, float salaire, int id_dep, QString id_poste, QDate date_emboche, QString id);
    QSqlQueryModel* rechercher(QString id);
    QSqlQueryModel* tri(Qt::SortOrder order, int columnIndex);




};

#endif // EMPLOYE_H

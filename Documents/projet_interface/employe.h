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
#include <QStandardItemModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidgetItem>


class Employe {
private:
    Connection conn; // Use your existing Connection class for database connection
    QString nom, prenom, sexe, email, mdp ,id_p,id_c,adresse,login,role,id_emp,raison_counge,raison;
    int salaire, id_dep,pf;
    QString id_poste,tel; // Change id_poste from int to QString
    QString generateNumericPassword();
    QString generateIdentifiant(const QString& nom, const QString& prenom);


public:
    Employe();
    Employe(QString, QString, QString, QString, float, QString, int, QString, QDate);
    QDate date_emboche , date_d ,date_f ,type_conge;
    QString departemnt_conge,poste_conge;
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
    QString gettel(){return tel;}
    QString getadresse(){return adresse;}
    QString getlogin(){return login;}
    QString getrole(){return role;}

    QString getidemp(){return id_emp;}
    QString getraison(){return raison;}
    QString getraison_counge(){return raison_counge;}

    QString getEmpUid(const QString &email);





    float getsalaire() { return salaire; }

    // Setters
    void setnom(QString n) { nom = n; }
    void setprenom(QString p) { prenom = p; }
    void setsexe(QString s) { sexe = s; }
    void setemail(QString e) { email = e; }
    void setsalaire(float s) { salaire = s; }
    void setmdp(QString m) { mdp = m; }
    void setdep(int dep) { id_dep = dep; }
    void settel(QString tele) { tel = tele; }
    void setadresse(QString ad) { adresse = ad; }
    void setposte(QString poste) { id_poste = poste; } // Updated to accept QString
    void setlogin(QString l) { login = l; }
    void setrole(QString r) { role = r; }
    void setidemp(QString iemp) {  id_emp = iemp; }
    void setraison(QString rai){raison =rai;}
    void setraison_counge(QString rc){raison_counge =rc;}






    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(QString);
    bool modifier(QString nom, QString prenom, QString sexe, QString email, float salaire, int id_dep, QString id_poste, QDate date_emboche, QString id);
    QSqlQueryModel* rechercher(QString id);
    QSqlQueryModel* tri(Qt::SortOrder order, int columnIndex);
    bool ajouter_client();
    bool getEmailAndPasswordByEmailAndMdp(const QString &login, const QString &mdp, QString &outlogin, QString &outmdp);
    bool getemployeemailpassword(const QString &id_p, const QString &mdp);
    QString getUserRole(const QString &id_p);
    QString getUserdepartement(const QString &id_p);
    QString getUserPrivileges(const QString &id_p);
    void telechargerPDF();
    QPair<QString, QString> getEmployeePassword(const QString &id);
    bool ajouterconge();
    QString generateId();
    bool getEmployeeDetailsById(const QString &id_emp, QString &nom, QString &prenom, QString &email, QString &poste, int &departement);
    bool hasOngoingLeave(const QString& id_emp);
    void updateTypeConge();
    void updateEmployeStatus();
    QString generateConseilMessage();
    QSqlQueryModel *afficherconge();
    void addActionButtons(QTableView *tableView);
    bool updateCongeStatus(const QString &id_conge, const QString &etat, const QString &type_conge);
    QSqlQueryModel *affichercongeemploye(const QString& id_emp);
    QMap<QString, QPair<int, int>> getCongeStatsByMonth();
    void sendEmailUsingGmailSmtp();
    QPair<QString, QString> getLastAddedEmailAndPassword();
    bool employeinfo(const QString &id_emp, QString &nom, QString &prenom, QString &email, QString &mdp);








































};

#endif // EMPLOYE_H

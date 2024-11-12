#include "employe.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVariant>





Employe::Employe() {
    // Establish a connection using the Connection class
    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to the database";
    }
    srand(QTime::currentTime().msec());
    nom = "";
    prenom = "";
    sexe = "";
    email = "";
    mdp = "";
    salaire = 0;
    id_dep = 0;
    id_poste = "";
    date_emboche = QDate();// Initializes to current date

}

Employe:: Employe(QString nom, QString prenom, QString sexe, QString email, float salaire, QString mdp, int id_dep, QString id_poste, QDate date_emboche)
{
    this->nom=nom;
    this->prenom=prenom;
    this->email=email;
    this->sexe=sexe;
    this->salaire=salaire;
    this->mdp=mdp;
    this->id_dep=id_dep;
    this->id_poste=id_poste;
    this->date_emboche=date_emboche;
}

QString Employe::generateNumericPassword() {
    QString password;
    for (int i = 0; i < 8; ++i) {
        int digit = rand() % 10; //
        password.append(QString::number(digit));
    }
    return password;
}

QString Employe::generateIdentifiant(const QString& nom, const QString& prenom) {
    if (nom.isEmpty() || prenom.isEmpty()) {
        return QString();
    }


    QChar firstInitial = nom.at(0).toUpper();
    QChar lastInitial = prenom.at(0).toUpper();


    int randomNum = QRandomGenerator::global()->bounded(100000, 999999);


    return QString("%1%2%3").arg(firstInitial).arg(lastInitial).arg(randomNum);
}

bool Employe::ajouter() {
    QString id_p = generateIdentifiant(nom, prenom); // Generate the identifier
    QString mdp = generateNumericPassword(); // Generate a numeric password

    QSqlQuery query;
    query.prepare("INSERT INTO empeloye (id_p, nom, prenom, dateemboche, email, salaire, id_dep, mdp, id_poste, sexe) "
                  "VALUES (:id_p, :nom, :prenom, :dateemboche, :email, :salaire, :id_dep, :mdp, :poste, :sexe)");

    // Bind values to placeholders
    query.bindValue(":id_p", id_p);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateemboche", date_emboche.toString("yyyy-MM-dd")); // Date format
    query.bindValue(":email", email);
    query.bindValue(":salaire", salaire);
    query.bindValue(":id_dep", id_dep);
    query.bindValue(":mdp", mdp);
    query.bindValue(":poste", id_poste);
    query.bindValue(":sexe", sexe);

    // Execute the query and check for success
    if (!query.exec()) {
        qDebug() << "Error adding employee:" << query.lastError().text(); // Log the error
        return false; // Return false if the query failed
    }

    return true; // Return true if the query was successful
}




void Employe::fillComboBox(QComboBox *comboBox, const QString &queryStr) {
    if (!conn.createconnect()) {
        qDebug() << "Database connection failed!";
        return;
    }

    QSqlQuery query(queryStr);

    comboBox->clear();  // Clear existing items in the combo box
    int i = 0;
    while (query.next()) {
        QString itemText = query.value(0).toString();
        comboBox->addItem(itemText,i);
        i++;
    }

}
bool Employe::idExists(const QString& id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM empeloye WHERE id_p = :id");
    query.bindValue(":id", id); // Convert the QString to int

    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toInt() > 0; // Return true if the count is greater than 0
        }
    } else {
        qDebug() << "ID existence check query error:" << query.lastError().text();
    }

    return false; // Return false if the query fails
}

bool Employe::supprimer(QString id_p){
    QSqlQuery query;
    QString res=id_p;
    query.prepare("Delete from empeloye where id_p= :id_p");
    query.bindValue(":id_p", res);

    return query.exec();

}

QSqlQueryModel *Employe::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT e.id_p, e.nom, e.prenom, e.sexe, e.mdp, e.email, e.salaire, e.DATEEMBOCHE, "
                    "d.nom AS nom_dep, p.nom AS nom_poste "
                    "FROM EMPELOYE e "
                    "JOIN DEPARTMENT d ON e.id_dep = d.id_dep "
                    "JOIN POSTE p ON e.id_poste = p.id_poste;");

    // Check if query succeeded
    if (model->lastError().isValid()) {
        qDebug() << "Query execution error:" << model->lastError().text();
        delete model; // Clean up memory if there was an error
        return nullptr;
    }

    // Set header data for each column
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mot de passe"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date d'embauche"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Département"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Poste"));

    return model;
}



bool Employe::modifier(QString nom, QString prenom, QString sexe, QString email, float salaire, int id_dep, QString id_poste, QDate date_emboche, QString id)
{
    QStringList updateFields;


    // Dynamically construct the fields to be updated
    if (!nom.isEmpty()) {
        updateFields << "nom = :nom";
    }
    if (!prenom.isEmpty()) {
        updateFields << "prenom = :prenom";
    }
    if (!sexe.isEmpty()) {
        updateFields << "sexe = :sexe";  // Added sexe
    }
    if (!email.isEmpty()) {
        updateFields << "email = :email";
    }
    if (salaire > 0) {  // Assuming salaire should be a positive number
        updateFields << "salaire = :salaire";
    }
    if (id_dep > 0) {  // Assuming id_dep should be a positive number
        updateFields << "id_dep = :id_dep"; // Added id_dep
    }
    if (id_poste!="0") {
        updateFields << "id_poste = :id_poste"; // Added id_poste
    }
    if (date_emboche.isValid()) {
        updateFields << "dateemboche = :dateemboche"; // Added date_emboche
    }

    // Check if there are fields to update
    if (updateFields.isEmpty()) {
        qDebug() << "No fields to update.";
        return false; // Nothing to update
    }

    // Join all fields with a comma to form the final query
    QString queryString = "UPDATE empeloye SET " + updateFields.join(", ") + " WHERE id_p = :id";


    // Prepare the query
    QSqlQuery query;
    query.prepare(queryString);

    // Bind only the non-empty values
    if (!nom.isEmpty()) query.bindValue(":nom", nom);
    if (!prenom.isEmpty()) query.bindValue(":prenom", prenom);
    if (!sexe.isEmpty()) query.bindValue(":sexe", sexe); // Bind sexe
    if (!email.isEmpty()) query.bindValue(":email", email);
    if (salaire > 0) query.bindValue(":salaire", salaire);
    if (id_dep > 0) query.bindValue(":id_dep", id_dep); // Bind id_dep
    if (!id_poste.isEmpty()) query.bindValue(":id_poste", id_poste); // Bind id_poste
    if (date_emboche.isValid()) query.bindValue(":dateemboche", date_emboche.toString("yyyy-MM-dd")); // Bind date_emboche
    query.bindValue(":id", id); // Assuming 'id' is the identifier for the employee

    // Execute and check for success
    if (!query.exec()) {
        qDebug() << "Update failed:" << query.lastError();
        return false;
    }

    return true;
}

QSqlQueryModel* Employe::rechercher(QString id) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM empeloye WHERE id_p = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        model->setQuery(std::move(query));
    } else {
        qDebug() << "Search query error:" << query.lastError();
    }

    return model;
}

QSqlQueryModel* Employe::tri(Qt::SortOrder order, int columnIndex) {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Determine the sorting order (ASC or DESC)
    QString sortOrder = (order == Qt::AscendingOrder) ? "ASC" : "DESC";

    QString query;

    if (columnIndex == 1) {
        // Sorting by salaire (salary)
        query = "SELECT e.id_p, e.nom, e.prenom, e.sexe, e.mdp, e.email, e.salaire, e.DATEEMBOCHE, "
                "d.nom AS nom_dep, p.nom AS nom_poste "
                "FROM EMPELOYE e "
                "JOIN DEPARTMENT d ON e.id_dep = d.id_dep "
                "JOIN POSTE p ON e.id_poste = p.id_poste "
                "ORDER BY e.salaire " + sortOrder;
    } else if (columnIndex == 2) {
        // Sorting by salaire (salary) ascending or descending (based on the order)
        query = "SELECT e.id_p, e.nom, e.prenom, e.sexe, e.mdp, e.email, e.salaire, e.DATEEMBOCHE, "
                "d.nom AS nom_dep, p.nom AS nom_poste "
                "FROM EMPELOYE e "
                "JOIN DEPARTMENT d ON e.id_dep = d.id_dep "
                "JOIN POSTE p ON e.id_poste = p.id_poste "
                "ORDER BY e.salaire " + sortOrder;
    } else if (columnIndex == 3) {
        // Sorting by DATEEMBOCHE (hire date)
        query = "SELECT e.id_p, e.nom, e.prenom, e.sexe, e.mdp, e.email, e.salaire, e.DATEEMBOCHE, "
                "d.nom AS nom_dep, p.nom AS nom_poste "
                "FROM EMPELOYE e "
                "JOIN DEPARTMENT d ON e.id_dep = d.id_dep "
                "JOIN POSTE p ON e.id_poste = p.id_poste "
                "ORDER BY e.DATEEMBOCHE " + sortOrder;
    } else if (columnIndex == 4) {
        // Sorting by DATEEMBOCHE (hire date) in descending order (bit décroissant)
        query = "SELECT e.id_p, e.nom, e.prenom, e.sexe, e.mdp, e.email, e.salaire, e.DATEEMBOCHE, "
                "d.nom AS nom_dep, p.nom AS nom_poste "
                "FROM EMPELOYE e "
                "JOIN DEPARTMENT d ON e.id_dep = d.id_dep "
                "JOIN POSTE p ON e.id_poste = p.id_poste "
                "ORDER BY e.DATEEMBOCHE DESC"; // Always descending order
    }

    // Set the query for the model
    model->setQuery(query);

    return model;
}






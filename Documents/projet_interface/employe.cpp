#include "employe.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QSqlError>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVariant>
#include <QPdfWriter>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QtCore>
#include <QtNetwork>
#include <QCoreApplication>
#include <QAuthenticator>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QAuthenticator>
#include <QByteArray>
#include <QDebug>
#include <QPair>
#include <cmath>
#include <QStandardItemModel>
#include <QSslSocket>
#include <QTcpSocket>
#include <QTextStream>


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
    query.prepare("INSERT INTO empeloye (id_p, nom, prenom, dateemboche, email, salaire, id_dep, mdp, id_poste, sexe, role) "
                  "VALUES (:id_p, :nom, :prenom, :dateemboche, :email, :salaire, :id_dep, :mdp, :poste, :sexe ,:role)");

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
    query.bindValue(":role", role);


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



bool Employe::getemployeemailpassword(const QString &id_p, const QString &mdp)
{
    QSqlQuery query;

    // Prepare the SQL query to select email and password based on login and password
    query.prepare("SELECT COUNT(*) FROM empeloye WHERE id_p = :id_p AND MDP = :mdp;");
    query.bindValue(":id_p", id_p);
    query.bindValue(":mdp", mdp);

    // Execute the query
    if (query.exec()) {
        // If the query succeeds, check if any record is found
        if (query.next() && query.value(0).toInt() > 0) {
            return true;  // Successfully retrieved email and password
        } else {
            return false;  // No matching record found
        }
    } else {
        // If the query fails, display the error
        qDebug() << "Query failed:" << query.lastError().text();
        return false;  // Query execution failed
    }
}

QString Employe::getUserRole(const QString &id_p)
{
    QSqlQuery query;
    query.prepare("SELECT role FROM empeloye WHERE id_p = :email");
    query.bindValue(":email", id_p);

    if (query.exec() && query.next()) {
        return query.value("role").toString();
    }

    return "";  // Return empty if not found
}

QString Employe::getUserdepartement(const QString &id_p)
{
    QSqlQuery query;
    query.prepare("SELECT d.nom AS nom_dep "
                  "FROM empeloye e "
                  "JOIN department d ON e.id_dep = d.id_dep "
                  "WHERE e.id_p = :id_p;");
    query.bindValue(":id_p", id_p);  // Bind the correct parameter

    if (query.exec() && query.next()) {
        return query.value("nom_dep").toString();  // Use the correct alias
    }

    return "";  // Return empty if not found
}

QString Employe::getUserPrivileges(const QString &id_p)
{
    QString department = getUserdepartement(id_p);  // Get the department of the user
    QString role = getUserRole(id_p);  // Get the department of the user

    qDebug() << "departement: " << department;
    qDebug() << "role: " << role;



    if (department == "employe" && role == "admin") {
        return "admin employe";  // Standard user
    }else if (department == "employe" && role == "user"){
        return "user employe";
    }else if (department == "matiere_premier") {
        return "user matiere_premier";  // Admin privileges for this department
    } else if (department == "articles") {
        return "user articles";  // Admin privileges for this department
    } else if (department == "commande") {
        return "User commande";  // User privileges for this department
    } else if (department == "client") {
        return "User client";  // User privileges for this department
    } else {
        return "No department found";  // If no matching department is found
    }
}

void Employe::telechargerPDF() {
    QPdfWriter pdf("C:/Users/GIGABYTE/Documents/EmployeProfiles.pdf");
    pdf.setPageSize(QPageSize(QPageSize::A3));
    pdf.setPageOrientation(QPageLayout::Portrait);
    QPainter painter(&pdf);

    // Background color
    painter.fillRect(QRect(0, 0, pdf.width(), pdf.height()), QColor("#B6E5E9"));

    // Title
    QFont titleFont("Arial", 30, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(3800, 1400, "PROFILS DES EMPLOYÉS");

    // Set up fonts for header and content
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Times New Roman", 11);
    painter.setFont(contentFont);

    // Execute query to get all employees
    QSqlQuery query;
    query.prepare("SELECT e.id_p, e.nom, e.prenom, e.sexe, e.email, e.salaire, e.DATEEMBOCHE, "
                  "d.nom AS nom_dep, p.nom AS nom_poste "
                  "FROM EMPELOYE e "
                  "JOIN DEPARTMENT d ON e.id_dep = d.id_dep "
                  "JOIN POSTE p ON e.id_poste = p.id_poste;");
    query.exec();

    // Set up table positioning
    int initialYPosition = 2000;
    int currentYPosition = initialYPosition;
    int cellHeight = 500;
    int tableMargin = 300;

    // Define custom column widths based on expected content length
    int columnWidths[] = {1200, 1200, 1200, 800, 2000, 1400, 1500, 1200, 2600};  // Adjust widths as needed

    // Column headers
    QStringList headers = {"ID", "Nom", "Prénom", "Sexe", "Poste", "Département", "Email", "Salaire", "Date d'embauche"};

    // Draw header row
    painter.setFont(headerFont);
    painter.setPen(Qt::black);
    int xPosition = tableMargin;
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawRect(xPosition, currentYPosition, columnWidths[i], cellHeight);
        painter.drawText(xPosition + 10, currentYPosition + cellHeight / 2 + 5, headers[i]);
        xPosition += columnWidths[i];
    }
    currentYPosition += cellHeight;  // Move to the next row

    // Set alternating row colors
    QColor rowColor1("#E0F7FA");
    QColor rowColor2("#FFFFFF");

    // Draw each employee's data in a table row
    painter.setFont(contentFont);
    bool alternate = false;
    while (query.next()) {
        xPosition = tableMargin;

        // Set row background color
        QColor rowColor = alternate ? rowColor1 : rowColor2;
        painter.fillRect(xPosition, currentYPosition, pdf.width() - 2 * tableMargin, cellHeight, rowColor);
        alternate = !alternate;

        // Draw each cell in the current row
        QStringList rowData = {
            query.value("id_p").toString(), query.value("nom").toString(), query.value("prenom").toString(),
            query.value("sexe").toString(), query.value("nom_poste").toString(), query.value("nom_dep").toString(),
            query.value("email").toString(), query.value("salaire").toString(),
            query.value("DATEEMBOCHE").toDate().toString("dd/MM/yyyy")
        };

        for (int i = 0; i < rowData.size(); ++i) {
            painter.drawRect(xPosition, currentYPosition, columnWidths[i], cellHeight);

            // Draw text within the cell
            painter.drawText(xPosition + 10, currentYPosition + cellHeight / 2 + 5, rowData[i]);
            xPosition += columnWidths[i];
        }

        currentYPosition += cellHeight;  // Move down for the next row

        // Check if we need a new page
        if (currentYPosition > pdf.height() - 2000) {
            pdf.newPage();
            currentYPosition = initialYPosition;  // Reset to top of the new page

            // Re-draw header row on the new page
            xPosition = tableMargin;
            painter.setFont(headerFont);
            for (int i = 0; i < headers.size(); ++i) {
                painter.drawRect(xPosition, currentYPosition, columnWidths[i], cellHeight);
                painter.drawText(xPosition + 10, currentYPosition + cellHeight / 2 + 5, headers[i]);
                xPosition += columnWidths[i];
            }
            currentYPosition += cellHeight;  // Move to the next row after header
        }
    }

    painter.end();
}

QPair<QString, QString> Employe::getEmployeePassword(const QString &id)
{
    QSqlQuery query;
    query.prepare("SELECT mdp, email FROM empeloye WHERE id_p = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError();
        return QPair<QString, QString>(); // Return an empty pair if there's an error
    }

    if (query.next()) {
        QString password = query.value("mdp").toString();  // Retrieve password
        QString email = query.value("email").toString();    // Retrieve email
        return QPair<QString, QString>(email, password);    // Return both email and password as a pair
    } else {
        return QPair<QString, QString>(); // Return an empty pair if no result
    }
}


QString Employe::generateId() {
    // Generate a random 6-digit number between 100000 and 999999
    int randomId = QRandomGenerator::global()->bounded(100000, 1000000);
    return QString::number(randomId); // Convert the integer to a QString
}

bool Employe::ajouterconge() {
    QString id_conge = generateId(); // Generate the identifier
    QString conseilMessage = generateConseilMessage();

    QSqlQuery query;
    query.prepare("INSERT INTO conge (id_conge, id_emp, nom, prenom, email, id_dep, raison, poste, date_d, date_f, conseig) "
                  "VALUES (:id_conge, :id_p, :nom, :prenom, :email, :id_dep, :raison, :poste, :date_d, :date_f, :conseig)");

    // Bind values to placeholders
    query.bindValue(":id_conge", id_conge);
    query.bindValue(":id_p", id_emp);  // Make sure id_emp is correctly initialized
    query.bindValue(":nom", nom);      // Ensure nom is initialized
    query.bindValue(":prenom", prenom); // Ensure prenom is initialized
    query.bindValue(":email", email);  // Ensure email is initialized
    query.bindValue(":raison", raison); // Ensure raison is initialized
    query.bindValue(":id_dep", departemnt_conge); // Ensure id_dep is initialized
    query.bindValue(":poste", poste_conge); // Ensure id_poste is initialized
    query.bindValue(":date_d", date_d);  // Format date
    query.bindValue(":date_f", date_f);  // Format date
    query.bindValue(":conseig",conseilMessage);  // Correct binding for string "vide"

    // Execute the query and check for success
    if (!query.exec()) {
        qDebug() << "Error adding employee:" << query.lastError().text(); // Log the error
        return false; // Return false if the query failed
    }

    return true; // Return true if the query was successful
}


bool Employe::getEmployeeDetailsById(const QString &id_emp, QString &nom, QString &prenom, QString &email, QString &poste, int &departement)
{
    // Assuming you have a valid QSqlDatabase connection set up already
    QSqlQuery query;

    // Prepare the SQL query to get employee details, using JOINs with the 'poste' and 'departement' tables
    query.prepare("SELECT nom, prenom, email, id_poste, id_dep FROM empeloye WHERE id_p = :id_emp");

    // Bind the id_emp parameter
    query.bindValue(":id_emp", id_emp);

    // Execute the query
    if (query.exec()) {
        if (query.next()) {
            // If a result is found, retrieve the data and set to the provided references
            nom = query.value("nom").toString();
            prenom = query.value("prenom").toString();
            email = query.value("email").toString();
            poste = query.value("id_poste").toString();
            departement = query.value("id_dep").toInt();

            return true; // Successfully retrieved employee details
        } else {
            // No employee found for the given id_emp
            return false;
        }
    } else {
        // Query execution failed
        return false;
    }
}

bool Employe::hasOngoingLeave(const QString& id_emp) {
    // Create an SQL query to check if the employee already has an ongoing leave
    QSqlQuery query;

    // Prepare the SQL query to check if there is an ongoing leave (status = 'en_cour') for the given employee ID
    query.prepare("SELECT COUNT(*) FROM conge WHERE id_emp = :id_emp and type_conge = 'en cour' ");

    // Bind the employee ID to the query
    query.bindValue(":id_emp", id_emp);

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Error checking ongoing leave: " << query.lastError().text();
        return false;  // Return false if there is an error executing the query
    }

    // Check the result
    query.next();
    int count = query.value(0).toInt();

    // If count > 0, it means the employee has an ongoing leave of type "en_cour"
    return count > 0;
}

void Employe::updateTypeConge() {
    // Get today's date in the format YYYY-MM-DD
    QString today = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query, updateEmploye;

    // Step 1: Update `type_conge` to 'fin' for records in the `conge` table where DATE_F matches today
    query.prepare("UPDATE conge "
                  "SET type_conge = 'fin' "
                  "WHERE DATE_F = TO_DATE(:today, 'YYYY-MM-DD')");
    query.bindValue(":today", today);

    if (!query.exec()) {
        qDebug() << "Error updating type_conge:" << query.lastError().text();
        return;
    }
    qDebug() << "type_conge updated successfully for records with DATE_F = " << today;

    // Step 2: Retrieve all `id_emp` affected by the above update
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT DISTINCT id_emp FROM conge WHERE DATE_F = TO_DATE(:today, 'YYYY-MM-DD')");
    selectQuery.bindValue(":today", today);

    if (!selectQuery.exec()) {
        qDebug() << "Error retrieving employee IDs:" << selectQuery.lastError().text();
        return;
    }

    // Step 3: Update the `status` column in the `employe` table for each affected `id_emp`
    while (selectQuery.next()) {
        QString idEmp = selectQuery.value(0).toString();

        updateEmploye.prepare("UPDATE empeloye "
                              "SET status = 'travailler' "
                              "WHERE id_p = :id_emp");
        updateEmploye.bindValue(":id_emp", idEmp);

        if (!updateEmploye.exec()) {
            qDebug() << "Error updating status for employee ID" << idEmp << ":" << updateEmploye.lastError().text();
        } else {
            qDebug() << "Status updated to 'inactive' for employee ID:" << idEmp;
        }
    }
}


void Employe::updateEmployeStatus() {
    QSqlQuery queryCheck, queryUpdate;

    // Step 1: Retrieve all distinct employee IDs (`id_emp`) from the `conge` table where `type_conge = 'en cour'`
    queryCheck.prepare("SELECT DISTINCT id_emp FROM conge WHERE type_conge = 'en cour'");

    if (!queryCheck.exec()) {
        qDebug() << "Error retrieving employee IDs:" << queryCheck.lastError().text();
        return;
    }

    // Step 2: Update the `employe` table for each `id_emp` found in the previous query
    while (queryCheck.next()) {
        QString idEmp = queryCheck.value(0).toString(); // Get the current `id_emp`

        // Prepare the update query for the `employe` table
        queryUpdate.prepare("UPDATE empeloye SET status = 'en conge' WHERE id_p = :id_emp");
        queryUpdate.bindValue(":id_emp", idEmp);

        // Execute the update query
        if (!queryUpdate.exec()) {
            qDebug() << "Error updating status for employee ID" << idEmp << ":" << queryUpdate.lastError().text();
        } else {
            qDebug() << "Status updated to 'en conge' for employee ID:" << idEmp;
        }
    }
}

QString Employe::generateConseilMessage() {
    QSqlQuery queryCommande;

    // Step 1: Retrieve the number of tapis and couettes from the 'commande' table
    queryCommande.prepare("SELECT SUM(QUANT_COUETTE), SUM(QUANT_TAPIS) FROM commande"); // Replace 'commande' with your table name
    if (!queryCommande.exec()) {
        qDebug() << "Error retrieving commande count:" << queryCommande.lastError().text();
        return "Error retrieving the commande count.";
    }

    queryCommande.next();
    int numCouettes = queryCommande.value(0).toInt(); // Get the total number of couettes
    int numTapis = queryCommande.value(1).toInt();    // Get the total number of tapis
    qDebug() << "numCouettes:" << numCouettes;
    qDebug() << "numTapis:" << numTapis;
    // Step 2: Retrieve the number of employees with status 'travailler'
    QSqlQuery queryEmployee;
    queryEmployee.prepare("SELECT count(*) FROM empeloye WHERE status = 'travailler'"); // Replace 'empeloye' with your table name
    if (!queryEmployee.exec()) {
        qDebug() << "Error retrieving employee count:" << queryEmployee.lastError().text();
        return "Error retrieving the employee count.";
    }

    queryEmployee.next();
    int numEmployees = queryEmployee.value(0).toInt(); // Get the count of employees
    qDebug() << "numEmployees:" << numEmployees;

    // Step 3: Generate the advice message based on the counts
    QString message;


    double ratio = (numTapis + numCouettes) / (double)numEmployees;  // Cast to double for precise division
    int intRatio = (int)floor(ratio);  // Take the integer part

    qDebug() << "intRatio:" << intRatio;
    // Concatenate the message with the counts
    message = "Je vous conseille de ne pas accepter ce conge car le nombre de commandes : "
              + QString::number(numTapis) + " tapis et " + QString::number(numCouettes) +
              " couettes est plus que le nombre des employes : " + QString::number(numEmployees);

    if (intRatio > 1) {
        message += ". Le nombre d'employes est insuffisant (moins de 3 employes disponibles par rapport aux produits).";
    } else {
        message = ". Le conge peut etre accepte.";
    }

    // Return the generated message
    return message;
}

QSqlQueryModel *Employe::afficherconge() {
    QSqlQueryModel *model = new QSqlQueryModel();

    // Update the SQL query to fetch data from the conge table
    model->setQuery("SELECT id_conge, NOM, PRENOM, EMAIL, ID_DEP, POSTE, RAISON, DATE_D, DATE_F, TYPE_CONGE, CONSEIG FROM conge where type_conge='En attente';");

    // Check if the query executed successfully
    if (model->lastError().isValid()) {
        qDebug() << "Query execution error:" << model->lastError().text();
        delete model;
        return nullptr;
    }

    // Set header data for each column
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id_conge"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID_DEP"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Raison"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Debut"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Type Conge"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Conseil"));
    return model;
}

bool Employe::updateCongeStatus(const QString &id_conge, const QString &etat, const QString &type_conge)
{
    // Prepare the SQL query to update the 'etat' and 'type_conge' columns
    QSqlQuery query;
    query.prepare("UPDATE conge SET etat = :etat, type_conge = :type_conge WHERE id_conge = :id_conge");

    // Bind the values to the query
    query.bindValue(":etat", etat);
    query.bindValue(":type_conge", type_conge);
    query.bindValue(":id_conge", id_conge);

    // Execute the query and check for success
    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error updating record: " << query.lastError().text();
        return false;
    }
}


QSqlQueryModel *Employe::affichercongeemploye(const QString& id_emp) {
    QSqlQueryModel *model = new QSqlQueryModel();

    // Prepare the SQL query to fetch data from the conge table
    QSqlQuery query;
    query.prepare("SELECT id_conge, NOM, PRENOM, EMAIL, ID_DEP, POSTE, RAISON, DATE_D, DATE_F, TYPE_CONGE FROM conge WHERE id_emp = :id_emp");
    query.bindValue(":id_emp", id_emp);

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Query execution error:" << query.lastError().text();
        delete model;
        return nullptr;
    }

    // Use move semantics to set the query result to the model
    model->setQuery(std::move(query));

    // Check if the model has an error
    if (model->lastError().isValid()) {
        qDebug() << "Model error:" << model->lastError().text();
        delete model;
        return nullptr;
    }

    // Set header data for each column
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id_conge"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID_DEP"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Raison"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Debut"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Type Conge"));

    return model;
}



QMap<QString, QPair<int, int>> Employe::getCongeStatsByMonth() {
    QMap<QString, QPair<int, int>> stats; // Key: Month, Value: Pair of (Accepted, Refused)

    QSqlQuery query;
    query.prepare(R"(
        SELECT TO_CHAR(date_d, 'MM') AS month,
               SUM(CASE WHEN etat = 'yes' THEN 1 ELSE 0 END) AS accepted,
               SUM(CASE WHEN etat = 'no' THEN 1 ELSE 0 END) AS refused
        FROM conge
        GROUP BY TO_CHAR(date_d, 'MM')
        ORDER BY TO_CHAR(date_d, 'MM') ASC
    )");

    if (query.exec()) {
        while (query.next()) {
            QString month = query.value("month").toString(); // Extract month (e.g., "01", "02", ...)
            int accepted = query.value("accepted").toInt();
            int refused = query.value("refused").toInt();
            stats[month] = QPair<int, int>(accepted, refused);
        }
    } else {
        qDebug() << "Error fetching monthly stats:" << query.lastError().text();
    }

    return stats;
}


#include <tuple>


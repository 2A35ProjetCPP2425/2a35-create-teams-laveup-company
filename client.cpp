#include "client.h"
#include "qsqlquery.h"
#include <QRandomGenerator>
//#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QPrinter>      // Include QPrinter
#include <QPainter>      // Include QPainter
#include <QFileDialog>   // Include QFileDialog for file saving dialog
#include <QPageSize>
#include <QTextDocument>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QUrlQuery>
#include <QProcess>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QMessageBox>
#include <QObject>
//#include <QtCharts>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QByteArray>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QDebug>

#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QNetworkReply>



client::client(int ID_CLIENT , QString NOM_CLIENT , QString PRENOM_CLIENT , int TELEPHONE , QString ADRESSE,QString MAIL) {
    this->ID_CLIENT=ID_CLIENT;
    this->NOM_CLIENT=NOM_CLIENT;
    this->PRENOM_CLIENT=PRENOM_CLIENT;
    this->TELEPHONE=TELEPHONE;
    this->ADRESSE=ADRESSE;
    this->MAIL=MAIL;


}
// Constructor without ID_CLIENT (ID is generated automatically)
//Constructor without ID_CLIENT (ID is generated automatically)
client::client(QString NOM_CLIENT, QString PRENOM_CLIENT, int TELEPHONE, QString ADRESSE, QString MAIL)
    : NOM_CLIENT(NOM_CLIENT), PRENOM_CLIENT(PRENOM_CLIENT), ADRESSE(ADRESSE), MAIL(MAIL), TELEPHONE(TELEPHONE)
{
    this->ID_CLIENT = generateIdentifiant();  // Generate the client ID
}

/*
// Generate a unique client ID based on the name (you can modify this logic)
   int client::generateIdentifiant(QString nom, QString prenom) {
    // For example: generate an ID by concatenating first 3 letters of the name and a random number.
    QString base_id = nom.left(3).toUpper() + prenom.left(3).toUpper(); // Taking first 3 letters of each
    int random_number = QRandomGenerator::global()->bounded(100, 999); // Random number between 100 and 999

    // Combine base ID with random number
    QString id_str = base_id + QString::number(random_number);

    // Convert the ID string to integer (if needed for your database)
    return id_str.toInt(); // You could also use QString::toLongLong() if the ID is too large
}*/
int client::generateIdentifiant() {
    // Generate a random number between 100000 and 999999 (6-digit number)
    return QRandomGenerator::global()->bounded(100000, 999999);  // Random number between 100000 and 999999
}

bool client::ajouter()
{
    /*if (ID_CLIENT == 0) {
        ID_CLIENT = generateIdentifiant(NOM_CLIENT, PRENOM_CLIENT); // Use the method to generate the ID
    }*/
    int ID_CLIENT=generateIdentifiant();

    QSqlQuery query;

    // Préparation de la requête SQL
    query.prepare("INSERT INTO CLIENT (ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE,MAIL) "
                  "VALUES (:ID_CLIENT, :NOM_CLIENT, :PRENOM_CLIENT, :TELEPHONE, :ADRESSE ,:MAIL)");

    // Binding des valeurs

    query.bindValue(":ID_CLIENT", ID_CLIENT);
    query.bindValue(":NOM_CLIENT", NOM_CLIENT);
    query.bindValue(":PRENOM_CLIENT", PRENOM_CLIENT);
    query.bindValue(":TELEPHONE", TELEPHONE);
    query.bindValue(":ADRESSE", ADRESSE);
    query.bindValue(":MAIL",MAIL);

    // Exécution de la requête et retour du succès ou de l'échec
    return query.exec();
}

/*bool client::ajouter(){
    QSqlQuery query;
    QString res =QString::number(ID_CLIENT);

    query.prepare("insert into CLIENT(ID_CLIENT,NOM,PRENOM,TELEPHONE,ADRESSE,MAIL,DATE_DINSCRIPTION,PT_FIDELE)""values(:ID_CLIENT, :NOM, :PRENOM, :TELEPHONE ,:ADRESSE,: MAIL ,: DATE_DINSCRIPTION, :PT_FIDELE)");
    query.bindValue(":ID_CLIENT",res);
    query.bindValue(":NOM_CLIENT",NOM_CLIENT);
    query.bindValue(":PRENOM_CLIENT",PRENOM_CLIENT);
    query.bindValue(":TELEPHONE",TELEPHONE);
    query.bindValue(":ADRESSE",ADRESSE);

    bool success = query.exec();
    qDebug() << "ID_CLIENT:" << res << ", NOM:" << NOM_CLIENT << ", PRENOM:" << PRENOM_CLIENT
             << ", TELEPHONE:" << TELEPHONE << ", ADRESSE:" << ADRESSE;
    return success;
}*/

/*bool client::supprimer(int ID_CLIENT)
{
    QSqlQuery query;
    QString res =QString::number(ID_CLIENT);
    query.prepare("delete from client where ID_CLIENT= :id");
    query.bindValue(":ID_CLIENT ",res);
    return query.exec();
}*/
bool client::supprimer(int ID_CLIENT)
{
    QSqlQuery query;
    QString res = QString::number(ID_CLIENT);

    // Préparation de la requête SQL pour supprimer un client par son ID
    query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT = :id");

    // Binding de la valeur de l'ID
    query.bindValue(":id", res);

    // Exécution de la requête et retour du succès ou de l'échec
    return query.exec();
}
QSqlQueryModel* client::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CLIENT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_CLIENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_CLIENT"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM_CLIENT"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("POINT_DE_FIDELITE"));
    return model;
}
bool client::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE CLIENT SET NOM_CLIENT=:nom, PRENOM_CLIENT=:prenom, TELEPHONE=:tel, ADRESSE=:adresse "
                  "WHERE ID_CLIENT=:id");
    query.bindValue(":id", ID_CLIENT);
    query.bindValue(":nom", NOM_CLIENT);
    query.bindValue(":prenom", PRENOM_CLIENT);
    query.bindValue(":tel", TELEPHONE);
    query.bindValue(":adresse", ADRESSE);
    return query.exec();
}
void client::generatePdfReport() {
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

    // Titre
    QFont titleFont("Arial", 20, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(2500, 200, "Rapport des Clients");

    // Police pour l'en-tête et le contenu
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Arial", 10);

    painter.setFont(headerFont);
    int xMargin = 100;   // Marge gauche
    int yMargin = 800;   // Position initiale verticale
    int rowHeight = 250; // Hauteur entre les lignes
    int colWidth[] = {1500, 1200, 1200, 1200, 1200, 1200}; // Colonnes plus espacées

    // En-têtes des colonnes
    QStringList headers = {"ID", "Nom", "Prénom", "Téléphone", "Adresse", "Mail"};

    // Dessiner l'en-tête avec espacement
    int x = xMargin;
    for (const QString& header : headers) {
        painter.drawText(x, yMargin, header);
        x += colWidth[headers.indexOf(header)];
    }

    yMargin += rowHeight; // Avancer pour la première ligne de données

    QSqlQuery query;
    query.prepare("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE, MAIL FROM CLIENT");
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la requête pour récupérer les clients.");
        return;
    }

    painter.setFont(contentFont);

    while (query.next()) {
        x = xMargin;

        // Dessiner les données avec des colonnes espacées
        painter.drawText(x, yMargin, query.value("ID_CLIENT").toString());
        x += colWidth[0];
        painter.drawText(x, yMargin, query.value("NOM_CLIENT").toString());
        x += colWidth[1];
        painter.drawText(x, yMargin, query.value("PRENOM_CLIENT").toString());
        x += colWidth[2];
        painter.drawText(x, yMargin, query.value("TELEPHONE").toString());
        x += colWidth[3];
        painter.drawText(x, yMargin, query.value("ADRESSE").toString());
        x += colWidth[4];
        painter.drawText(x, yMargin, query.value("MAIL").toString());

        yMargin += rowHeight; // Ajouter de l'espace vertical après chaque ligne
    }

    painter.end();
    QMessageBox::information(nullptr, "Succès", "Rapport PDF généré avec succès !");
}

/*QSqlQueryModel* client::rechercherParPremiereLettre(const QString& lettre) {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Création de la requête SQL avec une clause LIKE paramétrée pour éviter les injections SQL
    QSqlQuery query;
    query.prepare("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE, MAIL "
                  "FROM client "
                  "WHERE NOM_CLIENT LIKE :lettre");

    // Ajouter le caractère '%' pour la recherche de noms commençant par `lettre`
    query.bindValue(":lettre", lettre + "%");

    // Exécution de la requête et gestion des erreurs
    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche par première lettre:" << query.lastError().text();
        delete model;
        return nullptr; // Retourne nullptr si la requête échoue
    }

    // Associer la requête au modèle si la requête a réussi
    model->setQuery(query);

    // Définir les en-têtes de colonnes pour un affichage clair dans l'interface utilisateur
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_CLIENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_CLIENT"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM_CLIENT"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("MAIL"));

    return model; // Retourne le modèle de données rempli
}*/
/*void client::generatePdfReport() {
    // Open a file dialog for the user to choose where to save the PDF
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        return; // If the user cancels, exit the function
    }

    // Ensure the file has a ".pdf" extension if the user didn't provide one
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        filePath.append(".pdf");
    }

    // Set up QPrinter to output to a PDF file
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Set up the page layout and size (A4, Portrait)
    printer.setPageSize(QPageSize(QPageSize::A4));
    QPageLayout pageLayout = printer.pageLayout();
    pageLayout.setOrientation(QPageLayout::Portrait);
    printer.setPageLayout(pageLayout);

    // Set up QPainter to draw on the printer
    QPainter painter(&printer);
    painter.setPen(Qt::black);

    // Set font size and font for better readability
    QFont font = painter.font();
    font.setPointSize(12);  // Set font size to 12 points
    painter.setFont(font);

    // Calculate the page rectangle
    QRect pageRect = printer.pageRect();

    // Set up sample text for the report
    QString reportText = "Client Report\n\n";
    reportText += "ID_CLIENT: " + QString::number(ID_CLIENT) + "\n";
    reportText += "NOM_CLIENT: " + NOM_CLIENT + "\n";
    reportText += "PRENOM_CLIENT: " + PRENOM_CLIENT + "\n";
    reportText += "TELEPHONE: " + QString::number(TELEPHONE) + "\n";
    reportText += "ADRESSE: " + ADRESSE + "\n";
    reportText += "MAIL: " + MAIL + "\n";

    // Set a margin around the text (for aesthetics)
    int margin = 20; // 20 pixels margin from edges
    QRect textRect = pageRect.adjusted(margin, margin, -margin, -margin); // Adjusted rectangle with margins

    // Draw the text onto the PDF with proper alignment and margins
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop, reportText);

    // End the QPainter and finish the PDF generation
    painter.end();
}

*/
/*void client::generatePdfReport() {
    // Open a file dialog for the user to choose where to save the PDF
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save PDF", "", "*.pdf");
    if (filePath.isEmpty()) {
        return; // If the user cancels, exit the function
    }

    // Set up QPrinter to output to a PDF file
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Set up QPainter to draw on the printer
    QPainter painter(&printer);
    painter.setPen(Qt::black);

    // Calculate page rectangle using a specific unit (QPrinter::Point)
    QRectF pageRectF = printer.pageRect(QPrinter::Point);  // This is a QRectF

    // Convert QRectF to QRect (for integer-based coordinates)
    QRect pageRect = pageRectF.toRect();  // Convert to QRect

    // Set up sample text for the report
    QString reportText = "Client Report\n";
    reportText += "ID_CLIENT: " + QString::number(ID_CLIENT) + "\n";
    reportText += "NOM_CLIENT: " + NOM_CLIENT + "\n";
    reportText += "PRENOM_CLIENT: " + PRENOM_CLIENT + "\n";
    reportText += "TELEPHONE: " + QString::number(TELEPHONE) + "\n";
    reportText += "ADRESSE: " + ADRESSE + "\n";
    reportText += "MAIL: " + MAIL + "\n";

    // Draw the text onto the PDF within the page rectangle
    painter.drawText(pageRect, Qt::AlignLeft | Qt::AlignTop, reportText);

    painter.end(); // End the QPainter
}*/
QSqlQueryModel* client::trierParID(bool ordreCroissant) {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Construire la requête SQL avec l'ordre de tri
    QString queryStr = "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE, MAIL "
                       "FROM client ORDER BY ID_CLIENT ";
    queryStr += ordreCroissant ? "ASC" : "DESC";

    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_CLIENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_CLIENT"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM_CLIENT"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("MAIL"));

    return model;
}

QSqlQueryModel* client::rechercherParPremiereLettre(const QString& lettre) {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Utilisez UPPER pour ignorer la casse dans la recherche
    model->setQuery("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE, MAIL "
                    "FROM client WHERE UPPER(NOM_CLIENT) LIKE UPPER('" + lettre + "%')");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_CLIENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_CLIENT"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM_CLIENT"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("MAIL"));

    return model;
}
//#include <QPrinter>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QFont>


#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>


#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
/*void client::generatePdfReport() {
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
    painter.drawText(300, 200, "Rapport des Clients"); // Avancer davantage le titre horizontalement

    // Police pour l'en-tête et le contenu
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Arial", 10);

    painter.setFont(headerFont);

    // Marges et espacements ajustés
    int xMargin = 200;   // Marge gauche augmentée
    int yMargin = 600;   // Position verticale plus grande pour plus d'espace avant les données
    int rowHeight = 350; // Hauteur des lignes augmentée
    int colWidth[] = {500, 1500, 1500, 1200, 3500, 2500}; // Colonnes encore plus espacées

    // En-têtes des colonnes avec espacement initial plus large
    QStringList headers = {"ID", "Nom", "Prénom", "Téléphone", "Adresse", "Mail"};
    int x = xMargin;
    for (const QString& header : headers) {
        painter.drawText(x, yMargin, header);
        x += colWidth[headers.indexOf(header)];
    }

    yMargin += rowHeight; // Avancer après l'en-tête

    QSqlQuery query;
    query.prepare("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE, MAIL FROM CLIENT");
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la requête pour récupérer les clients.");
        return;
    }

    painter.setFont(contentFont);

    // Ajouter plus d'espacement initial avant les données
    yMargin += 200;

    while (query.next()) {
        x = xMargin;

        // Dessiner les données avec un espacement maximal
        painter.drawText(x, yMargin, query.value("ID_CLIENT").toString());
        x += colWidth[0];
        painter.drawText(x, yMargin, query.value("NOM_CLIENT").toString());
        x += colWidth[1];
        painter.drawText(x, yMargin, query.value("PRENOM_CLIENT").toString());
        x += colWidth[2];
        painter.drawText(x, yMargin, query.value("TELEPHONE").toString());
        x += colWidth[3];
        painter.drawText(x, yMargin, query.value("ADRESSE").toString());
        x += colWidth[4];
        painter.drawText(x, yMargin, query.value("MAIL").toString());

        yMargin += rowHeight; // Ajouter de l'espace vertical après chaque ligne
    }

    painter.end();
    QMessageBox::information(nullptr, "Succès", "Rapport PDF généré avec succès !");
}

*/

/*void client::generatePdfReport() {
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

    QFont titleFont("Arial", 20, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(200, 200, "Rapport des Clients");

    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Arial", 10);

    painter.setFont(headerFont);
    int xMargin = 100;
    int yMargin = 400;
    int rowHeight = 150;
    int colWidth[] = {200, 600, 600, 400, 1000, 800};

    QStringList headers = {"ID", "Nom", "Prénom", "Téléphone", "Adresse", "Mail"};

    int x = xMargin;
    for (const QString& header : headers) {
        painter.drawText(x, yMargin, header);
        x += colWidth[headers.indexOf(header)];
    }

    yMargin += rowHeight;

    QSqlQuery query;
    query.prepare("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE, MAIL FROM CLIENT");
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la requête pour récupérer les clients.");
        return;
    }

    painter.setFont(contentFont);

    while (query.next()) {
        x = xMargin;
        painter.drawText(x, yMargin, query.value("ID_CLIENT").toString());
        x += colWidth[0];
        painter.drawText(x, yMargin, query.value("NOM_CLIENT").toString());
        x += colWidth[1];
        painter.drawText(x, yMargin, query.value("PRENOM_CLIENT").toString());
        x += colWidth[2];
        painter.drawText(x, yMargin, query.value("TELEPHONE").toString());
        x += colWidth[3];
        painter.drawText(x, yMargin, query.value("ADRESSE").toString());
        x += colWidth[4];
        painter.drawText(x, yMargin, query.value("MAIL").toString());
        x += colWidth[5];

        yMargin += rowHeight;
    }

    painter.end();
    QMessageBox::information(nullptr, "Succès", "Rapport PDF généré avec succès !");
}*/

/*
void client::generatePdfReport() {
    // Ouvrir une boîte de dialogue pour que l'utilisateur sélectionne où enregistrer le PDF
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Enregistrer le rapport PDF", "", "*.pdf");
    if (filePath.isEmpty()) {
        return; // L'utilisateur a annulé l'opération
    }

    // Création de l'objet QPrinter pour générer le PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Configuration de la taille de la page et de l'orientation
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);

    // Création de QPainter pour dessiner sur le PDF
    QPainter painter(&printer);

    // Vérification que QPainter a bien été initialisé
    if (!painter.isActive()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'initialiser le dessin PDF.");
        return;
    }

    // Titre du rapport
    QFont titleFont("Arial", 20, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(200, 200, "Rapport des Clients");

    // Police pour l'en-tête et le contenu
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Arial", 10);

    painter.setFont(headerFont);
    int xMargin = 100;   // Marge à gauche
    int yMargin = 400;   // Position du début de la table
    int rowHeight = 300; // Hauteur des lignes
    int colWidth[] = {500, 1000, 1000, 800, 2000, 1200}; // Largeur des colonnes

    // En-têtes des colonnes
    QStringList headers = {"ID", "Nom", "Prénom", "Téléphone", "Adresse", "Mail"};

    // Dessiner l'en-tête
    int x = xMargin;
    for (const QString& header : headers) {
        painter.drawText(x, yMargin, header);
        x += colWidth[headers.indexOf(header)];
    }

    yMargin += rowHeight; // Avancer pour la première ligne

    // Récupérer les données depuis la base
    QSqlQuery query;
    query.prepare("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE, MAIL FROM CLIENT");
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la requête pour récupérer les clients.");
        return;
    }

    // Dessiner les lignes de données
    painter.setFont(contentFont);
    bool alternateColor = false;

    while (query.next()) {
        // Dessiner chaque ligne de données
        x = xMargin;
        painter.drawText(x, yMargin, query.value("ID_CLIENT").toString());
        x += colWidth[0];
        painter.drawText(x, yMargin, query.value("NOM_CLIENT").toString());
        x += colWidth[1];
        painter.drawText(x, yMargin, query.value("PRENOM_CLIENT").toString());
        x += colWidth[2];
        painter.drawText(x, yMargin, query.value("TELEPHONE").toString());
        x += colWidth[3];
        painter.drawText(x, yMargin, query.value("ADRESSE").toString());
        x += colWidth[4];
        painter.drawText(x, yMargin, query.value("MAIL").toString());
        x += colWidth[5];

        yMargin += rowHeight;

        // Gestion de la pagination
        if (yMargin > printer.pageLayout().paintRect().height() - 200) {
            printer.newPage();
            yMargin = 400; // Revenir au début de la page

            // Dessiner l'en-tête à nouveau sur la nouvelle page
            x = xMargin;
            for (const QString& header : headers) {
                painter.drawText(x, yMargin, header);
                x += colWidth[headers.indexOf(header)];
            }
            yMargin += rowHeight; // Avancer après l'en-tête
        }
    }

    painter.end(); // Fin de l'écriture dans le PDF

    QMessageBox::information(nullptr, "Succès", "Rapport PDF généré avec succès !");
}*/



/*void client::generatePdfReport() {
    // Open a file dialog for the user to choose where to save the PDF
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save PDF", "", "*.pdf");
    if (filePath.isEmpty()) {
        return; // If the user cancels, exit the function
    }

    // Set up QPrinter to output to a PDF file
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Use QPageSize::A4 for setting the page size
    printer.setPageSize(QPageSize(QPageSize::A4));

    // Set the page orientation using QPageLayout::Portrait
    QPageLayout pageLayout = printer.pageLayout();
    pageLayout.setOrientation(QPageLayout::Portrait);
    printer.setPageLayout(pageLayout);

    // Set up QPainter to draw on the printer
    QPainter painter(&printer);
    painter.setPen(Qt::black);

    // Set a font for better readability
    QFont font = painter.font();
    font.setPointSize(12); // Set font size to 12 points
    painter.setFont(font);

    // Get the page size and layout (use the layout's paintRect to get the drawing area)
    QRectF pageRectF = pageLayout.paintRect();  // This provides the drawing area of the page (where content can be placed)

    // Convert QRectF to QRect (for integer-based coordinates)
    QRect pageRect = pageRectF.toRect(); // Convert to QRect

    // Set up sample text for the report
    QString reportText = "Client Report\n\n";
    reportText += "ID_CLIENT: " + QString::number(ID_CLIENT) + "\n";
    reportText += "NOM_CLIENT: " + NOM_CLIENT + "\n";
    reportText += "PRENOM_CLIENT: " + PRENOM_CLIENT + "\n";
    reportText += "TELEPHONE: " + QString::number(TELEPHONE) + "\n";
    reportText += "ADRESSE: " + ADRESSE + "\n";
    reportText += "MAIL: " + MAIL + "\n";

    // Set a margin around the text (for aesthetics)
    int margin = 20; // 20 pixels margin from edges
    QRect textRect = pageRect.adjusted(margin, margin, -margin, -margin); // Adjusted rectangle with margins

    // Draw the text onto the PDF with proper alignment and margins
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop, reportText);

    // End the QPainter and finish the PDF generation
    painter.end();
}*/
#include "client.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

/*client::client(int clientId, QSqlDatabase& db) : clientId(clientId), db(db) {}

int client::calculerPointsFidelite() const
{
    int totalMontant = 0;

    // Préparation de la requête SQL pour obtenir le total des prix de tapis et couvertures
    QSqlQuery query((db);
    query.prepare("SELECT COALESCE(SUM(prix_tapis), 0) + COALESCE(SUM(prix_couvertures), 0) AS total_montant "
                  "FROM commandes WHERE client_id = :clientId");
    query.bindValue(":clientId", clientId);

    // Exécution de la requête
    if (query.exec()) {
        if (query.next()) {
            totalMontant = query.value("total_montant").toInt();  // Récupérer le total des montants
        }
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
        return 0;
    }

    // Calcul des points de fidélité
    int points = 0;
    if (totalMontant > 100) {
        points = 1;  // Ajouter 1 point si le total dépasse 100 DT
    }

    return points;
}*/

/*void client::generatePdfReport() {
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Enregistrer le PDF", "", "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty()) return;
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) filePath += ".pdf";

    QTextDocument document;
    QString htmlContent;

    // Créer le contenu HTML pour le PDF
    htmlContent += "<h1 style='text-align: center;'>Rapport du Client</h1>";
    htmlContent += "<p><b>ID_CLIENT :</b> " + QString::number(ID_CLIENT) + "</p>";
    htmlContent += "<p><b>NOM_CLIENT :</b> " + NOM_CLIENT + "</p>";
    htmlContent += "<p><b>PRENOM_CLIENT :</b> " + PRENOM_CLIENT + "</p>";
    htmlContent += "<p><b>TELEPHONE :</b> " + QString::number(TELEPHONE) + "</p>";
    htmlContent += "<p><b>ADRESSE :</b> " + ADRESSE + "</p>";
    htmlContent += "<p><b>MAIL :</b> " + MAIL + "</p>";

    document.setHtml(htmlContent);
    document.print(filePath);
}*/
//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QHttpMultiPart>
//#include <QUrlQuery>
//#include <QJsonDocument>
//#include <QMessageBox>
#include <QDebug>

/*bool client::envoyerSMS(const QString& numero, const QString& message) {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    // Twilio API URL
    QUrl apiUrl("https://api.twilio.com/2010-04-01/Accounts/ACXXXXXXXXXXXXXXXXX/Messages.json");
    QNetworkRequest request(apiUrl);

    // Basic Authentication for Twilio
    QString authHeader = "Basic " + QByteArray("ACXXXXXXXXXXXXXXXXX:auth_token").toBase64();
    request.setRawHeader("Authorization", authHeader.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Construct the body of the request using QUrlQuery for form data
    QUrlQuery query;
    query.addQueryItem("To", numero);
    query.addQueryItem("From", "+12345678901");  // Your Twilio phone number
    query.addQueryItem("Body", message);

    // Send the request with form data
    QByteArray postData = query.toString(QUrl::FullyEncoded).toUtf8();

    // Set up the POST request
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(postData.size()));

    // Send the request
    QNetworkReply* reply = manager->post(request, postData);

    // Handle reply asynchronously
    QAbstractSocket::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(nullptr, "Success", "SMS sent successfully!");
        } else {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QString errorMessage = jsonResponse["message"].toString();

            qDebug() << "Error sending SMS:" << reply->errorString();
            qDebug() << "Twilio response:" << jsonResponse;
            QMessageBox::warning(nullptr, "Error", "Failed to send SMS: " + errorMessage);
        }
        reply->deleteLater();
    });

    return true;
}
*/
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
void client::sendSMS(const QString &phoneNumber, const QString &message)
{
    // Créer un gestionnaire de requêtes réseau
    QNetworkAccessManager *manager = new QNetworkAccessManager(this); // `this` will make the manager parented to Client, no need for manual cleanup

    // Utiliser ton SID et Auth Token
    QString accountSid = "ACe13007ae7ba2fc9815ad9892c505a285";  // SID Twilio
    QString authToken = "00d40a3b4d57bbd486cb6ffedb358025"; // Auth Token Twilio

    // URL de l'API Twilio (ajout de l'accountSid dans l'URL)
    QUrl url("https://api.twilio.com/2010-04-01/Accounts/ACe13007ae7ba2fc9815ad9892c505a285/Messages.json");

    QNetworkRequest request(url);

    // Définir les paramètres de la requête POST
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("To", phoneNumber);  // Numéro du destinataire
    params.addQueryItem("From", "+12672145846"); // Ton numéro Twilio
    params.addQueryItem("Body", message);    // Contenu du message

    QByteArray postData = params.toString(QUrl::FullyEncoded).toUtf8();

    // Authentification de l'API Twilio en base64
    QString authString = accountSid + ":" + authToken;
    QByteArray authData = authString.toUtf8().toBase64();
    request.setRawHeader("Authorization", "Basic " + authData);

    // Envoyer la requête POST
    QNetworkReply *reply = manager->post(request, postData);
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        // Vérifier si la requête a abouti sans erreur
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            qDebug() << "Twilio Response:" << jsonObject;

            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200 || statusCode == 201) { // Twilio renvoie 201 pour un message créé avec succès
                emit smsSent(true, "SMS sent successfully");
            } else {
                emit smsSent(false, "HTTP Error: " + QString::number(statusCode));
            }
        } else {
            emit smsSent(false, "Network Error: " + reply->errorString());
        }
        reply->deleteLater();
    });
}
    // Connecter le signal "finished" pour gérer la réponse
    /*QObject::connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        // Vérifier si la requête a abouti sans erreur
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            qDebug() << "Twilio Response:" << jsonObject;

            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200 || statusCode == 201) { // Twilio renvoie 201 pour un message créé avec succès
                emit smsSent(true, "SMS sent successfully");
            } else {
                emit smsSent(false, "HTTP Error: " + QString::number(statusCode));
            }
        } else {
            emit smsSent(false, "Network Error: " + reply->errorString());
        }
        reply->deleteLater();
    });*/
 /*   QObject::connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray responseBody = reply->readAll();
        qDebug() << "Response body:" << responseBody;

        if (reply->error() == QNetworkReply::NoError) {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "HTTP status code:" << statusCode;

            if (statusCode == 200 || statusCode == 201) {
                emit smsSent(true, "SMS sent successfully");
            } else {
                emit smsSent(false, "HTTP Error: " + QString::number(statusCode) + " - " + responseBody);
            }
        } else {
            emit smsSent(false, "Network Error: " + reply->errorString());
        }

        reply->deleteLater();
    });

}*/
/*
void client::sendSMS(const QString &phoneNumber, const QString &message)
{
    // Créer un gestionnaire de requêtes réseau
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // Utiliser ton SID et Auth Token
    QString accountSid = "AC5f3c9838b9935ea3906cf383242660cc";  // SID Twilio
    QString authToken = "953b2c8937131f61eaa9490f416e5a8d"; // Auth Token Twilio

    // URL de l'API Twilio (ajout de l'accountSid dans l'URL)
    //QUrl url("https://api.twilio.com/2010-04-01/Accounts/"+accountSid+"/Messages.json");
     QUrl url("https://api.twilio.com/2010-04-01/Accounts/AC5f3c9838b9935ea3906cf383242660cc/Messages.json");


    QNetworkRequest request(url);

    // Définir les paramètres de la requête POST
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("To", phoneNumber);  // Numéro du destinataire
    params.addQueryItem("From", "+15073386136"); // Ton numéro Twilio
    params.addQueryItem("Body", message);    // Contenu du message

    QByteArray postData = params.toString(QUrl::FullyEncoded).toUtf8();

    // Authentification de l'API Twilio en base64
    QString authString = accountSid + ":" + authToken;
    QByteArray authData = authString.toUtf8().toBase64();
    request.setRawHeader("Authorization", "Basic " + authData);

    // Envoyer la requête POST
    QNetworkReply *reply = manager->post(request, postData);

    // Connecter le signal "finished" pour gérer la réponse
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        // Vérifier si la requête a abouti sans erreur
        if (reply->error() == QNetworkReply::NoError) {
            // Vérifier le code de statut HTTP
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200) {
                // Envoi réussi
                emit smsSent(true, "SMS envoyé avec succès.");
            } else {
                // Code d'erreur HTTP retourné par Twilio
                emit smsSent(false, "Erreur HTTP : " + QString::number(statusCode));
            }
        } else {
            // Erreur d'envoi (problème de réseau ou autre)
            emit smsSent(false, "Erreur réseau : " + reply->errorString());
        }

        // Libérer les ressources
        reply->deleteLater();
    });
}
*/

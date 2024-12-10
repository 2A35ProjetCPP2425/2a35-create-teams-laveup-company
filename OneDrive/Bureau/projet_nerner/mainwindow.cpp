#include "mainwindow.h"
#include "client.h"  // Ensure this is included
#include "ui_mainwindow.h"

#include <QTableView>
#include <QRegularExpression>
#include <QMessageBox>
#include<QSqlQueryModel>
#include <QSqlQuery>
#include <QObject>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->tableView_2->setModel (CLS.afficher());
    connect(&CLS, &client::smsSent, this, &::MainWindow::onsmsSent);
    // Connect the Generate PDF button to the generatePdfReport slot
    connect(ui->pushButton_generatePdf, &QPushButton::clicked, this, &MainWindow::onGeneratePdfClicked);
    connect(ui->btnCalculateFidelity, &QPushButton::clicked, this, &MainWindow::calculateAndStoreFidelityPoints);
    client CLS;
    connect(&CLS, &client::smsSent, this, &MainWindow::onsmsSent);
    displayRegionStatistics();
    displayLoyaltyPointsStatistics();
    ui->eventComboBox->addItem("Votre commande est prête.");
    ui->eventComboBox->addItem("Bravo ! Vous avez accumulé plus de 100 points de fidélité.");
    ui->eventComboBox->addItem("Votre commande aura du retard.");


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_ajouter_clicked()
{
    QString idText = ui->lineEdit_ID->text(); int ID_CLIENT = idText.toInt();
    //int ID_CLIENT = ui->lineEdit_ID->text().toInt();       // Convertir la chaîne saisie en entier pour l'ID
    QString NOM_CLIENT = ui->lineEdit_nom->text();         // Récupérer le nom saisi
    QString PRENOM_CLIENT = ui->lineEdit_prenom->text();   // Récupérer le prénom saisi
    //int TELEPHONE = ui->lineEdit_TEL->text().toInt();
    QString telText = ui->lineEdit_TEL->text();
    int TELEPHONE = telText.toInt(); // Convertir le téléphone en entier

    // Convertir le téléphone en entier
    QString ADRESSE = ui->lineEdit_ADRESSE->text();        // Récupérer l'adresse saisie
    QString MAIL = ui->lineEdit_MAIL->text();              // Récupérer le mail saisi

    // Vérifier que tous les champs sont remplis
    if (ui->lineEdit_ID->text().isEmpty() || NOM_CLIENT.isEmpty() || PRENOM_CLIENT.isEmpty() ||
        ui->lineEdit_TEL->text().isEmpty() || ADRESSE.isEmpty() || MAIL.isEmpty())
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Tous les champs doivent être remplis.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
        return;
    }
    // Vérifier que l'ID a exactement 8 chiffres
    if (idText.length() != 8)
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("L'ID doit comporter exactement 8 chiffres.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        return;
    }

    // Vérifier que le mail contient un @ et un .
    static const QRegularExpression regex(R"(^[\w.-]+@[\w.-]+\.[a-zA-Z]{2,}$)");
    QRegularExpressionMatch match = regex.match(MAIL);
    if (!match.hasMatch())
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("L'adresse email doit contenir un @ et un .\n" "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        return;
    }
    /*// Vérifier que le mail se termine par .com
    static const QRegularExpression regex(R"(^[\w.-]+@[\w.-]+\.com$)");
    QRegularExpressionMatch match = regex.match(MAIL);
    if (!match.hasMatch()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("L'adresse email doit se terminer par .com.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
        return;
    }*/

    // Instancier un objet de la classe Client en utilisant les informations saisies
    client c(ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE, MAIL);

    // Insérer l'objet client instancié dans la table client et récupérer la valeur de retour de ajouter()
    bool test = c.ajouter();
    if (test) {  // Si la requête est exécutée
        ui->tableView_2->setModel(c.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\n"
                                             "Cliquez sur Annuler pour quitter."),
                                 QMessageBox::Cancel);

        // Message de confirmation
        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Le client a été ajouté avec succès.\n"
                                             "ID: %1\nNom: %2\nPrénom: %3\nTéléphone: %4\nAdresse: %5\nEmail: %6")
                                     .arg(ID_CLIENT)
                                     .arg(NOM_CLIENT)
                                     .arg(PRENOM_CLIENT)
                                     .arg(TELEPHONE)
                                     .arg(ADRESSE)
                                     .arg(MAIL),
                                 QMessageBox::Ok);
    } else {  // Si la requête n'est pas exécutée
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Ajout non effectué.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
    }
}


void MainWindow::on_pushButton_supprimer_clicked()
{
    int ID_CLIENT = ui->lineEdit_IDS->text().toInt();

    // Vérifier que l'ID est valide
    if (ui->lineEdit_IDS->text().isEmpty() || ID_CLIENT == 0)
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("ID client invalide.\n"
                                          "Veuillez entrer un ID valide.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
        return;
    }

    // Suppression de l'objet client instancié de la table client
    bool test = CLS.supprimer(ID_CLIENT);
    if (test) {  // Si la requête est exécutée
        ui->tableView_2->setModel(CLS.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\n"
                                             "Cliquez sur Annuler pour quitter."),
                                 QMessageBox::Cancel);
    } else {  // Si la requête n'est pas exécutée
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Suppression non effectuée.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    int id = ui->lineEdit_ID1->text().toInt();
    QString NOM_CLIENT = ui->lineEdit_nom1->text();
    QString PRENOM_CLIENT = ui->lineEdit_prenom1->text();
    QString ADRESSE_CLIENT = ui->lineEdit_ADRESSE1->text();
    int TELEPHONE = ui->lineEdit_TEL1->text().toInt();
    QString MAIL = ui->lineEdit_MAIL->text();
    // Vérifier que tous les champs sont remplis
    if (ui->lineEdit_ID1->text().isEmpty() || NOM_CLIENT.isEmpty() || PRENOM_CLIENT.isEmpty() ||
        ui->lineEdit_ADRESSE1->text().isEmpty() || ui->lineEdit_TEL1->text().isEmpty())
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Tous les champs doivent être remplis.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
        return;
    }


    client c(id, NOM_CLIENT, PRENOM_CLIENT, TELEPHONE, ADRESSE_CLIENT,MAIL);


    bool test = c.modifier();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Modifier"),
                                 QObject::tr("Client modifié.\n"
                                             "Cliquez sur Annuler pour quitter."),
                                 QMessageBox::Cancel);
        ui->tableView_2->setModel(c.afficher());
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Modification non effectuée.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
    }
}

/*void MainWindow::on_pushButton_trier_par_nom_clicked()
{ ui->tableView_2->setModel(CLS.trier_par_nom()); }*/

void MainWindow::on_searchButton_clicked() {
    // Récupère la première lettre entrée par l'utilisateur
    QString lettre = ui->searchLineEdit->text().trimmed().toUpper();

    if (lettre.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une lettre pour la recherche.");
        return;
    }

    // Appel de la fonction de recherche du client
    QSqlQueryModel* model = CLS.rechercherParPremiereLettre(lettre);

    if (model) {
        ui->tableView_2->setModel(model); // Affiche le modèle dans le tableau
    } else {
        QMessageBox::warning(this, "Erreur", "Aucun client trouvé ou erreur de recherche.");
    }
}
void MainWindow::on_pushButton_trier_par_id_clicked() {
    static bool ordreCroissant = true;  // Mémoriser l'état actuel de tri

    QSqlQueryModel* model = CLS.trierParID(ordreCroissant);
    ui->tableView_2->setModel(model);

    // Inverser l'ordre pour le prochain clic
    ordreCroissant = !ordreCroissant;
}
void MainWindow::onGeneratePdfClicked() {
    // Assuming CLS is a valid instance of your client class
    CLS.generatePdfReport();  // Generate the PDF report for the client
}

/*void MainWindow::updateClientTable()
{
    // Refresh the table with the updated client list
    client c;
    QSqlQueryModel* model = c.afficher();
    ui->tableView2->setModel(model);
}*/
/*void MainWindow::sendSMS()
{
    QString numeroClient = ui->phoneNumberEdit->text(); // Récupère le numéro
    QString message = ui->messageEdit->toPlainText();   // Récupère le message

    if (numeroClient.isEmpty() || message.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    QString accountSid = "VOTRE_ACCOUNT_SID"; // Remplacez par votre SID Twilio
    QString authToken = "VOTRE_AUTH_TOKEN";   // Remplacez par votre token Twilio
    QString fromNumber = "VOTRE_NUMERO_TWILIO"; // Numéro Twilio qui envoie le SMS

    // Construire l'URL de l'API Twilio
    QString url = "https://api.twilio.com/2010-04-01/Accounts/" + accountSid + "/Messages.json";

    // Créer une requête avec les paramètres SMS
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Encoder l'authentification en Base64 pour l'en-tête Authorization
    QString credentials = accountSid + ":" + authToken;
    QByteArray authHeader = "Basic " + credentials.toLocal8Bit().toBase64();
    request.setRawHeader("Authorization", authHeader);

    // Préparer les données de la requête
    QUrlQuery params;
    params.addQueryItem("To", numeroClient);      // Numéro du destinataire
    params.addQueryItem("From", fromNumber);      // Numéro de l'expéditeur (Twilio)
    params.addQueryItem("Body", message);         // Contenu du SMS (texte modifiable par l'utilisateur)

    // Envoyer la requête POST
    QNetworkReply *reply = manager->post(request, params.query().toUtf8());

    // Gérer la réponse
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Succès", "SMS envoyé avec succès.");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur d'envoi du SMS : " + reply->errorString());
        }
        reply->deleteLater();
    });
}*/



/*void MainWindow::on_pushButton_generatePdf_clicked() {
    CLS.generatePdfReport();
}
*/
/* this true version  void MainWindow::calculateAndStoreFidelityPoints() {
    // Charger les données des clients
    QSqlQuery query;
    if (!query.exec("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT FROM CLIENT")) {
        qDebug() << "Erreur lors de la récupération des clients:" << query.lastError().text();
        return;
    }

    // Démarrer une transaction pour garantir l'intégrité des mises à jour
    QSqlDatabase::database().transaction();

    // Parcourir les clients pour calculer les points de fidélité
    while (query.next()) {
        QString idClient = query.value("ID_CLIENT").toString();
        QString nomClient = query.value("NOM_CLIENT").toString();
        QString prenomClient = query.value("PRENOM_CLIENT").toString();

        int nbrCommandes = 0;       // Initialiser le nombre de commandes
        double totalMontant = 0.0;  // Initialiser le montant total

        // Charger les commandes pour ce client
        QSqlQuery commandQuery;
        commandQuery.prepare("SELECT PR_TAPIS, PR_COUETTE FROM COMMANDE WHERE ID_CLIENT1 = :idClient");
        commandQuery.bindValue(":idClient", idClient);

        if (!commandQuery.exec()) {
            qDebug() << "Erreur lors de la récupération des commandes pour le client" << idClient << ":" << commandQuery.lastError().text();
            continue;
        }

        // Calculer les totaux pour chaque commande
        while (commandQuery.next()) {
            double prTapis = commandQuery.value("PR_TAPIS").toDouble();
            double prCouette = commandQuery.value("PR_COUETTE").toDouble();

            totalMontant += (prTapis + prCouette); // Ajouter au montant total
            nbrCommandes++; // Compter la commande
        }

        // Vérifier s'il y a des commandes et calculer les points de fidélité
        if (nbrCommandes > 0) {
            double pointsFidelite = totalMontant * 0.1;  // Calculer les points de fidélité (10% du montant total)

            // Mettre à jour les points de fidélité dans la base de données
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE CLIENT SET \"POINTS_FIDELITE\" = :points WHERE ID_CLIENT = :idClient");
            updateQuery.bindValue(":points", pointsFidelite);
            updateQuery.bindValue(":idClient", idClient);

            if (!updateQuery.exec()) {
                qDebug() << "Erreur lors de la mise à jour des points pour le client" << idClient << ":" << updateQuery.lastError().text();
                continue;
            } else {
                qDebug() << "Points de fidélité mis à jour pour le client" << idClient << ": " << pointsFidelite;
            }
        } else {
            qDebug() << "Aucune commande trouvée pour le client" << idClient;
        }
    }

    // Si tout a fonctionné, valider la transaction
    if (!QSqlDatabase::database().commit()) {
        qDebug() << "Erreur lors de la validation de la transaction:" << QSqlDatabase::database().lastError().text();
    }

    // Créer un modèle SQL pour afficher les données des clients
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, \"POINTS_FIDELITE\" FROM CLIENT");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'exécution de la requête:" << model->lastError().text();
        return;
    }

    // Configurer les en-têtes de colonnes
    model->setHeaderData(0, Qt::Horizontal, "ID Client");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Points Fidélité");

    // Associer le modèle à la QTableView pour afficher les résultats
    ui->tableView_3->setModel(model);

    qDebug() << "Mise à jour des points de fidélité terminée.";
}*/
void MainWindow::calculateAndStoreFidelityPoints() {
    // Charger les données des clients
    QSqlQuery query;
    if (!query.exec("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT FROM CLIENT")) {
        qDebug() << "Erreur lors de la récupération des clients:" << query.lastError().text();
        return;
    }

    // Démarrer une transaction pour garantir l'intégrité des mises à jour
    if (!QSqlDatabase::database().transaction()) {
        qDebug() << "Erreur lors du démarrage de la transaction:" << QSqlDatabase::database().lastError().text();
        return;
    }

    // Parcourir les clients pour calculer les points de fidélité
    while (query.next()) {
        QString idClient = query.value("ID_CLIENT").toString();
        QString nomClient = query.value("NOM_CLIENT").toString();
        QString prenomClient = query.value("PRENOM_CLIENT").toString();

        int nbrCommandes = 0;       // Initialiser le nombre de commandes
        double totalMontant = 0.0;  // Initialiser le montant total

        // Charger les commandes pour ce client
        QSqlQuery commandQuery;
        commandQuery.prepare("SELECT PR_TAPIS, PR_COUETTE FROM COMMANDE WHERE ID_CLIENT1 = :idClient");
        commandQuery.bindValue(":idClient", idClient);

        if (!commandQuery.exec()) {
            qDebug() << "Erreur lors de la récupération des commandes pour le client" << idClient << ":" << commandQuery.lastError().text();
            continue;
        }

        // Calculer les totaux pour chaque commande
        while (commandQuery.next()) {
            double prTapis = commandQuery.value("PR_TAPIS").toDouble();
            double prCouette = commandQuery.value("PR_COUETTE").toDouble();

            totalMontant += (prTapis + prCouette); // Ajouter au montant total
            nbrCommandes++; // Compter la commande
        }

        // Vérifier s'il y a des commandes et calculer les points de fidélité
        if (nbrCommandes > 0) {
            int pointsFidelite = static_cast<int>(qFloor(totalMontant * 0.1)); // Calculer les points de fidélité (10% du montant total)

            // Mettre à jour les points de fidélité dans la base de données
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE CLIENT SET POINTS_FIDELITE = :points WHERE ID_CLIENT = :idClient");
            updateQuery.bindValue(":points", pointsFidelite);
            updateQuery.bindValue(":idClient", idClient);

            if (!updateQuery.exec()) {
                qDebug() << "Erreur lors de la mise à jour des points pour le client" << idClient << ":" << updateQuery.lastError().text();
                continue;
            } else {
                qDebug() << "Points de fidélité mis à jour pour le client" << idClient << ": " << pointsFidelite;
            }
        } else {
            qDebug() << "Aucune commande trouvée pour le client" << idClient;
        }
    }

    // Si tout a fonctionné, valider la transaction
    if (!QSqlDatabase::database().commit()) {
        qDebug() << "Erreur lors de la validation de la transaction:" << QSqlDatabase::database().lastError().text();
        return;
    }

    // Recharger les données des clients pour afficher les nouvelles données
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(""); // Efface la requête
    model->setQuery("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, POINTS_FIDELITE FROM CLIENT");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'exécution de la requête:" << model->lastError().text();
        return;
    }

    // Configurer les en-têtes de colonnes
    model->setHeaderData(0, Qt::Horizontal, "ID Client");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Points Fidélité");

    // Associer le modèle à la QTableView pour afficher les résultats
    ui->tableView_3->setModel(model);

    qDebug() << "Mise à jour des points de fidélité terminée.";
}


/*void MainWindow::calculateAndStoreFidelityPoints() {
    // Charger les données des clients
    QSqlQuery query;
    if (!query.exec("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT FROM CLIENT")) {
        qDebug() << "Erreur lors de la récupération des clients:" << query.lastError().text();
        return;
    }

    // Démarrer une transaction pour garantir l'intégrité des mises à jour
    QSqlDatabase::database().transaction();

    // Parcourir les clients pour calculer les points de fidélité
    while (query.next()) {
        QString idClient = query.value("ID_CLIENT").toString();
        QString nomClient = query.value("NOM_CLIENT").toString();
        QString prenomClient = query.value("PRENOM_CLIENT").toString();

        int nbrCommandes = 0;       // Initialiser le nombre de commandes
        double totalMontant = 0.0;  // Initialiser le montant total

        // Charger les commandes pour ce client
        QSqlQuery commandQuery;
        commandQuery.prepare("SELECT PR_TAPIS, PR_COUETTE FROM COMMANDE WHERE ID_CLIENT1 = :idClient");
        commandQuery.bindValue(":idClient", idClient);

        if (!commandQuery.exec()) {
            qDebug() << "Erreur lors de la récupération des commandes pour le client" << idClient << ":" << commandQuery.lastError().text();
            continue;
        }

        // Calculer les totaux pour chaque commande
        while (commandQuery.next()) {
            double prTapis = commandQuery.value("PR_TAPIS").toDouble();
            double prCouette = commandQuery.value("PR_COUETTE").toDouble();

            totalMontant += (prTapis + prCouette); // Ajouter au montant total
            nbrCommandes++; // Compter la commande
        }

        // Vérifier s'il y a des commandes et calculer les points de fidélité
        if (nbrCommandes > 0) {
            double pointsFidelite = totalMontant * 0.1;  // Calculer les points de fidélité (10% du montant total)

            // Tronquer les points de fidélité à un entier
            pointsFidelite = qFloor(pointsFidelite);

            // Ajouter le symbole `%` pour stockage ou affichage
            QString pointsAvecPourcentage = QString::number(static_cast<int>(pointsFidelite)) + "%";

            // Mettre à jour les points de fidélité dans la base de données
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE CLIENT SET \"POINTS_FIDELITE\" = :points WHERE ID_CLIENT = :idClient");
            updateQuery.bindValue(":points", pointsAvecPourcentage);
            updateQuery.bindValue(":idClient", idClient);

            if (!updateQuery.exec()) {
                qDebug() << "Erreur lors de la mise à jour des points pour le client" << idClient << ":" << updateQuery.lastError().text();
                continue;
            } else {
                qDebug() << "Points de fidélité mis à jour pour le client" << idClient << ": " << pointsAvecPourcentage;
            }
        } else {
            qDebug() << "Aucune commande trouvée pour le client" << idClient;
        }
    }

    // Si tout a fonctionné, valider la transaction
    if (!QSqlDatabase::database().commit()) {
        qDebug() << "Erreur lors de la validation de la transaction:" << QSqlDatabase::database().lastError().text();
    }

    // Créer un modèle SQL pour afficher les données des clients
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, " POINTS_FIDELITE" FROM CLIENT");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'exécution de la requête:" << model->lastError().text();
        return;
    }

    // Configurer les en-têtes de colonnes
    model->setHeaderData(0, Qt::Horizontal, "ID Client");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Points Fidélité");

    // Associer le modèle à la QTableView pour afficher les résultats
    ui->tableView_3->setModel(model);

    qDebug() << "Mise à jour des points de fidélité terminée.";
}
*/

/*void MainWindow::calculateAndStoreFidelityPoints() {
    // Charger les données des clients
    QSqlQuery query;
    if (!query.exec("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT FROM CLIENT")) {
        qDebug() << "Erreur lors de la récupération des clients:" << query.lastError().text();
        return;
    }

    // Démarrer une transaction pour garantir l'intégrité des mises à jour
    QSqlDatabase::database().transaction();

    // Parcourir les clients pour calculer les points de fidélité
    while (query.next()) {
        QString idClient = query.value("ID_CLIENT").toString();

        int nbrCommandes = 0;       // Initialiser le nombre de commandes
        double totalMontant = 0.0;  // Initialiser le montant total

        // Charger les commandes pour ce client
        QSqlQuery commandQuery;
        commandQuery.prepare("SELECT PR_TAPIS, PR_COUETTE FROM COMMANDE WHERE ID_CLIENT1 = :idClient");
        commandQuery.bindValue(":idClient", idClient);

        if (!commandQuery.exec()) {
            qDebug() << "Erreur lors de la récupération des commandes pour le client" << idClient << ":" << commandQuery.lastError().text();
            continue;
        }

        // Calculer les totaux pour chaque commande
        while (commandQuery.next()) {
            double prTapis = commandQuery.value("PR_TAPIS").toDouble();
            double prCouette = commandQuery.value("PR_COUETTE").toDouble();

            totalMontant += (prTapis + prCouette); // Ajouter au montant total
            nbrCommandes++; // Compter la commande
        }

        // Vérifier s'il y a des commandes et calculer les points de fidélité
        if (nbrCommandes > 0) {
            double pointsFidelite = totalMontant * 0.1;  // Calculer les points de fidélité (10% du montant total)

            // Mettre à jour les points de fidélité dans la base de données
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE CLIENT SET POINTS_FIDELITE = :points WHERE ID_CLIENT = :idClient");
            updateQuery.bindValue(":points", pointsFidelite);
            updateQuery.bindValue(":idClient", idClient);

            if (!updateQuery.exec()) {
                qDebug() << "Erreur lors de la mise à jour des points pour le client" << idClient << ":" << updateQuery.lastError().text();
            } else {
                qDebug() << "Points de fidélité mis à jour pour le client" << idClient << ": " << pointsFidelite;
            }
        }
    }

    // Si tout a fonctionné, valider la transaction
    if (!QSqlDatabase::database().commit()) {
        qDebug() << "Erreur lors de la validation de la transaction:" << QSqlDatabase::database().lastError().text();
    }

    // Créer un modèle SQL pour afficher les données des clients
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, POINTS_FIDELITE FROM CLIENT");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'exécution de la requête:" << model->lastError().text();
        return;
    }

    // Configurer les en-têtes de colonnes
    model->setHeaderData(0, Qt::Horizontal, "ID Client");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Points Fidélité");

    // Associer le modèle à la QTableView pour afficher les résultats
    ui->tableView_3->setModel(model);

    qDebug() << "Mise à jour des points de fidélité terminée.";
}*/

/*void MainWindow::calculateAndStoreFidelityPoints() {
    // Charger les données clients
    QSqlQuery query;
    if (!query.exec("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT FROM CLIENT")) {
        qDebug() << "Erreur lors de la récupération des clients:" << query.lastError().text();
        return;
    }

    // Parcourir les clients pour calculer les points de fidélité
    while (query.next()) {
        QString idClient = query.value("ID_CLIENT").toString();

        int nbrCommandes = 0;       // Initialiser le nombre de commandes
        double totalMontant = 0.0;  // Initialiser le montant total

        // Charger les commandes pour ce client
        QSqlQuery commandQuery;
        commandQuery.prepare("SELECT PR_TAPIS, PR_COUETTE FROM COMMANDE WHERE ID_CLIENT1 = :idClient");
        commandQuery.bindValue(":idClient", idClient);

        if (!commandQuery.exec()) {
            qDebug() << "Erreur lors de la récupération des commandes pour le client" << idClient << ":" << commandQuery.lastError().text();
            continue;
        }

        // Calculer les totaux pour chaque commande
        while (commandQuery.next()) {
            double prTapis = commandQuery.value("PR_TAPIS").toDouble();
            double prCouette = commandQuery.value("PR_COUETTE").toDouble();

            totalMontant += (prTapis + prCouette); // Ajouter au montant total
            nbrCommandes++; // Compter la commande
        }

        // Calculer les points de fidélité (10% du montant total)
        double pointsFidelite = totalMontant * 0.1;

        // Mettre à jour les points de fidélité dans la base de données
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE CLIENT SET POINTS_FIDELITE = :points WHERE ID_CLIENT = :idClient");
        updateQuery.bindValue(":points", pointsFidelite);
        updateQuery.bindValue(":idClient", idClient);

        if (!updateQuery.exec()) {
            qDebug() << "Erreur lors de la mise à jour des points pour le client" << idClient << ":" << updateQuery.lastError().text();
        } else {
            qDebug() << "Points de fidélité mis à jour pour le client" << idClient << ":" << pointsFidelite;
        }
    }

    // Créer un modèle SQL pour afficher les données des clients
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(
        "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, POINTS_FIDELITE FROM CLIENT");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'exécution de la requête:" << model->lastError().text();
        return;
    }

    // Configurer les en-têtes de colonnes
    model->setHeaderData(0, Qt::Horizontal, "ID Client");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Points Fidélité");

    // Associer le modèle à la QTableView pour afficher les résultats
    ui->tableView_3->setModel(model);

    qDebug() << "Mise à jour des points de fidélité terminée.";
}
*/
QString MainWindow::formatPhoneNumber(const QString &phoneNumber) {
    QString formattedPhoneNumber = phoneNumber;  // Create a local variable to store the formatted number

    // Check if the number starts with '+'
    if (!formattedPhoneNumber.startsWith("+")) {
        // Add the country prefix (example for Tunisia: +216)
        formattedPhoneNumber = "+216" + formattedPhoneNumber;
    }

    return formattedPhoneNumber;  // Return the formatted phone number
}
void MainWindow::on_sendSMSButton_clicked()
{
    // Récupérer le numéro de téléphone depuis l'interface
    QString phoneNumber = ui->phoneNumberLineEdit->text();

    // Vérifier que le numéro de téléphone n'est pas vide
    if (phoneNumber.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un numéro de téléphone.");
        return;
    }

    // Vérifier la connexion à la base de données
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur", "La connexion à la base de données est fermée.");
        return;
    }

    // Récupérer le nom et prénom du client associé au numéro
    QString clientName;
    QString clientSurname;

    QSqlQuery query;
    query.prepare("SELECT nom_client, prenom_client FROM client WHERE telephone = :phone");
    query.bindValue(":phone", phoneNumber);

    if (query.exec()) {
        if (query.next()) {
            clientName = query.value("nom_client").toString();  // Utilisation correcte des noms de colonnes
            clientSurname = query.value("prenom_client").toString();
        } else {
            QMessageBox::warning(this, "Erreur", "Aucun client trouvé avec ce numéro.");
            return;
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des informations du client : " + query.lastError().text());
        return;
    }

    // Récupérer le message sélectionné dans le ComboBox
    QString selectedEvent = ui->eventComboBox->currentText();

    // Ajouter le nom et prénom au message
    QString personalizedMessage = QString("Bonjour %1 %2,\n\n%3")
                                      .arg(clientName)
                                      .arg(clientSurname)
                                      .arg(selectedEvent);

    // Appeler la méthode sendSMS du client
    client CLS;
    CLS.sendSMS(phoneNumber, personalizedMessage);

    // Afficher un message de confirmation (ou une gestion des erreurs)
    QMessageBox::information(this, "Envoi SMS", "Le message a été envoyé avec succès au client " + clientName + " " + clientSurname + ".");
}


// Slot pour afficher un message à l'utilisateur après l'envoi du SMS
void MainWindow::onsmsSent(bool success, const QString &message)
{
    // Afficher une boîte de dialogue avec le résultat
    if (success) {
        QMessageBox::information(this, "Succès", message);
    } else {
        QMessageBox::critical(this, "Erreur", message);
    }
}

/*void MainWindow::on_sendSMSButton_clicked()
{
    // Récupérer le numéro et le message depuis l'interface (par exemple, via des champs de texte)
    QString phoneNumber = ui->phoneNumberLineEdit->text();
    QString message = ui->messageTextEdit->text();

    client client;

        // Appeler la méthode sendSMS du client
       CLS.sendSMS(phoneNumber, message);
}

// Slot pour afficher un message à l'utilisateur après l'envoi du SMS
void MainWindow::onsmsSent(bool success, const QString &message)
{
    // Afficher une boîte de dialogue avec le résultat
    if (success) {
        QMessageBox::information(this, "Succès", message);
    } else {
        QMessageBox::critical(this, "Erreur", message);}
}*/

/*void MainWindow::displayRegionStatistics()
{
    // Initialize counts for each region
    int northCount = 0;
    int centerCount = 0;
    int southCount = 0;

    // Define governorates for each region
    QStringList northGovernorates = {"Tunis", "Ariana", "Ben Arous", "La Manouba", "Bizerte", "Nabeul", "Zaghouan", "Béja", "Jendouba"};
    QStringList centerGovernorates = {"Sousse", "Monastir", "Mahdia", "Kairouan", "Sidi Bouzid", "Kasserine"};
    QStringList southGovernorates = {"Gafsa", "Tozeur", "Kebili", "Gabès", "Médenine", "Tataouine"};

    // Query the database to count employees by governorate
    QSqlQuery query;
    query.prepare("SELECT governorate, COUNT(*) as count FROM EMPLOYE GROUP BY governorate");
    query.exec();

    // Process the query results to assign counts to regions
    while (query.next()) {
        QString governorate = query.value(0).toString(); // Get governorate value from database
        int count = query.value(1).toInt(); // Get the count for the governorate

        if (northGovernorates.contains(governorate)) {
            northCount += count;
        } else if (centerGovernorates.contains(governorate)) {
            centerCount += count;
        } else if (southGovernorates.contains(governorate)) {
            southCount += count;
        }
    }

    // Create the Pie Chart Series
    QPieSeries *series = new QPieSeries();
    series->append("North", northCount);
    series->append("Center", centerCount);
    series->append("South", southCount);

    // Create a chart and add the series to it
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Employee Distribution by Region");

    // Customize the slices to add labels
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1) + "%");
    }

    // Create the QChartView for displaying the pie chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set the QChartView size to fit inside the QGraphicsView
    chartView->setFixedSize(ui->pie->size());

    // Clear the previous scene and set the new scene with the chart view
    ui->pie->setScene(new QGraphicsScene());
    ui->pie->scene()->addWidget(chartView); // Add the chart view to the scene

    // Refresh the QGraphicsView
    ui->pie->scene()->update();
    ui->pie->show(); // Make sure the pie chart is displayed
}
*/
/*
void MainWindow::displayRegionStatistics()
{
    int group0_50 = 0;
    int group51_100 = 0;
    int group101_200 = 0;
    int group200plus = 0;

    // Requête pour récupérer les points de fidélité des clients
    QSqlQuery query;
    if (!query.exec("SELECT points_fidelite FROM client")) { // Remplacez "points_fidelite" par le nom correct de votre colonne
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    // Diviser les clients en groupes selon leurs points de fidélité
    while (query.next()) {
        int points = query.value(0).toInt(); // Récupérer les points de fidélité du client

        if (points <= 50) {
            group0_50++;
        } else if (points <= 100) {
            group51_100++;
        } else if (points <= 200) {
            group101_200++;
        } else {
            group200plus++;
        }
    }

    // Création du graphique pour les groupes de points de fidélité
    QPieSeries *series = new QPieSeries();
    series->append("0-50 Points", group0_50);
    series->append("51-100 Points", group51_100);
    series->append("101-200 Points", group101_200);
    series->append("200+ Points", group200plus);

    // Ajouter un message si aucune donnée
    if (group0_50 == 0 && group51_100 == 0 && group101_200 == 0 && group200plus == 0) {
        series->append("No Data", 1);
    }

    // Configuration du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Employee Distribution by Loyalty Points");

    // Personnalisation des étiquettes
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1) + "%");
    }

    // Affichage du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(ui->pie->width(), ui->pie->height()); // Cette ligne garantit que le graphique s'adapte à la taille de votre QGraphicsView

    // Affectation du graphique à la vue
    ui->pie->setScene(new QGraphicsScene());
    ui->pie->scene()->addWidget(chartView); // Ajouter le graphique à la scène

    ui->pie->scene()->update();
    ui->pie->show(); // S'assurer que le graphique est affiché

    qDebug() << "Points chart displayed successfully.";
}*/
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
void MainWindow::displayRegionStatistics() {

        // Vérifier la connexion à la base de données
        if (!QSqlDatabase::database().isOpen()) {
            QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");  // Utiliser "QSQLITE" si vous utilisez SQLite
            db.setHostName("clean");  // Remplacer par l'adresse de votre serveur
            db.setDatabaseName("clean");  // Remplacer par le nom de votre base
            db.setUserName("nermine");  // Remplacer par votre nom d'utilisateur
            db.setPassword("minou");  // Remplacer par votre mot de passe

            if (!db.open()) {
                QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter à la base de données:\n" + db.lastError().text());
                return;
            }
        }

        // Variables pour stocker les statistiques
        int northCount = 0, centerCount = 0, southCount = 0;

        // Récupérer le nombre de clients dans le Nord
        QSqlQuery query;
        if (query.exec("SELECT COUNT(*) FROM client WHERE adresse IN ('Tunis', 'Ariana', 'Ben Arous', 'La Manouba', 'Bizerte', 'Nabeul', 'Zaghouan', 'Béja', 'Jendouba')")) {
            if (query.next()) {
                northCount = query.value(0).toInt();
            }
        } else {
            qDebug() << "Erreur lors de la récupération des données pour le Nord:" << query.lastError().text();
        }

        // Récupérer le nombre de clients dans le Centre
        if (query.exec("SELECT COUNT(*) FROM client WHERE adresse IN ('Sousse', 'Monastir', 'Mahdia', 'Kairouan', 'Sidi Bouzid', 'Kasserine')")) {
            if (query.next()) {
                centerCount = query.value(0).toInt();
            }
        } else {
            qDebug() << "Erreur lors de la récupération des données pour le Centre:" << query.lastError().text();
        }

        // Récupérer le nombre de clients dans le Sud
        if (query.exec("SELECT COUNT(*) FROM client WHERE adresse IN ('Gafsa', 'Tozeur', 'Kebili', 'Gabès', 'Médenine', 'Tataouine')")) {
            if (query.next()) {
                southCount = query.value(0).toInt();
            }
        } else {
            qDebug() << "Erreur lors de la récupération des données pour le Sud:" << query.lastError().text();
        }

        qDebug() << "Clients dans le Nord:" << northCount;
        qDebug() << "Clients dans le Centre:" << centerCount;
        qDebug() << "Clients dans le Sud:" << southCount;

        // Création d'une série de données pour le graphique
        QPieSeries *series = new QPieSeries();
        if (northCount > 0) series->append("Nord", northCount);
        if (centerCount > 0) series->append("Centre", centerCount);
        if (southCount > 0) series->append("Sud", southCount);
        if (series->slices().isEmpty()) {
            series->append("Aucune donnée", 1);
        }

        // Création du graphique
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Distribution des clients par région");

        // Personnaliser les étiquettes
        for (QPieSlice *slice : series->slices()) {
            slice->setLabelVisible(true);
            slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
        }

        // Affichage du graphique dans la vue
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setFixedSize(ui->pie->width(), ui->pie->height()); // Ajuster la taille du graphique

        // Créer une scène graphique pour afficher le graphique
        QGraphicsScene *scene = new QGraphicsScene(this);
        scene->addWidget(chartView);
        ui->pie->setScene(scene);
        ui->pie->show();

        qDebug() << "Graphique affiché avec succès.";
}


/*
// Function to display available drivers
void displayAvailableDrivers() {
    QStringList drivers = QSqlDatabase::drivers();
    qDebug() << "Available database drivers:" << drivers;
}

void MainWindow::displayRegionStatistics() {
   displayAvailableDrivers();// Display available drivers

    int northCount = 0;
    int centerCount = 0;
    int southCount = 0;

    QStringList northGovernorates = {"Tunis", "Ariana", "Ben Arous", "La Manouba", "Bizerte", "Nabeul", "Zaghouan", "Béja", "Jendouba"};
    QStringList centerGovernorates = {"Sousse", "Monastir", "Mahdia", "Kairouan", "Sidi Bouzid", "Kasserine"};
    QStringList southGovernorates = {"Gafsa", "Tozeur", "Kebili", "Gabès", "Médenine", "Tataouine"};
    // Vérifier la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Erreur: La connexion à la base de données est fermée.";
        qDebug() << db.lastError().text();
        return;
    } else {
        qDebug() << "Connexion à la base de données réussie.";
    }

    // Requête pour récupérer les adresses des clients
    QSqlQuery query;
    if (!query.exec("SELECT DISTINCT adresse FROM client ORDER BY adresse")) {
        qDebug() << "Erreur lors de la récupération des adresses des clients:" << query.lastError().text();
        return;
    } else {
        qDebug() << "Requête exécutée avec succès.";
    }

    // Initialiser rowCount pour compter le nombre d'adresses récupérées
    int rowCount = 0;

    // Analyse des adresses pour assigner les clients à une région
    while (query.next()) {
        QString adresse = query.value(0).toString(); // Récupérer l'adresse (gouvernorat)
        qDebug() << "Adresse:" << adresse;

        // Vérifier la région à partir de l'adresse
        if (northGovernorates.contains(adresse, Qt::CaseInsensitive)) {
            northCount++;
            qDebug() << "Assigné à la région Nord:" << adresse;
        } else if (centerGovernorates.contains(adresse, Qt::CaseInsensitive)) {
            centerCount++;
            qDebug() << "Assigné à la région Centre:" << adresse;
        } else if (southGovernorates.contains(adresse, Qt::CaseInsensitive)) {
            southCount++;
            qDebug() << "Assigné à la région Sud:" << adresse;
        } else {
            qDebug() << "Adresse inconnue ou non classée:" << adresse;
        }
        rowCount++;
    }

    qDebug() << "Nombre d'adresses récupérées:" << rowCount;

    // Création du graphique
    QPieSeries *series = new QPieSeries();
    series->append("North", northCount);
    series->append("Center", centerCount);
    series->append("South", southCount);

    // Ajouter un message si aucune donnée
    if (northCount == 0 && centerCount == 0 && southCount == 0) {
        series->append("No Data", 1);
    }

    // Configuration du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution des clients par région");

    // Personnalisation des étiquettes
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    // Affichage du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(ui->pie->width(), ui->pie->height()); // Ajuster la taille du graphique

    // Affectation du graphique à la vue
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addWidget(chartView);
    ui->pie->setScene(scene);
    ui->pie->show();

    qDebug() << "Graphique affiché avec succès.";
}

*/
/*void MainWindow::displayRegionStatistics() {
    int northCount = 0;
    int centerCount = 0;
    int southCount = 0;

    // Listes des gouvernorats par région
    QStringList northGovernorates = {"Tunis", "Ariana", "Ben Arous", "La Manouba", "Bizerte", "Nabeul", "Zaghouan", "Béja", "Jendouba"};
    QStringList centerGovernorates = {"Sousse", "Monastir", "Mahdia", "Kairouan", "Sidi Bouzid", "Kasserine"};
    QStringList southGovernorates = {"Gafsa", "Tozeur", "Kebili", "Gabès", "Médenine", "Tataouine"};

    // Requête pour récupérer les adresses des clients
   QSqlQuery query;
    if (!query.exec("SELECT DISTINCT adresse FROM client ORDER BY adresse")) {
        qDebug() << "Erreur lors de la récupération des adresses des clients:" << query.lastError().text();
        return;
    }

    // Analyse des adresses pour assigner les clients à une région
    while (query.next()) {
        QString adresse = query.value(0).toString(); // Récupérer l'adresse (gouvernorat)
        qDebug() << "Adresse:" << adresse;

        // Vérifier la région à partir de l'adresse
        if (northGovernorates.contains(adresse, Qt::CaseInsensitive)) {
            northCount++;
        } else if (centerGovernorates.contains(adresse, Qt::CaseInsensitive)) {
            centerCount++;
        } else if (southGovernorates.contains(adresse, Qt::CaseInsensitive)) {
            southCount++;
        } else {
            qDebug() << "Adresse inconnue ou non classée:" << adresse;
        }
    }

    // Création du graphique
    QPieSeries *series = new QPieSeries();
    series->append("North", northCount);
    series->append("Center", centerCount);
    series->append("South", southCount);

    // Ajouter un message si aucune donnée
    if (northCount == 0 && centerCount == 0 && southCount == 0) {
        series->append("No Data", 1);
    }

    // Configuration du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution des clients par région");

    // Personnalisation des étiquettes
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    // Affichage du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(ui->pie->width(), ui->pie->height()); // Ajuster la taille du graphique

    // Affectation du graphique à la vue
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addWidget(chartView);
    ui->pie->setScene(scene);
    ui->pie->show();

    qDebug() << "Graphique affiché avec succès.";
}*/
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QCategoryAxis>

/*void MainWindow::displayLoyaltyPointsStatistics() {
    // Vérifier la connexion à la base de données
    if (!QSqlDatabase::database().isOpen()) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");  // Modifier selon votre configuration (par ex. "QSQLITE")
        db.setHostName("clean");
        db.setDatabaseName("clean");
        db.setUserName("nermine");
        db.setPassword("minou");

        if (!db.open()) {
            QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter à la base de données:\n" + db.lastError().text());
            return;
        }
    }

    // Définir les tranches de points de fidélité
    QVector<QPair<QString, QPair<int, int>>> pointsRanges = {
        {"0-50", {0, 50}}, {"51-100", {51, 100}}, {"101-200", {101, 200}},
        {"201-500", {201, 500}}, {"501-1000", {501, 1000}}, {"1001-2000", {1001, 2000}},
        {"2001-5000", {2001, 5000}}, {"5001-10000", {5001, 10000}}, {"Others", {10001, INT_MAX}}
    };

    // Variables pour stocker les comptes
    QMap<QString, int> counts;
    for (const auto& range : pointsRanges) {
        counts[range.first] = 0;
    }
    QSqlQuery query(QSqlDatabase::database("clean"));
    if (!query.exec("SELECT points_fidelite FROM client")) {
        qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exécution de la requête : " + query.lastError().text());
        return;
    }

    // Parcourir les résultats et compter les clients dans chaque tranche
    while (query.next()) {
        int points = query.value("points_fidelite").toInt();
        for (const auto& range : pointsRanges) {
            if (points >= range.second.first && points <= range.second.second) {
                counts[range.first]++;
                break;
            }
        }
    }

    // Afficher les résultats dans les logs
    qDebug() << "Données des tranches :";
    for (const auto& key : counts.keys()) {
        qDebug() << key << ":" << counts[key];
    }

    // Création de la série de données pour le graphique
    QPieSeries *series = new QPieSeries();
    for (const auto& key : counts.keys()) {
        if (counts[key] > 0) {
            series->append(key, counts[key]);
        }
    }
    if (series->slices().isEmpty()) {
        series->append("Aucune donnée", 1);
    }

    // Configuration des étiquettes des tranches
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    // Création et configuration du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des clients par points de fidélité");

    // Affichage du graphique dans une vue
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(ui->pie_2->width(), ui->pie_2->height()); // Ajuster la taille du graphique

    // Configuration de la scène pour afficher le graphique
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addWidget(chartView);
    ui->pie_2->setScene(scene);
    ui->pie_2->show();

    qDebug() << "Graphique des points de fidélité généré avec succès.";
}*/

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QGraphicsScene>
#include <QMap>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVector>
#include <QDebug>

void MainWindow::displayLoyaltyPointsStatistics() {
    // Verify the database connection
    if (!QSqlDatabase::database("clean").isOpen()) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "clean");  // Modify according to your setup (e.g., "QSQLITE")
        db.setHostName("clean");
        db.setDatabaseName("clean");
        db.setUserName("nermine");
        db.setPassword("minou");

        if (!db.open()) {
            QMessageBox::critical(this, "Connection Error", "Unable to connect to the database:\n" + db.lastError().text());
            return;
        }
    }

    // Define the loyalty points ranges
    QVector<QPair<QString, QPair<int, int>>> pointsRanges = {
        {"0-50", {0, 50}}, {"51-100", {51, 100}}, {"101-200", {101, 200}},
        {"201-500", {201, 500}}, {"501-1000", {501, 1000}}, {"1001-2000", {1001, 2000}},
        {"2001-5000", {2001, 5000}}, {"5001-10000", {5001, 10000}}, {"Others", {10001, INT_MAX}}
    };

    // Variables to store the counts
    QMap<QString, int> counts;
    for (const auto& range : pointsRanges) {
        counts[range.first] = 0;
    }

    // Execute the query to retrieve loyalty points
    QSqlQuery query(QSqlDatabase::database("clean"));
    if (!query.exec("SELECT points_fidelite FROM client")) {
        qDebug() << "Error executing query:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Error executing query: " + query.lastError().text());
        return;
    }

    // Traverse results and count clients in each range
    while (query.next()) {
        int points = query.value("points_fidelite").toInt();
        for (const auto& range : pointsRanges) {
            if (points >= range.second.first && points <= range.second.second) {
                counts[range.first]++;
                break;
            }
        }
    }

    // Display the results in logs
    qDebug() << "Points ranges data:";
    for (const auto& key : counts.keys()) {
        qDebug() << key << ":" << counts[key];
    }

    // Create a line series for the chart
    QLineSeries *series = new QLineSeries();
    int index = 0;
    for (const auto& key : counts.keys()) {
        series->append(index, counts[key]);
        series->setPointLabelsFormat(key);
        series->setPointLabelsVisible(true);
        index++;
    }

    // Create and configure the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Customer Distribution by Loyalty Points");
    chart->createDefaultAxes();
    chart->axisX()->setTitleText("Loyalty Points Range");
    chart->axisY()->setTitleText("Number of Clients");
    chart->axisX()->setLabelsVisible(true);

    // Display the chart in a view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(ui->pie_2->width(), ui->pie_2->height());

    // Set up the scene to display the chart
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addWidget(chartView);
    ui->pie_2->setScene(scene);
    ui->pie_2->show();

    qDebug() << "Loyalty points line graph generated successfully.";
}


















void MainWindow::applyLoyaltyDiscount() {
    // Vérifier la connexion à la base de données
    if (!QSqlDatabase::database().isOpen()) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC"); // Modifier selon votre configuration
        db.setHostName("clean");
        db.setDatabaseName("clean");
        db.setUserName("nermine");
        db.setPassword("minou");

        if (!db.open()) {
            QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter à la base de données:\n" + db.lastError().text());
            return;
        }
    }

    // Requête pour identifier les clients avec au moins 100 points de fidélité
    QSqlQuery query;
    query.prepare("SELECT id_client, points_fidelite FROM client WHERE points_fidelite >= 100");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des points de fidélité:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des points de fidélité:\n" + query.lastError().text());
        return;
    }

    // Appliquer une remise pour les clients éligibles
    while (query.next()) {
        int clientId = query.value("id_client").toInt();
        int pointsFidelite = query.value("points_fidelite").toInt();

        // Vérifier si une remise est applicable sur la deuxième commande
        QSqlQuery orderQuery;
        orderQuery.prepare("SELECT COUNT(*) AS order_count FROM commande WHERE id_client = :id_client");
        orderQuery.bindValue(":id_client", clientId);

        if (!orderQuery.exec() || !orderQuery.next()) {
            qDebug() << "Erreur lors de la récupération des commandes pour le client" << clientId << ":" << orderQuery.lastError().text();
            continue;
        }

        int orderCount = orderQuery.value("order_count").toInt();

        // Appliquer une remise si c'est la deuxième commande
        if (orderCount == 1) { // Si c'est la deuxième commande
            QSqlQuery discountQuery;
            discountQuery.prepare("UPDATE commande SET remise = :remise WHERE id_client = :id_client AND remise IS NULL ORDER BY date_commande LIMIT 1");
            discountQuery.bindValue(":remise", 10); // Par exemple, une remise de 10%
            discountQuery.bindValue(":id_client", clientId);

            if (!discountQuery.exec()) {
                qDebug() << "Erreur lors de l'application de la remise pour le client" << clientId << ":" << discountQuery.lastError().text();
            } else {
                qDebug() << "Remise appliquée pour le client" << clientId << "avec" << pointsFidelite << "points de fidélité.";
            }
        }
    }

    QMessageBox::information(this, "Remises appliquées", "Les remises pour les clients éligibles ont été appliquées.");
}



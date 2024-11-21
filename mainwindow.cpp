#include "mainwindow.h"
#include "achat.h"
#include "qstandarditemmodel.h"
#include "ui_mainwindow.h"
#include "commande.h"
#include<QMessageBox>
#include<QDesktopServices>
#include<QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QLabel>

#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#include "commande.h"
#include <QPrinter>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include <QPdfWriter>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,ui(new Ui::MainWindow)

{
    ui->setupUi(this);
     ui->tableView_2->setModel(ach.afficher());

    ui->widjt_icon_text->setVisible(false);
    // Ensure Aceuille tab widget is initially displayed, hide all other tab widgets
    ui->aceuille->setVisible(true);   // Show Aceuille tab widget by default
    ui->demande_conge->setVisible(false);
    ui->metier_comm1->setVisible(false);
    ui->metier_comm2->setVisible(false);
    ui->metier_artic1->setVisible(false);
    ui->metier_artic2->setVisible(false);
    ui->metier_matie1->setVisible(false);
    ui->metier_matie2->setVisible(false);
    ui->metier_client1->setVisible(false);
    ui->metier_client2->setVisible(false);

    // Connect button clicks to corresponding slots
    connect(ui->pushButton_23, &QPushButton::clicked, this, &MainWindow::showAceuille);
    connect(ui->pushButton_24, &QPushButton::clicked, this, &MainWindow::showDemandeConge);
    connect(ui->pushButton_21, &QPushButton::clicked, this, &MainWindow::showMetierComm1);
    connect(ui->pushButton_22, &QPushButton::clicked, this, &MainWindow::showMetierComm2);
    ui->tableView->setModel(cmd.afficher());
    connect(ui->mod, &QPushButton::clicked, this, &::MainWindow::on_pushButton_mod_clicked);
    connect(ui->pushButton_17, &QPushButton::clicked, this, &::MainWindow::on_pushButton_17_clicked);
    connect(ui->pushButton_25, &QPushButton::clicked, this, &MainWindow::showMetierArtic1);
    connect(ui->pushButton_26, &QPushButton::clicked, this, &MainWindow::showMetierArtic2);
    connect(ui->pushButton_27, &QPushButton::clicked, this, &MainWindow::showMetierMatie1);
    connect(ui->pushButton_28, &QPushButton::clicked, this, &MainWindow::showMetierMatie2);
    connect(ui->pushButton,    &QPushButton::clicked, this, &MainWindow::showMetierClient1);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::showMetierClient2);
    connect(ui->ajoutAchatButton, &QPushButton::clicked, this, &MainWindow::ajoutachat);

    //5edma men hnee
    connect(ui->ok_2, &QPushButton::clicked, this, &MainWindow::on_ok_2_clicked);

    connect(ui->supprimer_2, &QPushButton::clicked, this, &::MainWindow::on_pushButton_supprimer_2_clicked);
    connect(ui->PDF, &QPushButton::clicked, this, &MainWindow::on_PDF_clicked);

      connect(ui->trieButton, &QPushButton::clicked, this, &MainWindow::on_trieButton_clicked);
       connect(ui->statButton, &QPushButton::clicked, this, &MainWindow::on_statButton_clicked);



                  }







MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hideAllTabWidgets()
{
    // Hide all tab widgets
    ui->aceuille->setVisible(false);
    ui->demande_conge->setVisible(false);
    ui->metier_comm1->setVisible(false);
    ui->metier_comm2->setVisible(false);
    ui->metier_artic1->setVisible(false);
    ui->metier_artic2->setVisible(false);
    ui->metier_matie1->setVisible(false);
    ui->metier_matie2->setVisible(false);
    ui->metier_client1->setVisible(false);
    ui->metier_client2->setVisible(false);
}

void MainWindow::showAceuille()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->aceuille->setVisible(true);  // Show Aceuille tab widget
}

void MainWindow::showDemandeConge()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->demande_conge->setVisible(true);  // Show Demande Congé tab widget
}

void MainWindow::showMetierComm1()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->metier_comm1->setVisible(true);  // Show Metier Comm1 tab widget
}

void MainWindow::showMetierComm2()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->metier_comm2->setVisible(true);  // Show Metier Comm2 tab widget
}

void MainWindow::showMetierArtic1()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->metier_artic1->setVisible(true);  // Show Metier Artic1 tab widget
}

void MainWindow::showMetierArtic2()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->metier_artic2->setVisible(true);  // Show Metier Artic2 tab widget
}

void MainWindow::showMetierMatie1()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->metier_matie1->setVisible(true);  // Show Metier Matie1 tab widget
}

void MainWindow::showMetierMatie2()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->metier_matie2->setVisible(true);  // Show Metier Matie2 tab widget
}

void MainWindow::showMetierClient1()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->metier_client1->setVisible(true);  // Show Metier Client1 tab widget
}

void MainWindow::showMetierClient2()
{
    hideAllTabWidgets();  // Hide all before showing
    ui->metier_client2->setVisible(true);  // Show Metier Client2 tab widget
}

void MainWindow::on_ok_2_clicked()
{

    int id=ui->id_2->text().toInt();
    QString nom=ui->nom_3->text();
    QString prenom=ui->prenom_3->text();
    int telephone=ui->telephone_2->text().toInt();
    QString adresse=ui->adresse_2->text();
    int qte_couette=ui->qte_couette_2->text().toInt();
    int qte_tapis=ui->qte_tapis_2->text().toInt();
    /*float prix_tapis = ui->prix_tapis->text().toFloat();
    float prix_couette = ui->prix_couette->text().toFloat();*/


    commande C( id ,telephone ,qte_couette,qte_tapis, nom ,prenom, adresse);
    bool test=C.ajouter();

    if(test)
    {
        ui->tableView->setModel(cmd.afficher());
        QMessageBox::information(nullptr, QObject::tr("ok"),
                    QObject::tr("Ajout successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                    QObject::tr("Ajout failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_pushButton_supprimer_2_clicked()
{

    int id=ui->iddd->text().toInt();
    bool test=cmd.supprimer(id);


    if(test)
    {

        ui->tableView->setModel(cmd.afficher());
        QMessageBox::information(nullptr, QObject::tr("ok"),
                    QObject::tr("suppression efffectuée.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                    QObject::tr("suppression non effectuée.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_pushButton_17_clicked()
{

    int id=ui->idd ->text().toInt();
    bool test=cmd.recherche(id);

    if(test)
    {

        QMessageBox::information(nullptr, QObject::tr("ok"),
                    QObject::tr("id founded.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                    QObject::tr("id not founded.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
void MainWindow::on_pushButton_mod_clicked(){

    int id=ui->idd->text().toInt();
    QString nom=ui->nom->text();
    QString prenom=ui->pren->text();
    int telephone=ui->tel->text().toInt();
    QString adresse=ui->ad->text();
    int qte_couette=ui->qte_c->text().toInt();
    int qte_tapis=ui->lineEdit_4->text().toInt();
    commande c;
    bool test=c.modifier(id,  telephone ,  qte_couette,  qte_tapis,  nom ,  prenom, adresse);

    if(test)
    {

        QMessageBox::information(nullptr, QObject::tr("ok"),
                    QObject::tr("suppression efffectuée.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                    QObject::tr("suppression non effectuée.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}



void MainWindow::on_PDF_clicked()
{
    int id = 1; // Replace with the actual ID you want to export
    cmd.pdf_downloader(id);

    QMessageBox::information(this, tr("PDF Export"), tr("The PDF has been successfully generated."));
}





#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commande.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QDebug>




void MainWindow::on_trieButton_clicked()
{
    // Utiliser la fonction trieButton de commande pour trier par le nom, par exemple
    QSqlQueryModel* model = cmd.trieButton("PRENOM", true);  // Tri ascendant

    // Définir le modèle sur le TableView (utilisez le nom correct du QTableView)
    ui->tableView->setModel(model);

    qDebug() << "Les commandes ont été triées et affichées dans le tableau.";
}


void MainWindow::on_statButton_clicked() {
    qDebug() << "statButton clicked";
    QString count = cmd.count();
    qDebug() << "Number of ID:" << count;
    ui->nbc->setText("Nombre total de ID: " + count);
}


void MainWindow::ajoutachat()
{
    // Récupérer les données saisies par l'utilisateur dans le formulaire
    int id = ui->lineEdit_2->text().toInt();
    int qt_tapis = ui->lineEdit_6->text().toInt();
    int qt_couette = ui->lineEdit_7->text().toInt();
    QString nom = ui->lineEdit_3->text();
    QString prenom = ui->lineEdit_5->text();

    qDebug() << "ID:" << id;
    qDebug() << "Quantité Tapis:" << qt_tapis;
    qDebug() << "Quantité Couette:" << qt_couette;
    qDebug() << "Nom:" << nom;
    qDebug() << "Prénom:" << prenom;

    // Créer un objet achat avec les prix passés en dur
    achat a(id, qt_tapis, qt_couette, 50, 60, nom, prenom);

    // Ajouter l'achat à la base de données
    if (a.ajouter()) {
         ui->tableView_2->setModel(ach.afficher());
        QMessageBox::information(this, "Succès", "Achat ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'achat. Vérifiez la base de données.");
    }
}

void MainWindow::afficherAchats()
{ // Utiliser la fonction afficher de achat pour afficher les achats
    ui->tableView_2->setModel(ach.afficher());

}



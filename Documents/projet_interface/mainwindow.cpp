#include "mainwindow.h"
#include <QSqlError>
#include <QMessageBox>
#include "dialog.h"
#include "employe.h"
#include "qthread.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSortFilterProxyModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGraphicsView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>



MainWindow::MainWindow(const QString &email, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    userEmail(email)
{
    ui->setupUi(this);




    setupComboBoxes();
    privilege();
    displayGenderStatistics();
    bienmessage();
    onUpdateTypeCongeClicked();
    emp.updateEmployeStatus();
    displayCongeStats();








    ui->tableView->setModel(emp.afficher());
    ui->tableView->resizeColumnsToContents();


    ui->role->setVisible(false);
    ui->label_148->setVisible(false);
    ui->rasion->setVisible(false);
    ui->bar->setVisible(false);

    QSqlQueryModel *model = emp.afficherconge();
    ui->afficher_conge->setModel(model);
    ui->afficher_conge->resizeColumnsToContents();

    afficherCongeEmploye();
    refreshTableView();





    ui->widjt_icon_text->setVisible(false);

    // Connect button clicks to corresponding slots
    connect(ui->ajouter, &QPushButton::clicked, this, &MainWindow::on_addEmployeeButton_clicked);
    connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_supprimerEmploye_clicked);
    connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::on_modifyEmployeeButton_clicked);
    connect(ui->chercher, &QPushButton::clicked, this, &MainWindow::on_searchButton_clicked);
    connect(ui->annuler, &QPushButton::clicked, this, &MainWindow::onclearbutton);
    connect(ui->pdf, &QPushButton::clicked, this, &MainWindow::employetelechargerPDF);
    connect(ui->comboBox_tri, SIGNAL(currentIndexChanged(int)), this, SLOT(onSortSelectionChanged(int)));
    connect(ui->departement, &QComboBox::currentIndexChanged, this, &MainWindow::onDepartmentChanged);
    connect(ui->raison_conge, &QComboBox::currentIndexChanged, this, &MainWindow::onrasionChanged);
    connect(ui->aceuille, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    connect(ui->id_emp, &QLineEdit::textChanged, this, &MainWindow::onIdChanged);
    connect(ui->id_emp, &QLineEdit::editingFinished, this, &MainWindow::on_idEmpChanged);
    connect(ui->soumettre, &QPushButton::clicked, this, &MainWindow::onaddconge);
    connect(ui->afficher_conge->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);
    connect(ui->acceptation, &QPushButton::clicked, this, &MainWindow::on_acceptation_clicked);
    connect(ui->refuser, &QPushButton::clicked, this, &MainWindow::on_refuser_clicked);
    connect(ui->recherche_conge, &QPushButton::clicked, this, &MainWindow::on_searchcongeButton_clicked);
    connect(ui->annule_conge, &QPushButton::clicked, this, &MainWindow::onclearcongebutton);
    connect(ui->rechercher, &QPushButton::clicked, this, &MainWindow::on_searchcongeButtonhistorique_clicked);
    connect(ui->annuler_histo, &QPushButton::clicked, this, &MainWindow::onclearcongebuttonhistorique);
    connect(ui->stat1, &QPushButton::clicked, this, &MainWindow::displayGenderStatistics);
    connect(ui->stat2, &QPushButton::clicked, this, &MainWindow::displayCongeStats);
    connect(ui->id_rech, &QLineEdit::textChanged, this, &MainWindow::filterEmployees);








    ui->nom_conge->setEnabled(false);
    ui->prenom_conge->setEnabled(false);
    ui->email_conge->setEnabled(false);
    ui->departement_conge_2->setEnabled(false);
    ui->poste_conge->setEnabled(false);
    ui->rasion->setEnabled(false);
    ui->raison_conge->setEnabled(false);
    ui->date_debut->setEnabled(false);
    ui->date_fin->setEnabled(false);
    ui->soumettre->setEnabled(false);

    ui->acceptation->setEnabled(false);
    ui->refuser->setEnabled(false);







    connect(ui->p1, &QPushButton::clicked, this, &MainWindow::showAceuille);
    connect(ui->p2, &QPushButton::clicked, this, &MainWindow::showDemandeConge);
    connect(ui->mcom, &QPushButton::clicked, this, &MainWindow::showMetierComm1);
    connect(ui->mcom2, &QPushButton::clicked, this, &MainWindow::showMetierComm2);
    connect(ui->ma, &QPushButton::clicked, this, &MainWindow::showMetierArtic1);
    connect(ui->ma2, &QPushButton::clicked, this, &MainWindow::showMetierArtic2);
    connect(ui->mm, &QPushButton::clicked, this, &MainWindow::showMetierMatie1);
    connect(ui->mm2, &QPushButton::clicked, this, &MainWindow::showMetierMatie2);
    connect(ui->mc,    &QPushButton::clicked, this, &MainWindow::showMetierClient1);
    connect(ui->mc2, &QPushButton::clicked, this, &MainWindow::showMetierClient2);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::filterEmployees(const QString &searchText) {
    // Prepare the model
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    QSqlQueryModel* model = new QSqlQueryModel(this);

    // Fetch data from the database
    model->setQuery("SELECT * FROM empeloye"); // Replace with your actual query
    // Set up proxy model
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive); // Case-insensitive filtering
    proxyModel->setFilterKeyColumn(-1); // Filter across all columns
    proxyModel->setFilterFixedString(searchText); // Apply the filter text

    // Set the proxy model to the table view
    ui->tableView->setModel(proxyModel);
}


void MainWindow::on_searchcongeButtonhistorique_clicked() {
    QString id = ui->id_conge->text();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid ID.");
        return;
    }

    // Get the model currently set in the table view
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->historique->model());

    if (!model) {
        QMessageBox::warning(this, "Model Error", "No model found for the table view.");
        return;
    }

    // Select the matching row in the table view
    QModelIndex matchingIndex;
    for (int row = 0; row < model->rowCount(); ++row) {
        // Check if the ID in the first column matches the entered ID
        if (model->index(row, 0).data().toString() == id) {
            matchingIndex = model->index(row, 0); // Select the first column in the matching row
            break;
        }
    }

    if (matchingIndex.isValid()) {
        // Select the matching row
        ui->historique->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->historique->setSelectionBehavior(QAbstractItemView::SelectRows);

        QItemSelectionModel* selectionModel = ui->historique->selectionModel();
        selectionModel->select(matchingIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);

        // Scroll to the selected row to ensure it is visible
        ui->historique->scrollTo(matchingIndex);

        // Highlight the selected row (Blue by default in most styles)
    } else {
        QMessageBox::information(this, "Search Result", "No matching employee found.");
    }
}
void MainWindow::onclearcongebuttonhistorique(){
    ui->id_conge->clear();

    ui->historique->selectionModel()->clearSelection();

    ui->historique->scrollToTop(); // This will scroll back to the top


}
void MainWindow::afficherCongeEmploye() {
    QSqlQueryModel *model = emp.affichercongeemploye(userEmail);

    if (model) {
        // Set the model to the QTableView
        ui->historique->setModel(model);
        ui->historique->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Error", "Failed to load congés for the given employee.");
    }
}



void MainWindow::on_searchcongeButton_clicked() {
    QString id = ui->id_emp_2->text();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid ID.");
        return;
    }

    // Get the model currently set in the table view
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->afficher_conge->model());

    if (!model) {
        QMessageBox::warning(this, "Model Error", "No model found for the table view.");
        return;
    }

    // Select the matching row in the table view
    QModelIndex matchingIndex;
    for (int row = 0; row < model->rowCount(); ++row) {
        // Check if the ID in the first column matches the entered ID
        if (model->index(row, 0).data().toString() == id) {
            matchingIndex = model->index(row, 0); // Select the first column in the matching row
            break;
        }
    }

    if (matchingIndex.isValid()) {
        // Select the matching row
        ui->afficher_conge->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->afficher_conge->setSelectionBehavior(QAbstractItemView::SelectRows);

        QItemSelectionModel* selectionModel = ui->afficher_conge->selectionModel();
        selectionModel->select(matchingIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);

        // Scroll to the selected row to ensure it is visible
        ui->afficher_conge->scrollTo(matchingIndex);

        // Highlight the selected row (Blue by default in most styles)
    } else {
        QMessageBox::information(this, "Search Result", "No matching employee found.");
    }
}
void MainWindow::onclearcongebutton(){
    ui->id_emp_2->clear();

    ui->afficher_conge->selectionModel()->clearSelection();

    ui->afficher_conge->scrollToTop(); // This will scroll back to the top


}

void MainWindow::on_acceptation_clicked()
{
    // Get the selected index from the table view
    QModelIndexList selectedIndexes = ui->afficher_conge->selectionModel()->selectedRows();

    // Check if there is a selection
    if (!selectedIndexes.isEmpty()) {
        // Get the first selected row (we assume it's a single row selection)
        QModelIndex index = selectedIndexes.first();

        // Get the 'id_conge' from the selected row (assuming it's in column 0)
        QString id_conge = ui->afficher_conge->model()->data(index.sibling(index.row(), 0)).toString();

        // Call the method in Employe.cpp to update the database
        Employe employe;
        if (employe.updateCongeStatus(id_conge, "yes", "en cour")) {
            qDebug() << "Record updated successfully!";
        } else {
            qDebug() << "Error updating record!";
        }
    }
    QSqlQueryModel *model = emp.afficherconge();

    // Set the model to the table view
    ui->afficher_conge->setModel(model);

    // Optionally, resize the column to fit the content
    ui->afficher_conge->resizeColumnsToContents();
    ui->id_emp_3->clear();

    // Disable the buttons when no row is selected
    ui->acceptation->setEnabled(false);
    ui->refuser->setEnabled(false);
    afficherCongeEmploye();
    refreshTableView();

}
void MainWindow::on_refuser_clicked()
{
    // Get the selected index from the table view
    QModelIndexList selectedIndexes = ui->afficher_conge->selectionModel()->selectedRows();

    // Check if there is a selection
    if (!selectedIndexes.isEmpty()) {
        // Get the first selected row (we assume it's a single row selection)
        QModelIndex index = selectedIndexes.first();

        // Get the 'id_conge' from the selected row (assuming it's in column 0)
        QString id_conge = ui->afficher_conge->model()->data(index.sibling(index.row(), 0)).toString();

        // Set the 'id_conge' value in the QLineEdit (id_emp_3)
        ui->id_emp_3->setText(id_conge);

        // Enable the buttons
        ui->acceptation->setEnabled(true);
        ui->refuser->setEnabled(true);

        // Call the method in Employe.cpp to update the database
        Employe employe;
        if (employe.updateCongeStatus(id_conge, "no", "refuser")) {
            qDebug() << "Record updated successfully!";
        } else {
            qDebug() << "Error updating record!";
        }
    } else {
        // If no row is selected (i.e., selection is cleared)
        ui->id_emp_3->clear();

        // Disable the buttons
        ui->acceptation->setEnabled(false);
        ui->refuser->setEnabled(false);
    }

    // Refresh the table view to reflect database changes
    QSqlQueryModel *model = emp.afficherconge();
    ui->afficher_conge->setModel(model);

    // Optionally, resize the column to fit the content
    ui->afficher_conge->resizeColumnsToContents();

    // Clear the selection after update
    ui->afficher_conge->selectionModel()->clearSelection();
    ui->id_emp_3->clear();

    // Disable the buttons when no row is selected
    ui->acceptation->setEnabled(false);
    ui->refuser->setEnabled(false);
    refreshTableView();
    afficherCongeEmploye();


}
void MainWindow::refreshTableView()
{
    // Refresh the table model
    QSqlQueryModel *model = emp.afficherconge();
    ui->afficher_conge->setModel(model);

    // Resize the columns to fit content
    ui->afficher_conge->resizeColumnsToContents();

    // Reconnect the selection model to the onSelectionChanged slot
    connect(ui->afficher_conge->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::onSelectionChanged);
}


void MainWindow::onSelectionChanged(const QItemSelection &selected)
{
    // Check if something is selected
    if (!selected.isEmpty()) {
        // Get the first selected index (row, column)
        QModelIndex index = selected.indexes().first();

        // Check if the selected column is the one containing the 'id_conge' (assuming column 0)
        if (index.column() == 0) {
            // Retrieve the 'id_conge' from the model (assuming it's the first column)
            QString id_conge = ui->afficher_conge->model()->data(index).toString();

            // Set the 'id_conge' value in the QLineEdit (id_emp_3)
            ui->id_emp_3->setText(id_conge);

            // Enable the buttons (assuming buttons are named acceptation and refuser)
            ui->acceptation->setEnabled(true);
            ui->refuser->setEnabled(true);
        }
    }
    else {
        // If no row is selected (i.e., selection is cleared), clear the QLineEdit
        ui->id_emp_3->clear();

        // Disable the buttons when no row is selected
        ui->acceptation->setEnabled(false);
        ui->refuser->setEnabled(false);
    }
}


void MainWindow::onaddconge(){
    QDate date_debut = ui->date_debut->date();
    QDate date_fin = ui->date_fin->date();
    QString nom_conge = ui->nom_conge->text();
    QString prenom_conge =  ui->prenom_conge->text();
    QString email_conge = ui->email_conge->text();
    QString departemnt_conge =  ui->departement_conge_2->currentText();
    QString poste_conge = ui->poste_conge->currentText();
    QString id_emp = ui->id_emp->text();
    QString raison_counge = ui->raison_conge->currentText();
    QString raison = ui->rasion->text();

    // Get today's date
    QDate today = QDate::currentDate();

    // Check if the reason for the leave is selected
    if (ui->raison_conge->currentText() == "veuillez sélectionner") {
        QMessageBox::warning(this, "Input Error", "Choisir un raison de congé.");
        return;
    }

    // Check if the reason field is visible and empty
    if (ui->rasion->isVisible() && ui->rasion->text().isEmpty()){
        QMessageBox::warning(this, "Input Error", "Ecrire votre raison de congé.");
        return;
    }

    // Check if date_fin is later than today
    if (date_fin <= today) {
        QMessageBox::warning(this, "Date invalide", "La date de fin doit être postérieure à la date d'aujourd'hui.");
        return; // Exit the function if the date is invalid
    }

    // Check if date_debut is today or later
    if (date_debut < today) {
        QMessageBox::warning(this, "Date invalide", "La date de début doit être aujourd'hui ou plus tard.");
        return; // Exit the function if the date is invalid
    }

    emp.date_d = ui->date_debut->date(); // Date from QDateEdit widget
    emp.date_f = ui->date_fin->date();

    emp.setnom(nom_conge);  // Set the 'nom' value using setter
    emp.setprenom(prenom_conge);  // Set the 'prenom' value using setter
    emp.setemail(email_conge);  // Set the 'email' value using setter
    emp.setidemp(id_emp);  // Set the 'id_emp' value using setter

    // Directly assigning public attributes
    emp.departemnt_conge = departemnt_conge;  // Direct assignment for public attributes
    emp.poste_conge = poste_conge;  // Direct assignment for public attributes

    // Set 'raison' based on the condition
    if (raison_counge == "Autre") {
        qDebug() << "Raison (si Autre): " << raison;
        emp.setraison(raison);  // Direct assignment for public attributes
    } else {
        qDebug() << "Raison de congé: " << raison_counge;
        raison = raison_counge;
        emp.setraison(raison); // Direct assignment for public attributes
    }
    if (emp.hasOngoingLeave(id_emp)){
        QMessageBox::warning(this, "Error", "employe a deja un conge."); // Add method to get last error from emp class
        QSqlQueryModel *model = emp.afficherconge();
        ui->afficher_conge->setModel(model);
        // Optionally, resize the column to fit the content
        ui->afficher_conge->resizeColumnsToContents();
        // Clear the selection after update
        ui->afficher_conge->selectionModel()->clearSelection();
        refreshTableView();
        afficherCongeEmploye();
        return;
    }

    bool test = emp.ajouterconge();

    if (ui->confiramtion_conge->isChecked()) {
        if (test) {
                 QMessageBox::warning(this, "Error", "conge ajouter avec succcess.");
                ui->nom_conge->clear();
                ui->prenom_conge->clear();
                ui->email_conge->clear();
                ui->id_emp->clear();
                ui->departement_conge_2->setCurrentIndex(0);
                ui->poste_conge->setCurrentIndex(0);
                ui->date_debut->setDate(QDate::currentDate());
                ui->date_fin->setDate(QDate::currentDate());
                ui->raison_conge->setCurrentIndex(0);
                ui->rasion->clear();

            } else {
                QMessageBox::warning(this, "Error", "Failed to add conge."); // Add method to get last error from emp class
            }
        }else{
             QMessageBox::warning(this, "Error", "must confirm.");
        }
        QSqlQueryModel *model = emp.afficherconge();
        ui->afficher_conge->setModel(model);
        ui->afficher_conge->resizeColumnsToContents();
        refreshTableView();
        afficherCongeEmploye();

}

void MainWindow::onIdChanged()
{
    // Enable all other fields if the ID field is not empty
    QString id_emp=ui->id_emp->text();
    if (id_emp.isEmpty()){
        ui->nom_conge->clear();
        ui->prenom_conge->clear();
        ui->email_conge->clear();
        ui->departement_conge_2->setCurrentIndex(0);
        ui->poste_conge->setCurrentIndex(0);
        ui->date_debut->setDate(QDate::currentDate());
        ui->date_fin->setDate(QDate::currentDate());
        ui->nom_conge->setEnabled(false);
        ui->prenom_conge->setEnabled(false);
        ui->email_conge->setEnabled(false);
        ui->departement_conge_2->setEnabled(false);
        ui->poste_conge->setEnabled(false);
        ui->rasion->setEnabled(false);
        ui->raison_conge->setEnabled(false);
        ui->date_debut->setEnabled(false);
        ui->date_fin->setEnabled(false);
        ui->soumettre->setEnabled(false);
    }else{
        ui->rasion->setEnabled(true);
        ui->raison_conge->setEnabled(true);
        ui->date_debut->setEnabled(true);
        ui->date_fin->setEnabled(true);
        ui->soumettre->setEnabled(true);
    }


}
void MainWindow::populateEmployeeInfo()
{
    // Create an instance of the Employe class
    Employe emp;
    QString id_emp = ui->id_emp->text();

    // Variables to hold the employee details
    QString nom, prenom, email, poste;
    int departement;

    // Attempt to retrieve employee details by ID
    bool employeeFound = emp.getEmployeeDetailsById(id_emp, nom, prenom, email, poste, departement);

    int id_poste = poste.toInt();


    if (employeeFound) {
        // If employee is found, update the UI fields
        ui->nom_conge->setText(nom);
        ui->prenom_conge->setText(prenom);
        ui->email_conge->setText(email);
        ui->date_debut->setDate(QDate::currentDate());
        ui->date_fin->setDate(QDate::currentDate());
        // Ensure the "poste" and "departement" fields are populated correctly.
        // The combo boxes must have items that correspond to these values.
        if (id_poste >= 0 && id_poste < ui->poste_conge->count()) {
            ui->poste_conge->setCurrentIndex(id_poste);
        }

        if (departement >= 0 && departement < ui->departement_conge_2->count()) {
            ui->departement_conge_2->setCurrentIndex(departement);
        }
    } else {
        // Show a message if the employee is not found
        QMessageBox::warning(this, "Error", "Employee with ID " + id_emp + " not found.");
        ui->id_emp->clear();
    }
}


void MainWindow::on_idEmpChanged() {
    populateEmployeeInfo();  // Call the function when the ID is changed
}



void MainWindow::onTabChanged(int index)
{
    // Check if the second tab (index 1) is selected
    if (index == 1) {
        displayGenderStatistics(); // Call your statistics function
    }
}

void MainWindow::bienmessage()
{
    QString message; // Message to be returned
    QString userName; // User name fetched from the database

    // Check if userEmail is empty
    if (userEmail.isEmpty()) {
        message = "<span style='font-size:18px;'>Bienvenue cher <b><u>client</u></b> dans notre application</span>";
    } else {
        // Query to fetch the user's name
        QSqlQuery query;
        query.prepare("SELECT nom FROM empeloye WHERE id_p = :id_p");
        query.bindValue(":id_p", userEmail);

        if (query.exec() && query.next()) {
            userName = query.value(0).toString(); // Fetch the 'nom' field
            // Create the formatted message with the name styled
            message = QString(
                "<span style='font-size:18px;'>Bienvenue <b><u>%1</u></b> dans notre application</span>"
            ).arg(userName);
        } else {
            // If the query fails or no result is found, provide a fallback message
            message = "<span style='font-size:18px;'>Bienvenue cher <b><u>client</u></b> dans notre application</span>";
        }
    }

    // Set the message to the label_2 with HTML formatting
    ui->text->setText(message);


}



void MainWindow::privilege() {
    // Verify that userEmail is set correctly

    // Assuming emp is properly initialized
    Employe emp;
    QString role = emp.getUserPrivileges(userEmail);

    // Apply privileges based on the role
    if (role.trimmed() == "user employe") {
        showAceuille();
        ui->demande_conge->setTabVisible(2, false);  // Hide the second tab if role is "user employe"
    } else if (role.trimmed() == "user matiere_premier") {
        // Ensure metier_matie1 tab is visible and buttons are enabled
        showMetierMatie1();
        ui->p1->setEnabled(false);
        ui->p2->setEnabled(true);
        ui->mcom->setEnabled(false);
        ui->mcom2->setEnabled(false);
        ui->ma->setEnabled(false);
        ui->ma2->setEnabled(false);
        ui->mm->setEnabled(true);
        ui->mm2->setEnabled(true);
        ui->mc->setEnabled(false);
        ui->mc2->setEnabled(false);
        ui->demande_conge->setTabVisible(2, false);
    }else if (role.trimmed() == "user articles") {
        // Ensure metier_matie1 tab is visible and buttons are enabled
        showMetierArtic1();
        ui->p1->setEnabled(false);
        ui->p2->setEnabled(true);
        ui->mcom->setEnabled(false);
        ui->mcom2->setEnabled(false);
        ui->ma->setEnabled(true);
        ui->ma2->setEnabled(true);
        ui->mm->setEnabled(false);
        ui->mm2->setEnabled(false);
        ui->mc->setEnabled(false);
        ui->mc2->setEnabled(false);
        ui->demande_conge->setTabVisible(2, false);
    }else if (role.trimmed() == "User commande") {
        // Ensure metier_matie1 tab is visible and buttons are enabled
        showMetierComm1();
        ui->p1->setEnabled(false);
        ui->p2->setEnabled(true);
        ui->mcom->setEnabled(true);
        ui->mcom2->setEnabled(true);
        ui->ma->setEnabled(false);
        ui->ma2->setEnabled(false);
        ui->mm->setEnabled(false);
        ui->mm2->setEnabled(false);
        ui->mc->setEnabled(false);
        ui->mc2->setEnabled(false);
        ui->demande_conge->setTabVisible(2, false);
    }else if (role.trimmed() == "User client") {
        // Ensure metier_matie1 tab is visible and buttons are enabled
        showMetierClient1();
        ui->p1->setEnabled(false);
        ui->p2->setEnabled(true);
        ui->mcom->setEnabled(false);
        ui->mcom2->setEnabled(false);
        ui->ma->setEnabled(false);
        ui->ma2->setEnabled(false);
        ui->mm->setEnabled(false);
        ui->mm2->setEnabled(false);
        ui->mc->setEnabled(true);
        ui->mc2->setEnabled(true);
    }else if (role.trimmed() == "admin employe") {
        // Ensure metier_matie1 tab is visible and buttons are enabled
        showAceuille();
        ui->p1->setEnabled(true);
        ui->p2->setEnabled(true);
        ui->mcom->setEnabled(true);
        ui->mcom2->setEnabled(true);
        ui->ma->setEnabled(true);
        ui->ma2->setEnabled(true);
        ui->mm->setEnabled(true);
        ui->mm2->setEnabled(true);
        ui->mc->setEnabled(true);
        ui->mc2->setEnabled(true);

    }else{
        showMetierArtic1();
        ui->p1->setEnabled(false);
        ui->p2->setEnabled(false);
        ui->mcom->setEnabled(true);
        ui->mcom2->setEnabled(false);
        ui->ma->setEnabled(true);
        ui->ma2->setEnabled(false);
        ui->mm->setEnabled(true);
        ui->mm2->setEnabled(false);
        ui->mc->setEnabled(true);
        ui->mc2->setEnabled(false);
        ui->demande_conge->setTabVisible(2, false);
    }
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
    refreshTableView();
    afficherCongeEmploye();
    ui->tableView->setModel(emp.afficher());
    ui->tableView->resizeColumnsToContents();
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
void MainWindow::setupComboBoxes()
{
    // Populate departement combo box
    emp.fillComboBox(ui->departement, "SELECT NOM FROM department");
    emp.fillComboBox(ui->poste, "SELECT NOM FROM poste");
    emp.fillComboBox(ui->departement_conge_2, "SELECT NOM FROM department");
    emp.fillComboBox(ui->poste_conge, "SELECT NOM FROM poste");
}


void MainWindow::on_addEmployeeButton_clicked()
{

    Dialog d;
    // Validate 'nom'
    QRegularExpression nameRegex("^[A-Za-z ]*$"); // Regex for only letters and spaces
    QRegularExpressionValidator nameValidator(nameRegex);

    // Validate 'nom'
    QString nom = ui->nom->text();
    int pos = 0;
    if (nom.isEmpty() || nameValidator.validate(nom, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Input Error", "Nom can only contain letters and spaces.");
        return;
    }

    // Validate 'prenom'
    QString prenom = ui->prenom->text();
    if (prenom.isEmpty() || nameValidator.validate(prenom, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Input Error", "Prenom can only contain letters and spaces.");
        return;
    }
    // Validate 'email' format using regex
    QString email = ui->email->text();
    QRegularExpression emailRegex(R"((\w+)(\.?[\w-]+)*@([\w-]+\.)+[\w-]{2,})");
    QRegularExpressionValidator emailValidator(emailRegex);
    int emailPos = 0;
    if (ui->email->text().isEmpty() || emailValidator.validate(email, emailPos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Input Error", "Email vide ou invalide !!");
        return;
    }

    // Validate 'salaire' input is numeric
    bool salaireOk;
    double salaire = ui->salaire->text().toDouble(&salaireOk);
    if (!salaireOk || salaire <= 0) {
        QMessageBox::warning(this, "Input Error", "Salaire vide ou invalide !!");
        return;
    }

    // Validate 'departement' and 'poste' selections
    if (ui->departement->currentText() == "chosir department") {
        QMessageBox::warning(this, "Input Error", "Choisir une departement valide.");
        return;
    }
    if (ui->poste->currentText() == "choisir poste") {
        QMessageBox::warning(this, "Input Error", "Choisir un poste valide.");
        return;
    }

    // Validate 'genre' selection
    if (!ui->homme->isChecked() && !ui->femme->isChecked()) {
        QMessageBox::warning(this, "Input Error", "Choisir votre genre.");
        return;
    }

    // Get values from UI elements and set to the 'Employe' object
    QString sexe = ui->homme->isChecked() ? "M" : "F"; // Radio buttons for male/female
    int depId = ui->departement->currentData().toInt();
    QString role= ui->role->currentText();
    int posteId = ui->poste->currentData().toInt();
    emp.date_emboche = ui->date_emboche->date(); // Date from QDateEdit widget

    emp.setnom(nom);
    emp.setprenom(prenom);
    emp.setsexe(sexe);
    emp.setemail(email);
    emp.setsalaire(salaire);
    emp.setdep(depId);
    emp.setposte(QString::number(posteId));
    emp.setrole(role);

    bool test=emp.ajouter();
    if (ui->confirmation->isChecked()) {
        if (test) {
            QMessageBox::information(this, "Success", "Employee added successfully: " + nom + " " + prenom);
            ui->nom->clear();
            ui->prenom->clear();
            ui->email->clear();
            ui->salaire->clear();
            ui->departement->setCurrentIndex(0);  // Reset to default "choose department"
            ui->poste->setCurrentIndex(0);        // Reset to default "choose poste"
            ui->homme->setChecked(false);         // Uncheck gender selection
            ui->femme->setChecked(false);
            ui->confirmation->setChecked(false);  // Uncheck confirmation checkbox
            ui->date_emboche->setDate(QDate::currentDate()); // Reset date to today

        } else {
            QMessageBox::warning(this, "Error", "Failed to add employee: " ); // Add method to get last error from emp class
        }
    } else {
        QMessageBox::warning(this, "Error", "You must confirm the addition of the employee.");
    }
    ui->tableView->setModel(emp.afficher());
    ui->tableView->resizeColumnsToContents();



}



void MainWindow::on_supprimerEmploye_clicked() {
    // Get the ID from the input field
    QString id = ui->id_sup->text();

    // Check if the input field is empty
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "L'ID pour supprimer est vide.");
        return;
    }


    bool exists = emp.idExists(id);
    bool test = emp.supprimer(id);

    if (exists) {
        if (test) {
            QMessageBox::information(this, "Success", "Employé supprimé avec succès.");
            ui->id_sup->clear();
            ui->tableView->setModel(emp.afficher());
            ui->tableView->resizeColumnsToContents();

        } else {
            QMessageBox::warning(this, "Error", "Échec de la suppression de l'employé.");
        }
    } else {
        QMessageBox::warning(this, "Error", "L'ID n'existe pas dans la base de données.");
        return;
    }



}


void MainWindow::on_modifyEmployeeButton_clicked()
{
    // Validate 'id' for modification
    QString id = ui->id_sup->text();
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "L'ID pour modifier est vide.");
        return;
    }

    // Validate 'nom' (only letters and spaces allowed, but can be empty)
    QRegularExpression nameRegex("^[A-Za-z ]*$"); // Regex for only letters and spaces
    QRegularExpressionValidator nameValidator(nameRegex);
    QString nom = ui->nom->text();
    int pos = 0;
    if (!nom.isEmpty() && nameValidator.validate(nom, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Input Error", "Nom can only contain letters and spaces.");
        return;
    }

    // Validate 'prenom' (only letters and spaces allowed, but can be empty)
    QString prenom = ui->prenom->text();
    if (!prenom.isEmpty() && nameValidator.validate(prenom, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Input Error", "Prenom can only contain letters and spaces.");
        return;
    }

    // Validate 'email' format using regex, but allow empty email
    QString email = ui->email->text();
    QRegularExpression emailRegex(R"((\w+)(\.?[\w-]+)*@([\w-]+\.)+[\w-]{2,})");
    QRegularExpressionValidator emailValidator(emailRegex);
    int emailPos = 0;
    if (!email.isEmpty() && emailValidator.validate(email, emailPos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Input Error", "Email vide ou invalide !!");
        return;
    }

    // Validate 'salaire' input is numeric and greater than zero, but can be empty
    bool salaireOk;
    double salaire = ui->salaire->text().toDouble(&salaireOk);
    if (!ui->salaire->text().isEmpty() && (!salaireOk || salaire <= 0)) {
        QMessageBox::warning(this, "Input Error", "Salaire vide ou invalide !!");
        return;
    }

    // Validate 'poste' selection (must be valid, but can be empty)
    // Get values from UI elements for each field
    QString sexe = ui->homme->isChecked() ? "M" : (ui->femme->isChecked() ? "F" : "");
    int depId = ui->departement->currentData().toInt();
    int posteId = ui->poste->currentData().toInt();
    QDate date_emboche = ui->date_emboche->date();

    // Create an Employe instance and call the modifier function
    bool success = emp.modifier(nom, prenom, sexe, email, salaire, depId, QString::number(posteId), date_emboche, id);
    bool exists = emp.idExists(id);

    if (exists) {
        if (ui->confirmation->isChecked()) {
            if (success) {
                QMessageBox::information(this, "Success", "Employee information modified successfully.");
                ui->id_sup->clear();
                ui->tableView->setModel(emp.afficher());
                ui->tableView->resizeColumnsToContents();
            } else {
                QMessageBox::warning(this, "Failure", "Failed to modify employee information.");
            }
        } else {
            QMessageBox::warning(this, "Failure", "Confirmer svp.");
        }
    } else {
        QMessageBox::information(this, "Failure", "L'ID pour modifier n'existe pas.");
    }
}

void MainWindow::on_searchButton_clicked() {
    QString id = ui->id_rech->text();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid ID.");
        return;
    }

    // Get the model currently set in the table view
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());

    if (!model) {
        QMessageBox::warning(this, "Model Error", "No model found for the table view.");
        return;
    }

    // Select the matching row in the table view
    QModelIndex matchingIndex;
    for (int row = 0; row < model->rowCount(); ++row) {
        // Check if the ID in the first column matches the entered ID
        if (model->index(row, 0).data().toString() == id) {
            matchingIndex = model->index(row, 0); // Select the first column in the matching row
            break;
        }
    }

    if (matchingIndex.isValid()) {
        // Select the matching row
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        QItemSelectionModel* selectionModel = ui->tableView->selectionModel();
        selectionModel->select(matchingIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);

        // Scroll to the selected row to ensure it is visible
        ui->tableView->scrollTo(matchingIndex);

        // Highlight the selected row (Blue by default in most styles)
    } else {
        QMessageBox::information(this, "Search Result", "No matching employee found.");
    }
}
void MainWindow::onclearbutton(){
    ui->id_rech->clear();

    ui->tableView->selectionModel()->clearSelection();

    ui->tableView->scrollToTop(); // This will scroll back to the top


}



void MainWindow::onSortSelectionChanged(int index) {
    Qt::SortOrder order;

    // Determine the order based on the index
    if (index == 0) {
        // For index 0, you may want to show the table in its normal state (no sorting)
        // Assuming you have a function to get all data in its default order:
        ui->tableView->setModel(emp.afficher()); // Show all data without sorting
    }
    else if (index == 1) {
        order = Qt::AscendingOrder;  // Ascending order for salaire
        QSqlQueryModel* sortedModel = emp.tri(order, 1); // 1 = sorting by salaire
        ui->tableView->setModel(sortedModel);
    }
    else if (index == 2) {
        order = Qt::DescendingOrder; // Descending order for salaire
        QSqlQueryModel* sortedModel = emp.tri(order, 1); // 1 = sorting by salaire
        ui->tableView->setModel(sortedModel);
    }
    else if (index == 3) {
        order = Qt::AscendingOrder;  // Ascending order for date d'embauche
        QSqlQueryModel* sortedModel = emp.tri(order, 3); // 3 = sorting by date d'embauche
        ui->tableView->setModel(sortedModel);
    }
    else if (index == 4) {
        // Always descending order for date d'embauche
        order = Qt::DescendingOrder;  // Bit décroissant
        QSqlQueryModel* sortedModel = emp.tri(order, 4); // 4 = sorting by date d'embauche
        ui->tableView->setModel(sortedModel);
    }

    // Resize columns to fit the content after sorting
    ui->tableView->resizeColumnsToContents();
}




void MainWindow::onrasionChanged()
{
    // Get the selected department
    QString selectedrasion = ui->raison_conge->currentText();

    // Show or hide the radio buttons based on the selected department
    if (selectedrasion == "Autre") {
        ui->rasion->setVisible(true);

    } else {
        ui->rasion->setVisible(false);

  }
}

void MainWindow::onDepartmentChanged()
{
    // Get the selected department
    QString selectedDepartment = ui->departement->currentText();

    // Show or hide the radio buttons based on the selected department
    if (selectedDepartment == "employe") {
        ui->role->setVisible(true);
        ui->label_148->setVisible(true);

    } else {
        ui->role->setVisible(false);
        ui->label_148->setVisible(false);

    }
}


void MainWindow::employetelechargerPDF(){
    emp.telechargerPDF();
}

void MainWindow::displayGenderStatistics()
{
    ui->bar->setVisible(false);
    ui->pie->setVisible(true);
    int maleCount = 0;
    int femaleCount = 0;

    // Query your database to get the counts of male and female
    QSqlQuery query;
    query.prepare("SELECT sexe, COUNT(*) as count FROM EMPELOYE GROUP BY sexe");
    query.exec();

    // Process the query results to assign counts to maleCount and femaleCount
    while (query.next()) {
        QString gender = query.value(0).toString().toLower(); // Get gender value from database
        int count = query.value(1).toInt(); // Get the count for the gender

        if (gender == "m") {
            maleCount = count;
        } else if (gender == "f") {
            femaleCount = count;
        }
    }

    // Create the Pie Chart Series
    QPieSeries *series = new QPieSeries();
    series->append("Male", maleCount);
    series->append("Female", femaleCount);

    // Create a chart and add the series to it
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Gender Distribution");

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
    ui->pie->scene()->addWidget(chartView);  // Add the chart view to the scene

    // Refresh the QGraphicsView
    ui->pie->scene()->update();
    ui->pie->show();  // Make sure the pie chart is displayed
}





void MainWindow::onUpdateTypeCongeClicked() {
    Employe employe;
    employe.updateTypeConge();
}


void MainWindow::displayCongeStats() {
    ui->bar->setVisible(true);
    ui->pie->setVisible(false);
    // Fetch statistics from the database for each month
    QMap<QString, QPair<int, int>> monthlyStats = emp.getCongeStatsByMonth();

    // Prepare data for the bar chart
    QBarSet *acceptedSet = new QBarSet("Accepted");
    QBarSet *refusedSet = new QBarSet("Refused");

    // Predefined list of months
    QStringList months = {"January", "February", "March", "April", "May", "June",
                          "July", "August", "September", "October", "November", "December"};

    int maxLeaves = 0; // Variable to keep track of the maximum number of leaves

    // Initialize bar set values for each month
    for (int i = 1; i <= 12; ++i) {
        QString monthNumber = QString("%1").arg(i, 2, 10, QChar('0')); // "01", "02", ...
        if (monthlyStats.contains(monthNumber)) {
            int accepted = monthlyStats[monthNumber].first;
            int refused = monthlyStats[monthNumber].second;
            acceptedSet->append(accepted);
            refusedSet->append(refused);
            maxLeaves = qMax(maxLeaves, qMax(accepted, refused)); // Update the max leaves value
        } else {
            acceptedSet->append(0);
            refusedSet->append(0);
        }
    }

    // Create a bar series and add the bar sets
    QBarSeries *series = new QBarSeries();
    series->append(acceptedSet);
    series->append(refusedSet);

    // Create a chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Conge Statistics by Month");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Set up X-axis (months)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(months);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Set up Y-axis (number of leaves) with a fixed range of 0 to 10
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 10); // Set the Y-axis range to 0 to 10 (fixed)
    axisY->setTitleText("Number of Leaves");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Display the chart in the QGraphicsView
    QGraphicsScene *scene = new QGraphicsScene(this);

    // Resize chart to fit the QGraphicsView
    QSize graphicsViewSize = ui->bar->size();
    chart->setMinimumSize(graphicsViewSize);
    chart->setMaximumSize(graphicsViewSize);
    chart->resize(graphicsViewSize);

    // Add the chart to the scene
    scene->addItem(chart);

    // Set the scene in the QGraphicsView
    ui->bar->setScene(scene);
    ui->bar->fitInView(chart, Qt::KeepAspectRatio); // Adjust to fit
    ui->bar->show();
}


































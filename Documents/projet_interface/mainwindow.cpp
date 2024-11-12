#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QMessageBox>
#include "employe.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QFrame>
#include <QPalette>
#include <QSpacerItem>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupComboBoxes();


    ui->tableView->setModel(emp.afficher());
    ui->tableView->resizeColumnsToContents();











    ui->widjt_icon_text->setVisible(false);
    // Ensure Aceuille tab widget is initially displayed, hide all other tab widgets
    ui->aceuille->setVisible(false);   // Show Aceuille tab widget by default
    ui->demande_conge->setVisible(false);
    ui->metier_comm1->setVisible(false);
    ui->metier_comm2->setVisible(false);
    ui->metier_artic1->setVisible(false);
    ui->metier_artic2->setVisible(false);
    ui->metier_matie1->setVisible(false);
    ui->metier_matie2->setVisible(false);
    ui->metier_client1->setVisible(true);
    ui->metier_client2->setVisible(false);

    // Connect button clicks to corresponding slots
    connect(ui->ajouter, &QPushButton::clicked, this, &MainWindow::on_addEmployeeButton_clicked);
    connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_supprimerEmploye_clicked);
    connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::on_modifyEmployeeButton_clicked);
    connect(ui->chercher, &QPushButton::clicked, this, &MainWindow::on_searchButton_clicked);
    connect(ui->annuler, &QPushButton::clicked, this, &MainWindow::onclearbutton);
    connect(ui->comboBox_tri, SIGNAL(currentIndexChanged(int)), this, SLOT(onSortSelectionChanged(int)));





    connect(ui->pushButton_23, &QPushButton::clicked, this, &MainWindow::on_pushButton_23_clicked);
    connect(ui->pushButton_24, &QPushButton::clicked, this, &MainWindow::showDemandeConge);
    connect(ui->pushButton_21, &QPushButton::clicked, this, &MainWindow::showMetierComm1);
    connect(ui->pushButton_22, &QPushButton::clicked, this, &MainWindow::showMetierComm2);
    connect(ui->pushButton_25, &QPushButton::clicked, this, &MainWindow::showMetierArtic1);
    connect(ui->pushButton_26, &QPushButton::clicked, this, &MainWindow::showMetierArtic2);
    connect(ui->pushButton_27, &QPushButton::clicked, this, &MainWindow::showMetierMatie1);
    connect(ui->pushButton_28, &QPushButton::clicked, this, &MainWindow::showMetierMatie2);
    connect(ui->pushButton,    &QPushButton::clicked, this, &MainWindow::showMetierClient1);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::showMetierClient2);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_23_clicked()
{
    // Create the login dialog window
    loginDialog = new QDialog(this);  // Create the dialog as a child of the main window
    loginDialog->setWindowTitle("Login");
    loginDialog->setFixedSize(350, 250); // Set a fixed size for the login dialog

    // Set the background color and other properties
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(42, 42, 42)); // Dark background (use Window instead of Background)
    loginDialog->setAutoFillBackground(true);
    loginDialog->setPalette(palette);

    // Create a layout for the login dialog
    QVBoxLayout *mainLayout = new QVBoxLayout(loginDialog);
    mainLayout->setContentsMargins(20, 20, 20, 20);  // Add some padding

    // Create title label with larger font size
    QLabel *titleLabel = new QLabel("Welcome to My Application");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; color: white;");
    mainLayout->addWidget(titleLabel);

    // Add a spacer item for spacing between title and form
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Create username and password labels and fields
    QHBoxLayout *usernameLayout = new QHBoxLayout;
    QLabel *usernameLabel = new QLabel("Username:");
    usernameLabel->setStyleSheet("color: white;");
    usernameLineEdit = new QLineEdit();
    usernameLineEdit->setStyleSheet("background-color: white; border-radius: 5px; padding: 5px;");
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameLineEdit);
    mainLayout->addLayout(usernameLayout);

    // Add some space after the username field
    mainLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QHBoxLayout *passwordLayout = new QHBoxLayout;
    QLabel *passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("color: white;");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet("background-color: white; border-radius: 5px; padding: 5px;");
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordLineEdit);
    mainLayout->addLayout(passwordLayout);

    // Add some space after the password field
    mainLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Create login button with custom style
    loginButton = new QPushButton("Login");
    loginButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 10px; padding: 10px;");
    loginButton->setFixedHeight(40);
    mainLayout->addWidget(loginButton);

    // Add a spacer item at the bottom
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Connect the login button to the login validation function
    connect(loginButton, &QPushButton::clicked, this, [this]() {
        QString username = usernameLineEdit->text();
        QString password = passwordLineEdit->text();

        // Check login credentials (simple check, you can replace this with actual validation)
        if (username == "admin" && password == "password") {
            QMessageBox::information(loginDialog, "Login Success", "Login successful!");
            loginDialog->accept();
            showAceuille();// Close the login dialog
        } else {
            QMessageBox::warning(loginDialog, "Login Failed", "Invalid username or password.");
        }
    });

    // Show the login dialog
    loginDialog->exec();  // Execute the dialog (blocking until closed)
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
void MainWindow::setupComboBoxes()
{
    // Populate departement combo box
    emp.fillComboBox(ui->departement, "SELECT NOM FROM department");

    // Populate poste combo box
    emp.fillComboBox(ui->poste, "SELECT NOM FROM poste");
}



void MainWindow::on_addEmployeeButton_clicked()
{


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
    int posteId = ui->poste->currentData().toInt();
    emp.date_emboche = ui->date_emboche->date(); // Date from QDateEdit widget

    emp.setnom(nom);
    emp.setprenom(prenom);
    emp.setsexe(sexe);
    emp.setemail(email);
    emp.setsalaire(salaire);
    emp.setdep(depId);
    emp.setposte(QString::number(posteId));

    bool test=emp.ajouter();
    // Check for confirmation
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





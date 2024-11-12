#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connexion.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test = c.createconnect(); // Établir la connexion à la base de données

    if (test) {
        MainWindow *w = new MainWindow(); // Instancier MainWindow sur le tas
        w->show(); // Afficher la fenêtre principale

        QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                 QObject::tr("Connection successful.\n"
                                             "Click OK to continue.")); // Message de succès
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                              QObject::tr("Connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return -1; // Quitter l'application si la connexion échoue
    }

    return a.exec(); // Démarrer l'événement loop
}

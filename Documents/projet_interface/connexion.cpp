#include "connexion.h"
#include "qstring.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test = false;




    // Remove the existing connection with the same name (if it exists)
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    // Create a new database connection with the unique name
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "qt_sql_default_connection");
    db.setDatabaseName("Projet_c++_2A35"); // Insert the name of the data source
    db.setUserName("malek"); // Insert the username
    db.setPassword("51712377"); // Insert the user's password

    // Try to open the database
    if (db.open())
    test = true;
    return test;
}

#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Projet_c++_2A35");//inserer le nom de la source de données
db.setUserName("malek");//inserer nom de l'utilisateur
db.setPassword("51712377");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}

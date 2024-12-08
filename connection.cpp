#include "connection.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

Connection* Connection::instance = nullptr;  // Initialize the static instance pointer

Connection::Connection() {
    // Initialize QSqlDatabase without opening the connection
    db = QSqlDatabase::addDatabase("QODBC");
}

Connection* Connection::getInstance() {
    // Ensure only one instance is created
    if (instance == nullptr) {
        instance = new Connection();
    }
    return instance;
}

bool Connection::createconnect() {
    bool test = false;

    // If already connected, do not attempt to reconnect
    if (db.isOpen()) {
        return true;
    }

    db.setDatabaseName("autocare");  // Database name
    db.setUserName("projetcpp2a30"); // Database user
    db.setPassword("esprit24");      // Database password

    // Try to open the database connection
    if (db.open()) {
        test = true;
    } else {
        qDebug() << "Database connection error: " << db.lastError().text();
    }

    return test;
}

void Connection::closeConnection() {
    if (db.isOpen()) {
        db.close(); // Close the database connection if it's open
    }
}

bool Connection::authenticateUser(const QString &email, const QString &motDePasse) {
    QSqlQuery query;
    query.prepare("SELECT * FROM USERS WHERE EMAIL = :email AND MOTDEPASSE = :motdepasse");
    query.bindValue(":email", email);
    query.bindValue(":motdepasse", motDePasse);

    // Execute the query and check if the user exists
    if (query.exec() && query.next()) {
        return true;  // User authenticated successfully
    }

    return false;  // User not found
}

Connection::~Connection() {
    closeConnection(); // Close the database connection when the object is destroyed
}

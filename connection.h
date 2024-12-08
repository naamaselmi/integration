#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlError>

class Connection
{
private:
    QSqlDatabase db;  // Database connection object
    static Connection* instance;  // Static instance pointer

    // Private constructor for singleton pattern
    Connection();

public:
    static Connection* getInstance(); // Singleton method
    bool createconnect(); // Create and open the database connection
    void closeConnection(); // Close the database connection
    bool authenticateUser(const QString &email, const QString &motDePasse); // Authenticate user
    ~Connection(); // Destructor
};

#endif // CONNECTION_H

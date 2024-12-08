#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "vehicule.h"
#include "technicien.h"
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Get the singleton instance of Connection
    Connection* c = Connection::getInstance();

    bool test = c->createconnect(); // Try to establish the database connection
    MainWindow w; // Create the main window
    w.show(); // Show the main window

    // If the connection is successful, show a success message
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Database is open"),
                    QObject::tr("Connection successful.\nClick Cancel to exit."), QMessageBox::Cancel);
    } else {
        // If the connection fails, show an error message
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                    QObject::tr("Connection failed.\nClick Cancel to exit."), QMessageBox::Cancel);
    }

    return a.exec(); // Start the application event loop
}

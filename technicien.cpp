#include "technicien.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qsqlerror.h"
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>


// Constructor for the technicien class
technicien::technicien(int id, QString nom, QString prenom, QString competence, QString disponibilite, QString phoneNumber, QString absent, QString adresse)
{
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->competence = competence;
    this->disponibilite = disponibilite;
    this->phoneNumber = phoneNumber;
    this->absent = absent;
    this->adresse = adresse;
}

// Method to add a new technician to the database

bool technicien::ajouter()
{
    QSqlQuery q;

    // Prepare the SQL statement with the correct table and column names
    q.prepare("INSERT INTO TECHNICEN (ID, NOM_T, PRENOM_T, COMPETENCES, DISPONIBILITER, PHONENUMBER, ABSENT, ADRESSE) "
              "VALUES (:id, :nom, :prenom, :competence, :disponibilite, :phoneNumber, :absent, :adresse)");

    // Bind values to the placeholders in the SQL statement
    q.bindValue(":id", id);
    q.bindValue(":nom", nom);
    q.bindValue(":prenom", prenom);
    q.bindValue(":competence", competence);
    q.bindValue(":disponibilite", disponibilite);
    q.bindValue(":phoneNumber", phoneNumber);
    q.bindValue(":absent", absent);
    q.bindValue(":adresse", adresse);

    // Execute the query and check if it succeeds
    if (!q.exec()) {
        // If the query fails, print the error
        qDebug() << "Failed to insert technician:" << q.lastError().text();
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------

bool technicien::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM TECHNICEN WHERE ID = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        // Print the error if the query fails
        qDebug() << "Failed to delete technician:" << q.lastError().text();
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------

technicien technicien::findById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM TECHNICEN WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.next()) { // If technicien is found
            technicien t;
            t.id = query.value("ID").toInt();
            t.nom = query.value("NOM_T").toString();
            t.prenom = query.value("PRENOM_T").toString();
            t.competence = query.value("COMPETENCES").toString();
            t.disponibilite = query.value("DISPONIBILITER").toString();
            t.phoneNumber = query.value("PHONENUMBER").toString();
            t.absent = query.value("ABSENT").toString();
            t.adresse = query.value("ADRESSE").toString();

            return t;
        }
    }
    // If not found, return an technicien object with id set to -1 to indicate not found
    technicien t;
    t.id = -1;
    return t;
}

//------------------------------------------------------------------------------------------------------------------------

technicien technicien::findByName(const QString& name)
{

    QSqlQuery query;
    query.prepare("SELECT * FROM TECHNICEN WHERE NOM_T = :name");
    query.bindValue(":name", name);

    if (query.exec()) {
        if (query.next()) { // If technicien is found
            technicien t;
            t.id = query.value("ID").toInt();
            t.nom = query.value("NOM_T").toString();
            t.prenom = query.value("PRENOM_T").toString();
            t.competence = query.value("COMPETENCES").toString();
            t.disponibilite = query.value("DISPONIBILITER").toString();
            t.phoneNumber = query.value("PHONENUMBER").toString();
            t.absent = query.value("ABSENT").toString();
            t.adresse = query.value("ADRESSE").toString();

            return t;
        }
    }

    // If not found, return a technicien object with id set to -1 to indicate not found
    technicien t;
    t.id = -1;
    return t;
}

//------------------------------------------------------------------------------------------------------------------------

bool technicien::mettreAJour(int newId)
{
    QSqlQuery q;

    // Prepare the SQL statement to update the technician data
    q.prepare("UPDATE TECHNICEN SET ID = :newId, NOM_T = :nom, PRENOM_T = :prenom, COMPETENCES = :competence, DISPONIBILITER = :disponibilite, PHONENUMBER = :phoneNumber, ABSENT = :absent, ADRESSE = :adresse WHERE ID = :currentId");

    q.bindValue(":newId", newId);
    q.bindValue(":nom", this->nom);
    q.bindValue(":prenom", this->prenom);
    q.bindValue(":competence", this->competence);
    q.bindValue(":disponibilite", this->disponibilite);
    q.bindValue(":phoneNumber", this->phoneNumber);
    q.bindValue(":absent", this->absent);
    q.bindValue(":adresse", this->adresse);
    q.bindValue(":currentId", this->id);

    // Execute the query and return true if successful, otherwise false
    if (q.exec()) {
        this->id = newId;  // Update the object's id attribute to the new ID
        return true;
    } else {
        qDebug() << "Failed to update technician:" << q.lastError().text();
        return false;
    }
}

//------------------------------------------------------------------------------------------------------------------------

technicien technicien::findByCompetence(const QString& competence)
{
     QSqlQuery query;
     query.prepare("SELECT * FROM TECHNICEN WHERE COMPETENCES = :competence");
     query.bindValue(":competence", competence);

     if (query.exec()) {
         if (query.next()) { // If technicien is found
             technicien t;
             t.id = query.value("id").toInt();
             t.nom = query.value("nom").toString();
             t.prenom = query.value("prenom").toString();
             t.competence = query.value("competence").toString();
             t.disponibilite = query.value("disponibilite").toString();
             t.phoneNumber = query.value("phoneNumber").toString();
             t.absent = query.value("absent").toString();
             t.adresse = query.value("adresse").toString();

             return t;
         }
     }

     // If not found, return a technicien object with id set to -1 to indicate not found
     technicien t;
     t.id = -1;
     return t;
 }

//-------------------------------------------------------------------------------------------------------------------------

QSqlQueryModel* technicien::afficher()
 {
     QSqlQueryModel* model = new QSqlQueryModel();

         // Utilisation d'une requête SQL explicite pour récupérer les colonnes dans l'ordre correct
         model->setQuery("SELECT * FROM TECHNICEN");

         // Définition des en-têtes des colonnes
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
         model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
         model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
         model->setHeaderData(3, Qt::Horizontal, QObject::tr("Compétence"));
         model->setHeaderData(4, Qt::Horizontal, QObject::tr("Disponibilité"));
         model->setHeaderData(5, Qt::Horizontal, QObject::tr("Numéro de téléphone"));
         model->setHeaderData(6, Qt::Horizontal, QObject::tr("Absent"));
         model->setHeaderData(7, Qt::Horizontal, QObject::tr("Adresse"));

         return model;
 }

//-------------------------------------------------------------------------------------------------------------------------

bool technicien::convertirEnPDF(const QString &filePath) const
{
     // Create a QPdfWriter with the specified file path
     QPdfWriter pdfWriter(filePath);
     pdfWriter.setPageSize(QPageSize(QPageSize::A4));    // Set page size
     pdfWriter.setResolution(300);                       // Set resolution (DPI)
     pdfWriter.setTitle("Technicien Report");

     // Create a QPainter to draw on the PDF
     QPainter painter(&pdfWriter);

     if (!painter.isActive()) {
         return false; // Return false if the painter failed to activate
     }

     // Set fonts and basic styles
     QFont titleFont("Arial", 16, QFont::Bold);
     QFont contentFont("Arial", 12);

     painter.setFont(titleFont);
     painter.drawText(100, 100, QObject::tr("Technicien Report"));

     // Move down a bit and draw the content
     painter.setFont(contentFont);
     painter.drawText(100, 200, QObject::tr("Technicien ID: %1").arg(getID()));
     painter.drawText(100, 250, QObject::tr("Nom: %1").arg(getNom()));
     painter.drawText(100, 300, QObject::tr("Prénom: %1").arg(getPrenom()));
     painter.drawText(100, 350, QObject::tr("Compétence: %1").arg(getcompetence()));
     painter.drawText(100, 400, QObject::tr("Disponibilité: %1").arg(getdisponibilite()));
     painter.drawText(100, 450, QObject::tr("phoneNumber: %1").arg(getphoneNumber()));
     painter.drawText(100, 500, QObject::tr("absent: %1").arg(getabsent()));
     painter.drawText(100, 550, QObject::tr("adresse: %1").arg(getadresse()));

     // End the painter to finalize the PDF
     painter.end();

     return true;  // Return true if successful
 }

//-------------------------------------------------------------------------------------------------------------------------

bool technicien::convertirEnPDFAbsents(const QString &filePath, const QList<technicien> &techniciensAbsents) {
    // Initialize the PDF writer
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);
    pdfWriter.setTitle(QObject::tr("Bilan des Absences"));

    QPainter painter(&pdfWriter);

    if (!painter.isActive()) {
        return false; // Return false if the painter failed to activate
    }

    // Set fonts and styles
    QFont titleFont("Arial", 20, QFont::Bold);
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Arial", 10);

    // Title
    painter.setFont(titleFont);
    painter.setPen(Qt::red);
    QRect titleRect(0, 50, pdfWriter.width(), 100);
    painter.drawText(titleRect, Qt::AlignCenter, QObject::tr("Bilan des Absences"));

    // Set up table headers
    int margin = 50;       // Left margin
    int rowHeight = 40;    // Height for each row
    int yPosition = 200;   // Starting y position for the table

    painter.setFont(headerFont);
    painter.setPen(Qt::black);

    // Column positions
    QVector<int> columnX = {margin, 150, 300, 500, 700, 1000, 1300, 1600};

    // Draw headers
    painter.drawText(columnX[0], yPosition, QObject::tr("ID"));
    painter.drawText(columnX[1], yPosition, QObject::tr("Nom"));
    painter.drawText(columnX[2], yPosition, QObject::tr("Prénom"));
    painter.drawText(columnX[3], yPosition, QObject::tr("Compétence"));
    painter.drawText(columnX[4], yPosition, QObject::tr("Disponibilité"));
    painter.drawText(columnX[5], yPosition, QObject::tr("Téléphone"));
    painter.drawText(columnX[6], yPosition, QObject::tr("Adresse"));

    // Line under headers
    yPosition += rowHeight;
    painter.drawLine(margin, yPosition, pdfWriter.width() - margin, yPosition);

    // Content rows
    painter.setFont(contentFont);
    yPosition += rowHeight;

    for (const technicien &t : techniciensAbsents) {
        if (yPosition > pdfWriter.height() - 100) { // Handle page break
            pdfWriter.newPage();
            yPosition = 200; // Reset y position for new page

            // Redraw headers on the new page
            painter.setFont(headerFont);
            painter.drawText(columnX[0], yPosition, QObject::tr("ID"));
            painter.drawText(columnX[1], yPosition, QObject::tr("Nom"));
            painter.drawText(columnX[2], yPosition, QObject::tr("Prénom"));
            painter.drawText(columnX[3], yPosition, QObject::tr("Compétence"));
            painter.drawText(columnX[4], yPosition, QObject::tr("Disponibilité"));
            painter.drawText(columnX[5], yPosition, QObject::tr("Téléphone"));
            painter.drawText(columnX[6], yPosition, QObject::tr("Adresse"));

            yPosition += rowHeight;
            painter.drawLine(margin, yPosition, pdfWriter.width() - margin, yPosition);
            yPosition += rowHeight;
        }

        // Draw content rows
        painter.drawText(columnX[0], yPosition, QString::number(t.getID()));
        painter.drawText(columnX[1], yPosition, t.getNom());
        painter.drawText(columnX[2], yPosition, t.getPrenom());
        painter.drawText(columnX[3], yPosition, t.getcompetence());
        painter.drawText(columnX[4], yPosition, t.getdisponibilite());
        painter.drawText(columnX[5], yPosition, t.getphoneNumber());
        painter.drawText(columnX[6], yPosition, t.getadresse());
        yPosition += rowHeight;
    }

    painter.end();
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------

QSqlQueryModel* technicien::trier(const QString& critere, const QString& ordre) {
     QSqlQueryModel* model = new QSqlQueryModel();

     QString queryStr = "SELECT * FROM TECHNICEN ORDER BY " + critere + " " + ordre;
     model->setQuery(queryStr);

     if (model->lastError().isValid()) {
         qDebug() << "Erreur de tri:" << model->lastError().text();
         return nullptr;
     }

     return model;
 }

//-------------------------------------------------------------------------------------------------------------------------

QSqlQueryModel* technicien::getAbsentTechniciens()
{
     QSqlQueryModel *model = new QSqlQueryModel();

         // Utilisation de 'oui' pour la colonne 'absent', et sélection de tous les techniciens absents
         model->setQuery("SELECT * FROM TECHNICEN WHERE absent = 'oui'");

         // Vérification des erreurs de requête
         if (model->lastError().isValid()) {
             qDebug() << "Error in query: " << model->lastError().text();
         }

         // Définition des en-têtes du modèle
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
         model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
         model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
         model->setHeaderData(3, Qt::Horizontal, QObject::tr("Numéro de téléphone"));

         return model;
 }

//-------------------------------------------------------------------------------------------------------------------------

QSqlQueryModel* technicien::obtenirStatistiquesCompetence()
{
     QSqlQueryModel* model = new QSqlQueryModel();

     // SQL Query to get competences and count of technicians for each competence
     QSqlQuery query;
     query.prepare("SELECT COMPETENCES, COUNT(*) as nombre FROM TECHNICEN GROUP BY COMPETENCES");

     // Execute the query and check for success
     if (!query.exec()) {
         qWarning() << "Query failed:" << query.lastError().text();  // Log if there's an error
         return model;  // Return empty model in case of error
     }

     model->setQuery(query);
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("Compétence"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre"));

     return model;
 }



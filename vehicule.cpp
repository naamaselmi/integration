#include "vehicule.h"
#include "mainwindow.h"
#include "qsqlerror.h"
#include "ui_mainwindow.h"
//#include <QZXing.h>


Vehicule::Vehicule(int id, QString marque, QString modele, QString annee, QString immatriculation ,
                   QString type, QString etat, QString historique_entretiens, QString nom_proprietaire ,QString email)
{
    this->id = id;
    this->marque = marque;
    this->modele = modele;
    this->annee = annee;
    this->immatriculation = immatriculation;
    this->type = type;
    this->etat = etat;
    this->email = email;
    this->historique_entretiens = historique_entretiens;
    this->nom_proprietaire = nom_proprietaire;
}
bool Vehicule::ajouter()
{
    QSqlQuery q;
    q.prepare("INSERT INTO vehicule (id, marque, modele, annee, immatriculation, type, etat, historique_entretiens, nom_proprietaire , email) "
               "VALUES (:id, :marque, :modele, :annee, :immatriculation, :type, :etat, :historique_entretiens, :nom_proprietaire, :email)");
    q.bindValue(":id", id);
    q.bindValue(":marque", marque);
    q.bindValue(":modele", modele);
    q.bindValue(":annee", annee);
    q.bindValue(":immatriculation", immatriculation);
    q.bindValue(":type", type);
    q.bindValue(":etat", etat);
    q.bindValue(":email", email);
    q.bindValue(":historique_entretiens", historique_entretiens);
    q.bindValue(":nom_proprietaire", nom_proprietaire);
    return q.exec();
}

bool Vehicule::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM vehicule WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec()){
        return q.numRowsAffected() > 0 ;
    }
    return false;
}

QSqlQueryModel* Vehicule::afficher(const QString& critereTri) {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Construct the SQL query with sorting criteria
    QString queryStr = "SELECT * FROM vehicule";
    if (critereTri == "annee") {
        queryStr += " ORDER BY annee";  // Sort by year
    } else if (critereTri == "immatriculation") {
        queryStr += " ORDER BY SUBSTR(immatriculation, 1, 3), immatriculation"; // Sort by registration number
    } else if (critereTri == "nom_proprietaire") {
        queryStr += " ORDER BY nom_proprietaire"; // Sort by owner name alphabetically
    }

    // Execute the query
    QSqlQuery q;
    if (q.exec(queryStr)) {
        model->setQuery(std::move(q));  // Pass by move to avoid copy warning
    }

    // Set the headers
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Modèle"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Année"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Immatriculation"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Historique entretiens"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Nom propriétaire"));

    return model;
}

Vehicule Vehicule::findById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM vehicule WHERE id = :id");
    query.bindValue(":id", id);


    if (query.exec()) {
        if (query.next()) { // Si le véhicule est trouvé
            Vehicule V;
            V.id = query.value("id").toInt();
            V.marque = query.value("marque").toString();
            V.modele = query.value("modele").toString();
            V.annee = query.value("annee").toString();
            V.immatriculation = query.value("immatriculation").toString();
            V.type = query.value("type").toString();
            V.etat = query.value("etat").toString();
                     V.email = query.value("email").toString();

            V.historique_entretiens = query.value("historique_entretiens").toString();
            V.nom_proprietaire = query.value("nom_proprietaire").toString();
            return V;
        }
        }


    Vehicule V;
    V.id = -1;
    return V;
}

bool Vehicule::mettreAJour(int newId)
{
    QSqlQuery query;

    // Prepare the SQL statement to update the vehicle data
    query.prepare("UPDATE vehicule SET id = :newId, marque = :marque, modele = :modele, annee = :annee, "
                  "immatriculation = :immatriculation, type = :type, etat = :etat, "
                  "historique_entretiens = :historique_entretiens, nom_proprietaire = :nom_proprietaire, email = :email "
                  "WHERE id = :currentId");
    query.bindValue(":newId", newId);
    query.bindValue(":marque", this->marque);
    query.bindValue(":modele", this->modele);
    query.bindValue(":annee", this->annee);
    query.bindValue(":immatriculation", this->immatriculation);
    query.bindValue(":type", this->type);
    query.bindValue(":etat", this->etat);
    query.bindValue(":email", this->email);
    query.bindValue(":historique_entretiens", this->historique_entretiens);
    query.bindValue(":nom_proprietaire", this->nom_proprietaire);
    query.bindValue(":currentId", this->id);

    // Execute the query and return true if successful, otherwise false
    if (query.exec()) {
        this->id = newId;  // Update the object's id attribute to the new ID
        return true;
    } else {
        return false;
    }
}

QSqlQueryModel* Vehicule::affi() {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Construct the SQL query without sorting
    QString queryStr = "SELECT * FROM vehicule"; // No sorting here

    // Execute the query
    QSqlQuery q;
    if (q.exec(queryStr)) {
        model->setQuery(std::move(q));  // Pass by move to avoid copy warning
    }

    // Set the headers (optional, but it helps to define them clearly)
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Modèle"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Année"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Immatriculation"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Historique entretiens"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Nom propriétaire"));

    return model;
}

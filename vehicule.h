#ifndef VEHICULE_H
#define VEHICULE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Vehicule
{
    QString marque, modele, annee, immatriculation, type, etat, historique_entretiens, nom_proprietaire,email;
    int id;
public:
    Vehicule(){};  // Default constructor
    Vehicule(int id, QString marque, QString modele, QString annee, QString immatriculation,
             QString type, QString etat, QString historique_entretiens, QString nom_proprietaire , QString email);

    // Getters
    QString getMarque() { return marque; }
    QString getModele() { return modele; }
    QString getAnnee() { return annee; }
    QString getImmatriculation() { return immatriculation; }
    QString getType() { return type; }
    QString getEtat() { return etat; }
    QString getHistoriqueEntretiens() { return historique_entretiens; }
    QString getNomProprietaire() { return nom_proprietaire; }
    QString getemail() { return email; }
    int getID() { return id; }

    // Setters
    void setMarque(QString m) { marque = m; }
    void setModele(QString m) { modele = m; }
    void setAnnee(QString a) { annee = a; }
    void setImmatriculation(QString im) { immatriculation = im; }
    void setType(QString t) { type = t; }
    void setEtat(QString e) { etat = e; }
    void setemail(QString em) { email = em; }
    void setHistoriqueEntretiens(QString h) { historique_entretiens = h; }
    void setNomProprietaire(QString n) { nom_proprietaire = n; }
    void setID(int id) { this->id = id; }

    // Member functions
    bool ajouter();
    bool mettreAJour(int newId);
    bool supprimer(int id);
    QSqlQueryModel* afficher(const QString& critereTri);
    Vehicule findById(int id);
    QSqlQueryModel* affi();
};

#endif // VEHICULE_H

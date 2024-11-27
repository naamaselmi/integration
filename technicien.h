#ifndef TECHNICIEN_H
#define TECHNICIEN_H
#include <QString>
#include<QSqlQuery>
#include<QSqlQueryModel>


class technicien
{
    QString nom, prenom,competence,disponibilite,phoneNumber,absent,adresse;
        int id;

public:
        technicien(){};
        technicien (int , QString, QString, QString, QString, QString,QString,QString);

            //QString getNom(){return nom;}
            int getID() const {return id;}
            QString getNom() const {return nom;}
            QString getPrenom() const {return prenom;}
            QString getcompetence() const {return competence;}
            QString getdisponibilite() const {return disponibilite;}
            QString getphoneNumber() const {return phoneNumber;}
            QString getabsent() const {return absent;}
            QString getadresse() const {return adresse;}
            technicien findById(int id);
            technicien findByName(const QString& name);
            technicien findByCompetence(const QString& competence);
            bool mettreAJour(int newId);

            void setNom (QString n){nom=n;}
            void setPrenom (QString p){prenom=p;}
            void setcompetence (QString c){competence=c;}
            void setdisponibilite (QString d){disponibilite=d;}
            void setphoneNumber (QString ph){phoneNumber=ph;}
            void setadresse (QString ad){phoneNumber=ad;}
            void setabsent (QString a){absent=a;}
            void setID (int id ){this->id=id;}
            bool ajouter();
            bool supprimer (int);
            bool convertirEnPDF(const QString &filePath) const;
            bool convertirEnPDFAbsents(const QString &filePath, const QList<technicien> &techniciensAbsents);

            QSqlQueryModel * afficher ();
            QSqlQueryModel* trier(const QString& critere, const QString& ordre);
            QSqlQueryModel* obtenirStatistiquesCompetence();
            QSqlQueryModel* getAbsentTechniciens();
};


#endif // TECHNICIEN_H

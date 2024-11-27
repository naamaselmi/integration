#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "vehicule.h"
#include "technicien.h"

#include "chartdialog.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}


class PieChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PieChartWidget(QWidget *parent = nullptr);
    void setData(const QVector<QPair<QString, int>> &data);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPair<QString, int>> chartData; // Store the data as pairs (name, value)
    QRegularExpression adresseRegex;

};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void switchToGestionVehicule();
    void switchToGestionTechnicien();
//------------------------------------------------------------hamza------------------------------------------------------
 void on_Rechercher_clicked();
 void on_RechercherID_clicked();
 void on_Sauvgarder_clicked();
 void on_pushButtonAjouter_clicked();
 void on_Supprimer_clicked();
 void on_affivher_clicked();
 void on_triparannee_clicked();
 void on_triparImmatriculation_clicked();
 void on_Grapheparannee_clicked();
 void on_Grapheparmarque_clicked();
 void on_exporterPDF_clicked();
 void on_pushButtonGenererQR_clicked();
 void on_mail_clicked();
 void on_triparNomProprietaire_clicked();
 void on_scann_clicked();
//---------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------naama---------------------------------------------------------
 void on_ajouter_clicked();
 void on_supprimer_clicked();
 void on_miseajour_clicked();
 void on_conversionenpdf_clicked();
 void on_statistique_clicked();
 void on_recherche_clicked();
 void on_trier_clicked();
 //void on_notification_clicked();
 void on_suivie_des_absences_clicked();
 void on_afficher_clicked();
 //void on_exportExcel_clicked();
 void on_modeButton_clicked();
 void on_afficherCarte_clicked();
 void on_ajouterDocument_clicked();
 void on_supprimerDocument_clicked();
 void on_visualiserDocument_clicked();
 void on_idTechDoc_textChanged();
 void on_trier_nom_clicked();
 void on_trier_absence_clicked();
 void on_recherchenom_clicked();
 void on_recherchecompetence_clicked();
 void on_bilan_clicked();

//----------------------------------------------------------------------------------------------------------------------
private:
 //----------------------------------------hamza-----------------------------------
    Ui::MainWindow *ui;
    Vehicule Vtmp;
     QLabel *labellepainture;
 //-------------------------------------------------------------------------------

 //---------------------------------------naama----------------------------------
     technicien tech;
     PieChartWidget *pieChartWidget;
     bool isNightMode = false;
     QTableWidget* documentsTable;
     QLineEdit* typeDocument;       // On utilise un simple LineEdit au lieu d'un ComboBox
     QString documentPath;
     void setupDocumentsUI();
     void chargerDocuments(int idTech);
 //-----------------------------------------------------------------------------
};

#endif // MAINWINDOW_H

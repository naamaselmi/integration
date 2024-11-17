#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "vehicule.h"
#include "chartdialog.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

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

private:
    Ui::MainWindow *ui;
    Vehicule Vtmp;
};

#endif // MAINWINDOW_H

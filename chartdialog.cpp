#include "chartdialog.h"
#include <QVBoxLayout>  // Pour le layout

ChartDialog::ChartDialog(QWidget *parent)
    : QDialog(parent),
      chartView(new QChartView())  // Créer le chartView
{
    // Créer un layout vertical
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);  // Ajouter le chartView au layout

    setLayout(layout);  // Appliquer le layout à la fenêtre de dialogue
    setWindowTitle("Graphique");  // Donner un titre à la fenêtre
}

void ChartDialog::setChart(QChart *chart)
{
    chartView->setChart(chart);  // Définir le graphique à afficher
}

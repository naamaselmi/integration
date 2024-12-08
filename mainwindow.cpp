#include "mainwindow.h"
#include "connection.h"

#include "ui_mainwindow.h"
#include <QMessageBox>
#include "vehicule.h"
#include "technicien.h"
#include <QDate>
#include <QRegularExpression>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>          // Pour QBarSeries
#include <QtCharts/QBarSet>             // Pour QBarSet
#include <QtCharts/QBarCategoryAxis>    // Pour QBarCategoryAxis
#include <QtCharts/QValueAxis>         // Pour QValueAxis
#include <QVBoxLayout>
#include <QSqlError>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPdfWriter>
#include <QPainter>
#include <QMessageBox>
#include <QFont>
#include <QTextDocument>
#include <QDir>
#include <QFileDialog>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QUrl>
#include <QFile>
#include <QProcess>
#include <QMessageBox>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>


#include "libs/QrCodeGenerator.h"  // Adjust the path to match the location




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Définir l'icône de la fenêtre avec le chemin absolu
    setWindowIcon(QIcon("C:/Users/zussl/Documents/crudvehicule/AUTACARE.png"));

    // Définir le titre de la fenêtre
    setWindowTitle("Auto care");
    ui->setupUi(this);
    ui->tableView->setModel(Vtmp.affi());
    labellepainture = ui->labellepainture;
     ui->stackedWidget->setCurrentIndex(0);

    connect(ui->GV, &QPushButton::clicked, this, &MainWindow::switchToGestionVehicule);
    connect(ui->GT, &QPushButton::clicked, this, &MainWindow::switchToGestionTechnicien);
    //connect(ui->login_2, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//-------------------------------------------------------------------------------------------------
void MainWindow::  switchToGestionVehicule()
{
    qDebug() << "Switching to Gestion Vehicule";
    ui->stackedWidget_2->setCurrentIndex(0); // Index of Gestion Vehicule page
}

void MainWindow::switchToGestionTechnicien()
{
    qDebug() << "Switching to Gestion Technicien";
    ui->stackedWidget_2->setCurrentIndex(2); // Index of Gestion Technicien page
}

//--------------------------------------------------------------------------------------------------



//---------------------------------------------hamza---------------------------------------------------


void MainWindow::on_pushButtonAjouter_clicked()
{
    bool ok;
    int id = ui->lineEditID->text().toInt(&ok); // Convert text to int and check if it's valid

    if (!ok) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("L'ID doit être un nombre entier."), QMessageBox::Ok);
        return; // Exit the function if ID is not valid
    }

    QString anneeStr = ui->lineEditAnnee->text();
    bool anneeOk;
    int anneeInt = anneeStr.toInt(&anneeOk); // Try to convert the year string to an integer

    if (!anneeOk) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("L'année doit être un nombre entier."), QMessageBox::Ok);
        return; // Exit the function if the year is not an integer
    }

    if (anneeInt < 1900 || anneeInt > QDate::currentDate().year()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("L'année doit être un nombre entier entre 1900 et l'année en cours."), QMessageBox::Ok);
        return; // Exit the function if the year is out of range
    }

    QString immatriculation = ui->lineEditImmatriculation->text();
    QRegularExpression regex("^\\d{3}[tT][uU][nN]\\d{4}$"); // 3 digits, followed by TUN or tun, followed by 4 digits
    if (!regex.match(immatriculation).hasMatch()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("L'immatriculation doit être composée de 3 chiffres, suivis de 'TUN' ou 'tun', et de 4 chiffres."), QMessageBox::Ok);
        return; // Exit the function if immatriculation does not match the pattern
    }

    immatriculation = immatriculation.toUpper(); // Convert immatriculation to uppercase

    QString marque = ui->lineEditMarque->text();
    QString modele = ui->lineEditModele->text();
    QString type = ui->lineEditType->text();

    // Toujours définir l'état à "En cours de traitement"
    QString etat = "En cours de traitement";

    QString email = ui->email->text();
    QRegularExpression emailRegex("^[a-zA-Z0-9_+&*-]+(?:\\.[a-zA-Z0-9_+&*-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,7}$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("L'email n'est pas valide.\nVeuillez entrer un email au format correct (exemple@example.com)."), QMessageBox::Ok);
        return; // Exit the function if email format is invalid
    }

    QString nom_proprietaire = ui->lineEditNomP->text();
    QRegularExpression nameRegex("^[A-Za-z ]+$");
    if (!nameRegex.match(nom_proprietaire).hasMatch()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le nom du propriétaire doit être composé uniquement de lettres."), QMessageBox::Ok);
        return; // Exit the function if the owner's name contains non-letter characters
    }

    QString historique_entretiens = ui->lineEditHE->text();

    Vehicule V(id, marque, modele, anneeStr, immatriculation, type, etat, historique_entretiens, nom_proprietaire, email);
    bool test = V.ajouter();

    if (test) {
        QMessageBox::information(this, tr("OK"),
                                 tr("Ajout effectué\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(this, tr("NOT OK"),
                              tr("Ajout non effectué\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    }
}

void MainWindow::on_Rechercher_clicked()
{
    int id = ui->lineEditIDR->text().toInt(); // Updated to match XML
    Vehicule V = Vtmp.findById(id); // Assuming Vtmp is a global or class instance of Vehicule

    if (V.getID() != -1) {
        // Populate fields with vehicle data for updating
        ui->lineEditIDRM->setText(QString::number(V.getID())); // Set the ID in the corresponding field
        ui->lineEditMarque->setText(V.getMarque());
        ui->lineEditModele->setText(V.getModele());
        ui->lineEditAnnee->setText(V.getAnnee());
        ui->lineEditImmatriculation->setText(V.getImmatriculation());
        ui->lineEditType->setText(V.getType());
        ui->lineEditEtat->setText(V.getEtat());
        ui->email->setText(V.getemail());
        ui->lineEditHE->setText(V.getHistoriqueEntretiens());
        ui->lineEditNomP->setText(V.getNomProprietaire());

        QMessageBox::information(this, tr("Véhicule trouvé"),
                                 tr("Véhicule trouvé."));
    } else {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Véhicule introuvable.\nVeuillez vérifier l'ID."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_affivher_clicked()
{
    ui->tableView->setModel(Vtmp.affi());

}

void MainWindow::on_Sauvgarder_clicked()
{
    int id = ui->lineEditIDRM->text().toInt(); // Updated to match XML
    QString marque = ui->lineEditNewmarque->text();
    QString modele = ui->lineEditNewmodele->text();
    QString annee = ui->lineEditNewannee->text();
    QString immatriculation = ui->lineEditNewimmat->text();
    QString type = ui->lineEditNewtyoe->text();
    QString etat = ui->lineEditNewetat->text();
   // QString email = ui->nemail->text();
    QString historique_entretiens = ui->lineEditNewhistorique->text(); // Updated to match XML
    QString nom_proprietaire = ui->lineEditNewnomprop->text(); // Updated to match XML
QString email = ui->nemail->text();


    Vehicule V(id, marque, modele, annee, immatriculation, type, etat, historique_entretiens, nom_proprietaire  , email);
    bool test = V.mettreAJour(id); // Assuming mettreAJour is defined correctly

    if (test) {
        QMessageBox::information(this, tr("Sauvegarde réussie"),
                                 tr("Les informations du véhicule ont été mises à jour."));
    } else {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("La mise à jour des informations du véhicule a échoué."),
                              QMessageBox::Cancel);
    }

    // Refresh the table view after saving
    ui->tableView->setModel(Vtmp.affi()); // Assuming Vtmp is a global or class instance of Vehicule

}

void MainWindow::on_Supprimer_clicked()
{
    // Retrieve the ID to delete from a line edit
    int id = ui->lineEditIDS->text().toInt(); // Adjust lineEditIDR to the correct QLineEdit widget for ID input

    // Call the delete function
    bool success = Vtmp.supprimer(id); // Assuming Vtmp is the instance of Vehicule

    // Display a message based on success or failure
    if (success) {
        QMessageBox::information(this, tr("Suppression réussie"),
                                 tr("Le véhicule a été supprimé avec succès."));
        // Refresh the table view after deletion
        ui->tableView->setModel(Vtmp.affi());
    } else {
        QMessageBox::critical(this, tr("Erreur de suppression"),
                              tr("La suppression a échoué. Veuillez vérifier l'ID et réessayer."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_RechercherID_clicked()
{
    int id = ui->lineEditIDRM->text().toInt(); // Updated to match XML
    Vehicule V = Vtmp.findById(id); // Assuming Vtmp is a global or class instance of Vehicule

    if (V.getID() != -1) {
        // Populate fields with vehicle data for updating
        ui->lineEditIDRM->setText(QString::number(V.getID())); // Set the ID in the corresponding field
        ui->lineEditNewmarque->setText(V.getMarque());
        ui->lineEditNewmodele->setText(V.getModele());
        ui->lineEditNewannee->setText(V.getAnnee());
        ui->lineEditNewimmat->setText(V.getImmatriculation());
        ui->lineEditNewtyoe->setText(V.getType());
        ui->lineEditNewetat->setText(V.getEtat());
        ui->nemail->setText(V.getemail());
        ui->lineEditNewhistorique->setText(V.getHistoriqueEntretiens());
        ui->lineEditNewnomprop->setText(V.getNomProprietaire());

        QMessageBox::information(this, tr("Véhicule trouvé"),
                                 tr("Véhicule trouvé."));
    } else {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Véhicule introuvable.\nVeuillez vérifier l'ID."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_triparannee_clicked()
{
    // Pass "Année" as a QString, not as a const char array
    ui->tableView->setModel(Vtmp.afficher(QString("annee")));
}

void MainWindow::on_triparImmatriculation_clicked()
{
   ui->tableView->setModel(Vtmp.afficher(QString("immatriculation")));
}

void MainWindow::on_Grapheparannee_clicked()
{
    // Create a pie chart series
    QPieSeries *series = new QPieSeries();

    // Retrieve years and their counts from the database
    QSqlQuery query;
    if (!query.exec("SELECT annee, COUNT(*) FROM vehicule GROUP BY annee")) {
        qDebug() << "Error fetching years: " << query.lastError().text();
        return;
    }

    // Create a map to store the frequency of each year
    QMap<int, int> yearCount;

    // Process the query results and count the occurrences of each year
    while (query.next()) {
        int year = query.value(0).toInt();  // Get the year
        int count = query.value(1).toInt();  // Get the count for that year
        yearCount[year] = count;  // Store the count in the map
    }

    // Debug: Print out the counts
    qDebug() << "Year Count:" << yearCount;

    // Define an array of custom colors
    QColor colors[] = { QColor(255, 99, 132), QColor(54, 162, 235), QColor(255, 206, 86),
                         QColor(75, 192, 192), QColor(153, 102, 255), QColor(255, 159, 64) };

    int colorIndex = 0;  // To cycle through the custom colors

    // Add the years to the pie chart series and set custom colors
    for (auto it = yearCount.begin(); it != yearCount.end(); ++it) {
        int year = it.key();
        int count = it.value();

        // Ensure non-zero count for valid pie slice creation
        if (count > 0) {
            QPieSlice *slice = series->append(QString::number(year) + " (" + QString::number(count) + ")", count);
            slice->setLabelVisible(true);
            slice->setPen(QPen(Qt::black, 1));  // Optional: Set the outline color of the slices
            slice->setBrush(QBrush(colors[colorIndex % (sizeof(colors) / sizeof(colors[0]))]));  // Apply custom color
            colorIndex++;
        }
    }

    // Create the chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution des véhicules par année");
    chart->legend()->hide();

    // Create a chart view to display the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Create a dialog to display the chart
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Graphe des véhicules par année");

    // Layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);

    // Set the layout and resize the dialog
    dialog->setLayout(layout);
    dialog->resize(800, 600);  // Set initial size of the dialog

    // Show the dialog as non-modal (does not block main window)
    dialog->show();  // Using show() instead of exec()

    // Ensures the dialog is deleted after closing
    connect(dialog, &QDialog::finished, dialog, &QDialog::deleteLater);  // Clean up dialog when finished
}

void MainWindow::on_Grapheparmarque_clicked()
{
    // Créer la série de barres
    QBarSeries *series = new QBarSeries();

    // Récupérer les marques de voiture depuis la base de données
    QSqlQuery query;
    if (!query.exec("SELECT marque, COUNT(*) FROM vehicule GROUP BY marque")) {
        qDebug() << "Error fetching marques: " << query.lastError().text();
        return;
    }

    // Créer une map pour stocker les marques et leurs comptages
    QMap<QString, int> marqueCount;

    // Traitement des résultats de la requête
    while (query.next()) {
        QString marque = query.value(0).toString();
        int count = query.value(1).toInt();
        marqueCount[marque] = count;
    }

    // Créer un QBarSet pour chaque marque et l'ajouter à la série
    QList<QBarSet *> barSets;
    for (auto it = marqueCount.begin(); it != marqueCount.end(); ++it) {
        QBarSet *set = new QBarSet(it.key());
        *set << it.value();  // Ajouter la valeur de comptage à la barre
        barSets.append(set);
    }

    // Ajouter les QBarSets à la série
    series->append(barSets);

    // Ajuster l'épaisseur des barres
    series->setBarWidth(0.2);  // Ajuste la largeur des barres (valeur entre 0.0 et 1.0)

    // Créer le graphique et ajouter la série
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des véhicules par marque");

    // Créer et configurer l'axe Y (valeurs)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de véhicules");
    axisY->setLabelFormat("%d");  // Afficher les valeurs entières
    axisY->setTickInterval(1);    // Intervalle des ticks à 1 pour avoir 1, 2, 3, ...

    // Trouver la valeur maximale dans marqueCount sans créer de conteneur temporaire
    int maxCount = 0;
    for (auto it = marqueCount.begin(); it != marqueCount.end(); ++it) {
        if (it.value() > maxCount) {
            maxCount = it.value();
        }
    }

    // Définir la plage de l'axe Y en fonction de maxCount
    axisY->setRange( 0 , maxCount );
    axisY->setTickCount(maxCount + 1 );
   // axisY->setMin(0);  // Fixer la valeur minimale à 0 pour éviter la duplication de 0

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Créer la vue du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Créer un dialogue pour afficher le graphique
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Graphique des véhicules par marque");

    // Layout pour le dialogue
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);

    // Définir le layout et redimensionner le dialogue
    dialog->setLayout(layout);
    dialog->resize(800, 600);  // Définir la taille initiale du dialogue

    // Afficher le dialogue
    dialog->show();

    // Assurer la suppression du dialogue après sa fermeture
    connect(dialog, &QDialog::finished, dialog, &QDialog::deleteLater);  // Nettoyage du dialogue lorsqu'il est terminé
}

void MainWindow::on_exporterPDF_clicked()
{
    // Retrieve vehicle information
    QString nom_proprietaire = ui->lineEditNomP->text().replace(" ", "_");  // Replace spaces with underscores
    QString marque = ui->lineEditMarque->text();
    QString modele = ui->lineEditModele->text();
    QString anneeStr = ui->lineEditAnnee->text();
    QString immatriculation = ui->lineEditImmatriculation->text();
    QString type = ui->lineEditType->text();
    QString etat = ui->lineEditEtat->text();
    QString historique_entretiens = ui->lineEditHE->text();

    // Create paragraph content
    QString paragraphe = QString("La voiture du monsieur/madame %1, de la marque %2, modèle %3.\n\n "
                                 "année %4, avec l'immatriculation %5, est un véhicule de type %6.\n\n"
                                 "L'état actuel du véhicule est %7. Son historique des entretiens est le suivant : %8.\n\n")
                                 .arg(nom_proprietaire)
                                 .arg(marque)
                                 .arg(modele)
                                 .arg(anneeStr)
                                 .arg(immatriculation)
                                 .arg(type)
                                 .arg(etat)
                                 .arg(historique_entretiens);

    // Generate a unique filename using the owner's name
    QString filePath = QString("C:/Users/zussl/Desktop/RR/vehicule_info_%1.pdf").arg(nom_proprietaire);

    // Set up PDF writer
    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMargins(30, 30, 30, 30));

    // Initialize QPainter for PDF
    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::critical(this, tr("Erreur PDF"), tr("Impossible de créer le fichier PDF."), QMessageBox::Ok);
        return;
    }

    // Set offset (translate origin)
    int offsetX = 20;  // Horizontal offset for title
    int offsetY = 100;  // Vertical offset from the top of the page

    // Title style: center, red color, reasonable font size
    QFont fontTitle("Agency FB", 50, QFont::Bold);  // Reduced font size from 200 to 20
    painter.setFont(fontTitle);
    painter.setPen(QColor("#B81F25"));

    // Calculate the width of the title to center it
    QString title = "DEEAILS DU VEHICULE";
    int textWidth = painter.fontMetrics().horizontalAdvance(title);

    // Set the horizontal position to center the title
    offsetX = (writer.width() - textWidth) / 2;  // Centering the title horizontally

    // Draw the title at the calculated position
    painter.drawText(offsetX, offsetY, title);

    // Paragraph text style
    QFont fontText("Agency FB", 20);
    painter.setFont(fontText);
    painter.setPen(Qt::black);

    // Apply offset to paragraph text
    offsetY += 5000;  // Starting position after title, with additional space for better separation

    // Split paragraph into lines and calculate the total width of each line to center them
    QStringList lines = paragraphe.split("\n");
    int lineHeight = 200;  // Increased line spacing for readability

    foreach (const QString& line, lines) {
        // Calculate the width of each line
        int textWidth = painter.fontMetrics().horizontalAdvance(line);

        // Calculate the x position to center the text
        int xPos = (writer.width() - textWidth) / 2;  // Centering the text

        // Draw the text at the calculated position
        painter.drawText(xPos, offsetY, line);

        // Update the vertical position for the next line
        offsetY += lineHeight;
    }

    // Signature placeholder in the bottom right
    QFont fontSignature("Agency FB", 10);
    fontSignature.setItalic(true);
    painter.setFont(fontSignature);
    painter.drawText(writer.width() - 200, writer.height() - 50, "Signature:");

    // Add logo at the bottom left
    // Add logo at the bottom left
    QImage logo("C:/Users/zussl/Desktop/photo/AUTACAREloding.png");  // Load the logo image
    if (!logo.isNull()) {  // Check if the image loaded correctly
        // Scale the logo by 400%
        int scaledWidth = logo.width() * 5;
        int scaledHeight = logo.height() * 5;
        QImage scaledLogo = logo.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Position at the bottom left with a margin
        int logoX = 30;  // Left margin for the logo
        int logoY = writer.height() - scaledHeight - 30;  // Bottom margin

        // Draw the scaled logo
        painter.drawImage(logoX, logoY, scaledLogo);
    } else {
        QMessageBox::warning(this, tr("Logo Warning"), tr("Le logo n'a pas pu être chargé."), QMessageBox::Ok);
    }

    painter.end();

    // Success message
    QMessageBox::information(this, tr("Exportation réussie"),
                             tr("Le fichier PDF a été exporté avec succès sous le nom: %1").arg(filePath),
                             QMessageBox::Ok);
}

void MainWindow::on_pushButtonGenererQR_clicked()
{
    // Get information to encode
    QString nom_proprietaire = ui->lineEditNomP->text().replace(" ", "_");
    QString marque = ui->lineEditMarque->text();
    QString modele = ui->lineEditModele->text();
    QString anneeStr = ui->lineEditAnnee->text();
    QString immatriculation = ui->lineEditImmatriculation->text();
    QString type = ui->lineEditType->text();
    QString etat = ui->lineEditEtat->text();
    QString historique_entretiens = ui->lineEditHE->text();

    // Create the text to encode into the QR code
    QString paragraphe = QString("La voiture du monsieur/madame %1, de la marque %2, modèle %3.\n\n "
                                 "année %4, avec l'immatriculation %5, est un véhicule de type %6.\n\n"
                                 "L'état actuel du véhicule est %7. Son historique des entretiens est le suivant : %8.\n\n")
                                 .arg(nom_proprietaire)
                                 .arg(marque)
                                 .arg(modele)
                                 .arg(anneeStr)
                                 .arg(immatriculation)
                                 .arg(type)
                                 .arg(etat)
                                 .arg(historique_entretiens);

    // Create QR code generator
    QrCodeGenerator generator;

    // Generate the QR code from the text
    QImage qrCodeImage = generator.generateQr(paragraphe);

    // Define the file path to save the image
    QString filePath = QString("C:/Users/zussl/Desktop/RR/qrcode_%1.png").arg(nom_proprietaire);

    // Save the QR code as PNG
    bool success = qrCodeImage.save(filePath);

    // Show appropriate message based on success or failure
    if (success) {
        QMessageBox::information(this, tr("QR Code Généré"),
                                 tr("Le QR code a été généré et enregistré avec succès sous : %1").arg(filePath));
    } else {
        QMessageBox::critical(this, tr("Erreur de génération du QR Code"),
                              tr("La génération du QR code a échoué. Veuillez réessayer."),
                              QMessageBox::Cancel);
    }

    // Optionally, show the QR code in a QLabel for preview

}

void MainWindow::on_mail_clicked()
{
    // Récupération des informations de l'interface utilisateur
    QString nom_proprietaire = ui->lineEditNomP->text().replace(" ", "_");
    QString marque = ui->lineEditMarque->text();
    QString modele = ui->lineEditModele->text();
    QString anneeStr = ui->lineEditAnnee->text();
    QString immatriculation = ui->lineEditImmatriculation->text();
    QString type = ui->lineEditType->text();
    QString etat = ui->lineEditEtat->text();
    QString historique_entretiens = ui->lineEditHE->text();
    QString email = ui->email->text();

    // Vérification des champs
    if (nom_proprietaire.isEmpty() || marque.isEmpty() || modele.isEmpty() ||
        anneeStr.isEmpty() || immatriculation.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis pour envoyer un email.");
        return;
    }

    // Commande pour lancer le script Python
    QString pythonPath = "python"; // Assurez-vous que Python est dans le PATH du système ou utilisez le chemin complet
    QString scriptPath = "C:/Users/zussl/Documents/crudvehicule/mailing.py"; // Remplacez par le chemin vers votre script

    // Arguments à passer au script Python
    QStringList arguments;
    arguments << nom_proprietaire
              << marque
              << modele
              << anneeStr
              << immatriculation
              << etat
              << historique_entretiens
              << email;

    // Exécution du script Python
    QProcess process;
    process.start(pythonPath, QStringList() << scriptPath << arguments);

    // Attendre la fin de l'exécution
    if (!process.waitForFinished()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'envoi de l'email.");
    } else {
        QMessageBox::information(this, "Email", "L'email a été envoyé avec succès !");
    }
}

void MainWindow::on_triparNomProprietaire_clicked()
{
    ui->tableView->setModel(Vtmp.afficher(QString("nom_proprietaire")));
}

void MainWindow::on_scann_clicked()
{
    // Chemins vers Python et les scripts
    QString pythonPath = "C:/Program Files/Python310/python.exe"; // Chemin vers Python
    QString scriptPathExtract = "C:/Users/zussl/Desktop/projet/mou7awla2.py"; // Script pour extraire la matricule
    QString scriptPathMail = "C:/Users/zussl/Documents/crudvehicule/mailing.py"; // Script pour envoyer des emails

    QProcess processExtract;
    QStringList argsExtract;
    processExtract.start(pythonPath, QStringList() << scriptPathExtract);

    // Attendre que le script d'extraction démarre
    if (!processExtract.waitForStarted() || !processExtract.waitForFinished()) {
        QMessageBox::warning(this, "Erreur", "Le script d'extraction n'a pas pu être exécuté.");
        return;
    }

    // Lire la sortie du script d'extraction
    QString outputExtract = processExtract.readAllStandardOutput().trimmed();
    qDebug() << "Output extract:" << outputExtract;

    // Extraction de la matricule via expression régulière
    QRegularExpression regex("([0-9]{3}[tT][uU][nN][0-9]{4})"); // Format attendu : 3 chiffres, TUN, 4 chiffres
    QRegularExpressionMatch match = regex.match(outputExtract);

    if (!match.hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Aucune matricule valide détectée.");
        return;
    }

    QString licensePlate = match.captured(1); // Matricule extraite
    ui->labellepainture->setText(licensePlate); // Afficher la matricule détectée
    QSqlQuery query;

    // Rechercher la voiture dans la base de données
    query.prepare("SELECT * FROM vehicule WHERE immatriculation = :licensePlate");
    query.bindValue(":licensePlate", licensePlate);

    if (query.exec() && query.next()) {
        // Matricule trouvée
        QString ownerName = query.value("proprietaire").toString();
        QString brand = query.value("marque").toString();
        QString model = query.value("modele").toString();
        QString year = query.value("annee").toString();
        QString email = query.value("email").toString();

        // Mettre à jour l'état à "Terminé"
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE vehicule SET etat = 'Terminé' WHERE immatriculation = :licensePlate");
        updateQuery.bindValue(":licensePlate", licensePlate);

        if (updateQuery.exec()) {
            QMessageBox::information(this, "Mise à jour réussie", "L'état de la voiture a été mis à jour à 'Terminé'.");

            // Construire le message pour le propriétaire
            QString message = QString(
                "Bonjour %1,\n\n"
                "Votre voiture de marque %2, modèle %3, année %4, immatriculée %5, est maintenant prête.\n"
                "Vous pouvez venir la récupérer.\n\n"
                "Merci pour votre confiance."
            ).arg(ownerName, brand, model, year, licensePlate);

            // Construire les arguments pour le script Python d'envoi d'email
            QStringList argsMail;
            argsMail << ownerName << brand << model << year << licensePlate << "Terminer" << message << email;

            // Exécuter le script Python pour envoyer l'email au propriétaire
            int exitCodeMail = QProcess::execute(pythonPath, QStringList() << scriptPathMail << argsMail);
            if (exitCodeMail == 0) {
                QMessageBox::information(this, "Email envoyé", "Un email a été envoyé au propriétaire.");
            } else {
                QMessageBox::warning(this, "Erreur", "Erreur lors de l'envoi de l'email au propriétaire.");
            }
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la mise à jour de l'état.");
        }
    } else {
        // Matricule non trouvée, envoyer un email clair à l'administrateur
        QString adminEmail = "zusslimani001122@gmail.com";

        // Construire le message à envoyer à l'administrateur
        QString message = QString(
            "Attention\n\n"
            "Bonjour Monsieur Hamza,\n\n"
            "Il y a une voiture sous la matricule %1 dans votre parking."
        ).arg(licensePlate);

        // Construire les arguments pour le script Python d'envoi d'email
        QStringList argsMail;
        argsMail << "Hamza" << "inconnu" << "inconnu" << "inconnu" << licensePlate << "Voiture Anonyme" << message << adminEmail;

        // Exécuter le script Python pour envoyer l'email à l'administrateur
        int exitCodeMail = QProcess::execute(pythonPath, QStringList() << scriptPathMail << argsMail);
        if (exitCodeMail == 0) {
            QMessageBox::information(this, "Email envoyé", "Un email a été envoyé à l'administrateur.");
        } else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de l'envoi de l'email à l'administrateur.");
        }
    }
}

//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------naama------------------------------------------------------------
void MainWindow::on_ajouter_clicked()
{
    bool ok;
    int id = ui->idtech->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("L'ID doit être un nombre entier."),
                             QMessageBox::Ok);
        return;
    }

    // Expression régulière pour les lettres uniquement
    QRegularExpression lettersRegex("^[A-Za-zÀ-ÿ]+$");

    QString nom = ui->nomtech->text();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Nom ne peut pas être vide."),
                             QMessageBox::Ok);
        return;
    } else if(!lettersRegex.match(nom).hasMatch()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Nom doit contenir uniquement des lettres."),
                             QMessageBox::Ok);
        return;
    }

    QString prenom = ui->pretech->text();
    if (prenom.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Prénom ne peut pas être vide."),
                             QMessageBox::Ok);
        return;
    } else if(!lettersRegex.match(prenom).hasMatch()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Prénom doit contenir uniquement des lettres."),
                             QMessageBox::Ok);
        return;
    }

    QString competence = ui->competence->text();
    if (competence.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Compétence ne peut pas être vide."),
                             QMessageBox::Ok);
        return;
    } else if(!lettersRegex.match(competence).hasMatch()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Compétence doit contenir uniquement des lettres."),
                             QMessageBox::Ok);
        return;
    }

    QString disponibilite = ui->dispotech->text();
    if (disponibilite.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Disponibilité ne peut pas être vide."),
                             QMessageBox::Ok);
        return;
    } else if(!lettersRegex.match(disponibilite).hasMatch()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Disponibilité doit contenir uniquement des lettres."),
                             QMessageBox::Ok);
        return;
    }

    QString phoneNumber = ui->tel->text();
    if (phoneNumber.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ N°téléphone ne peut pas être vide."),
                             QMessageBox::Ok);
        return;
    }

    QRegularExpression phoneRegex("^[0-9]{8}$");
    QRegularExpressionMatch phoneMatch = phoneRegex.match(phoneNumber);

    if (!phoneMatch.hasMatch()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ N°téléphone doit contenir exactement 8 chiffres."),
                             QMessageBox::Ok);
        return;
    }

    QString absent = ui->absence->text();
    if (absent.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Absent ne peut pas être vide."),
                             QMessageBox::Ok);
        return;
    }

    if (absent.compare("oui", Qt::CaseInsensitive) != 0 &&
        absent.compare("non", Qt::CaseInsensitive) != 0) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Absent doit contenir 'oui' ou 'non'."),
                             QMessageBox::Ok);
        return;
    }

    QString adresse = ui->adresse->text().trimmed();

        // Vérification de base
        if (adresse.isEmpty()) {
            QMessageBox::warning(this, tr("Erreur de saisie"),
                                tr("Le champ Adresse ne peut pas être vide."),
                                QMessageBox::Ok);

            return;
        }



    technicien t(id, nom, prenom, competence, disponibilite, phoneNumber, absent, adresse);
    bool test = t.ajouter();

    if (test) {
        QMessageBox::information(this, tr("OK"),
                                 tr("Ajout effectué\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(this, tr("NOT OK"),
                                 tr("Ajout non effectué\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    }
}

void MainWindow::on_supprimer_clicked()
{
    int id = ui->idtechsupp->text().toInt();
    bool test = tech.supprimer(id);

    if (test) {
        QMessageBox::information(nullptr,QObject::tr("OK"),
                                QObject::tr("Suppression effectué\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(this, tr("NOT OK"),
                                 tr("Suppression non effectué\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    }
}

void MainWindow::on_miseajour_clicked()
{
    int id = ui->idtech2->text().toInt(); // Updated to match XML
       QString nom = ui->nomtech2->text();
       QString prenom = ui->pretech2->text();
       QString competence = ui->competence2->text();
       QString disponabilite = ui->dispotech2->text();
       QString phoneNumber = ui->tel2->text();
       QString absent = ui->absence2->text();
       QString adresse = ui->adresse2->text();




       technicien t(id, nom, prenom, competence, disponabilite, phoneNumber, absent  ,adresse);
       bool test = t.mettreAJour(id); // Assuming mettreAJour is defined correctly

       if (test) {
           QMessageBox::information(this, tr("Sauvegarde réussie"),
                                    tr("Les informations du technicien ont été mises à jour."));
       } else {
           QMessageBox::critical(this, tr("Erreur"),
                                 tr("La mise à jour des informations du technicien a échoué."),
                                 QMessageBox::Cancel);
       }

}



void MainWindow::on_exportExcel_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Enregistrer le fichier Excel",
        "",
        "Fichier Excel (*.csv)");

    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier.");
        return;
    }

    QTextStream out(&file);
    // Écrire le BOM UTF-8 si nécessaire
    out.setEncoding(QStringConverter::Utf8);
    out << "\xEF\xBB\xBF"; // BOM UTF-8 pour Excel

    auto escapeCSV = [](const QString& str) -> QString {
        QString result = str;
        result.replace(";", ",");
        // Utiliser QRegularExpression à la place de QRegExp
        if (result.contains(QRegularExpression("[,\"\n\r]"))) {
            result.replace("\"", "\"\""); // Échapper les guillemets
            result = "\"" + result + "\""; // Entourer la chaîne de guillemets
        }
        return result;
    };

    out << "ID;Nom;Prénom;Compétence;Disponibilité;Téléphone;Absent;Adresse\n";

    QSqlQuery query;
    query.prepare("SELECT * "
                  "FROM TECHNICEN");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur",
            "Erreur lors de la récupération des données des techniciens");
        return;
    }

    while (query.next()) {
        QStringList row;
        for (int i = 0; i < 8; ++i) { // Ajusté pour correspondre à vos colonnes
            row << escapeCSV(query.value(i).toString());
        }
        out << row.join(";") << "\n";
    }

    file.close();

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Export terminé",
        "Les données ont été exportées avec succès. Voulez-vous ouvrir le fichier ?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    }
}




void MainWindow::on_recherche_clicked()
{
    // Get the ID entered by the user
    int id = ui->idtech2->text().toInt();

    // Search for the student using findById function
    technicien t = t.findById(id);

    // Check if the student was found
    if (t.getID() != -1) {
        // Display student details in the line edits
        ui->idtech2->setText(QString::number(t.getID()));
        ui->nomtech2->setText(t.getNom());
        ui->pretech2->setText(t.getPrenom());
        ui->competence2->setText(t.getcompetence());
        ui->dispotech2->setText(t.getdisponibilite());
        ui->tel2->setText(t.getphoneNumber());
        ui->absence2->setText(t.getabsent());
        ui->adresse2->setText(t.getadresse());


        // Show a message indicating that the technicien was found
        QMessageBox::information(this, tr("technicien trouvé"),
                                 tr("technicien trouvé."));
    } else {
        // Show an error message if the technicien is not found
        QMessageBox::critical(this, tr("Erreur"),
                              tr("technicien introuvable.\nVeuillez vérifier l'ID."),
                              QMessageBox::Cancel);

}}

void MainWindow::on_afficher_clicked()
{
    ui->tableView_2->setModel(tech.afficher());
}

void MainWindow::on_trier_clicked() {

    ui->tableView_2->setModel(tech.trier("id", "ASC"));
}

void MainWindow::on_trier_nom_clicked()
{
    ui->tableView_2->setModel(tech.trier("NOM_T", "ASC"));
}

void MainWindow::on_trier_absence_clicked()
{
    ui->tableView_2->setModel(tech.trier("absent", "ASC"));
}

void MainWindow::on_statistique_clicked() {
    // 1. Obtenez les données de statistiques pour la compétence
    QSqlQueryModel* model = tech.obtenirStatistiquesCompetence();

    // Vérifiez que la requête a réussi et que le modèle a des données
    if (model && model->rowCount() > 0) {
        // Créez un QDialog pour afficher le graphique en secteurs
        QDialog *chartDialog = new QDialog(this);
        chartDialog->setWindowTitle("Statistiques des Compétences");
        chartDialog->resize(800, 600); // Ajustez la taille selon vos besoins

        // Créez un QVBoxLayout pour organiser le graphique dans le QDialog
        QVBoxLayout *dialogLayout = new QVBoxLayout(chartDialog);

        // 2. Créez un QPieSeries pour le graphique en secteurs
        QPieSeries *pieSeries = new QPieSeries();

        // Calculer le total des valeurs pour les pourcentages
        int total = 0;
        for (int row = 0; row < model->rowCount(); ++row) {
            total += model->data(model->index(row, 1)).toInt();
        }

        // Ajoutez des données au QPieSeries avec pourcentages
        for (int row = 0; row < model->rowCount(); ++row) {
            QString competence = model->data(model->index(row, 0)).toString();
            int count = model->data(model->index(row, 1)).toInt();
            double percentage = static_cast<double>(count) / total * 100;

            // Créez une tranche avec un label affichant le pourcentage
            QPieSlice *slice = pieSeries->append(
                competence + QString(" (%1%)").arg(percentage, 0, 'f', 1),
                count
            );

            // Vous pouvez personnaliser chaque tranche ici si nécessaire (couleur, etc.)
        }

        // Créez le graphique pour le graphique en secteurs
        QChart *pieChart = new QChart();
        pieChart->addSeries(pieSeries);
        pieChart->setTitle("Répartition des compétences");
        pieChart->legend()->setAlignment(Qt::AlignRight);

        // Créez un QChartView pour afficher le graphique en secteurs
        QChartView *pieChartView = new QChartView(pieChart);
        pieChartView->setRenderHint(QPainter::Antialiasing);

        // Ajoutez le QChartView au layout du QDialog
        dialogLayout->addWidget(pieChartView);

        // Affichez le QDialog
        chartDialog->setLayout(dialogLayout);
        chartDialog->exec();
    } else {
        QMessageBox::warning(this, tr("Erreur"), tr("Échec de la récupération des statistiques pour le graphique."));
    }
}

void MainWindow::on_conversionenpdf_clicked()
{
    int id = ui->idtech->text().toInt();  // Récupère l'ID du technicien à partir de l'interface
    technicien t = tech.findById(id);      // Trouve le technicien correspondant à l'ID

    if (t.getID() == -1) {
        QMessageBox::warning(this, tr("Erreur"), tr("Technicien introuvable. Veuillez vérifier l'ID."));
        return;
    }

    QString nomFichier = QFileDialog::getSaveFileName(this, tr("Enregistrer en PDF"), "", "*.pdf");

    if (!nomFichier.isEmpty()) {
        if (t.convertirEnPDF(nomFichier)) {
            QMessageBox::information(this, tr("Succès"), tr("PDF créé avec succès."));
        } else {
            QMessageBox::critical(this, tr("Erreur"), tr("La création du PDF a échoué."));
        }
    }

}

PieChartWidget::PieChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(400, 400);  // Set a fixed size for the widget (adjust as needed)
}

void PieChartWidget::setData(const QVector<QPair<QString, int>> &data)
{
    chartData = data;
    update();  // Trigger repaint when data changes
}

void PieChartWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Total sum of the data for percentage calculations
    int total = 0;
    for (const auto &item : chartData) {
        total += item.second;
    }

    // Draw the pie chart
    int startAngle = 0;
    QRectF rect(50, 50, 300, 300);  // Rect for the pie chart
    for (const auto &item : chartData) {
        int spanAngle = (item.second * 360) / total;  // Convert value to angle for each slice

        QColor color;
        if (item.first == "Category 1") color = Qt::red;
        else if (item.first == "Category 2") color = Qt::blue;
        else color = Qt::green;

        painter.setBrush(color);
        painter.drawPie(rect, startAngle * 16, spanAngle * 16);

        startAngle += spanAngle;
    }

    // Optionally, you can draw labels here
    startAngle = 0;
    for (const auto &item : chartData) {
        int spanAngle = (item.second * 360) / total;
        QPointF labelPoint = rect.center();
        labelPoint.setX(labelPoint.x() + 150 * qCos((startAngle + spanAngle / 2) * M_PI / 180));
        labelPoint.setY(labelPoint.y() + 150 * qSin((startAngle + spanAngle / 2) * M_PI / 180));

        painter.drawText(labelPoint, item.first + ": " + QString::number(item.second));

        startAngle += spanAngle;
    }
}

void MainWindow::on_suivie_des_absences_clicked()
{
    // Retrieve the model with absent technicians
        QSqlQueryModel *model = tech.getAbsentTechniciens();

        // Set the model to the table view
        ui->tableabsence->setModel(model);
}

void MainWindow::on_notification_clicked()
{
        const QString ACCOUNT_SID = "AC2e8aa8918cc88b8a47705249e8307ac3";
        const QString AUTH_TOKEN = "b53accf8ffcfcb4d7d4417afda122d88";
        const QString FROM_NUMBER = "+14159664006";  // Votre numéro Twilio
        const QString TO_NUMBER = "+21655536568";    // Numéro qui recevra les SMS

        // Création du manager
        QNetworkAccessManager* manager = new QNetworkAccessManager(this);

        // Récupération des techniciens absents
        QSqlQuery query;
        query.prepare("SELECT * FROM TECHNICEN WHERE ABSENT = 'oui'");

        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur",
                "Erreur lors de la récupération des techniciens absents");
            manager->deleteLater();
            return;
        }

        //bool messageSent = false;

        while (query.next()) {
            QString nom = query.value(0).toString();
            QString prenom = query.value(1).toString();

            // Configuration de la requête
            QNetworkRequest request;
            request.setUrl(QUrl("http://api.twilio.com/2010-04-01/Accounts/" + ACCOUNT_SID + "/Messages.json"));


            // Authentification
            QString credentials = ACCOUNT_SID + ":" + AUTH_TOKEN;
            QByteArray auth = "Basic " + credentials.toUtf8().toBase64();
            request.setRawHeader("Authorization", auth);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

            // Préparation du message
            QUrlQuery postData;
            postData.addQueryItem("From", FROM_NUMBER);
            postData.addQueryItem("To", TO_NUMBER);
            postData.addQueryItem("Body", QString("Bonjour %1 %2, vous êtes marqué(e) comme absent(e).")
                                        .arg(prenom)
                                        .arg(nom));

            // Debug information
            qDebug() << "De (Twilio):" << FROM_NUMBER;
            qDebug() << "Vers:" << TO_NUMBER;
            qDebug() << "Message:" << postData.toString();

            // Envoi de la requête
            QNetworkReply* reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

            // Connexion des signaux pour ce reply

            connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                QByteArray response = reply->readAll();
                if (reply->error() == QNetworkReply::NoError) {
                    qDebug() << "Message envoyé avec succès. Réponse Twilio:" << response;

                    // Log success
                    QMessageBox::information(this, "Succès", "Notification envoyée avec succès !\nRéponse Twilio: " + response);
                } else {
                    qDebug() << "Erreur d'envoi:" << reply->errorString();
                    qDebug() << "Réponse Twilio:" << response;

                    // Log error
                    QMessageBox::warning(this, "Erreur", "Échec de l'envoi de la notification:\n" + reply->errorString() + "\nRéponse Twilio: " + response);
                }
                reply->deleteLater();
        });
    }
}

void MainWindow::on_modeButton_clicked()
{
    // Inverser l'état du mode (jour/nuit)
    isNightMode = !isNightMode;

    // Appliquer le style en fonction du mode
    if (isNightMode) {
        qApp->setStyleSheet(
                    "QWidget { background-color: black;color: white;}"
                                "QPushButton { background-color: #333; color: white; border: 1px solid #555; }"
                                "QLineEdit { background-color: #444; color: white; border: 1px solid #666; }"
                                "QTableWidget { background-color: black; color: white; }"
                                "QTableWidget::item { border-bottom: 1px solid #555; }"
                                "QHeaderView::section { background-color: #333; border: 1px solid #555; }" // Header with dark background but keeping text readable
        );
    } else {
        qApp->setStyleSheet(
                    "QWidget { background-color: white;color: black; }"
                                "QPushButton { background-color: #f0f0f0; color: black; border: 1px solid #ccc; }"
                                "QLineEdit { background-color: #fff; color: black; border: 1px solid #ccc; }"
                                "QTableWidget { background-color: white; color: black; }"
                                "QTableWidget::item { border-bottom: 1px solid #ccc; }"
                                "QHeaderView::section { background-color: #f0f0f0; border: 1px solid #ccc; }" // Header with light background
        );
    }
}

void MainWindow::on_afficherCarte_clicked()
{

    bool ok;
    QString idText = ui->idtechrecherche_2->text().trimmed();

    // Vérification plus stricte de l'ID
    if (idText.isEmpty()) {
        QMessageBox::warning(this,
            "Erreur de saisie",
            "Veuillez saisir un ID de technicien.");
        ui->idtechrecherche_2->setFocus();
        return;
    }

    // Conversion sécurisée de l'ID en entier
    int idTech = idText.toInt(&ok);
    if (!ok || idTech <= 0) {
        QMessageBox::warning(this,
            "Erreur de saisie",
            "L'ID doit être un nombre entier positif valide.");
        ui->idtechrecherche_2->setFocus();
        ui->idtechrecherche_2->selectAll();
        return;
    }

    // Curseur d'attente pendant la recherche
    QApplication::setOverrideCursor(Qt::WaitCursor);

    try {
        // Recherche du technicien
        technicien tech = tech.findById(idTech);

        // Restaurer le curseur
        QApplication::restoreOverrideCursor();

        // Vérification des informations du technicien
        if (tech.getNom().isEmpty()) {
            QMessageBox::warning(this,
                "Technicien introuvable",
                QString("Aucun technicien trouvé avec l'ID %1").arg(QString::number(idTech)));
            return;
        }

        // Récupération et validation de l'adresse
        QString adresse = tech.getadresse().trimmed();
        if (adresse.isEmpty()) {
            QMessageBox::warning(this,
                "Données manquantes",
                QString("Le technicien %1 %2 (ID: %3) n'a pas d'adresse enregistrée.")
                    .arg(tech.getNom())
                    .arg(tech.getPrenom())
                    .arg(QString::number(idTech)));
            return;
        }

        // Construction de l'URL en évitant l'ambiguïté
        QString encodedAddress = QUrl::toPercentEncoding(adresse);
        QString urlString = "https://www.google.com/maps/search/?api=1&query=" + encodedAddress;
        QUrl url(urlString);

        if (!url.isValid()) {
            QMessageBox::critical(this,
                "Erreur",
                "Impossible de construire l'URL avec l'adresse fournie.");
            return;
        }

        // Demander confirmation avant d'ouvrir le navigateur
        QString confirmMessage = QString("Voulez-vous afficher l'adresse de %1 %2 sur Google Maps ?\n\nAdresse : %3")
            .arg(tech.getNom())
            .arg(tech.getPrenom())
            .arg(adresse);

        QMessageBox::StandardButton reply = QMessageBox::question(this,
            "Confirmation",
            confirmMessage,
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Tentative d'ouverture du navigateur
            if (!QDesktopServices::openUrl(url)) {
                QMessageBox::critical(this,
                    "Erreur",
                    "Impossible d'ouvrir le navigateur. Veuillez vérifier votre configuration système.");
            }
        }
    }
    catch (const std::exception& e) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this,
            "Erreur système",
            QString("Une erreur est survenue lors de la recherche : %1").arg(QString::fromUtf8(e.what())));
    }
}



void MainWindow::on_recherchenom_clicked()
{
    // Récupérer le nom entré par l'utilisateur
        QString nom = ui->nomch->text().trimmed();

        if (nom.isEmpty()) {
            QMessageBox::warning(this, tr("Erreur"), tr("Veuillez entrer un nom."));
            return;
        }

        // Préparer la requête pour la recherche
        QSqlQuery query;
        query.prepare("SELECT * FROM TECHNICEN WHERE NOM_T = :nom");
        query.bindValue(":nom", nom);

        if (!query.exec()) {
            QMessageBox::critical(this, tr("Erreur"), tr("Échec de la recherche: ") + query.lastError().text());
            return;
        }

        // Configurer le modèle pour le QTableView
        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(query);

        // Définir les en-têtes des colonnes
        model->setHeaderData(0, Qt::Horizontal, tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, tr("Prénom"));
        model->setHeaderData(3, Qt::Horizontal, tr("Compétence"));
        model->setHeaderData(4, Qt::Horizontal, tr("Disponibilité"));
        model->setHeaderData(5, Qt::Horizontal, tr("Téléphone"));
        model->setHeaderData(6, Qt::Horizontal, tr("Absent"));
        model->setHeaderData(7, Qt::Horizontal, tr("Adresse"));

        // Appliquer le modèle au QTableView
        ui->tableView_2->setModel(model);
        ui->tableView_2->resizeColumnsToContents();

        // Vérifier s'il y a des résultats
        if (model->rowCount() == 0) {
            QMessageBox::information(this, tr("Aucun résultat"), tr("Aucun technicien trouvé avec ce nom."));
        }

}

void MainWindow::on_recherchecompetence_clicked()
{
    // Récupérer la compétence entrée par l'utilisateur
        QString competence = ui->competencech->text().trimmed();

        if (competence.isEmpty()) {
            QMessageBox::warning(this, tr("Erreur"), tr("Veuillez entrer une compétence."));
            return;
        }

        // Préparer la requête pour la recherche
        QSqlQuery query;
        query.prepare("SELECT * FROM TECHNICEN WHERE COMPETENCES = :competence");
        query.bindValue(":competence", competence);

        if (!query.exec()) {
            QMessageBox::critical(this, tr("Erreur"), tr("Échec de la recherche: ") + query.lastError().text());
            return;
        }

        // Configurer le modèle pour le QTableView
        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(query);

        // Définir les en-têtes des colonnes
        model->setHeaderData(0, Qt::Horizontal, tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, tr("Prénom"));
        model->setHeaderData(3, Qt::Horizontal, tr("Compétence"));
        model->setHeaderData(4, Qt::Horizontal, tr("Disponibilité"));
        model->setHeaderData(5, Qt::Horizontal, tr("Téléphone"));
        model->setHeaderData(6, Qt::Horizontal, tr("Absent"));
        model->setHeaderData(7, Qt::Horizontal, tr("Adresse"));

        // Appliquer le modèle au QTableView
        ui->tableView_2->setModel(model);
        ui->tableView_2->resizeColumnsToContents();

        // Vérifier s'il y a des résultats
        if (model->rowCount() == 0) {
            QMessageBox::information(this, tr("Aucun résultat"), tr("Aucun technicien trouvé avec cette compétence."));
        }

}


//-----------------------------------------------------------------------------------------------------------------------------------------



void MainWindow::on_login_2_clicked()
{

    QString email = ui->email_2->text();
    QString motDePasse = ui->mdp->text();

    if (email.isEmpty() || motDePasse.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    Connection* conn = Connection::getInstance();

       // Check if the connection is successful
       if (!conn->createconnect()) {
           QMessageBox::critical(this, "Database Error", "Connection failed!");
           return;
       }

       // Authenticate the user
       if (conn->authenticateUser(email, motDePasse)) {
           QMessageBox::information(this, "Login", "Login Successful");
           ui->stackedWidget->setCurrentIndex(1);
           // Proceed with login (you can navigate to another window or update UI)
       } else {
           QMessageBox::critical(this, "Login", "Invalid Credentials");
       }

       // Close the connection when done
       //conn->closeConnection();
}


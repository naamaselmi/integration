#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "vehicule.h"
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

}

MainWindow::~MainWindow()
{
    delete ui;
}



 // Include QRegularExpression header
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

    QString marque = ui->lineEditMarque->text();
    QString modele = ui->lineEditModele->text();
    QString type = ui->lineEditType->text();
    QString etat = ui->lineEditEtat->text();
    QString email = ui->email->text();

    // Email validation using a regular expression for a basic email format
    QRegularExpression emailRegex("^[a-zA-Z0-9_+&*-]+(?:\\.[a-zA-Z0-9_+&*-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,7}$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("L'email n'est pas valide.\nVeuillez entrer un email au format correct (exemple@example.com)."), QMessageBox::Ok);
        return; // Exit the function if email format is invalid
    }

    // Validation for the owner's name: only letters allowed
    QString nom_proprietaire = ui->lineEditNomP->text();
    QRegularExpression nameRegex("^[A-Za-z]+$"); // Only letters (upper and lower case)
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



//-----------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------
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
    QImage logo("C:/Users/zussl/Desktop/AUTACAREloding.png");  // Load the logo image
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
    // Utilisez le chemin confirmé vers outlook.exe
    QString outlookPath = "C:\\Program Files\\Microsoft Office\\root\\Office16\\outlook.exe";

    // Vérification si le fichier existe
    if (!QFile::exists(outlookPath)) {
        QMessageBox::warning(this, "Erreur", "Le chemin vers Outlook est incorrect ou Outlook n'est pas installé à cet emplacement.");
        return;
    }

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

    // Construction du corps de l'email
    QString body = QString("Bonjour %1,\n\n"
                           "Votre voiture de marque %2, modèle %3, année %4, immatriculée %5 est actuellement dans l'état suivant : %6.\n\n"
                           "Voici l'historique des entretiens effectués : %7.\n\n"
                           "Merci de votre confiance.\n")
                           .arg(nom_proprietaire)
                           .arg(marque)
                           .arg(modele)
                           .arg(anneeStr)
                           .arg(immatriculation)
                           .arg(etat)
                           .arg(historique_entretiens);

    // Détails de l'email
    QString to = email;
    QString subject = "Informations sur votre véhicule";
    QString bodyEncoded = QUrl::toPercentEncoding(body);

    // Construire l'URL mailto
    QString mailto = QString("mailto:%1?subject=%2&body=%3")
                                .arg(to)
                                .arg(QUrl::toPercentEncoding(subject))
                                .arg(bodyEncoded);

    // Lancer Outlook avec l'argument mailto
    QString command = "\"" + outlookPath + "\"";
    QProcess process;
    bool started = process.startDetached(command, QStringList() << mailto);

    // Vérification si le processus a démarré correctement
    if (!started) {
        QMessageBox::warning(this, "Erreur", "Outlook n'a pas pu être lancé.");
    } else {
        QMessageBox::information(this, "Email", "L'email a été ouvert dans Outlook !");
    }
}

void MainWindow::on_triparNomProprietaire_clicked()
{
    ui->tableView->setModel(Vtmp.afficher(QString("nom_proprietaire")));
}


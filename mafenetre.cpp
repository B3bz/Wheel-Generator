/****************************************************************************
**
** Copyright (C) 2016 ROZEROT Bertrand
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**
****************************************************************************/
#include "mafenetre.h"

void MaFenetre::lancer()
{
    QPixmap pxTest(*m_pathWeelTemplate); // import de la trame
    if(pxTest.isNull()) // si le fichier n'est pas lu par QPixmap :
    {
        QMessageBox::critical(this, windowTitle(), "Fichier " + *m_pathWeelTemplate + " illisible. Choisissez un nouveau fichier.");
        return;
    }
    if(m_listInput->isEmpty())
    {
        QMessageBox::critical(this, windowTitle(), "Vous devez sélectionner le dossier contenant les wheels au format png !");
        return;
    }
    if((!QDir(*m_pathDirWheelsOutput).exists()))
    {
        QMessageBox::critical(this, windowTitle(), "Vous devez sélectionner un dossier valide pour l'enregistrement des fichiers.");
        return;
    }
    //Total fichiers à traiter, fichiers traités pour la progressbar, largeur trame, hauteur trame, largeur png détouré, hauteur png détouré, point de mise en place sur la trame.
    int nbFichiersListes{m_listInput->size()}, nbFichiersCopies{0}, ya{pxTest.width()}, xa{pxTest.height()}, yb{0}, xb{0}, yc{0}, xc{0};
    //int progress bar
    m_progress->show();
    m_progress->setRange(0, nbFichiersListes);
    m_progress->setValue(nbFichiersCopies);
    QString errors, strTampon;
    //création du wheel
    QFile file;
    foreach(strTampon, *m_listInput)
    {
        //refresh de la fenêtre
        qApp->processEvents();
        QPixmap pxImageWheel(*m_pathDirWheelsInput + "/" + strTampon);
        QPixmap pxTemplate(*m_pathWeelTemplate);
        QPainter painter(&pxTemplate);
        yb = pxImageWheel.width(); // largeur png détouré
        xb = pxImageWheel.height(); // hauteur png détouré
        if(m_groupResize->isChecked()) // si fonction resize sur dépassement de taille image
        {
            if(yb > m_spinLLimit->value()) //si la largeur de l'image dépasse la taille autorisée
            {
                /* The returned image is scaled to the given width using the specified transformation mode.
                 * The height of the pixmap is automatically calculated so that the aspect ratio of the pixmap is preserved.*/
                pxImageWheel = pxImageWheel.scaledToWidth(m_spinLLimit->value(), Qt::SmoothTransformation); // The resulting image is transformed using bilinear filtering.
                yb = pxImageWheel.width(); // on récupère la nouvelle largeur de l'image
                xb = pxImageWheel.height(); // on récupère la nouvelle hauteur de l'image
            }
            if(xb > m_spinHLimit->value()) //si la hauteur de l'image dépasse la taille autorisée
            {
                pxImageWheel = pxImageWheel.scaledToHeight(m_spinHLimit->value(), Qt::SmoothTransformation);
                yb = pxImageWheel.width();
                xb = pxImageWheel.height();
            }
        }
        //on centre l'image dans le template:
        yc = (ya - yb)/2;
        xc = (xa - xb)/2;
        // si le fichier n'est pas lu par QPixmap :
        if(pxImageWheel.isNull())
        {
            errors.append(*m_pathDirWheelsInput + "/" + strTampon + "\n"); // on récupère le nom du fichier non traité
            painter.end();
        }
        else
        {
            painter.drawPixmap(yc, xc, pxImageWheel); // On applique l'image sur le template
            painter.end();
            m_previewWeelTemplate->setPixmap(pxTemplate); // copie de l'image dans le label preview
            file.setFileName(*m_pathDirWheelsOutput + "/" + strTampon);
            file.open(QIODevice::WriteOnly);
            pxTemplate.save(&file, "PNG"); // copie du fichier généré
            file.close();
            // on actualise la barre de progression
            nbFichiersCopies++;
            m_progress->setValue(nbFichiersCopies);
        }
    }
    if(!errors.isEmpty())
        QMessageBox::warning(this, windowTitle(), QString::number(nbFichiersListes) + " fichiers générés\n" + QString::number(nbFichiersListes - nbFichiersCopies) + " fichiers ignorés :\n " + errors);
    else
        QMessageBox::information(this, windowTitle(), QString::number(nbFichiersListes) + " fichiers générés");
    m_progress->setValue(nbFichiersListes);
}

void MaFenetre::selectTemplate()
{
    QString pathImage = QFileDialog::getOpenFileName(this, "Sélectionner une image", QString(), "Image (*.png)");//path du fichier
    QPixmap pxWheelTemplate(pathImage);//on tente une lecture
    if(pxWheelTemplate.isNull()) // si le fichier n'est pas lu par QPixmap :
    {
        QMessageBox::critical(this, windowTitle(), "Le fichier image " + pathImage + " est illisible.");
        return;
    }
    *m_pathWeelTemplate = pathImage;
    m_previewWeelTemplate->setPixmap(QPixmap(*m_pathWeelTemplate));
    m_labWeelTemplate->setText(pathImage);
}

void MaFenetre::selectDirWheelsInput()
{
    QString pathDir = QFileDialog::getExistingDirectory(this);
    *m_pathDirWheelsInput = pathDir;
    //création de la liste des wheels
    QDir dirInput(*m_pathDirWheelsInput);//QDir vers les wheels
    QStringList filters;
    filters << "*.png";
    m_listInput = new QStringList;
    *m_listInput = dirInput.entryList(filters);
    m_labInput->setText(pathDir);
}

void MaFenetre::selectDirWheelsOutput()
{
    QString pathDir = QFileDialog::getExistingDirectory(this);
    *m_pathDirWheelsOutput = pathDir;
    m_labOutput->setText(pathDir);
}

//création de la fenêtre
void MaFenetre::construireFenetre()
{
    setWindowIcon(QIcon(QPixmap(":wheel-icon.png")));
    setWindowTitle("Wheel generator 1.0.1");
    m_pathWeelTemplate = new QString;
    m_pathDirWheelsInput = new QString;
    m_pathDirWheelsOutput = new QString;
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    //1 VBox main_layout
    QVBoxLayout *main_layout = new QVBoxLayout();
    //1.1 HBox
    QHBoxLayout *sub_layout = new QHBoxLayout();
    //1.1.1   VBox
    //template
    QVBoxLayout *boutons_layout = new QVBoxLayout();
    m_pbSelectWeelTemplate = new QPushButton("1 - Sélectionner le wheel template");
    boutons_layout->addWidget(m_pbSelectWeelTemplate);
    m_labWeelTemplate = new QLabel(this);
    boutons_layout->addWidget(m_labWeelTemplate);
    //input dir
    m_pbSelectDirWheelsInput = new QPushButton("2 - Sélectionner le dossier contenant les png à appliquer");
    boutons_layout->addWidget(m_pbSelectDirWheelsInput);
    m_labInput = new QLabel(this);
    boutons_layout->addWidget(m_labInput);
    //output dir
    m_pbSelectDirWheelsOutput = new QPushButton("3 - Sélectionner le dossier où copier les wheels générés");
    boutons_layout->addWidget(m_pbSelectDirWheelsOutput);
    m_labOutput = new QLabel(this);
    boutons_layout->addWidget(m_labOutput);
    m_groupResize = new QGroupBox("Redimensionner le wheel détouré");
        m_groupResize->setCheckable(true);
        m_groupResize->setChecked(true);
    //1.1.1.1   VBox
    QVBoxLayout *resize_layout = new QVBoxLayout();
    //1.1.1.1.1   HBox
    QHBoxLayout *largeur_layout = new QHBoxLayout();
    QLabel *labLargeur = new QLabel("Si la largeur dépasse ");
    largeur_layout->addWidget(labLargeur);
    m_spinLLimit = new QSpinBox();
        m_spinLLimit->setMaximum(1000);
        m_spinLLimit->setValue(400);
    largeur_layout->addWidget(m_spinLLimit);
    QLabel *labLargeur2 = new QLabel("px redimensionner à cette valeur.");
    largeur_layout->addWidget(labLargeur2);
    resize_layout->addLayout(largeur_layout);

    //1.1.1.1.2   HBox
    QHBoxLayout *hauteur_layout = new QHBoxLayout();
    QLabel *labHauteur = new QLabel("Si la hauteur dépasse ");
    hauteur_layout->addWidget(labHauteur);
    m_spinHLimit = new QSpinBox();
        m_spinHLimit->setMaximum(1000);
        m_spinHLimit->setValue(210);
    hauteur_layout->addWidget(m_spinHLimit);
    QLabel *labHauteur2 = new QLabel("px redimensionner à cette valeur.");
    hauteur_layout->addWidget(labHauteur2);
    resize_layout->addLayout(hauteur_layout);

    m_groupResize->setLayout(resize_layout);

    boutons_layout->addWidget(m_groupResize);

    m_pbRun = new QPushButton("4- Lancer");
    boutons_layout->addWidget(m_pbRun);

    sub_layout->addLayout(boutons_layout);

    //1.1.2   VBox
    QVBoxLayout *preview_layout = new QVBoxLayout();
    m_previewWeelTemplate = new QLabel();
    preview_layout->addWidget(m_previewWeelTemplate);
    sub_layout->addLayout(preview_layout);

    //1.2   VBox
    QVBoxLayout *progress_layout = new QVBoxLayout();
    m_progress = new QProgressBar(this);
    progress_layout->addWidget(m_progress);
    m_progress->hide();

    main_layout->addLayout(sub_layout);
    main_layout->addLayout(progress_layout);

    this->setLayout(main_layout);
}

MaFenetre::MaFenetre() : QWidget()
{
    QCoreApplication::setApplicationName("Wheel Generator");

    //création de la fenêtre
    construireFenetre();

    //SIGNAUX
    QObject::connect(m_pbSelectWeelTemplate, SIGNAL(clicked(bool)), this, SLOT(selectTemplate()));
    QObject::connect(m_pbSelectDirWheelsInput, SIGNAL(clicked(bool)), this, SLOT(selectDirWheelsInput()));
    QObject::connect(m_pbSelectDirWheelsOutput, SIGNAL(clicked(bool)), this, SLOT(selectDirWheelsOutput()));
    QObject::connect(m_pbRun, SIGNAL(clicked(bool)), this, SLOT(lancer()));
}

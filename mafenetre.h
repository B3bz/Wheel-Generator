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
#ifndef MAFENETRE_H
#define MAFENETRE_H

#include <QApplication>
#include <QtWidgets>


class MaFenetre : public QWidget // On hérite de QWidget. Nous récupérons donc automatiquement toutes les propriétés de QWidget.
{
    Q_OBJECT
public:
    MaFenetre();

    void construireFenetre();

public slots:
    void selectTemplate();
    void selectDirWheelsInput();
    void selectDirWheelsOutput();
    void lancer();

private:
    QLabel *m_previewWeelTemplate;
    QString *m_pathWeelTemplate;
    QPushButton *m_pbSelectWeelTemplate;
    QLabel *m_labWeelTemplate;
    QString *m_pathDirWheelsInput;
    QStringList *m_listInput;
    QLabel *m_labInput;
    QPushButton *m_pbSelectDirWheelsInput;
    QString *m_pathDirWheelsOutput;
    QLabel *m_labOutput;
    QPushButton *m_pbSelectDirWheelsOutput;
    QPushButton *m_pbRun;
    QGroupBox *m_groupResize;
    QSpinBox *m_spinLLimit;
    QSpinBox *m_spinHLimit;
    QProgressBar *m_progress;
};

#endif // MAFENETRE_H

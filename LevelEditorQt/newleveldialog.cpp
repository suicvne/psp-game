#include "newleveldialog.h"
#include "ui_newleveldialog.h"
#include "mainwindow.h"
#include <QDir>
#include <iostream>


NewLevelDialog::NewLevelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewLevelDialog)
{
    ui->setupUi(this);
    fillComboBox();
}

NewLevelDialog::~NewLevelDialog()
{
    delete ui;
}

void NewLevelDialog::fillComboBox()
{
    ui->layer2ComboBox->addItem("None");
    ui->layer2ComboBox->setIconSize(QSize(16, 16));
    ui->tilesetComboBox->setIconSize(QSize(16, 16));

    QDir recoredDir(MainWindow::getResourcesDirectory());
    QStringList allFiles = recoredDir.entryList(QDir::NoDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    for(int i = 0; i < allFiles.length(); i++)
    {
        QString itemToAdd = allFiles.at(i);
        if(itemToAdd.endsWith(".png", Qt::CaseInsensitive)) //only add png files
        {
            QIcon cmbIcon(QPixmap(MainWindow::getResourcesDirectory() + "/" + itemToAdd));
            std::cout << itemToAdd.toStdString() << " ends with .png" << std::endl;
            ui->tilesetComboBox->addItem(cmbIcon, itemToAdd);
            ui->layer2ComboBox->addItem(cmbIcon, itemToAdd);
        }
    }
}

QString NewLevelDialog::getLevelName()
{
    if(ui->levelNameTextBox->text().isEmpty())
        return "Level Name";
    else
        return ui->levelNameTextBox->text();
}

int NewLevelDialog::getLevelWidth()
{
    return ui->widthSpinner->value();
}

int NewLevelDialog::getLevelHeight()
{
    return ui->heightSpinner->value();
}

QString NewLevelDialog::getTilesetPath()
{
    return ui->tilesetComboBox->currentText();
}

QString NewLevelDialog::getLayer2Path()
{
    return ui->layer2ComboBox->currentText();
}

#include "newleveldialog.h"
#include "ui_newleveldialog.h"
#include "mainwindow.h"
#include <QDir>

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
    QDir recoredDir(MainWindow::getResourcesDirectory());
    QStringList allFiles = recoredDir.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    for(int i = 0; i < allFiles.length(); i++)
    {
        ui->tilesetComboBox->addItem(allFiles.at(i));
    }
}

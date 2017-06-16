#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <gl.h>
#include <iostream>

#include <rocklevel/tilemap.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    //grabMouse();
    qApp->installEventFilter(this);

    //ui->gameDrawWidget->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    std::cout << "mouse move shit" << std::endl;
    ui->gameDrawWidget->update();
    if(ui->gameDrawWidget->rect().contains(event->pos()))
    {
        ui->gameDrawWidget->update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(this->isActiveWindow())
    {
        if(event->button() == Qt::MouseButton::LeftButton)
        {
            if(ui->gameDrawWidget->rect().contains(QCursor::pos()))
            {
                std::cout << "triggered" << std::endl;
            }
        }
    }
}
*/


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseMove)
    {
        ui->gameDrawWidget->update();
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        if(this->isActiveWindow())
        {
            if(ui->gameDrawWidget->rect().contains(QCursor::pos()))
            {
                std::cout << "triggered" << std::endl;
                return true;
            }
            else
            {
                //this->mousePressEvent((QMouseEvent*)event);
                QWidget::eventFilter(watched, event);
                return false;
            }
        }
        else
            return false;
    }
    return false;
}


void MainWindow::on_gameDrawWidget_aboutToCompose()
{
}

void MainWindow::on_gameDrawWidget_frameSwapped()
{
}

void MainWindow::onFileSelected(const QString& filename)
{
    if(!filename.isEmpty())
    {
        if(ui->gameDrawWidget->loadTilemap(filename))
        {
            QFileInfo levelFileInfo(filename);
            this->currentLevelPath = filename;
            ui->levelNameTextBox->setText(ui->gameDrawWidget->getCurrentTilemap()->map_name);
            setWindowFilePath(filename);
            setWindowTitle(levelFileInfo.fileName() + " - Level Editor");
        }
    }
    else
        return; //do nothing
}

void MainWindow::onFileSelectedForSaving(const QString &filename)
{
    if(!filename.isEmpty())
    {
        tilemap_write_to_file(currentLevelPath.toStdString().c_str(), ui->gameDrawWidget->getCurrentTilemap());

        this->currentLevelPath = filename;

        setWindowModified(false);
        setWindowTitle(QFileInfo(filename).fileName() + " - Level Editor");
        setWindowFilePath(filename);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName;
    QFileDialog* f = new QFileDialog(this, Qt::Sheet);
#ifdef __APPLE__
    f->setWindowModality(Qt::WindowModal);
#endif
    QDir d("");
    d.setNameFilters(QStringList() << "Bin File (*.bin)");
    f->setFilter(d.filter());
    connect(f, &QFileDialog::fileSelected, this, &MainWindow::onFileSelected);
    f->open();
}

void MainWindow::openLevel()
{}

void MainWindow::saveLevel()
{}

void MainWindow::on_levelNameTextBox_returnPressed()
{
    setWindowModified(true);
    ui->gameDrawWidget->setTileMapName(ui->levelNameTextBox->text());
}

void MainWindow::performSaveAs()
{
    QString fileName;
    QFileDialog* f = new QFileDialog(this, Qt::Sheet);
    f->setAcceptMode(QFileDialog::AcceptSave);
#if __APPLE__
    f->setWindowModality(Qt::WindowModal);
#endif
    QDir d("");
    d.setNameFilters(QStringList() << "Bin File (*.bin)");
    f->setFilter(d.filter());
    connect(f, &QFileDialog::fileSelected, this, &MainWindow::onFileSelectedForSaving);
    f->open();
}

void MainWindow::on_actionSave_triggered()
{
    if(!currentLevelPath.isEmpty())
    {
        tilemap_write_to_file(currentLevelPath.toStdString().c_str(), ui->gameDrawWidget->getCurrentTilemap());
        setWindowModified(false);
    }
    else
        performSaveAs();
}

void MainWindow::on_actionSave_As_triggered()
{
    performSaveAs();
}

void MainWindow::on_actionOpen_resource_path_triggered()
{
    if(!QDir(qApp->applicationDirPath() + "/res/").exists())
    {
        std::cout << "resource path didn't exist, making" << std::endl;
        QDir().mkdir(qApp->applicationDirPath() + "/res/");
    }

    QString path = QDir::toNativeSeparators(qApp->applicationDirPath() + "/res/");
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

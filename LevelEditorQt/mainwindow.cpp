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

void MainWindow::OpenResourcesDirectory()
{
    if(!QDir(qApp->applicationDirPath() + "/res/").exists())
    {
        std::cout << "resource path didn't exist, making" << std::endl;
        QDir().mkdir(qApp->applicationDirPath() + "/res/");
    }

    QString path = QDir::toNativeSeparators(qApp->applicationDirPath() + "/res/");
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
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

QImage createSubImage(QImage* image, const QRect & rect) {
    size_t offset = rect.x() * image->depth() / 8
                    + rect.y() * image->bytesPerLine();
    return QImage(image->bits() + offset, rect.width(), rect.height(),
                  image->bytesPerLine(), image->format());
}

void MainWindow::on_gameDrawWidget_frameSwapped()
{
    if(!initialSetupDone)
    {
        populateTileList();
        /*
        QListWidgetItem* test = new QListWidgetItem("Test", ui->listWidget);
        test->setIcon(QIcon(QPixmap::fromImage(ui->gameDrawWidget->getMainTexture()->toQImage())));
        */

        initialSetupDone = true;
    }
}

void MainWindow::populateTileList()
{
    ui->listWidget->clear();
    /*
    for(int x = 0; x < 7; x++)
    {
        for(int y = 0; y < 7; y++)
        {
            QListWidgetItem* test = new QListWidgetItem(QString("Tile"), ui->listWidget);
            QRect rect;
            rect.setX(x * 32);
            rect.setY(y * 32);
            rect.setWidth(32);
            rect.setHeight(32);

            QImage atlas = ui->gameDrawWidget->getMainTexture()->toQImage();

            test->setIcon(QIcon(QPixmap::fromImage(createSubImage(&atlas, rect))));
        }
    }
    */

    for(int i = 0; i < (8*8); i++)
    {
        //tile_get_location_by_id
        QListWidgetItem* test = new QListWidgetItem("Tile " + QString::number(i), ui->listWidget);
        QRect rect;
        vector_t tile_area = tile_get_location_by_id(i);
        rect.setX(tile_area.x);
        rect.setY(tile_area.y);
        rect.setWidth(TILE_WIDTH);
        rect.setHeight(TILE_HEIGHT);

        QImage atlas = ui->gameDrawWidget->getMainTexture()->toQImage();

        test->setIcon(QIcon(QPixmap::fromImage(createSubImage(&atlas, rect))));
    }
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

            populateTileList(); //repopulate the tile list
        }
    }
    else
        return; //do nothing
}

void MainWindow::onFileSelectedForSaving(const QString &filename)
{
    if(!filename.isEmpty())
    {
        tilemap_write_to_file(filename.toStdString().c_str(), ui->gameDrawWidget->getCurrentTilemap());

        this->currentLevelPath = filename;

        setWindowModified(false);
        setWindowTitle(QFileInfo(filename).fileName() + " - Level Editor");
        setWindowFilePath(filename);
    }
    else
        std::cout << "filename blank" << std::endl;
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
    MainWindow::OpenResourcesDirectory();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString* string = new QString(item->text());
    QStringRef substr(string, 5, 2);

    std::cout << "Selected '" << substr.toString().toStdString() << "'" << std::endl;

    ui->gameDrawWidget->setPlacingTileID(item->text().split(" ")[1].toInt());

}

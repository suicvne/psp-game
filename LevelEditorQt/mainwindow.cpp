#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <gl.h>
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
            //if(ui->gameDrawWidget->rect().contains(QCursor::pos()))
            if(ui->gameDrawWidget->underMouse())
            {
                ui->gameDrawWidget->placeTileAction();
                setWindowModified(true);
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
    else if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = (QKeyEvent*)event;
        switch(keyEvent->key())
        {
        case Qt::Key_Right:
            ui->gameDrawWidget->moveCamera(-16, 0);
            break;
        }
        return true;
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
        initialSetupDone = true;
    }
}

void MainWindow::populateTileList()
{
    ui->listWidget->clear();
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

        QImage* atlas = ui->gameDrawWidget->getMainTexture()->toQImage();

        test->setIcon(QIcon(QPixmap::fromImage(createSubImage(atlas, rect))));
    }

    //we also adjust the scroll bar values
    ui->horizontalScrollBar->setMaximum(ui->gameDrawWidget->getCurrentTilemap()->width);
    ui->verticalScrollBar->setMaximum(ui->gameDrawWidget->getCurrentTilemap()->height);
    //1 notch on the scroll bar will move exactly one tile
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
            setWindowTitle(levelFileInfo.fileName() + "[*] - Level Editor");
            setWindowModified(false);

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
        setWindowTitle(QFileInfo(filename).fileName() + "[*] - Level Editor");
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
    //this is all super hacky and i'm not proud
    QString* string = new QString(item->text());
    QStringRef substr(string, 5, 2);

    //std::cout << "Selected '" << substr.toString().toStdString() << "'" << std::endl;

    ui->gameDrawWidget->setPlacingTileID(item->text().split(" ")[1].toInt());
    ui->gameDrawWidget->setPlacingTileRotation(0);
}

void MainWindow::on_pushButton_clicked()
{
    ui->gameDrawWidget->setPlacingTileRotation(ui->gameDrawWidget->getPlacingTileRotation() + 90);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->gameDrawWidget->setPlacingTileRotation(ui->gameDrawWidget->getPlacingTileRotation() - 90);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->gameDrawWidget->setPlacingTileRotation(0);
}

void MainWindow::on_horizontalScrollBar_sliderMoved(int position)
{
    //ui->gameDrawWidget->adjustCamera(-position * 32, 0);
    ui->gameDrawWidget->setCameraPosition(-position * 32, -1);
}

void MainWindow::on_verticalScrollBar_sliderMoved(int position)
{
    ui->gameDrawWidget->setCameraPosition(-1, -position * 32);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static void OpenResourcesDirectory();

private slots:
    void on_gameDrawWidget_aboutToCompose();

    void on_gameDrawWidget_frameSwapped();

    //void mouseMoveEvent(QMouseEvent* event) override;
    //void mousePressEvent(QMouseEvent* event) override;

    void on_actionOpen_triggered();

    void onFileSelected(const QString& filename);
    void onFileSelectedForSaving(const QString& filename);

    void on_levelNameTextBox_returnPressed();

    void on_actionSave_triggered();
    void on_actionSave_As_triggered();

    void on_actionOpen_resource_path_triggered();

private:

    bool eventFilter(QObject *watched, QEvent *event) override;

    Ui::MainWindow *ui;
    void update();
    void draw();
    void openLevel();
    void saveLevel();
    void performSaveAs();
    QString currentLevelPath;
};

#endif // MAINWINDOW_H

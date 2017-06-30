#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

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
    static QString getResourcesDirectory();

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

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_horizontalScrollBar_sliderMoved(int position);

    void on_verticalScrollBar_sliderMoved(int position);

    void on_horizontalScrollBar_valueChanged(int value);

    void on_verticalScrollBar_valueChanged(int value);

    void closeEvent(QCloseEvent* event) override;

    void on_actionClose_triggered();

    void on_actionMinimize_triggered();

    void on_checkBox_toggled(bool checked);

    void on_actionNewLevel_triggered();

private:

    bool eventFilter(QObject *watched, QEvent *event) override;

    void populateTileList();

    Ui::MainWindow *ui;
    void update();
    void draw();
    void openLevel();
    void saveLevel();
    void performSaveAs();
    QString currentLevelPath;
    bool initialSetupDone;
    bool isQuitting = false;


};

#endif // MAINWINDOW_H

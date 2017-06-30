#ifndef NEWLEVELDIALOG_H
#define NEWLEVELDIALOG_H

#include <QDialog>

namespace Ui {
class NewLevelDialog;
}

class NewLevelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewLevelDialog(QWidget *parent = 0);
    ~NewLevelDialog();

    QString getLevelName();
    int getLevelWidth();
    int getLevelHeight();
    QString getTilesetPath();
    QString getLayer2Path();

private:
    Ui::NewLevelDialog *ui;
    void fillComboBox();
};

#endif // NEWLEVELDIALOG_H

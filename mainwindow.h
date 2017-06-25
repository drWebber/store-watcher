#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings/settings.h"
#include "store-remainings/storeremainings.h"
#include <qsqlrelationaltablemodel.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void showSettingsWnd();
    void executeFile();
private:
    Ui::MainWindow *ui;
    Settings *settingsWnd;
    QSqlRelationalTableModel *model;
};

#endif // MAINWINDOW_H

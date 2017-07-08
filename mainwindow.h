#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings/settings.h"
#include "store-remainings/storeremainings.h"
#include <QtSql/qsqlrelationaltablemodel.h>
#include "delegates/rowcolorerdelegate.h"

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
    void updateTable();
    void markBusyFile(QString path);
private:
    Ui::MainWindow *ui;
    Settings *settingsWnd;
    QSqlRelationalTableModel *model;
    StoreWatcher *sw;
    RowColorerDelegate *rowColorerDelegate;
};

#endif // MAINWINDOW_H

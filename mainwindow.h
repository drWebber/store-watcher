#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings/settings.h"
#include "store-remainings/storeremainings.h"
#include <QtSql/qsqlrelationaltablemodel.h>
#include "delegates/rowcolorerdelegate.h"
#include <qmenu.h>
#include <qsystemtrayicon.h>

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
    void markBusyFile(int smid);
    void markUpdatedFile(StoreRemainings *sr);
    void setTrayIconActions();
private slots:
    void showTrayIcon();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
private:
    Ui::MainWindow *ui;
    Settings *settingsWnd;
    QSqlRelationalTableModel *model;
    QSortFilterProxyModel *proxy;
    StoreWatcher *sw;
    RowColorerDelegate *rowColorerDelegate;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *showAction;
    QAction *hideAction;
    QAction *quitAction;

    // QWidget interface
protected:
    void changeEvent(QEvent *event) override;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H

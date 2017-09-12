#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings/settings.h"
#include "store-remainings/storeremainings.h"
#include <QtSql/qsqlquerymodel.h>
#include <qsortfilterproxymodel.h>
#include "models/coloredsqlquerymodel.h"
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
    QModelIndex findIndex(int smid);
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
    void on_btnSetFilter_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    Settings *settingsWnd;
    ColoredSqlQueryModel *model;
    QSortFilterProxyModel *proxy;
    StoreWatcher *sw;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *showAction;
    QAction *hideAction;
    QAction *quitAction;
    enum Column {SMID, MANUFACTURER, PLACE, DATE};
protected:
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H

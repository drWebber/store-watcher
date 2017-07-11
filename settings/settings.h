#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <qmenu.h>
#include <QtSql/qsqlrelationaltablemodel.h>
#include "settings/addstore.h"
#include "settings/editstore.h"
#include "store-remainings/storewatcher.h"
#include <qsortfilterproxymodel.h>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(StoreWatcher &sw, QWidget *parent = 0);
    ~Settings();
protected:
   StoreWatcher *sw;
private:
    void setTableViewMenu();
private slots:
   void enableActionButtons();
   void on_btnAdd_clicked();
   void on_btnEdit_clicked();
   void on_btnRemove_clicked();
   void tableContextMenuRequested(QPoint pos);
   void on_Action_SetFilter();
private:
    Ui::Settings *ui;    
    QSqlRelationalTableModel *model;
    AddStore *addWnd;
    EditStore *editWnd;
    QMenu *tableMenu;
    QSortFilterProxyModel *proxy;
    QAction *filterAction;
    bool filterChecked = false;
    QMap<int, QString> customFilter;
};

#endif // SETTINGS_H

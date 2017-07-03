#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <qsqlrelationaltablemodel.h>
#include "settings/addstore.h"
#include "settings/editstore.h"
#include "store-remainings/storewatcher.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(StoreWatcher &sw, QWidget *parent = 0);
    ~Settings();
private slots:
   void addRow();
   void editRow();
   void delRow();
   void enableActionButtons();
protected:
   StoreWatcher *sw;
private:
    Ui::Settings *ui;    
    QSqlRelationalTableModel *model;
    AddStore *addWnd;
    EditStore *editWnd;
};

#endif // SETTINGS_H

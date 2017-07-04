#ifndef ACTIONWND_H
#define ACTIONWND_H

#include <QWidget>
#include <QtSql/qsqlrelationaltablemodel.h>
#include <qtableview.h>
#include "sql/sqlquery.h"
#include <qmessagebox.h>
#include "store-remainings/storewatcher.h"

namespace Ui {
class ActionWnd;
}

class ActionWnd : public QWidget
{
    Q_OBJECT

public:
    explicit ActionWnd(QSqlRelationalTableModel &model,
                       QTableView &tableView,
                       StoreWatcher &sw,
                       QWidget *parent = 0);
    ~ActionWnd();
    virtual void setUp() = 0;
    void submit();
private slots:
    void chooseDir();
    void tryRegExp();
    void submitClicked();
protected:
    Ui::ActionWnd *ui;
    QSqlRelationalTableModel *model;
    QTableView *tableView;
    int currentRow;
    SqlQuery query;
    QString getFilePath();
    void setModelData();
    StoreWatcher *sw;
    virtual void beforeSubmit() = 0;
};

#endif // ACTIONWND_H

#ifndef ACTIONWND_H
#define ACTIONWND_H

#include <QWidget>
#include <qsqlrelationaltablemodel.h>
#include <qtableview.h>
#include "sql/sqlquery.h"

namespace Ui {
class ActionWnd;
}

class ActionWnd : public QWidget
{
    Q_OBJECT

public:
    explicit ActionWnd(QSqlRelationalTableModel &model,
                       QTableView &tableView,
                       QWidget *parent = 0);
    ~ActionWnd();
    virtual void setUp() = 0;
    void submit();
private:
    void setModelData();
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
};

#endif // ACTIONWND_H

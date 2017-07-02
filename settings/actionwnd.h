#ifndef ACTIONWND_H
#define ACTIONWND_H

#include <QWidget>
#include <qsqlrelationaltablemodel.h>
#include <qtableview.h>
#include "sql/sqlquery.h"
#include <qmessagebox.h>

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
private slots:
    void chooseDir();
    void tryRegExp();
    virtual void submitClicked() = 0;
protected:
    Ui::ActionWnd *ui;
    QSqlRelationalTableModel *model;
    QTableView *tableView;
    int currentRow;
    SqlQuery query;
    QString getFilePath();
    void setModelData();
};

#endif // ACTIONWND_H

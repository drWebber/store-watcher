#ifndef ADDSTORE_H
#define ADDSTORE_H
#include "settings/actionwnd.h"

class AddStore : public ActionWnd
{
public:
    explicit AddStore(QSqlRelationalTableModel &model,
                      QTableView &tableView,
                      QWidget *parent = 0);
    void setUp();
public slots:
    void submitClicked();
};

#endif // ADDSTORE_H

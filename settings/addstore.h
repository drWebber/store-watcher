#ifndef ADDSTORE_H
#define ADDSTORE_H
#include "actionwnd.h"

class AddStore : public ActionWnd
{
public:
    explicit AddStore(QSqlRelationalTableModel &model,
                      QTableView &tableView,
                      QWidget *parent = 0);
    void setUp();
};

#endif // ADDSTORE_H

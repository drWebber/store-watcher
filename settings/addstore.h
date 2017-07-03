#ifndef ADDSTORE_H
#define ADDSTORE_H
#include "settings/actionwnd.h"
#include "store-remainings/storewatcher.h"

class AddStore : public ActionWnd
{
public:
    explicit AddStore(QSqlRelationalTableModel &model,
                      QTableView &tableView,
                      StoreWatcher &sw,
                      QWidget *parent = 0);
    void setUp();
private:
    void beforeSubmit();
};

#endif // ADDSTORE_H

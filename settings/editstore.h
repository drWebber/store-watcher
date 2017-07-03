#ifndef EDITSTORE_H
#define EDITSTORE_H
#include "settings/actionwnd.h"
#include "store-remainings/storewatcher.h"

class EditStore : public ActionWnd
{
public:
    explicit EditStore(QSqlRelationalTableModel &model,
                        QTableView &tableView,
                        StoreWatcher &sw,
                        QWidget *parent = 0);
    void setUp();
private:
    void beforeSubmit();
    QString oldPath;
};

#endif // EDITSTORE_H

#ifndef EDITSTORE_H
#define EDITSTORE_H
#include "settings/actionwnd.h"

class EditStore : public ActionWnd
{
public:
    explicit EditStore(QSqlRelationalTableModel &model,
                        QTableView &tableView,
                        QWidget *parent = 0);
    void setUp();
};

#endif // EDITSTORE_H

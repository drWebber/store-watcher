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
public slots:
    void submitClicked();
};

#endif // EDITSTORE_H

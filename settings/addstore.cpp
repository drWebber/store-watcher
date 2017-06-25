#include "addstore.h"

AddStore::AddStore(QSqlRelationalTableModel &model, QTableView &tableView, QWidget *parent) :
    ActionWnd(model, tableView, parent)
{

}

void AddStore::setUp()
{
    currentRow = model->rowCount();
    model->insertRow(currentRow);
    tableView->selectRow(currentRow);
}

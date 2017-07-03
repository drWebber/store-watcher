#include "addstore.h"
#include "ui_actionwnd.h"

#include <qfile.h>

AddStore::AddStore(QSqlRelationalTableModel &model, QTableView &tableView, StoreWatcher &sw, QWidget *parent) :
    ActionWnd(model, tableView, sw, parent)
{

}

void AddStore::setUp()
{
    currentRow = model->rowCount();
    model->insertRow(currentRow);
    tableView->selectRow(currentRow);
}

void AddStore::beforeSubmit()
{
    QSqlQuery query;
    query.prepare("INSERT INTO store_date(smid) VALUES((SELECT smid FROM store_manufacturer WHERE mid = (SELECT mid FROM manufacturers WHERE name = :manName) AND storePlace = :storePlace))");
    query.bindValue(":manName", ui->cmbManufacturer->currentText());
    query.bindValue(":storePlace", ui->cmbStorePlacement->currentText());
    query.exec();
}

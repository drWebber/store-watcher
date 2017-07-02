#include "addstore.h"
#include "ui_actionwnd.h"

#include <qfile.h>

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

void AddStore::submitClicked()
{
    QFile rFile(getFilePath());
    if (rFile.exists()) {
        this->setModelData();
        model->submit();

        QSqlQuery query;
        query.prepare("INSERT INTO store_date(smid) VALUES((SELECT smid FROM store_manufacturer WHERE mid = (SELECT mid FROM manufacturers WHERE name = :manName) AND storePlace = :storePlace))");
        query.bindValue(":manName", ui->cmbManufacturer->currentText());
        query.bindValue(":storePlace", ui->cmbStorePlacement->currentText());
        query.exec();

        this->close();
    } else {
        QMessageBox::warning(NULL, "Ошибка", "Файл по указанному регулятрому выражению не найден!");
    }
}

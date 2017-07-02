#include "editstore.h"
#include "ui_actionwnd.h"

#define STOREPLACE_COL 1
#define MANUFACTURER_COL 2
#define DIRPATH_COL 3
#define REGEXP_COL 4
#define CURR_PATH 5
#define START_ROW 6
#define ARTICLE_COL 7
#define ITEM_COUNT_COL 8

EditStore::EditStore(QSqlRelationalTableModel &model, QTableView &tableView, QWidget *parent) :
    ActionWnd(model, tableView, parent)
{

}

void EditStore::setUp()
{
    currentRow = tableView->currentIndex().row();

    QModelIndex indx = tableView->model()->index(currentRow, STOREPLACE_COL);
    ui->cmbStorePlacement->setCurrentText(tableView->model()->data(indx).toString());

    indx = tableView->model()->index(currentRow, MANUFACTURER_COL);
    ui->cmbManufacturer->setCurrentText(tableView->model()->data(indx).toString());

    indx = tableView->model()->index(currentRow, DIRPATH_COL);
    ui->leCurrentDir->setText(tableView->model()->data(indx).toString());

    indx = tableView->model()->index(currentRow, REGEXP_COL);
    ui->leRegExp->setText(tableView->model()->data(indx).toString());    

    indx = tableView->model()->index(currentRow, START_ROW);
    ui->sbStartRow->setValue(tableView->model()->data(indx).toInt());

    indx = tableView->model()->index(currentRow, ARTICLE_COL);
    ui->sbArticle->setValue(tableView->model()->data(indx).toInt());

    indx = tableView->model()->index(currentRow, ITEM_COUNT_COL);
    ui->sbItemCount->setValue(tableView->model()->data(indx).toInt());
}

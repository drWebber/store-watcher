#include "settings.h"
#include "ui_settings.h"
#include <qdebug.h>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    //запрещаем редактирование
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model = new QSqlRelationalTableModel();
    model->setTable("store_manufacturer");
    model->setRelation(2, QSqlRelation("manufacturers", "mid", "name"));
    model->setHeaderData(1, Qt::Horizontal, "Расположение");
    model->setHeaderData(2, Qt::Horizontal, "Бренд");
    model->setHeaderData(3, Qt::Horizontal, "Директория");
    model->setHeaderData(4, Qt::Horizontal, "RegExp");
    model->setHeaderData(5, Qt::Horizontal, "Путь");
    model->setHeaderData(6, Qt::Horizontal, "Начальная строка");
    model->setHeaderData(7, Qt::Horizontal, "Колонка артикул");
    model->setHeaderData(8, Qt::Horizontal, "Колонка кол-во");
    model->select();

    ui->pbEdit->setDisabled(true);
    ui->pdDel->setDisabled(true);

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::addRow()
{
    addWnd = new AddStore(*model, *ui->tableView);
    addWnd->setWindowTitle("Добавить файл остатков");
    addWnd->setUp();
    addWnd->show();
}

void Settings::editRow()
{
    editWnd = new EditStore(*model, *ui->tableView);
    editWnd->setWindowTitle("Редактор параметров");
    editWnd->setUp();
    editWnd->show();
}

void Settings::delRow()
{
    int currentRow = ui->tableView->currentIndex().row();
    model->removeRow(currentRow);
    ui->tableView->hideRow(currentRow);
    ui->tableView->selectRow(currentRow-1);
}

void Settings::enableActionButtons()
{
    ui->tableView->selectRow(ui->tableView->currentIndex().row());
    ui->pbEdit->setDisabled(false);
    ui->pdDel->setDisabled(false);
}

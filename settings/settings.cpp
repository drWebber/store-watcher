#include "settings.h"
#include "ui_settings.h"
#include <qdebug.h>

#define CURR_PATH 5

Settings::Settings(StoreWatcher &sw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(tableContextMenuRequested(QPoint)));

    this->sw = &sw;

    //запрещаем редактирование
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

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

    proxy = new SortFilterProxyModel();
    proxy->setSourceModel(model);

    ui->btnEdit->setDisabled(true);
    ui->btnRemove->setDisabled(true);

    ui->tableView->setModel(proxy);
    ui->tableView->hideColumn(0);

    QSize btnSize(24, 24);
    ui->btnAdd->setIcon(QIcon(":/recources/images/add.png"));
    ui->btnAdd->setFixedWidth(32);
    ui->btnAdd->setIconSize(btnSize);
    ui->btnRemove->setIcon(QIcon(":/recources/images/remove.png"));
    ui->btnRemove->setIconSize(btnSize);
    ui->btnEdit->setIcon(QIcon(":/recources/images/configure.png"));
    ui->btnEdit->setIconSize(btnSize);
    ui->btnSetFilter->setIcon(QIcon(":/recources/images/filter.png"));
    ui->btnSetFilter->setIconSize(btnSize);
    ui->btnUnSetFilter->setIcon(QIcon(":/recources/images/filter-remove.png"));
    ui->btnUnSetFilter->setIconSize(btnSize);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::enableActionButtons()
{
    ui->btnEdit->setDisabled(false);
    ui->btnRemove->setDisabled(false);
}

void Settings::on_btnAdd_clicked()
{
    addWnd = new AddStore(*model, *ui->tableView, *sw);
    addWnd->setWindowIcon(QIcon(":/recources/images/Programs.png"));
    addWnd->setWindowTitle("Добавить файл остатков");
    addWnd->setUp();
    addWnd->show();
}

void Settings::on_btnEdit_clicked()
{
    editWnd = new EditStore(*model, *ui->tableView, *sw);
    editWnd->setWindowIcon(QIcon(":/recources/images/Programs.png"));
    editWnd->setWindowTitle("Редактор параметров");
    editWnd->setUp();
    editWnd->show();
}

void Settings::on_btnRemove_clicked()
{
    int currentRow = ui->tableView->currentIndex().row();
    QModelIndex indx = ui->tableView->model()->index(currentRow, CURR_PATH);
    sw->removeFile(ui->tableView->model()->data(indx).toString());

    model->removeRow(currentRow);

    ui->tableView->hideRow(currentRow);
    ui->tableView->selectRow(currentRow-1);
}

void Settings::tableContextMenuRequested(QPoint pos)
{
    tableMenu = new QMenu(this);

    filterAction = new QAction(QIcon(":/recources/images/filter.png"), "Фильтр", this);
    connect(filterAction, SIGNAL(triggered()),
            this, SLOT(on_Action_SetFilter()));
    tableMenu->addAction(filterAction);

    removeFilterAction = new QAction("Снять фильтр");
    connect(removeFilterAction,SIGNAL(triggered()),
            this, SLOT(on_Action_RemoveFilter()));
    tableMenu->addAction(removeFilterAction);

    tableMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void Settings::on_Action_SetFilter()
{
    proxy->addFilterFixedString(ui->tableView->currentIndex().column(),
                                ui->tableView->currentIndex().data().toString());
    ui->tableView->hideColumn(0);
}

void Settings::on_Action_RemoveFilter()
{
    proxy->removeFilter();
    ui->tableView->hideColumn(0);
}

void Settings::on_btnSetFilter_clicked()
{
    proxy->addFilterFixedString(ui->tableView->currentIndex().column(),
                                ui->tableView->currentIndex().data().toString());
    ui->tableView->hideColumn(0);
}

void Settings::on_btnUnSetFilter_clicked()
{
    proxy->removeFilter();
    ui->tableView->hideColumn(0);
}

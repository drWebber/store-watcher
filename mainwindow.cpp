#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sql/sqlconnector.h"
#include "store-remainings/storewatcher.h"
#include <qdebug.h>
#include <QTime>
#include <qfile.h>
#include <qdir.h>
#include <QtSql/qsqlquery.h>
#include <qurl.h>
#include <qdesktopservices.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SqlConnector sc;
    sc.connect();

    QSqlQuery query;
    query.exec("SET NAMES 'cp1251'");

    model = new QSqlRelationalTableModel();
    model->setTable("store_manufacturer");
    model->setRelation(2, QSqlRelation("manufacturers", "mid", "name"));
    model->select();

    sw = new StoreWatcher();

    //запрещаем редактирование
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(3);
    ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);
    ui->tableView->hideColumn(7);
    ui->tableView->hideColumn(8);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSettingsWnd()
{
    settingsWnd = new Settings(*sw);

    settingsWnd->setAttribute(Qt::WA_DeleteOnClose);
    connect(settingsWnd, SIGNAL(destroyed(QObject*)), this, SLOT(updateTable()));

    settingsWnd->setMinimumWidth(1024);
    settingsWnd->setWindowTitle("Расположение файлов остатков");
    settingsWnd->show();
}

void MainWindow::executeFile()
{
    int currentRow = ui->tableView->currentIndex().row();
    QString path = ui->tableView->model()->index(currentRow, 5).data().toString();
    if(QFile::exists(path)){
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    } else{

    }
}

void MainWindow::updateTable()
{
    model->select();
}

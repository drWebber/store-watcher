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
#include <qsortfilterproxymodel.h>

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

    //Лев Алексеевский уроки Qt
    //http://imaginativethinking.ca/use-qt-qsortfilterproxymodel/

    sw = new StoreWatcher();
    connect(sw, SIGNAL(fileIsBusy(QString)), this, SLOT(markBusyFile(QString))); // slot

    QSortFilterProxyModel *proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);

    //запрещаем редактирование
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(proxy);
//    ui->tableView->hideColumn(0);
//    ui->tableView->hideColumn(3);
//    ui->tableView->hideColumn(4);
//    ui->tableView->hideColumn(5);
//    ui->tableView->hideColumn(6);
//    ui->tableView->hideColumn(7);
//    ui->tableView->hideColumn(8);
    QBrush brush;
    brush.setColor(QColor(Qt::red));
    ui->tableView->model()->setData(ui->tableView->model()->index(1,1), QVariant(brush), Qt::BackgroundRole);
    proxy->sort(2, Qt::AscendingOrder);
    ui->tableView->update();
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
        //
    }
}

void MainWindow::updateTable()
{
    model->select();
}

void MainWindow::markBusyFile(QString path)
{
    QModelIndexList foundIndexes = ui->tableView->model()->match(model->index(0,5), Qt::DisplayRole, path);
    foreach (QModelIndex indx, foundIndexes) {
        QBrush yellow;
        yellow.setColor(Qt::yellow);
        model->setData(ui->tableView->model()->index(0,1), QVariant(QBrush(Qt::red)), Qt::BackgroundRole);
        qDebug() << QString::number(indx.row());
    }
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sql/sqlconnector.h"
#include "store-remainings/storewatcher.h"
#include <QTime>
#include <qfile.h>
#include <qdir.h>
#include <QtSql/qsqlquery.h>
#include <qurl.h>
#include <qdesktopservices.h>
#include <QCloseEvent>

#include <QModelIndexList>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SqlConnector sc;
    sc.connect();

    if(!sc.isOpen()){
        QMessageBox::critical(this, "Ошибка", "Соединение с базой данных не установлено. \nРабота приложения завершена.");
        exit(0);
    }

    QSqlQuery query;
    query.exec("SET character_set_client=cp1251");
    query.exec("SET character_set_connection=UTF8");
    query.exec("SET character_set_results=cp1251");

    query.exec("SET character_set_database=UTF8");

    model = new ColoredSqlQueryModel(this);
    model->setQuery("SELECT sm.smid, mn.name, sm.storePlace, sd.date "
                    "FROM `store_manufacturer` AS sm LEFT JOIN `store_date` AS sd ON sm.smid = sd.smid, `manufacturers` AS mn "
                    "WHERE sm.mid = mn.mid");

    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);

    proxy->setHeaderData(Column::MANUFACTURER, Qt::Horizontal, "Производитель");
    proxy->setHeaderData(Column::PLACE, Qt::Horizontal, "Расположение");
    proxy->setHeaderData(Column::DATE, Qt::Horizontal, "Дата");


    //запрещаем редактирование
//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(proxy);

    ui->tableView->hideColumn(Column::SMID);

    proxy->sort(2, Qt::AscendingOrder);
    ui->tableView->update();

    sw = new StoreWatcher();
    connect(sw, SIGNAL(fileIsBusy(int)),
            this, SLOT(markBusyFile(int)));
    connect(sw, SIGNAL(updateFinished(StoreRemainings*)),
            this, SLOT(markUpdatedFile(StoreRemainings*)));
    sw->setUp();

    this->setTrayIconActions();
    this->showTrayIcon();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSettingsWnd()
{
    settingsWnd = new Settings(*sw);
    settingsWnd->setWindowIcon(QIcon(":/recources/images/Programs.png"));

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
    }
}

void MainWindow::updateTable()
{
    model->query();
}

//const QModelIndex &MainWindow::findIndex(int smid)
QModelIndex MainWindow::findIndex(int smid)
{
    QModelIndexList findIndexes = proxy->match(proxy->index(0, Column::SMID),
                                               Qt::DisplayRole,
                                               QVariant(smid), 1);
    if (findIndexes.isEmpty()) {
        return *(new QModelIndex());
    }
    return findIndexes.at(0);
}

void MainWindow::markBusyFile(int smid)
{
    QModelIndex indx = findIndex(smid);
    if (indx.isValid()) {
        for (int i = 0; i < proxy->columnCount(); ++i) {
            proxy->setData(proxy->index(indx.row(), i),
                           QColor(Qt::gray),
                           Qt::TextColorRole);
        }
    }
}

void MainWindow::markUpdatedFile(StoreRemainings *sr)
{
    QModelIndex indx = findIndex(sr->getSmid());
    if (indx.isValid()) {
        if(sr->getCurrentFilePath().isEmpty()){
            for (int i = 0; i < proxy->columnCount(); ++i) {
                proxy->setData(proxy->index(indx.row(), i),
                               QColor(Qt::red),
                               Qt::TextColorRole);
            }
        } else{
            for (int i = 0; i < proxy->columnCount(); ++i) {
                proxy->setData(proxy->index(indx.row(), i),
                               QColor(Qt::black),
                               Qt::TextColorRole);
            }
        }
    }
}

void MainWindow::setTrayIconActions()
{
    showAction = new QAction("Показать");
    hideAction = new QAction("Свернуть");
    quitAction = new QAction("Выход");

    trayMenu = new QMenu(this);

    connect(showAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect(hideAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    trayMenu->addAction(showAction);
    trayMenu->addAction(hideAction);
    trayMenu->addAction(quitAction);
}

void MainWindow::showTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/recources/images/Programs.png"));
    trayIcon->setContextMenu(trayMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        emit hide();
        break;
    case QSystemTrayIcon::DoubleClick:
        emit show();
        this->setFocus();
        break;
    default:
        break;
    }
}

void MainWindow::changeEvent(QEvent * event)
{
    //при нажатии свернуть - прячем mainWindow в трей
    if(event->type() == QEvent::WindowStateChange && isMinimized()){
        this->hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(this->isVisible()){
        //При нажатии на крестик прячем MainWindow вместо закрытия
        this->hide();
        event->ignore();
    } else {
        trayIcon->hide();
        event->accept();
    }
}

void MainWindow::on_btnSetFilter_triggered(bool checked)
{
    if (checked) {
        proxy->setFilterKeyColumn(ui->tableView->currentIndex().column());
        proxy->setFilterFixedString(ui->tableView->currentIndex().data().toString());
    } else {
        proxy->setFilterFixedString("");
    }
}

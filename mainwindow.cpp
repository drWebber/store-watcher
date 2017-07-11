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
#include <QCloseEvent>

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
    query.exec("SET NAMES 'cp1251'");

    model = new QSqlRelationalTableModel();
    model->setTable("store_manufacturer");
    model->setRelation(2, QSqlRelation("manufacturers", "mid", "name"));
    model->select();

    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);

    proxy->setHeaderData(1, Qt::Horizontal, "Расположение");
    proxy->setHeaderData(2, Qt::Horizontal, "Производитель");

    //запрещаем редактирование
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(proxy);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(3);
    ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);
    ui->tableView->hideColumn(7);
    ui->tableView->hideColumn(8);

    rowColorerDelegate = new RowColorerDelegate(*proxy);
    ui->tableView->setItemDelegate(rowColorerDelegate);
    proxy->sort(2, Qt::AscendingOrder);
    ui->tableView->update();

    sw = new StoreWatcher();
    connect(sw, SIGNAL(fileIsBusy(int)), this, SLOT(markBusyFile(int))); // slot
    connect(sw, SIGNAL(updateFinished(StoreRemainings*)), this, SLOT(markUpdatedFile(StoreRemainings*)));
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
    model->select();
}

void MainWindow::markBusyFile(int smid)
{
    rowColorerDelegate->setSmIdColor(smid, "gray");
    model->select();
}

void MainWindow::markUpdatedFile(StoreRemainings *sr)
{
    if(sr->getCurrentFilePath().isEmpty()){
        rowColorerDelegate->unsetSmidColor(sr->getSmid());
        rowColorerDelegate->setSmIdColor(sr->getSmid(), "red");
    } else{
        rowColorerDelegate->unsetSmidColor(sr->getSmid());
    }
    model->select();
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
        break;
    default:
        break;
    }
}

void MainWindow::changeEvent(QEvent * event)
{
    //при нажатии свернуть - прячем mainWindow в трей
    if(event->type() == QEvent::WindowStateChange && isMinimized()){
        this -> hide();
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

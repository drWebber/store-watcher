#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sql/sqlconnector.h"
#include "store-remainings/storewatcher.h"
#include <QTime>
#include <qfile.h>
#include <qdir.h>
#include <qsqlquery.h>
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
    model->update();

    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);

    proxy->setHeaderData(MANUFACTURER, Qt::Horizontal, "Производитель");
    proxy->setHeaderData(PLACE, Qt::Horizontal, "Расположение");
    proxy->setHeaderData(DATE, Qt::Horizontal, "Дата");


    //запрещаем редактирование
//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(proxy);

    ui->tableView->hideColumn(SMID);

    proxy->sort(2, Qt::AscendingOrder);
    ui->tableView->update();

    sw = new StoreWatcher();
    connect(sw, SIGNAL(fileIsBusy(int)),
            this, SLOT(markBusyFile(int)));
    connect(sw, SIGNAL(updateFinished(StoreRemainings*)),
            this, SLOT(markUpdatedFile(StoreRemainings*)));
    sw->setUp();


    tableMenu = new QMenu(this);
    refreshRemainings = new QAction("Обновить остатки", this);
    connect(refreshRemainings, SIGNAL(triggered(bool)),
            this, SLOT(on_UpdateRemainings()));
    tableMenu->addAction(refreshRemainings);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(tableContextMenuRequested(QPoint)));

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
    model->update();

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

QModelIndex MainWindow::findIndex(int smid)
{
    QModelIndexList findIndexes = proxy->match(proxy->index(0, SMID),
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
    model->update();
}

void MainWindow::setTrayIconActions()
{
    showAction = new QAction("Показать", this);
    hideAction = new QAction("Свернуть", this);
    quitAction = new QAction("Выход", this);

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

void MainWindow::on_UpdateRemainings()
{
    int row = ui->tableView->currentIndex().row();
    int smid = proxy->data(proxy->index(row, 0), Qt::DisplayRole).toInt();
    QSqlQuery q;
    q.prepare("SELECT `lastPath` FROM `store_manufacturer` WHERE smid = :smid");
    q.bindValue(":smid", smid);
    q.exec();
    if (q.next()) {
        sw->updateRemainings(q.value(0).toString());
    } else {
        QMessageBox::warning(this, "Ошибка обновления остков",
                             "Обновить остатки в строке " + QString::number(row) +
                             " не удалось.");
    }

}

void MainWindow::tableContextMenuRequested(QPoint pos)
{
    tableMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sql/sqlconnector.h"
#include "store-remainings/storewatcher.h"
#include <qdebug.h>
#include <QTime>
#include <qfile.h>
#include <qdir.h>
#include <qsqlquery.h>
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

    StoreWatcher *sw = new StoreWatcher();

    model = new QSqlRelationalTableModel();
    model->setTable("store_manufacturer");
    model->setRelation(2, QSqlRelation("manufacturers", "mid", "name"));
    model->select();

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

    /*
    StoreRemainings sr(12, 10, "F:/Загрузки", "inwcredible_dracula_2_ce");
    qDebug() << sr.getCurrentFile()->fileName();
    StoreRemainings sr2(12, 10, "F:/Загрузки", "in3wcredible_dracula_2_ce");

    QFileSystemWatcher *fsw = new QFileSystemWatcher;
    connect(fsw, SIGNAL(fileChanged(QString)), this, SLOT(storeRemainsChanged(QString)));

    fsw->addPath(sr2.getCurrentFile()->fileName());
    fsw->addPath(sr.getCurrentFile()->fileName());
    */

    /*
    QFileSystemWatcher *fsw = new QFileSystemWatcher;
    connect(fsw, SIGNAL(fileChanged(QString)), this, SLOT(storeRemainsChanged()));
    fsw->addPath("D:\\!Data\\i2016426\\tmp\\tmp.txt");
    //fsw->removePath("");

    //многопоточность
    StoreWatcher *sw = new StoreWatcher;
    connect(sw, SIGNAL(remainsFileChanged(QString)),this,SLOT(updateStoreRemains(QString)));
    sw->start();

    QDir dir;
    dir.setPath("D:\\Андрей");
    QStringList files = dir.entryList();

    foreach (QString file, files) {
        qDebug() << dir.absoluteFilePath(file);
    }
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSettingsWnd()
{
    settingsWnd = new Settings();
    settingsWnd->setMinimumWidth(1024);
    settingsWnd->setWindowTitle("Расположение файлов остатков");
    settingsWnd->show();
}

void MainWindow::executeFile()
{
    int currentRow = ui->tableView->currentIndex().row();
    QString path = ui->tableView->model()->index(currentRow, 5).data().toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

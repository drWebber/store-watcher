#include "actionwnd.h"
#include "ui_actionwnd.h"

#include <qfiledialog.h>
#include <qdebug.h>
#include <qsqlerror.h>

#define STOREPLACE_COL 1
#define MANUFACTURER_COL 2
#define DIRPATH_COL 3
#define REGEXP_COL 4
#define CURR_PATH 5
#define START_ROW 6
#define ARTICLE_COL 7
#define ITEM_COUNT_COL 8

ActionWnd::ActionWnd(QSqlRelationalTableModel &model, QTableView &tableView, StoreWatcher &sw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActionWnd)
{
    ui->setupUi(this);

    this->model = &model;
    this->tableView = &tableView;
    this->sw = &sw;

    ui->cmbManufacturer->addItems(query.getSingleValsList("SELECT name FROM manufacturers"));

    ui->cmbStorePlacement->addItem("Витебск");
    ui->cmbStorePlacement->addItem("Минск");
    ui->cmbStorePlacement->addItem("Внешнее");
}

ActionWnd::~ActionWnd()
{
    delete ui;
}

void ActionWnd::setModelData()
{
    QModelIndex indx = model->index(currentRow, STOREPLACE_COL);
    model->setData(indx, ui->cmbStorePlacement->currentText());

    QSqlQuery sq;
    sq.prepare("SELECT `mid` FROM manufacturers WHERE name = :name;");
    sq.bindValue(":name", ui->cmbManufacturer->currentText());    
    QString mid = query.getSingleVal(sq).toString();
    indx = model->index(currentRow, MANUFACTURER_COL);
    model->setData(indx, mid);

    indx = model->index(currentRow, DIRPATH_COL);
    model->setData(indx, ui->leCurrentDir->text());

    indx = model->index(currentRow, REGEXP_COL);
    model->setData(indx, ui->leRegExp->text() + ".+xlsx?$");

    indx = model->index(currentRow, CURR_PATH);
    model->setData(indx, getFilePath());

    indx = model->index(currentRow, START_ROW);
    model->setData(indx, ui->sbStartRow->text());

    indx = model->index(currentRow, ARTICLE_COL);
    model->setData(indx, ui->sbArticle->text());

    indx = model->index(currentRow, ITEM_COUNT_COL);
    model->setData(indx, ui->sbItemCount->text());
}

void ActionWnd::chooseDir()
{
    QString dirPath = QFileDialog::getExistingDirectory(NULL, "Выбирите расположение каталога");
    ui->leCurrentDir->setText(dirPath);
}

QString ActionWnd::getFilePath()
{
    QString absoluteFilePath;
    QString regExp = ui->leRegExp->text();
    QDir dir;
    dir.setPath(ui->leCurrentDir->text());
    QStringList files = dir.entryList();
    foreach (QString file, files) {
        if (file.contains(QRegularExpression(regExp))) {
             absoluteFilePath = dir.absoluteFilePath(file);
        }
    }
    return absoluteFilePath;
}

void ActionWnd::tryRegExp()
{
    ui->lbRegExpResult->setText("");
    ui->lbRegExpResult->setText(getFilePath());
}

void ActionWnd::submitClicked()
{
    qDebug() << "submitClicked";
    QFile rFile(getFilePath());
    if (rFile.exists()) {
        this->setModelData();
        model->submit();
        qDebug() << "ActionWnd::submitClicked()"
                 << model->lastError();

        sw->addFile(getFilePath());

        beforeSubmit();

        this->close();
    } else {
        QMessageBox::warning(NULL, "Ошибка", "Файл по указанному регулятрому выражению не найден!");
    }
}

#include "storeupdater.h"
#include <qfileinfo.h>
#include <qdatetime.h>
#include <qsystemtrayicon.h>
#include <qregularexpression.h>
#include "dataimport.h"
#include "vitebskdataimport.h"
#include <qdebug.h>

StoreUpdater::StoreUpdater(QFileSystemWatcher &fsw, StoreRemainings &sr)
{
    xlsFilePath = sr.getCurrentFilePath();
    updateCsvFilePath(xlsFilePath);
    this->fsw = &fsw;
    this->sr = &sr;
}

void StoreUpdater::update()
{
    qDebug() << getCurrentTime() << "обновление остатков" << sr->getCurrentFilePath();

    DataImport *di;
    if (sr->getStorePlace() == "Витебск") {
        di = new VitebskDataImport(sr);
    } else {
        di = new DataImport(sr);
    }

    if (!di->connect()) {
        emit importError("Обновление " + sr->getCurrentFilePath() + "уже запущено");
        return;
    }

    if (!di->convertXlsToCsv()) {
        emit importError("Ошибка создания файла " + sr->getCurrentFilePath() +
                         ".csv - обновление прервано");
        return;
    }

    if (!di->createTxt()) {
        emit importError("Ошибка создания файла " + sr->getCurrentFilePath() +
                         ".csv.txt - обновление прервано");
        return;
    }

    QString importResult = di->import(xlsFilePath);
    if (!importResult.isEmpty()) emit importError(importResult);

    if (di->getUnknownProducts().count() > 0) {
        QString msg("Во время импорта обнаружены отсутствующие в номеклатуре артикулы:\n");
        QHash<QString, QString> unknownProducts = di->getUnknownProducts();
        QHash<QString, QString>::iterator it = unknownProducts.begin();
        while (it != unknownProducts.end()) {
            msg.append(it.key() + " " + it.value() + "\n");
            ++it;
        }
        msg.append("Их необходимо внести вручную в номенклатуру, иначе в остатках складов "
                   "они отображаться не будут!");
        emit importError(msg);
    }

    qDebug() << getCurrentTime() << "обновление остатков"
             << sr->getCurrentFilePath() << "завершено";

}

void StoreUpdater::run()
{
    int freeze = 1;
    do {
        QThread::sleep(15*freeze);
        sr->updateCurrentFile();
        freeze++;
    } while (!QFile::exists(sr->getCurrentFilePath()) && freeze < 20);
    if(QFile::exists(sr->getCurrentFilePath())){
        xlsFilePath = sr->getCurrentFilePath();
        updateCsvFilePath(xlsFilePath);
        update();
        fsw->addPath(xlsFilePath); //добавляем в watcher новый файл
        qDebug() << "новый файл " + xlsFilePath + " добавлен в watcher";
    } else {
        QSqlQuery query = QSqlQuery();
        query.prepare("SELECT mn.name, sm.storePlace "
                      "FROM store_manufacturer AS sm INNER JOIN manufacturers AS mn ON sm.mid = mn.mid "
                      "WHERE sm.smid = :smid");
        query.bindValue(":smid", sr->getSmid());
        query.exec();
        query.next();

        QSystemTrayIcon trayIcon;
        trayIcon.setVisible(true);
        trayIcon.showMessage("Файл не найден", "Файл остатков " + query.value(0).toString() +
                                 " " + query.value(1).toString() + " не найден");
    }
    emit updateFinished(sr);
}

void StoreUpdater::updateCsvFilePath(QString xlsFilePath)
{
    csvFilePath = xlsFilePath.replace(QRegularExpression(".xlsx?"), ".csv").replace("/", "\\");
}

QString StoreUpdater::getCurrentTime()
{
    return QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
}


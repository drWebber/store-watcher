#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T16:20:27
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = store-watcher
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    settings/settings.cpp \
    settings/actionwnd.cpp \
    sql/sqlconnector.cpp \
    sql/sqlquery.cpp \
    settings/addstore.cpp \
    settings/editstore.cpp \
    store-remainings/storeremainings.cpp \
    store-remainings/storewatcher.cpp \
    store-updater/storeupdater.cpp \
    filters/sortfilteproxyrmodel.cpp \
    excel/xlsreader.cpp \
    import/csvreader.cpp \
    import/dataparcer.cpp \
    import/datawriter.cpp \
    models/coloredsqlquerymodel.cpp \
    store-updater/dataimport.cpp \
    store-updater/vitebskdataimport.cpp

HEADERS  += mainwindow.h \
    settings/settings.h \
    settings/actionwnd.h \
    sql/sqlconnector.h \
    sql/sqlquery.h \
    settings/addstore.h \
    settings/editstore.h \
    store-remainings/storeremainings.h \
    store-remainings/storewatcher.h \
    store-updater/storeupdater.h \
    filters/sortfilteproxyrmodel.h \
    excel/xlsreader.h \
    import/csvreader.h \
    import/dataparcer.h \
    import/datawriter.h \
    models/coloredsqlquerymodel.h \
    store-updater/dataimport.h \
    store-updater/vitebskdataimport.h

FORMS    += mainwindow.ui \
    settings/settings.ui \
    settings/actionwnd.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    ../../Иконки/toolbar/filter_add-64.png

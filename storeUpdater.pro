#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T18:18:14
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = storeUpdater
TEMPLATE = app


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
    xls/xlsreader.cpp

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
    xls/xlsreader.h

FORMS    += mainwindow.ui \
    settings/settings.ui \
    settings/actionwnd.ui

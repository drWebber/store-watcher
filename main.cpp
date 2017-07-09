#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/recources/images/Programs.png"));
    w.setWindowTitle("Store Updater");
    w.show();    

    return a.exec();
}

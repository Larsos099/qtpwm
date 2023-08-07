#include "mainwindow.h"

#include <QApplication>

using namespace Qt::StringLiterals;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    a.setStyle("fusion");
    w.show();
    return a.exec();
}

#include "dialog.h"
#include <QApplication>
#include <QtWidgets>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    QObject::connect(qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));
    w.showMaximized();
    w.show();

    return a.exec();
}

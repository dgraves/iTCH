#include <QtGui/QApplication>
#include "PiTCHWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PiTCHWindow w;
    w.show();
    return a.exec();
}

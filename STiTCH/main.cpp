#include <QtGui/QApplication>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMessageBox>
#include "STiTCHDialog.h"

int main(int argc, char *argv[])
{
//  Q_INIT_RESOURCE(STiTCH);

  QApplication a(argc, argv);

  if (!QSystemTrayIcon::isSystemTrayAvailable())
  {
    QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("No system tray was detected on this system."));
    return 1;
  }

//  QApplication::setQuitOnLastWindowClosed(false);

  STiTCHDialog d;
  d.show();
  return a.exec();
}

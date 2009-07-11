#ifndef ITCHCONTROLLER_H
#define ITCHCONTROLLER_H

#include <QtCore/QObject>
#include "iTCHMethod.h"

class iTCHController : public QObject
{
  Q_OBJECT

public:
  iTCHController();

  void callMethod(const iTCHMethod& method);
};

#endif // ITCHCONTROLLER_H

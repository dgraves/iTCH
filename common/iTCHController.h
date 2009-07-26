#ifndef ITCHCONTROLLER_H
#define ITCHCONTROLLER_H

#include <QtCore/QObject>
#include "iTCHMethod.h"

struct IiTunes;

class iTCHController : public QObject
{
  Q_OBJECT

public:
  iTCHController();

  ~iTCHController();

  void callMethod(const iTCHMethod& method);

protected:
  void createInstance();

  void destroyInstance();

protected:
  IiTunes *itunes_;
};

#endif // ITCHCONTROLLER_H

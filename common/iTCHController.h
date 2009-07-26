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

  bool hasInstance() const;

  void callMethod(const iTCHMethod& method);

  void createInstance();

  void destroyInstance();

signals:
  void createdInstance();
  void destroyedInstance();

protected:
  IiTunes *itunes_;
};

#endif // ITCHCONTROLLER_H

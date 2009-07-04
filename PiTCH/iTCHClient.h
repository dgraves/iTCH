#ifndef ITCHCLIENT_H
#define ITCHCLIENT_H

class iTCHClient : public QObject
{
  Q_OBJECT

public:
  iTCHClient(QWidget *parent = 0);
  ~iTCHClient();
};

#endif // ITCHCLIENT_H

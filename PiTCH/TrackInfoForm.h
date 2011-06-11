#ifndef TRACKINFOFORM_H
#define TRACKINFOFORM_H

#include <QtGui/QWidget>

namespace Ui {
  class TrackInfoForm;
}

class TrackInfoForm : public QWidget
{
  Q_OBJECT

public:
  explicit TrackInfoForm(QWidget *parent = 0);

  ~TrackInfoForm();

  void setArtist(const QString &artist);

  void setSongTitle(const QString &title);

private:
  Ui::TrackInfoForm *ui_;
};

#endif // TRACKINFOFORM_H

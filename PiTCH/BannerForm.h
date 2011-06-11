#ifndef BANNERFORM_H
#define BANNERFORM_H

#include <QtGui/QWidget>

namespace Ui {
  class BannerForm;
}

class BannerForm : public QWidget
{
  Q_OBJECT

public:
  explicit BannerForm(QWidget *parent = 0);

  ~BannerForm();

  virtual void resizeEvent(QResizeEvent *event);

private:
  Ui::BannerForm *ui_;
};

#endif // BANNERFORM_H

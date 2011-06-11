#include <QtGui/QResizeEvent>
#include "BannerForm.h"
#include "ui_BannerForm.h"

BannerForm::BannerForm(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::BannerForm)
{
    ui_->setupUi(this);
}

BannerForm::~BannerForm()
{
    delete ui_;
}

void BannerForm::resizeEvent (QResizeEvent *event)
{
  ui_->banner->setFixedWidth(event->size().height());
}

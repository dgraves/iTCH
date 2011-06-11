#include "TrackInfoForm.h"
#include "ui_TrackInfoForm.h"

TrackInfoForm::TrackInfoForm(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::TrackInfoForm)
{
  ui_->setupUi(this);
}

TrackInfoForm::~TrackInfoForm()
{
  delete ui_;
}

void TrackInfoForm::setArtist(const QString &artist)
{
  ui_->artist->setText(artist);
}

void TrackInfoForm::setSongTitle(const QString &title)
{
  ui_->songTitle->setText(title);
}

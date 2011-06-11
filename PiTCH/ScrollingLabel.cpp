/********************************************************************
 * The iTCH project - Copyright (c) 2011 Dustin Graves
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>
#include "ScrollingLabel.h"

namespace
{
  enum 
  {
    DEFAULT_SCROLL_TIMEOUT = 20,
    DEFAULT_SCROLL_RATE    = 15,
    DEFAULT_SCROLL_DELAY   = 2000
  };

  const QString SCROLL_TEXT_GAP = "     "; // Gap between repeated text strings in the scroll
}

ScrollingLabel::ScrollingLabel(QWidget *parent) :
  QLabel(parent),
  scroll_(false),
  scrollRate_(DEFAULT_SCROLL_RATE),
  scrollDelay_(DEFAULT_SCROLL_DELAY)
{
  textWidth_ = fontMetrics().width(text());
  computeScroll();
  connect(&scrollTimer_, SIGNAL(timeout()), this, SLOT(repaint()));
}

int ScrollingLabel::getScrollRate() const
{
  return scrollRate_;
}

int ScrollingLabel::getScrollDelay() const
{
  return scrollDelay_;
}

void ScrollingLabel::setText(const QString &newText)
{
  // Disable scrolling when new text is specified
  stopScroll();

  QLabel::setText(newText);

  textWidth_ = fontMetrics().width(text());
  computeScroll();
}

void ScrollingLabel::setScrollRate(int rate)
{
  scrollRate_ = rate;
}

void ScrollingLabel::setScrollDelay(int msec)
{
  scrollDelay_ = msec;
}

void ScrollingLabel::paintEvent(QPaintEvent *evt)
{
  if (scroll_)
  {
    QStyle *style = QWidget::style();
    QPainter painter(this);
    drawFrame(&painter);
    QRect cr = contentsRect();
    cr.adjust(margin(), margin(), -margin(), -margin());
    int align = QStyle::visualAlignment(Qt::LeftToRight, QFlag(alignment()));

    QStyleOption opt;
    opt.initFrom(this);

    int flags = align | Qt::TextForceLeftToRight;

    qreal rh = cr.height();
    qreal yo = 0;
    if (align & Qt::AlignVCenter)
        yo = qMax((cr.height()-rh)/2, qreal(0));
    else if (align & Qt::AlignBottom)
        yo = qMax(cr.height()-rh, qreal(0));

    int position = computePosition();
    QRectF lr(position, yo + cr.y(), scrollTextWidth_, cr.height());
    style->drawItemText(&painter, lr.toRect(), flags, opt.palette, isEnabled(), scrollText_, foregroundRole());
    painter.translate(position, 0);
  }
  else
  {
    QLabel::paintEvent(evt);
  }
}

void ScrollingLabel::resizeEvent(QResizeEvent *evt)
{
  computeScroll();
}

void ScrollingLabel::computeScroll()
{
  if (textWidth_ > width())
  {
    // If not already scrolling, we need to start
    startScroll();
  }
  else
  {
    // If scrolling, we need to stop
    stopScroll();
  }
}

int ScrollingLabel::computePosition()
{
  int elapsed = scrollTime_.elapsed();
  if (elapsed < scrollDelay_)
  {
    return 0;
  }
  else
  {
    int position;

    elapsed -= scrollDelay_;
    position = (elapsed * -scrollRate_) / 1000;

    // Reset when the text has scrolled past the first occurance of the string
    if (-position >= resetPosition_)
    {
      scrollTime_.restart();
      return 0;
    }
    else
    {
      return position;
    }
  }
}

void ScrollingLabel::startScroll()
{
  if (!scroll_)
  {
    scroll_ = true;

    scrollText_ = QString("%1%2%1").arg(text()).arg(SCROLL_TEXT_GAP);
    scrollTextWidth_ = fontMetrics().width(scrollText_);
    resetPosition_ = textWidth_ + fontMetrics().width(SCROLL_TEXT_GAP);

    scrollTime_.start();
    scrollTimer_.start(DEFAULT_SCROLL_TIMEOUT);
  }
}

void ScrollingLabel::stopScroll()
{
  if (scroll_)
  {
    scroll_ = false;
    scrollTimer_.stop();
  }
}

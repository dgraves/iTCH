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
#ifndef SCROLLINGLABEL_H
#define SCROLLINGLABEL_H

#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QLabel>

// The ScrollignLabel scrolls its text from right to left when the text is
// wider than the label.  The text wraps around and pauses each time the
// text completes a single revolution.
class ScrollingLabel : public QLabel
{
  Q_OBJECT
public:
  explicit ScrollingLabel(QWidget *parent = 0);

  int getScrollRate() const;
  int getScrollDelay() const;

public slots:
  void setText(const QString &newText);
  void setScrollRate(int rate);         // Pixels per second
  void setScrollDelay(int msec);        // Time to pause before starting to scroll, in milliseconds

protected:
  virtual void paintEvent(QPaintEvent *evt);
  virtual void resizeEvent(QResizeEvent *evt);

private:
  void computeScroll();
  int computePosition();
  void startScroll();
  void stopScroll();

private:
  bool    scroll_;          // Scrolling is enabled or disabled, based on length of text
  int     scrollRate_;      // Pixels per second to scroll
  int     scrollDelay_;     // Time to pause before starting to scroll, in milliseconds
  QTime   scrollTime_;      // Time that scrolling started
  QString scrollText_;      // String formed as "text  text" to provide wrap around effect
  int     scrollTextWidth_; // Width of the scrolling text
  int     resetPosition_;   // Position where scrolling should restart from beginning of text,
                            // set to length of the "text  " part of scrollText_
  int     textWidth_;       // Actual width of label's text
  QTimer  scrollTimer_;
};

#endif // SCROLLINGLABEL_H

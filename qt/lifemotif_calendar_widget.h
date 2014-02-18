#ifndef LIFEMOTIF_CALENDAR_WIDGET_H
#define LIFEMOTIF_CALENDAR_WIDGET_H

#include <QCalendarWidget>

// all headers you use in this class should be included.
#include <QBrush>
#include <QMap>
#include <QPainter>
#include <QSet>
#include <QTextCharFormat>
#include "message_types.h"

class LifeMotifCalendarWidget : public QCalendarWidget
{
  Q_OBJECT

public:
  LifeMotifCalendarWidget(QWidget *parent = 0);
  ~LifeMotifCalendarWidget();

  void SetDate(const LocalStructureType& structure);
  void UpdateUI();

protected:
  void
    paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

private:
  void CustomPaintCell(QPainter *painter, const QRect &rect) const;

  QSet<QDate> datesToPaint;
};

#endif // LIFEMOTIF_CALENDAR_WIDGET_H

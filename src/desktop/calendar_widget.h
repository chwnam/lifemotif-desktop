#ifndef LIFEMOTIF_CALENDAR_WIDGET_H
#define LIFEMOTIF_CALENDAR_WIDGET_H

#include <QCalendarWidget>
#include <QBrush>
#include <QMap>
#include <QPainter>
#include <QSet>
#include <QTextCharFormat>

#include "message_types.h"

using namespace LifeMotif;

class CalendarWidget : public QCalendarWidget
{
  Q_OBJECT

public:
  CalendarWidget(QWidget *parent = 0);
  ~CalendarWidget();

  void SetDate(const GoogleLocalStructureType& structure);
  void UpdateUI();

protected:
  void
  paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

private:
  void CustomPaintCell(QPainter *painter, const QRect &rect) const;

  QSet<QDate> datesToPaint;
};

#endif // LIFEMOTIF_CALENDAR_WIDGET_H

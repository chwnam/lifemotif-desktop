#include "calendar_widget.h"
#include "utils.h"

// when linker error about 'vtable' occurs, 'run qmake' and build again.
CalendarWidget::CalendarWidget(QWidget *parent)
  : QCalendarWidget(parent)
{
}

CalendarWidget::~CalendarWidget()
{
}

void
  CalendarWidget::paintCell(
    QPainter *painter, const QRect &rect, const QDate &date) const
{
  // Draw only have articles
  if (datesToPaint.find(date) != datesToPaint.end()) {
    QCalendarWidget::paintCell(painter, rect, date);
  }

//  Draw normally, and draw ellipse
//  QCalendarWidget::paintCell(painter, rect, date);
//  if (datesToPaint.find(date) != datesToPaint.end()) {
//    painter->save();
//    CustomPaintCell(painter, rect);
//    painter->restore();
//  }
}

void CalendarWidget::CustomPaintCell(
    QPainter *painter, const QRect &rect) const
{
  QBrush brush;
  brush.setColor(Qt::blue);

  painter->setBackground(brush);
  painter->drawEllipse(rect);
}

void CalendarWidget::SetDate(const GoogleLocalStructureType& structure)
{
  if (structure.empty() == false) {
    if (datesToPaint.isEmpty() == false) QSet<QDate>().swap(datesToPaint);

    GoogleLocalStructureType::const_iterator it;
    for(it = structure.constBegin(); it != structure.constEnd(); ++it) {
      const QString dateString = it.key();
      const QDate   date       = QDate::fromString(dateString, "yyyyMMdd");
      datesToPaint.insert(date);
    }
  }
}

void CalendarWidget::UpdateUI()
{
  if (datesToPaint.isEmpty() == true) { setDisabled(true); }

  else {

    QSet<QDate>::iterator minDate, maxDate, it;

    minDate = datesToPaint.begin();
    maxDate = datesToPaint.begin();

    for(it = minDate + 1; it != datesToPaint.end(); ++it) {
      if (*it < *minDate) minDate = it;
      if (*it > *maxDate) maxDate = it;
    }

    setMinimumDate(*minDate);
    setMaximumDate(*maxDate);
    setEnabled(true);

    // calendar formatting.
    for(it = datesToPaint.begin(); it != datesToPaint.end(); ++it) {

      QTextCharFormat charFormat = dateTextFormat(*it);

      // currently underline, bold
      charFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
      charFormat.setFontWeight(QFont::Bold);

      setDateTextFormat(*it, charFormat);
    }
  }
}

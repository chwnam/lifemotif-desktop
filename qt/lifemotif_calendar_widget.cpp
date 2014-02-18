#include "lifemotif_calendar_widget.h"

//LifeMotifCalendarWidget::LifeMotifCalendarWidget(QWidget *parent)
//  : QCalendarWidget(parent)
//{
//}

//LifeMotifCalendarWidget::~LifeMotifCalendarWidget()
//{
//}

//void
//  LifeMotifCalendarWidget::paintCell(
//    QPainter *painter, const QRect &rect, const QDate &date) const
//{
//  QCalendarWidget::paintCell(painter, rect, date);
//  if (datesToPaint.find(date) != datesToPaint.end()) {
//  }
//}

//void LifeMotifCalendarWidget::CustomPaintCell(
//    QPainter *painter, const QRect &rect) const
//{
//}

//void LifeMotifCalendarWidget::SetDate(const LocalStructureType& structure)
//{
//  if (structure.empty() == false) {
//    if (datesToPaint.isEmpty() == false) {
//      QSet<QDate>().swap(datesToPaint);
//    }

//    LocalStructureType::const_iterator it;
//    for(it = structure.begin(); it != structure.end(); ++it) {
//      const QString dt = QString::fromStdString(it->first);
//      datesToPaint.insert(QDate::fromString(dt, "yyyyMMdd"));
//    }
//  }
//}

//void LifeMotifCalendarWidget::UpdateUI()
//{
//  if (datesToPaint.isEmpty()) {
//    setDisabled(true);
//  } else {

//    QSet<QDate>::iterator minDate, maxDate, it;

//    minDate = datesToPaint.begin();
//    maxDate = datesToPaint.begin();

//    for(it = minDate + 1; it != datesToPaint.end(); ++it) {
//      if (*it < *minDate) minDate = it;
//      if (*it > *maxDate) maxDate = it;
//    }

//    setMinimumDate(*minDate);
//    setMaximumDate(*maxDate);
//    setEnabled(true);

//    // calendar formatting.
//    const QMap<QDate, QTextCharFormat> &format = dateTextFormat();
//    for(it = datesToPaint.begin(); it != datesToPaint.end(); ++it) {

//      QTextCharFormat charFormat = format[*it];

//      // currently underline, bold
//      charFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
//      charFormat.setFontWeight(QFont::Bold);

//      setDateTextFormat(*it, charFormat);
//    }
//  }
//}

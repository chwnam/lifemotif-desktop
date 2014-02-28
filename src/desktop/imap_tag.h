#ifndef IMAP_TAG_H
#define IMAP_TAG_H

#include <QChar>
#include <QByteArray>

class ImapTag
{
public:
  ImapTag();

public:
  QByteArray Issue();

  void Reset(
      const QChar   newPrefix = QChar('A'),
      const quint32 newNum = 0);

  inline void Advance() { ++num; }

private:
  QChar   prefix;
  quint32 num;
};

#endif // IMAP_TAG_H

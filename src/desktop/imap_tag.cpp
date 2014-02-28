#include "imap_tag.h"

namespace LifeMotif {
ImapTag::ImapTag()
{
  Reset();
}

QByteArray ImapTag::Issue()
{
  return prefix.toUpper().toLatin1() + QByteArray::number(num);
}

void ImapTag::Reset(const QChar newPrefix, const quint32 newNum)
{
  prefix = newPrefix;
  num    = newNum;
}
}

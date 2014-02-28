#ifndef LIFEMOTIF_ATTACHMENT_H
#define LIFEMOTIF_ATTACHMENT_H

#include <QString>

namespace LifeMotif {
class Attachment
{
public:
  QString    name;
  QString    type;
  QString    subType;
  QByteArray data;
};
}

#endif // LIFEMOTIF_ATTACHMENT_H

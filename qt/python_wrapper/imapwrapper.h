#ifndef IMAPWRAPPER_H
#define IMAPWRAPPER_H

#include "basepythonwrapper.h"

class ImapWrapper : public BasePythonWrapper
{
public:
  ImapWrapper(
      const std::string& _moduleName,
      const std::string& _className);

  void
  ListMailBox();
};

#endif // IMAPWRAPPER_H

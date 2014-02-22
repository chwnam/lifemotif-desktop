#ifndef IMAPWRAPPER_H
#define IMAPWRAPPER_H

#include "basepythonwrapper.h"

class ImapWrapper : public BasePythonWrapper
{
public:
  ImapWrapper(
      const std::string& _moduleName,
      const std::string& _className);

  // very simple listing. print to stdout
  void
  ListMailbox();

  // very simple listing. Unparsed, undecoded list of string.
  bp::object
  GetMailboxList();
};

#endif // IMAPWRAPPER_H

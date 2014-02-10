#ifndef GOOGLEIMAPWRAPPER_H
#define GOOGLEIMAPWRAPPER_H

#include "imapwrapper.h"
#include <string>

class GoogleImapWrapper : public ImapWrapper
{
public:
  GoogleImapWrapper(
      const std::string& _moduleName,
      const std::string& _className,
      bp::object& _imapObject);

  bp::object
  FetchThreadStructure(const std::string label);

  std::string
  FetchMail(const std::string& label, unsigned long long messageId);

private:
    bp::object imapTuple;
};

#endif // GOOGLEIMAPWRAPPER_H

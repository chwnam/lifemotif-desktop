#include "imapwrapper.h"

ImapWrapper::ImapWrapper(
    const std::string& _moduleName,
    const std::string& _className)
  : BasePythonWrapper(_moduleName, _className)
{  
}

void
ImapWrapper::ListMailBox()
{
  PyObject_CallMethod(
        instance.ptr(),
        "list_mailbox", NULL);
}

#include "imapwrapper.h"

ImapWrapper::ImapWrapper(
    const std::string& _moduleName,
    const std::string& _className)
  : BasePythonWrapper(_moduleName, _className)
{  
}

void
ImapWrapper::ListMailbox()
{
  PyObject_CallMethod(
        instance.ptr(),
        "list_mailbox", NULL);
}

bp::object
ImapWrapper::GetMailboxList()
{
  return
      CreateObject(
        PyObject_CallMethod(instance.ptr(), "get_mailbox_list", NULL));
}

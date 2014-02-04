#include "googleimapwrapper.h"

GoogleImapWrapper::GoogleImapWrapper(
    const std::string& _moduleName,
    const std::string& _className,
    bp::object& _imapObject)
  : ImapWrapper(_moduleName, _className)
{
  std::cout << "Init GoogleImapWrapper...\n";

  // _imapObject is copied, and passed to python object as tuple type.
  imapTuple = CreateObject(Py_BuildValue("(O)", _imapObject.ptr()));

  InitWrapper(imapTuple.ptr());
}

bp::object
GoogleImapWrapper::FetchThreadStructure(const std::string label)
{
  bp::object structure;
  bp::object unicodeLabel;

  unicodeLabel = CreateObject(PyUnicode_FromString(label.c_str()));
  structure = CreateObject(
                PyObject_CallMethod(
                  instance.ptr(),
                  "fetch_thread_structure",
                  "O",
                  unicodeLabel.ptr()));

  return structure;
}

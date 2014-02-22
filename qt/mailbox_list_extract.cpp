#include "mailbox_list_extract.h"
#include <QDebug>

MailboxListExtract::MailboxListExtract(bp::object& input, QStringList& output)
{
  ExtractPyObject(input, output);
}

void MailboxListExtract::ExtractPyObject(
    bp::object& input, QStringList& output)
{
  if (PyList_Check(input.ptr()) == false) {
    return;
  }

  if (output.empty() == false) {
    output.clear();
  }

  bp::list list = bp::extract<bp::list>(input);
  unsigned long len = bp::extract<unsigned long>(list.attr("__len__")());

  for(unsigned long i = 0; i < len; ++i) {
    bp::object  item = list[i];
    const char* cstr = bp::extract<const char*>(item);

    qDebug() << cstr;
    output.push_back(QString::fromUtf8(cstr));
  }
}

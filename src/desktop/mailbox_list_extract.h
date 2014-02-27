#ifndef MAILBOX_LIST_EXTRACT_H
#define MAILBOX_LIST_EXTRACT_H

#include <boost/python.hpp>
#include <QStringList>
#include <QTextCodec>

namespace bp = boost::python;

class MailboxListExtract
{
public:
  MailboxListExtract(
      bp::object&  input,
      QStringList& output);

private:
  void ExtractPyObject(bp::object& input, QStringList& output);
};

#endif // MAILBOX_LIST_EXTRACT_H

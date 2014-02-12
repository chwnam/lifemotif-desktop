#ifndef EMAIL_PARSER_H
#define EMAIL_PARSER_H

#include <mimetic/mimetic.h>

class EmailParser
{
public:
  EmailParser();

  void FeedParser(const std::string& message);
};

#endif // EMAIL_PARSER_H

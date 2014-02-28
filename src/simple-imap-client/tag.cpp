#include "tag.h"
#include <cstdio>

char * Tag::itoa(const unsigned int val)
{
  static char buf[33];
  snprintf(buf, 33, "%u", val);

  return buf;
}


Tag::Tag()
{
  reset();
}

std::string Tag::issue()
{
  return prefix + std::string(itoa(num));
}

void Tag::reset(const char newPrefix, const unsigned int newNum)
{
  prefix = newPrefix;
  num = newNum;
}


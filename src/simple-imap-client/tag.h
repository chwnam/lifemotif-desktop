#ifndef TAG_H
#define TAG_H

#include <string>

class Tag
{
public:
  Tag();

public:
  std::string issue();
  void reset(
      const char newPrefix = 'A', const unsigned int newNum = 0);

  inline void advance() { ++num; }

private:
  char* itoa(const unsigned int val);

private:
  char prefix;
  unsigned int num;
};

#endif // TAG_H

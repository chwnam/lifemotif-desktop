#ifndef LIFEMOTIFEXCEPTIONS_H
#define LIFEMOTIFEXCEPTIONS_H

#include <exception>

class LifeMotifException : public std::exception {
public:
  LifeMotifException(const char* _desc)
    : std::exception()  { desc = _desc; }
  ~LifeMotifException() throw() {}
  const char* what() const throw() {
    return desc;
  }

private:
  const char* desc;
};

class FileNotFoundException : public LifeMotifException {
public:
  FileNotFoundException(const char* _fileName)
    : LifeMotifException(_fileName)  { }
  // ~FileNotFoundException() throw() {}
};

class FileInvalidException : public LifeMotifException {
public:
  FileInvalidException(const char* _fileName)
    : LifeMotifException(_fileName)  {}
  //~FileInvalidException() throw() {}
};

#endif // LIFEMOTIFEXCEPTIONS_H

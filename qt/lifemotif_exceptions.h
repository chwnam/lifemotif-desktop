#ifndef LIFEMOTIFEXCEPTIONS_H
#define LIFEMOTIFEXCEPTIONS_H

#include <exception>
#include <string>

class LifeMotifException : public std::exception {
public:
  LifeMotifException(const std::string& _desc)
    : std::exception()  { desc = _desc; }
  ~LifeMotifException() throw() {}
  const char* what() const throw() {
    return desc.c_str();
  }

private:
  std::string desc;
};

class FileNotFoundException : public LifeMotifException {
public:
  FileNotFoundException(const std::string& _fileName)
    : LifeMotifException(_fileName)  { }
  // ~FileNotFoundException() throw() {}
};

// file format is invalid
class FileInvalidException : public LifeMotifException {
public:
  FileInvalidException(const std::string& _fileName)
    : LifeMotifException(_fileName)  {}
  //~FileInvalidException() throw() {}
};

// program setting is invalid
class LifeMotifInvalidSetting : public LifeMotifException {
public:
  LifeMotifInvalidSetting(const std::string& _desc)
    : LifeMotifException(_desc) {}
};

#endif // LIFEMOTIFEXCEPTIONS_H

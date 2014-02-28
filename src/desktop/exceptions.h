#ifndef LIFEMOTIF_EXCEPTIONS_H
#define LIFEMOTIF_EXCEPTIONS_H

#include <exception>
#include <string>

namespace LifeMotif {

class Exception : public std::exception {
public:
  Exception(const std::string& _desc)
    : std::exception()  { desc = _desc; }
  ~Exception() throw() {}
  const char* what() const throw() {
    return desc.c_str();
  }
private:
  std::string desc;
};

class FileNotFoundException : public Exception {
public:
  FileNotFoundException(const std::string& _fileName)
    : Exception(_fileName)  { }
  // ~FileNotFoundException() throw() {}
};

class DirectoryNotFoundException : public Exception {
public:
  DirectoryNotFoundException(const std::string& _dirName)
    : Exception(_dirName)  { }
  // ~DirectoryNotFoundException() throw() {}
};

// file format is invalid
class FileInvalidException : public Exception {
public:
  FileInvalidException(const std::string& _fileName)
    : Exception(_fileName)  {}
  //~FileInvalidException() throw() {}
};

// program setting is invalid
class InvalidSetting : public Exception {
public:
  InvalidSetting(const std::string& _desc)
    : Exception(_desc) {}
};

}

#endif // LIFEMOTIF_EXCEPTIONS_H

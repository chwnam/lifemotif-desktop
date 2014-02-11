#ifndef LIFEMOTIF_PATH_HELPER_H
#define LIFEMOTIF_PATH_HELPER_H

#include <stdarg.h>
#include <string>

class LifeMotifPathHelper
{
public:
  static std::string Join(int n, ...);
  static std::string CanonicalPath(const std::string& input);
};

#endif // LIFEMOTIF_PATH_HELPER_H

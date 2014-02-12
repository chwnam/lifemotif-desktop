#include "lifemotif_utils.h"


std::string LifeMotifUtils::JoinPath(int n, ...)
{
  std::string out;

  va_list argp;

  va_start(argp, n);
  for (int i = 0; i < n; ++i) {
    char *p = va_arg(argp, char *);

    if (p) {
      out.append(p);
      if (i < n-1 && *(out.end()-1) != '/') out.push_back('/');
    }
  }
  va_end(argp);

  return out;
}

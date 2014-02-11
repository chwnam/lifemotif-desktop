#include "lifemotif_path_helper.h"

std::string LifeMotifPathHelper::Join(int n, ...)
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

std::string LifeMotifPathHelper::CanonicalPath(const std::string& input)
{
  std::string out(input);

  for (std::size_t i = 0; i < input.size(); ++i) {
    if (input[i] == '\\') {
      out[i] = '/';
    }
  }

  return out;
}

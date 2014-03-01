#ifndef LIFEMOTIF_CONFIG_H
#define LIFEMOTIF_CONFIG_H

#include <cstdio>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif


namespace LifeMotif {
class Config
{
public:
  static const int majorVersion;
  static const int minorVersion;
  static const int revision;

  static const char organizationName[];
  static const char organizationDomain[];
  static const char applicationName[];

  static const char* Version();

  class DefaultValues
  {
  public:
    static const bool useFileCache;
    static const char fileCacheDir[];
    static const char localStructureDir[];

    class Google {
    public:
      static const char authUri[];
      static const char tokenUri[];
      static const char revokeUri[];
      static const char clientId[];
      static const char clientSecret[];
      static const char redirectUri[];
    }; // class Google
  }; // class DefaultValues
}; // class Config
} // namespace LifeMotif

#endif // LIFEMOTIF_CONFIG_H



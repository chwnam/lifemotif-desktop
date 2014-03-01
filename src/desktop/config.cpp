#include "config.h"


namespace LifeMotif
{
const int Config::majorVersion = 0;
const int Config::minorVersion = 1;
const int Config::revision     = 34;

const char Config::organizationName[]   = "lifemotif-desktop";
const char Config::organizationDomain[] = "changwoo";
const char Config::applicationName[]    = "lifemotif-desktop";

const bool Config::DefaultValues::useFileCache        = true;
const char Config::DefaultValues::fileCacheDir[]      = "cache";
const char Config::DefaultValues::localStructureDir[] = "local";

const char
Config::DefaultValues::Google::scope[]
= "https://mail.google.com/";

const char
Config::DefaultValues::Google::authUri[]
= "https://accounts.google.com/o/oauth2/auth";

const char
Config::DefaultValues::Google::tokenUri[]
= "https://accounts.google.com/o/oauth2/token";

const char
Config::DefaultValues::Google::revokeUri[]
= "https://accounts.google.com/o/oauth2/revoke";

const char
Config::DefaultValues::Google::clientId[]
= "557620163613.apps.googleusercontent.com";

const char
Config::DefaultValues::Google::clientSecret[]\
= "C4-xzuAaIVz4iE_UzyYRGlxq";

const char
Config::DefaultValues::Google::redirectUri[]
= "urn:ietf:wg:oauth:2.0:oob";

const char* Config::Version()
{
  static char ver[20] = "";
  if (ver[0] = 0) {
    snprintf(
          ver, 20, "%d.%d.%04d", majorVersion, minorVersion, revision);
  }
  return ver;
}
}

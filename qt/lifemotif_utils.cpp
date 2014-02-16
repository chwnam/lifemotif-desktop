#include "lifemotif_utils.h"

#include <QDebug>

#include "lifemotif_config.h"
#include "lifemotif_settings.h"

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

GoogleImapWrapperPtr
LifeMotifUtils::CreateImapWrapper(GoogleOauth2WrapperPtr ptr)
{
  GoogleImapWrapperPtr imapWrapper;

  if (ptr) {
    const QString storage = LifeMotifSettings::StorageName();
    const QString emailAddress = LifeMotifSettings::EmailAddress();
    const int debugLevel = LifeMotifSettings::DebugLevel();

    bp::object imapObject =
        ptr->ImapAuthenticate(
          storage.toStdString(),
          emailAddress.toStdString(),
          debugLevel);

    imapWrapper = GoogleImapWrapperPtr(new GoogleImapWrapper(
            LIFEMOTIF_GOOGLE_IMAP_WRAPPER_MODULE,
            LIFEMOTIF_GOOGLE_IMAP_WRAPPER_CLASS,
            imapObject));

    if (imapWrapper == NULL) throw std::bad_alloc();
    qDebug() << "imapwrapper assigned";
  }

  return imapWrapper;
}

GoogleOauth2WrapperPtr LifeMotifUtils::CreateOauth2Wrapper()
{
  GoogleOauth2WrapperPtr ptr
      = GoogleOauth2WrapperPtr(new GoogleOauth2Wrapper(
        LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_MODULE,
        LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_CLASS));

  if (ptr == NULL) throw std::bad_alloc();
  qDebug() << "oauth2wrapper assigned";
  return ptr;
}

LocalStructureWrapperPtr LifeMotifUtils::CreateLocalStructureWrapper()
{
  LocalStructureWrapperPtr ptr
    = LocalStructureWrapperPtr(new LocalStructureWrapper(
        LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_MODULE,
        LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_CLASS));

  if (ptr == NULL) throw std::bad_alloc();
  qDebug() << "localstructurewrapper assigned";
  return ptr;
}

QString LifeMotifUtils::Trim(const QString& input, const QChar& s)
{
  int b, e;

  b = (*input.begin() == s) ? 1 : 0;
  e = (*(input.end()-1) == s) ? input.size() - 2 : input.size() - 1;

  return input.mid(b, e);
}
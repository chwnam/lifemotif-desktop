#include "lifemotif_utils.h"

#include <QDebug>
#include <QJsonDocument>
#include <QFile>

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

GoogleImapWrapper*
LifeMotifUtils::CreateImapWrapper(GoogleOauth2Wrapper* ptr)
{
  GoogleImapWrapper* imapWrapper = NULL;

  if (ptr) {
    const QString storage      = LifeMotifSettings::StorageName();
    const QString emailAddress = LifeMotifSettings::EmailAddress();
    const int     debugLevel   = LifeMotifSettings::DebugLevel();

    bp::object imapObject =
        ptr->ImapAuthenticate(
          storage.toStdString(),
          emailAddress.toStdString(),
          debugLevel);

    // ImapAuthenticate may fail if 'strage_name' does not exist.
    if (imapObject.ptr() != Py_None) {
      imapWrapper
        = new GoogleImapWrapper(
            LIFEMOTIF_GOOGLE_IMAP_WRAPPER_MODULE,
            LIFEMOTIF_GOOGLE_IMAP_WRAPPER_CLASS,
            imapObject);
    }
  }

  return imapWrapper;
}

GoogleOauth2Wrapper* LifeMotifUtils::CreateOauth2Wrapper()
{
  GoogleOauth2Wrapper* ptr
      = new GoogleOauth2Wrapper(
        LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_MODULE,
        LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_CLASS);
  return ptr;
}

LocalStructureWrapper* LifeMotifUtils::CreateLocalStructureWrapper()
{
  LocalStructureWrapper* ptr
    = new LocalStructureWrapper(
        LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_MODULE,
        LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_CLASS);
  return ptr;
}

QString LifeMotifUtils::Strip(const QString& input, const QChar& s)
{
  int b, e;

  b = input.startsWith(s) ? 1 : 0;
  e = input.endsWith(s)   ? input.size()-1 : input.size();

  return input.mid(b, e - b);
}

bool LifeMotifUtils::HasCredentials(bool combineWithPythonScriptPath)
{
  return IsFileReadableWritable(
          LifeMotifSettings::StorageName(combineWithPythonScriptPath));
}

#include <QJsonObject>
bool
  LifeMotifUtils::SaveJson(
    const QString& fileName, const QVariantMap& map)
{
  const QByteArray array
    = QJsonDocument( QJsonObject::fromVariantMap(map) ).toJson();
  QFile w(fileName);
  if (w.open(QIODevice::WriteOnly|QIODevice::Text)) {
    w.write(array);
    w.close();
    return true;
  }
  return false;
}

QVariant
  LifeMotifUtils::LoadJson(
    const QString& fileName, bool* isQVariantMap)
{
  QVariant v;
  QFile    r(fileName);
  if (r.open(QIODevice::ReadOnly|QIODevice::Text)) {
    QJsonDocument doc = QJsonDocument::fromJson(r.readAll());
    *isQVariantMap = doc.isObject();
    v = doc.toVariant();
  }
  return v;
}
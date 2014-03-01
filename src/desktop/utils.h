#ifndef LIFEMOTIF_UTILS_H
#define LIFEMOTIF_UTILS_H

#include <string>
#include <QDir>
#include <QFileInfo>
#include <QString>


namespace LifeMotif {

class Utils
{
public:
  static QString
    inline JoinPath(const QString& pre, const QString& add);

  /* is path or file exists? */
  inline static bool Exists(const QString& path);

  inline static bool IsFile(const QString& path);
  inline static bool IsFileReadable(const QString& path);
  inline static bool IsFileReadableWritable(const QString& path);

  inline static bool IsDirectory(const QString& path);
  inline static bool IsDirectoryAccessible(const QString& path);
  inline static bool IsDirectoryAccessibleWritable(const QString& path);

  /* text manipulation */
  static QString Strip(const QString& input, const QChar& s);

  /* JSON - QVariantMap save and load */
  static bool
  SaveJson(const QString& fileName, const QVariantMap& map);

  static QVariant
  LoadJson(const QString& fileName, bool* isQVariantMap);
};

QString Utils::JoinPath(const QString& pre, const QString& add)
{
  return QDir(pre + QDir::separator() + add).absolutePath();
}

bool Utils::Exists(const QString& path)
{
  return QFileInfo(path).exists();
}

bool Utils::IsFile(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isFile();
}

bool Utils::IsFileReadable(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isFile() && fi.isReadable();
}

bool Utils::IsFileReadableWritable(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isFile() && fi.isReadable() && fi.isWritable();
}

bool Utils::IsDirectory(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isDir();
}

bool Utils::IsDirectoryAccessible(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isDir() && fi.isExecutable();
}

bool Utils::IsDirectoryAccessibleWritable(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isDir() && fi.isExecutable() && fi.isWritable();
}

}
#endif // LIFEMOTIF_UTILS_H

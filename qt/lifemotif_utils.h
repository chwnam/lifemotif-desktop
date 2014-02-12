#ifndef LIFEMOTIF_UTILS_H
#define LIFEMOTIF_UTILS_H

#include <stdarg.h>
#include <string>
#include <QFileInfo>

class LifeMotifUtils
{
public:
  /* join N path c-strings */
  static std::string JoinPath(int n, ...);

  /* is path or file exists? */
  inline static bool Exists(const QString& path);

  inline static bool IsFile(const QString& path);
  inline static bool IsFileReadable(const QString& path);

  inline static bool IsDirectory(const QString& path);
  inline static bool IsDirectoryAccessible(const QString& path);
  inline static bool IsDirectoryAccessibleWritable(const QString& path);
};

bool LifeMotifUtils::Exists(const QString& path)
{
  return QFileInfo(path).exists();
}

bool LifeMotifUtils::IsFile(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isFile();
}

bool LifeMotifUtils::IsFileReadable(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isFile() && fi.isReadable();
}

bool LifeMotifUtils::IsDirectory(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isDir();
}

bool LifeMotifUtils::IsDirectoryAccessible(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isDir() && fi.isExecutable();
}

bool LifeMotifUtils::IsDirectoryAccessibleWritable(const QString& path)
{
  QFileInfo fi(path);
  return fi.exists() && fi.isDir() && fi.isExecutable() && fi.isWritable();
}

#endif // LIFEMOTIF_UTILS_H

#ifndef LIFEMOTIF_DIARY_H
#define LIFEMOTIF_DIARY_H

#include <mimetic/mimetic.h>
#include <QString>
#include <QStringList>
#include <QVector>

struct LifeMotifAttachment
{
  QString    name;
  QString    type;
  QString    subType;
  QByteArray data;
};

class LifeMotifDiary
{
public:
  LifeMotifDiary(const std::string& content);

  inline const QString& From() const { return from; }
  inline const QString& To() const { return to; }
  inline const QString& Subject() const { return subject; }
  
  inline const QString&
    TextPlainContent() const { return textPlainContent; }
  
  inline const QString&
    TextHtmlContent() const { return textHtmlContent; }

  int NumberOfAttachments() const {
    return attachments.size();
  }

  const LifeMotifAttachment&
    GetAttachment(const int n) const {
    return attachments[n];
  }

private:
  void ParseHeader(const mimetic::Header& header);
  void ParseBody(const mimetic::Body& body);
  QString ParseMailBox(const mimetic::Mailbox& mailbox);

  QString
    Base64DecodeBody(
      const mimetic::Body& body,
      const std::string& charset = "");

  bool
    GetAttachment(
      const mimetic::Body& body,
      LifeMotifAttachment& attachment);
  
  QString
    DecodeEncodedWords(const std::string& input);

  void
    LifeMotifDiary::Dew(
    const char* input,
    const std::size_t len,
    std::string& charset,
    std::string& text);

private:
  QString from;
  QString to;
  QString subject;
  QString textPlainContent;
  QString textHtmlContent;
  QVector<LifeMotifAttachment> attachments;

  mimetic::MimeEntity root;
};

#endif // LIFEMOTIF_DIARY_H

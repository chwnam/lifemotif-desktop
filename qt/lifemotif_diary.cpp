#include "lifemotif_diary.h"
#include <QStack>
#include <QPair>
#include <QDebug>
#include <QTextCodec>
#include <cctype>

#include "lifemotif_utils.h"

LifeMotifDiary::LifeMotifDiary(const std::string& content)
  : root(content.begin(), content.end())
{
  const mimetic::Header& header = root.header();
  const mimetic::Body&   body   = root.body();

  ParseTopHeader(header);
  ParseBody(body);
}

QString LifeMotifDiary::ParseMailBox(const mimetic::Mailbox& mailbox)
{  
  QString mbstr  = QString::fromStdString(mailbox.mailbox());
  QString domain = QString::fromStdString(mailbox.domain());
  QString result;

  if (mailbox.label() != "") { // email address with label
    QString label = DecodeEncodedWords(mailbox.label());  
    // trim double quote
    if (label.left(1) == QString("\"")) {
      label = LifeMotifUtils::Trim(label, QChar('\"'));    
    }
    result = QString("\"%1\" <%2@%3>").arg(label, mbstr, domain);
  } else { // without label
    result = QString("%1@%2").arg(mbstr, domain);
  }

  return result;
}

void LifeMotifDiary::ParseTopHeader(const mimetic::Header& header)
{
  // collect "from" information
  const mimetic::MailboxList &mbListFrom = header.from();
  mimetic::MailboxList::const_iterator mbit;
  for(mbit = mbListFrom.begin(); mbit != mbListFrom.end(); ++mbit) {
    from += ParseMailBox(*mbit);
    from += QString(", ");
  }

  // collect "to" information
  const mimetic::AddressList &adListTo   = header.to();
  mimetic::AddressList::const_iterator adit;
  for(adit = adListTo.begin(); adit != adListTo.end(); ++adit) {
    to += ParseMailBox(adit->mailbox());
    to += QString(", ");
  }

  // collect "subject" information
  subject = DecodeEncodedWords(header.subject());
}

void LifeMotifDiary::ParseBody(const mimetic::Body& body)
{
  const mimetic::ContentType &ct = body.owner()->header().contentType();

  if (ct.type() == std::string("text")) {
    if (ct.subtype() == std::string("plain")) {
      // plain type text entity
      CallbackForText(body, textPlainContent);
    } else if (ct.subtype() == std::string("html")) {
      // html type text entity
      CallbackForText(body, textHtmlContent);
    }
  } else if (ct.type() == std::string("image")) {
    LifeMotifAttachment att;
    CallbackForBinary(body, att);
    attachments.push_back(att);
  } else if (ct.type() == std::string("audio")) {
    // audio. skip.
  } else if (ct.type() == std::string("multipart")) {
    mimetic::MimeEntityList::const_iterator it;
    for (it = body.parts().begin(); it != body.parts().end(); ++it) {
      ParseBody((*it)->body());
    }
  } else {
    // unknown content type.
  }
}

void
  LifeMotifDiary::CallbackForText(
    const mimetic::Body &body,
    QString &out)
{
  const mimetic::ContentTransferEncoding& cte;
  const std::string charset;
}

void
  LifeMotifDiary::CallbackForBinary(
    const mimetic::Body &body,
    LifeMotifAttachment &out)
{

}

QByteArray
  LifeMotifDiary::Base64DecodeBody(
    const mimetic::Body& body)
{
  QByteArray result;

  if (body.owner()->header().contentTransferEncoding().mechanism() ==
      mimetic::ContentTransferEncoding::base64) {
    std::stringstream        stream;
    mimetic::Base64::Decoder decoder;

    qDebug() << "Body message:";
    qDebug() << body.c_str();

    mimetic::decode(
      body.begin(),
      body.end(),
      decoder,
      std::ostreambuf_iterator<char>(stream));

    std::string str = stream.str();
    qDebug() << str.c_str();
    qDebug() << "decoded size:" << str.size();

    result = QByteArray(str.c_str(), str.size());
  }
  return result;
}

QString
  LifeMotifDiary::DecodeByteArray(const QByteArray& array, const std::string& charset)
{
  QString result;

  QTextCodec *codec = QTextCodec::codecForName(charset.c_str());
  if (codec) {
    result = codec->toUnicode(array);
  }
  return result;
}

bool
  LifeMotifDiary::GetAttachment(
    const mimetic::Body& body,
    LifeMotifAttachment& attachment)
{
  const mimetic::Header&
    header = body.owner()->header();

  const mimetic::ContentDisposition&
    cd = header.contentDisposition();

  const mimetic::ContentType&
    ct = header.contentType();

  const mimetic::ContentTransferEncoding& cte
    = header.contentTransferEncoding();

  if (cd.type() == "attachment" &&
      cte.mechanism() == mimetic::ContentTransferEncoding::base64) {
    attachment.type = QString::fromStdString(ct.type());
    attachment.subType = QString::fromStdString(ct.subtype());
    attachment.name = QString::fromStdString(cd.param("filename"));
    attachment.data = Base64DecodeBody(body);

    return true;
  }

  return false;
}

QString LifeMotifDiary::DecodeEncodedWords(const std::string& input)
{
  enum {LF = 0xA, CR = 0xD, TAB = 0x9, SP = 0x20};
  
  std::size_t b, e;
  QString output;

  b = 0;
  e = 1;

  while (e < input.size()) {

    while (e < input.size() &&
           (LF != input[e]  && CR != input[e] &&
            TAB != input[e] && SP != input[e])) { ++e; }

    std::string charset, subtext;
    Dew(input.c_str() + b, (e - b), charset, subtext);
    b = ++e;

    QTextCodec* codec = QTextCodec::codecForName(charset.c_str());
    if (codec) {
      QString qtSubText = codec->toUnicode(subtext.c_str());
      output += qtSubText;
    } else {
      output += QString::fromStdString(subtext);
    }

    while (e < input.size() &&
           (LF == input[e]  || CR == input[e] ||
            TAB == input[e] || SP == input[e])) { ++e; }
  }

  //qDebug() << "Decoded text:" << output;
  return output;
}

void
  LifeMotifDiary::Dew(
    const char* input,
    const std::size_t len,
    std::string& charset,
    std::string& text)
{
  text = std::string(input, input + len); // fallback text

  // identify input length. Must be longer than 7 (=??Q??=).
  if (7 < len) {
    
    const std::string& first2
      = std::string(input, input+2); // first two
    
    const std::string& last2
      = std::string(input + len - 2, input + len); // last two

    // identify first two characters are =?, the last two characters are ?=
    if (first2 == "=?" && last2 == "?=") {
      std::size_t b = 2, e = b;
      std::string encoding, encodedText;
      
      while (e != len && input[e] != '?') { ++e; }// find next '?'
      charset = std::string(input + b, input + e);
      b = ++e;

      while (e != len && input[e] != '?') { ++e; } // find next '?'
      encoding = std::string(input + b, input + e);
      b = ++e;

      while (e != len && input[e] != '?') { ++e; } // find next '?'
      encodedText = std::string(input + b, input + e);

      std::stringstream stream;
      if ('Q' == toupper(encoding[0])) { // Quoted-printable
        mimetic::QP::Decoder decoder;
        mimetic::decode(
          encodedText.begin(), encodedText.end(),
          decoder, std::ostreambuf_iterator<char>(stream));
      } else if ('B' == toupper(encoding[0])) { // Base64
        mimetic::Base64::Decoder decoder;
        mimetic::decode(encodedText.begin(), encodedText.end(),
          decoder, std::ostreambuf_iterator<char>(stream));
      }
      text = stream.str();
      /*qDebug() << "\tcharset:" << charset.c_str();
      qDebug() << "\tencoding:" << encoding.c_str();
      qDebug() << "\ttext:" << encodedText.c_str();*/
    } 
  }
}

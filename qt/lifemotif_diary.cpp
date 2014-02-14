#include "lifemotif_diary.h"
#include <QStack>
#include <QPair>
#include <QDebug>

LifeMotifDiary::LifeMotifDiary(const std::string& content)
  : root(content.begin(), content.end())
{
  const mimetic::Header& header = root.header();
  const mimetic::Body&   body   = root.body();

  ParseHeader(header);
  ParseBody(body);
}

void LifeMotifDiary::ParseHeader(const mimetic::Header& header)
{
  // collect "from" information
  const mimetic::MailboxList &mbListFrom = header.from();
  mimetic::MailboxList::const_iterator mbit;
  for(mbit = mbListFrom.begin(); mbit != mbListFrom.end(); ++mbit) {
    from += QString::fromStdString(mbit->str());
    from += QString(", ");
  }

  // collect "to" information
  const mimetic::AddressList &adListTo   = header.to();
  mimetic::AddressList::const_iterator adit;
  for(adit = adListTo.begin(); adit != adListTo.end(); ++adit) {
    to += QString::fromStdString(adit->mailbox().str());
    to += QString(", ");
  }

  // collect "subject" information
  subject = QString::fromStdString(header.subject());

  //Decode(header.subject);
}

void LifeMotifDiary::ParseBody(const mimetic::Body& body)
{
  // skip text/plain content.
  // find text/html content and get the text
  // find content-disposition attachment and get the attachment
  QStack<
      QPair<
      mimetic::MimeEntityList::const_iterator,
      mimetic::MimeEntityList::const_iterator
      >
      > stack;


  // TODO: when content-type is multipart/alternative, carefully choose.
  mimetic::MimeEntityList::const_iterator beg = body.parts().begin();
  mimetic::MimeEntityList::const_iterator end = body.parts().end();

  do {

    while (beg == end) {
      beg = stack.top().first;
      end = stack.top().second;
      stack.pop();
      qDebug() << "stack pop()";
    }

    const mimetic::ContentType &ct = (*beg)->header().contentType();
    const mimetic::Body        &bd = (*beg)->body();

    if (ct.isMultipart()) {
      qDebug() << "multipart:" << QString::fromStdString(ct.str());
      stack.push(qMakePair(++beg, end));
      qDebug() << "stack push()";
      beg = bd.parts().begin();
      end = bd.parts().end();
    } else {
      qDebug() << "*NOT* multipart:" << QString::fromStdString(ct.str());
      // check content type
      if (ct.type() == "text") {

        if (ct.subtype() == "plain") {
          qDebug() << "text/plain";
          textPlainContent = Base64DecodeBody((*beg)->body());
        } else if (ct.subtype() == "html") {
          qDebug() << "text/html";
          textHtmlContent = Base64DecodeBody((*beg)->body());
        }
      } else {
        // collect
        qDebug() << "attachment";
        LifeMotifAttachment att;
        if (GetAttachment((*beg)->body(), att)) {
          attachments.push_back(att);
        }
      }
      ++beg;
    }
  } while(stack.empty() == false);
}

QString LifeMotifDiary::Base64DecodeBody(const mimetic::Body& body)
{
  if (body.owner()->header().contentTransferEncoding().mechanism() == mimetic::ContentTransferEncoding::base64) {
    std::stringstream        stream;
    mimetic::Base64::Decoder decoder;

    mimetic::decode(body.begin(), body.end(), decoder, std::ostreambuf_iterator<char>(stream));
    return QString::fromStdString(stream.str());
  }
  return QString();
}

bool LifeMotifDiary::GetAttachment(const mimetic::Body& body, LifeMotifAttachment& attachment)
{
  const mimetic::Header& header = body.owner()->header();
  const mimetic::ContentDisposition& cd = header.contentDisposition();
  const mimetic::ContentType& ct = header.contentType();
  const mimetic::ContentTransferEncoding& cte = header.contentTransferEncoding();

  if (cd.type() == "attachment" &&
      cte.mechanism() == mimetic::ContentTransferEncoding::base64) {
    attachment.type = QString::fromStdString(ct.type());
    attachment.subType = QString::fromStdString(ct.subtype());
    attachment.name = QString::fromStdString(cd.param("filename"));

    attachment.data = QByteArray(Base64DecodeBody(body).toStdString().c_str());

    return true;
  }

  return false;
}
